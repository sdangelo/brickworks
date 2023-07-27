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

public class MainActivity extends Activity {
	private WebView webView;

	public class WebAppInterface {
		@JavascriptInterface
		public boolean hasAudioPermission() {
			return MainActivity.this.checkCallingOrSelfPermission(android.Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED;
		}

		@JavascriptInterface
		public boolean requestAudioPermission() {
			ActivityCompat.requestPermissions(MainActivity.this, new String[] { android.Manifest.permission.RECORD_AUDIO }, 0);
			return false;
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