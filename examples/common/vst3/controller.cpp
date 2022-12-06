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
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#include "controller.h"

#include "pluginterfaces/base/conststringtable.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "common.h"

tresult PLUGIN_API Controller::initialize(FUnknown *context) {
	tresult r = EditController::initialize(context);
	if (r != kResultTrue)
		return r;

	// add parameters
	for (int i = 0; i < NUM_PARAMETERS; i++)
		parameters.addParameter(
			ConstStringTable::instance()->getString(config_parameters[i].name),
			config_parameters[i].units ? ConstStringTable::instance()->getString(config_parameters[i].units) : nullptr,
			config_parameters[i].steps,
			config_parameters[i].defaultValueUnmapped,
			(config_parameters[i].out ? ParameterInfo::kIsReadOnly | ParameterInfo::kIsHidden : ParameterInfo::kCanAutomate)
			| (config_parameters[i].bypass ? ParameterInfo::kIsBypass : 0),
			i,
			0,
			config_parameters[i].shortName ? ConstStringTable::instance()->getString(config_parameters[i].shortName) : nullptr
		);

#ifdef P_PITCH_BEND
	parameters.addParameter(
			ConstStringTable::instance()->getString("MIDI Pitch Bend"),
			nullptr,
			0,
			0.5f,
			ParameterInfo::kCanAutomate,
			TAG_PITCH_BEND,
			0,
			nullptr
		);
#endif

#ifdef P_MOD_WHEEL
	parameters.addParameter(
			ConstStringTable::instance()->getString("MIDI Mod Wheel"),
			nullptr,
			0,
			0.5f,
			ParameterInfo::kCanAutomate,
			TAG_MOD_WHEEL,
			0,
			nullptr
		);
#endif

	return kResultTrue;
}

tresult PLUGIN_API Controller::setComponentState(IBStream *state) {
	if (!state)
		return kResultFalse;

	IBStreamer streamer(state, kLittleEndian);

	float f;
	for (int i = 0; i < NUM_PARAMETERS; i++) {
		if (config_parameters[i].out)
			continue;
		if (streamer.readFloat(f) == false)
			return kResultFalse;
		setParamNormalized(i, f);
	}

	return kResultTrue;
}

#if defined(P_PITCH_BEND) || defined(P_MOD_WHEEL)
tresult PLUGIN_API Controller::getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id) {
	if (busIndex != 0)
		return kResultFalse;

#ifdef P_PITCH_BEND
	if (midiControllerNumber == Vst::kPitchBend) {
		id = TAG_PITCH_BEND;
		return kResultTrue;
	}
#endif

#ifdef P_MOD_WHEEL
	if (midiControllerNumber == Vst::kCtrlModWheel) {
		id = TAG_MOD_WHEEL;
		return kResultTrue;
	}
#endif

	return kResultFalse;
}
#endif
