/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licenses for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licensing:
*
*   http://www.hise.audio/
*
*   HISE is based on the JUCE library,
*   which must be separately licensed for cloused source applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

#ifndef HI_SAMPLER_INCLUDED
#define HI_SAMPLER_INCLUDED

#include "sampler/MonolithAudioFormat.h"
#include "sampler/StreamingSampler.h"

#include "sampler/dywapitchtrack/dywapitchtrack.h"
#include "sampler/PitchDetection.h"

#include "sampler/ModulatorSamplerData.h"
#include "sampler/ModulatorSamplerSound.h"
#include "sampler/ModulatorSamplerVoice.h"
#include "sampler/ModulatorSampler.h"

#if USE_BACKEND

#include "sampler/SampleImporter.h"
#include "sampler/SfzImporter.h"
#include "sampler/MachFiveImporter.h"

#include "sampler/components/FileNamePartComponent.h"
#include "sampler/components/FileNameImporterDialog.h"
#include "sampler/components/FileImportDialog.h"
#include "sampler/components/SfzGroupSelectorComponent.h"
#include "sampler/components/SampleEditorComponents.h"
#include "sampler/components/SamplerSettings.h"
#include "sampler/components/ValueSettingComponent.h"
#include "sampler/components/SamplerToolbar.h"
#include "sampler/components/SampleEditor.h"
#include "sampler/components/SampleMapEditor.h"
#include "sampler/components/SamplerTable.h"
#include "sampler/components/SampleEditHandler.h"
#include "sampler/components/SamplerBody.h"

#endif

#endif   // HI_SAMPLER_INCLUDED
