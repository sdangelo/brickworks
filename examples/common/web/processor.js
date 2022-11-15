/*
 * Brickworks
 *
 * Copyright (C) 2022 Orastron Srl unipersonale
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
 *
 * File author: Stefano D'Angelo
 */

class BWExample extends AudioWorkletProcessor {
	constructor(options) {
		super();

		var module = new WebAssembly.Module(options.processorOptions.wasmBytes);
		var instance = new WebAssembly.Instance(module, { env: {} });
		this.module = instance.exports;

		this.instance = this.module.wrapper_new(sampleRate);
		if (!this.instance)
			// TODO: what to do if this is the case?
			throw "Could not instantiate";

		function getBuffers(p, output) {
			var ret = [];
			for (var i = 0; i < buses.length; i++) {
				if ((!output && buses[i].output) || (output && !buses[i].output))
					continue;
				var b = [ new Float32Array(this.module.memory.buffer, p, 128) ];
				p += 128 * 4;
				if (buses[i].stereo) {
					b.push(new Float32Array(this.module.memory.buffer, p, 128));
					p += 128 * 4;
				}
				ret.push(b);
			}
			return ret;
		}
		this.ins = getBuffers.call(this, this.module.wrapper_get_ins(this.instance), false);
		this.outs = getBuffers.call(this, this.module.wrapper_get_outs(this.instance), true);

		if (parameters.length) {
			this.paramValues = new Float32Array(this.module.memory.buffer, this.module.wrapper_get_param_values(this.instance), parameters.length);
			this.paramValuesPrev = new Float32Array(this.paramValues);
		} else {
			this.paramValues = [];
			this.paramValuesPrev = [];
		}

		var self = this;
		this.port.onmessage = function (e) {
			switch (e.data.type) {
			case "noteOn":
				self.module.wrapper_note_on(self.instance, e.data.note, e.data.velocity);
				break;
			case "noteOff":
				self.module.wrapper_note_off(self.instance, e.data.note);
				break;
			}
		};
	}

	// TODO: "destructor"

	process(inputs, outputs, params) {
		for (var i = 0; i < parameters.length; i++) {
			if (parameters[i].output)
				continue;
			var n = parameters[i].name;
			var v = params[n][0];
			if (v != this.paramValues[n]) {
				this.module.wrapper_set_parameter(this.instance, i, v);
				this.paramValues[n] = v;
			}
		}

		//mmm...
		var n = outputs[0][0].length;
		var i = 0;
		while (i < n) {
			var s = Math.min(n - i, 128);

			for (var j = 0; j < this.ins.length; j++) {
				var input = inputs[j];
				if (!input.length) {
					for (var k = 0; k < this.ins[j].length; k++)
						this.ins[j][k].fill(0);
				} else {
					if (this.ins[j].length != input.length)
						throw "input " + j + " has " + this.ins[j].length + " channels, not " + input.length;
					for (var k = 0; k < this.ins[j].length; k++)
						for (var l = 0; l < s; l++)
							this.ins[j][k][l] = input[k][i + l];
				}
			}

			this.module.wrapper_process(this.instance, s);

			for (var j = 0; j < this.outs.length; j++) {
				var output = outputs[j];
				for (var k = 0; k < output.length; k++)
					for (var l = 0; l < s; l++)
						output[k][i + l] = this.outs[j][k][l];
			}

			i += s;
		}

		for (var i = 0; i < parameters.length; i++) {
			if (!parameters[i].output)
				continue;
			if (this.paramValuesPrev[i] != this.paramValues[i])
				this.port.postMessage({ index: i, value: this.paramValues[i] });
		}
		this.paramValuesPrev.set(this.paramValues);

		return true;
	}

	static get parameterDescriptors() {
		var ret = [];
		for (var i = 0; i < parameters.length; i++) {
			var p = parameters[i];
			if (p.output)
				continue;
			ret.push({ name: p.name, minValue: 0.0, maxValue: 1.0, defaultValue: p.defaultValue, automationRate: "k-rate" });
		}
		return ret;
	}
}

registerProcessor("BWExample", BWExample);
