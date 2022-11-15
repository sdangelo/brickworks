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

class Controller : EditController {
public:
	static FUnknown *createInstance(void *context) {
		return (IEditController *) new Controller();
	}

	tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
	tresult PLUGIN_API setComponentState(IBStream *state) SMTG_OVERRIDE;

private:

};

#endif
