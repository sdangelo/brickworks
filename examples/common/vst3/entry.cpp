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

#include "plugin.h"
#include "controller.h"

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "public.sdk/source/main/pluginfactory.h"

BEGIN_FACTORY_DEF(COMPANY_NAME, COMPANY_WEBSITE, COMPANY_MAILTO)
DEF_CLASS2(
	INLINE_UID(PLUGIN_GUID_1, PLUGIN_GUID_2, PLUGIN_GUID_3, PLUGIN_GUID_4),	// GUID
	PClassInfo::kManyInstances,						// cardinality
	kVstAudioEffectClass,							// VST component class
	PLUGIN_NAME,								// plugin name
	Vst::kDistributable,							// plugin and controller can work on different computers
	PLUGIN_SUBCATEGORY,							// VST subcategory
	PLUGIN_VERSION,								// plugin version
	kVstVersionString,							// VST version
	Plugin::createInstance							// instatiation function pointer
)

DEF_CLASS2(
	INLINE_UID(CTRL_GUID_1, CTRL_GUID_2, CTRL_GUID_3, CTRL_GUID_4),		// GUID
	PClassInfo::kManyInstances,						// cardinality
	kVstComponentControllerClass,						// VST component class
	PLUGIN_NAME "Controller",						// controller name
	0,									// unused
	"",									// unused
	PLUGIN_VERSION,								// plugin version
	kVstVersionString,							// VST version
	Controller::createInstance						// instatiation function pointer
)

END_FACTORY
