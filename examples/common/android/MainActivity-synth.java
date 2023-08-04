/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo, Paolo Marrone
 */

package com.orastron.@NAME@;

import android.app.Activity;
import android.os.Bundle;
import android.webkit.WebView;
import android.webkit.WebSettings;
import android.webkit.WebChromeClient;
import android.webkit.WebViewClient;
import android.webkit.JavascriptInterface;
import android.content.Context;
import android.content.pm.PackageManager;
import androidx.core.app.ActivityCompat;
import android.media.midi.MidiManager;
import android.media.midi.MidiManager.DeviceCallback;
import android.media.midi.MidiDeviceInfo;
import android.media.midi.MidiDeviceInfo.PortInfo;
import android.media.midi.MidiDevice;
import java.util.ArrayList;

import android.util.Log;
public class MainActivity extends Activity {
	static {
		System.loadLibrary("@NAME@");
	}

	public native boolean nativeAudioStart();
	public native void nativeAudioStop();
	public native float nativeGetParameter(int i);
	public native void nativeSetParameter(int i, float v);
	public native void addMidiPort(MidiDevice d, int p);
	public native void removeMidiPort(MidiDevice d, int p);

	private WebView webView;

	public class WebAppInterface {
		private MidiManager midiManager;
		private MidiDeviceCallback midiDeviceCallback;
		public ArrayList<MidiDevice> midiDevices = new ArrayList<MidiDevice>();

		public void addMidiDevices(MidiDeviceInfo[] devices) {
			for (int i = 0; i < devices.length; i++) {
				if (devices[i].getOutputPortCount() == 0)
					continue;
				midiManager.openDevice(devices[i],
					new MidiManager.OnDeviceOpenedListener() {
						@Override
						public void onDeviceOpened(MidiDevice device) {
							PortInfo[] ports = device.getInfo().getPorts();
							for (int i = 0; i < ports.length; i++)
								if (ports[i].getType() == PortInfo.TYPE_OUTPUT)
									addMidiPort(device, ports[i].getPortNumber());
							WebAppInterface.this.midiDevices.add(device);
						}
					}, null);
			}
		}

		public void removeMidiDevices(MidiDeviceInfo[] devices) {
			for (int i = 0; i < midiDevices.size(); i++) {
				MidiDevice device = midiDevices.get(i);
				int id = device.getInfo().getId();
				int j = 0;
				for (; j < devices.length; j++)
					if (id == devices[j].getId())
						break;
				if (j == devices.length)
					continue;
				PortInfo[] ports = device.getInfo().getPorts();
				for (j = 0; j < ports.length; j++)
					if (ports[j].getType() == PortInfo.TYPE_OUTPUT)
						removeMidiPort(device, ports[j].getPortNumber());
				midiDevices.remove(i);
			}
		}

		public void removeAllMidiDevices() {
			for (int i = 0; i < midiDevices.size(); i++) {
				MidiDevice device = midiDevices.get(i);
				PortInfo[] ports = device.getInfo().getPorts();
				for (int j = 0; j < ports.length; j++)
					if (ports[j].getType() == PortInfo.TYPE_OUTPUT)
						removeMidiPort(device, ports[j].getPortNumber());
			}
			midiDevices.clear();
		}

		public class MidiDeviceCallback extends MidiManager.DeviceCallback {
			@Override
			public void onDeviceAdded(MidiDeviceInfo device) {
				WebAppInterface.this.addMidiDevices(new MidiDeviceInfo[]{device});
			}

			@Override
			public void onDeviceRemoved(MidiDeviceInfo device) {
				WebAppInterface.this.removeMidiDevices(new MidiDeviceInfo[]{device});
			}
		}

		@JavascriptInterface
		public boolean hasAudioPermission() {
			return MainActivity.this.checkCallingOrSelfPermission(android.Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED;
		}

		@JavascriptInterface
		public void requestAudioPermission() {
			ActivityCompat.requestPermissions(MainActivity.this, new String[] { android.Manifest.permission.RECORD_AUDIO }, 0);
		}

		@JavascriptInterface
		public boolean audioStart() {
			midiManager = (MidiManager)getSystemService(Context.MIDI_SERVICE);

			addMidiDevices(midiManager.getDevices());

			midiDeviceCallback = new MidiDeviceCallback();
			midiManager.registerDeviceCallback(midiDeviceCallback, null);

			return nativeAudioStart();
		}

		@JavascriptInterface
		public void audioStop() {
			nativeAudioStop();

			midiManager.unregisterDeviceCallback(midiDeviceCallback);

			removeAllMidiDevices();
		}

		@JavascriptInterface
		public float getParameter(int i) {
			return nativeGetParameter(i);
		}

		@JavascriptInterface
		public void setParameter(int i, float v) {
			nativeSetParameter(i, v);
		}
	}

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		webView = new WebView(this);
		setContentView(webView);
		WebSettings webSettings = webView.getSettings();
		webSettings.setJavaScriptEnabled(true);
		webView.setWebChromeClient(new WebChromeClient());
		webView.setWebViewClient(new WebViewClient());
		webSettings.setDomStorageEnabled(true);
		webView.addJavascriptInterface(new WebAppInterface(), "Android");
		webView.loadUrl("file:///android_asset/index.html");
	}

	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		if (grantResults.length > 0)
			webView.loadUrl("javascript:gotAudioPermission()");
	}
}
