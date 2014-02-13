/*
 * File: FMODAudioLoader.cpp
 * Date: 16/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Platform/Android/FMOD/FMODAudioLoader.h>
#include <ChilliSource/Platform/Android/FMOD/FMODAudioResource.h>
#include <ChilliSource/Core/Application.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		const std::string kstrWAVExtension("wav");
		const std::string kstrAACExtension("aac");
		const std::string kstrOGGExtension("ogg");
		const std::string kstrAIFFExtension("aiff");
		
		//-------------------------------------------------------------------------
		/// Constructor
		///
		/// Register this object as a model provider
		//-------------------------------------------------------------------------
		CFMODAudioLoader::CFMODAudioLoader(Audio::IAudioSystem* inpFMODSystem)
		: CAudioLoader(inpFMODSystem), mpFMODSystem(static_cast<CFMODSystem*>(inpFMODSystem))
		{
			mstrCacheDirectory = moFlo::Core::CApplication::GetFileSystemPtr()->GetStorageLocationDirectory(Core::SL_CACHE);
			mstrDLCDirectory = moFlo::Core::CApplication::GetFileSystemPtr()->GetStorageLocationDirectory(Core::SL_DLC);
			mstrSaveDataDirectory = moFlo::Core::CApplication::GetFileSystemPtr()->GetStorageLocationDirectory(Core::SL_SAVEDATA);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		///
		/// @param Type to compare
		/// @param Extension to compare
		/// @return Whether the object can create a resource with the given extension
		//----------------------------------------------------------------------------
		bool CFMODAudioLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return	(inExtension == kstrWAVExtension) || (inExtension == kstrAACExtension) || (inExtension == kstrOGGExtension) ||
					(inExtension == kstrAIFFExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		///
		/// We load sound effects
		/// @param Filename
		/// @return Handle to resource of local type
		//----------------------------------------------------------------------------
		bool CFMODAudioLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			switch (ineStorageLocation)
			{
			case Core::SL_PACKAGE:
				WARNING_LOG("Android fmod cannot load or stream audio from the package so trying cache instead.");
				mpFMODSystem->Create3DSound(mstrCacheDirectory + inFilePath, (Audio::IAudioResource*)outpResource.get());
				break;
			case Core::SL_CACHE:
				mpFMODSystem->Create3DSound(mstrCacheDirectory + inFilePath, (Audio::IAudioResource*)outpResource.get());
				break;
			case Core::SL_DLC:
				WARNING_LOG("Android fmod cannot fall back on audio contained in the package DLC Directory.");
				mpFMODSystem->Create3DSound(mstrDLCDirectory + inFilePath, (Audio::IAudioResource*)outpResource.get());
				break;
			case Core::SL_SAVEDATA:
				mpFMODSystem->Create3DSound(mstrSaveDataDirectory + inFilePath, (Audio::IAudioResource*)outpResource.get());
				break;
			default:
				ERROR_LOG("Android fmod does not have access to this storage location.");
				return false;
            }

			return true;
		}
		//----------------------------------------------------------------------------
		/// Stream Resource From File
		///
		/// @param Filename
		/// @return Handle to resource of local type
		//----------------------------------------------------------------------------
		bool CFMODAudioLoader::StreamResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			switch (ineStorageLocation)
			{
			case Core::SL_PACKAGE:
				WARNING_LOG("Android fmod cannot load or stream audio from the package so trying cache instead.");
				mpFMODSystem->CreateStream(mstrCacheDirectory + inFilePath, (Audio::IAudioResource*)outpResource.get());
				break;
			case Core::SL_CACHE:
				mpFMODSystem->CreateStream(mstrCacheDirectory + inFilePath, (Audio::IAudioResource*)outpResource.get());
				break;
			case Core::SL_DLC:
				WARNING_LOG("Android fmod cannot fall back on audio contained in the package DLC Directory.");
				mpFMODSystem->CreateStream(mstrDLCDirectory + inFilePath, (Audio::IAudioResource*)outpResource.get());
				break;
			case Core::SL_SAVEDATA:
				mpFMODSystem->CreateStream(mstrSaveDataDirectory + inFilePath, (Audio::IAudioResource*)outpResource.get());
				break;
			default:
				ERROR_LOG("Android fmod does not have access to this storage location.");
				return false;
            }

			return true;
		}
		//----------------------------------------------------------------------------
		/// Create Audio Listener
		///
		/// @return Audio listener
		//----------------------------------------------------------------------------
		Audio::AudioListenerPtr CFMODAudioLoader::CreateAudioListener()
		{
			return mpFMODSystem->CreateAudioListener();
		}
	}
}
