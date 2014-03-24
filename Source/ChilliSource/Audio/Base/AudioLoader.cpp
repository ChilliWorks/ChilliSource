//
//  AudioLoader.cpp
//  Chilli Source
//
//  Created by Scott Downie on 16/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Audio/Base/AudioLoader.h>
#include <ChilliSource/Audio/Base/AudioResource.h>

#ifdef CS_TARGETAUDIO_FMOD
#include <ChilliSource/Backend/Audio/FMOD/Base/AudioLoader.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>
#endif

namespace ChilliSource
{
	namespace Audio
	{
        //-------------------------------------------------------
        //-------------------------------------------------------
        AudioLoaderUPtr AudioLoader::Create(AudioSystem* in_system)
        {
#ifdef CS_TARGETAUDIO_FMOD
            //TODO: Revert the dependencies so that the loader doesn't delegate the loading to the system
            return AudioLoaderUPtr(new FMOD::AudioLoader(static_cast<FMOD::FMODSystem*>(in_system)));
#endif
            
            return nullptr;
        }
	}
}