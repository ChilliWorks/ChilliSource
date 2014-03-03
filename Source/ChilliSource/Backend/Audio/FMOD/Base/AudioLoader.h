/*
 * File: FMODAudioLoader.h
 * Date: 16/11/2010 2010 
 * Description: Concrete loader that uses FMOD to load sound files
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */


#ifndef _MO_FLO_AUDIO_FMOD_FMOD_AUDIO_LOADER_H_
#define _MO_FLO_AUDIO_FMOD_FMOD_AUDIO_LOADER_H_

#include <ChilliSource/Audio/Base/AudioLoader.h>
#include <ChilliSource/Backend/Audio/FMOD/ForwardDeclarations.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>

namespace ChilliSource
{
	namespace FMOD
	{
        class FMODSystem;
        
		class AudioLoader : public Audio::AudioLoader
		{
		public:
			
			AudioLoader(Audio::AudioSystem* inpFMODSystem);
	
			//----------------------------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Type to compare
			/// @param Extension to compare
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
			
			//----------------------------------------------------------------------------
			/// Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Success
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource);
			
			//----------------------------------------------------------------------------
			/// Stream Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Success
			//----------------------------------------------------------------------------
			bool StreamResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource);
			
			//----------------------------------------------------------------------------
			/// Create Audio Listener
			///
			/// @return Audio listener
			//----------------------------------------------------------------------------
			Audio::AudioListenerSPtr CreateAudioListener();
			
		private:
#ifdef CS_TARGETPLATFORM_ANDROID
            std::string m_DLCDirectory;
            std::string m_cacheDirectory;
            std::string m_saveDataDirectory;
#endif
			
			FMODSystem* mpFMODSystem;
		};
	}
}

#endif

