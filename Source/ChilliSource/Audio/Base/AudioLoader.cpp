/*
 * File: AudioLoader.cpp
 * Date: 16/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/Base/AudioLoader.h>
#include <ChilliSource/Audio/Base/AudioResource.h>

#ifdef CS_TARGETAUDIO_FMOD
#include <ChilliSource/Backend/Audio/FMOD/Base/AudioLoader.h>
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
		//-------------------------------------------------------------------------
		/// Is A
		///
		/// @param Interface to compare
		/// @return Whether the object implements the given interface
		//-------------------------------------------------------------------------
		bool AudioLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		///
		/// @param Type to compare
		/// @return Whether the object can create a resource of given type
		//----------------------------------------------------------------------------
		bool AudioLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Audio::AudioResource::InterfaceID);
		}
		//-------------------------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------------------------
		AudioLoader::~AudioLoader() 
		{

		}
	}
}