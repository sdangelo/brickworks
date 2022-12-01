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

#ifndef _VST3_CONTROLLER_H
#define _VST3_CONTROLLER_H

#include "config.h"
#include "config_vst3.h"

#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

#if defined(P_PITCH_BEND) || defined(P_MOD_WHEEL)
class Controller : EditController, IMidiMapping {
#else
class Controller : EditController {
#endif
public:
	static FUnknown *createInstance(void *context) {
		return (IEditController *) new Controller();
	}

	tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
	tresult PLUGIN_API setComponentState(IBStream *state) SMTG_OVERRIDE;
#if defined(P_PITCH_BEND) || defined(P_MOD_WHEEL)
	tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id) SMTG_OVERRIDE;

	OBJ_METHODS (Controller, EditController)
	DEFINE_INTERFACES
		DEF_INTERFACE (IMidiMapping)
	END_DEFINE_INTERFACES (EditController)
	REFCOUNT_METHODS (EditController)
#endif

private:

};

#endif
