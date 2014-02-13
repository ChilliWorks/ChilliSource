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


#ifndef _MO_FLO_AUDIO_WINDOWS_FMOD_FMOD_AUDIO_LOADER_H_
#define _MO_FLO_AUDIO_WINDOWS_FMOD_FMOD_AUDIO_LOADER_H_

#include <ChilliSource/Audio/AudioLoader.h>
#include <ChilliSource/Platform/Windows/FMOD/FMODSystemWindows.h>

namespace moFlo
{
	namespace WindowsPlatform
	{
        class CFMODSystem;
        
		class CFMODAudioLoader : public Audio::CAudioLoader
		{
		public:
			
			CFMODAudioLoader(Audio::IAudioSystem* inpFMODSystem);
	
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
			bool CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			
			//----------------------------------------------------------------------------
			/// Stream Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Success
			//----------------------------------------------------------------------------
			bool StreamResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			
			//----------------------------------------------------------------------------
			/// Create Audio Listener
			///
			/// @return Audio listener
			//----------------------------------------------------------------------------
			Audio::AudioListenerPtr CreateAudioListener();
			
		private:
			
			CFMODSystem* mpFMODSystem;
			std::string mstrBundlePath;
			std::string mstrDocumentsPath;
            std::string mstrDLCPath;
		};
	}
}

#endif

