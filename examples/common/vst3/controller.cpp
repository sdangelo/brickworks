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

#include "controller.h"

#include "pluginterfaces/base/conststringtable.h"
#include "base/source/fstreamer.h"

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
