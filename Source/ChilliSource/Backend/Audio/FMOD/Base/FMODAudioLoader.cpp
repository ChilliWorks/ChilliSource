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

#include <ChilliSource/Backend/Audio/FMOD/Base/FMODAudioLoader.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODAudioResource.h>
#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>

#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
	namespace FMOD
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
		CFMODAudioLoader::CFMODAudioLoader(Audio::AudioSystem* inpFMODSystem) 
		: AudioLoader(inpFMODSystem), mpFMODSystem(static_cast<CFMODSystem*>(inpFMODSystem))
		{
#ifdef CS_TARGETPLATFORM_ANDROID
            m_cacheDirectory = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(Core::StorageLocation::k_cache);
			m_DLCDirectory = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(Core::StorageLocation::k_DLC);
			m_saveDataDirectory = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(Core::StorageLocation::k_saveData);
#endif
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
        /// @param The storage location to load from
		/// @param Filename
		/// @param Out: Resource object
		/// @return Success
		//----------------------------------------------------------------------------
		bool CFMODAudioLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)  
		{
#ifdef CS_TARGETPLATFORM_ANDROID
            switch (ineStorageLocation)
			{
                case Core::StorageLocation::k_package:
                    CS_LOG_WARNING("Android fmod cannot load or stream audio from the package so trying cache instead.");
                    mpFMODSystem->Create3DSound(m_cacheDirectory + inFilePath, (Audio::AudioResource*)outpResource.get());
                    break;
                case Core::StorageLocation::k_cache:
                    mpFMODSystem->Create3DSound(m_cacheDirectory + inFilePath, (Audio::AudioResource*)outpResource.get());
                    break;
                case Core::StorageLocation::k_DLC:
                    CS_LOG_WARNING("Android fmod cannot fall back on audio contained in the package DLC Directory.");
                    mpFMODSystem->Create3DSound(m_DLCDirectory + inFilePath, (Audio::AudioResource*)outpResource.get());
                    break;
                case Core::StorageLocation::k_saveData:
                    mpFMODSystem->Create3DSound(m_saveDataDirectory + inFilePath, (Audio::AudioResource*)outpResource.get());
                    break;
                default:
                    CS_LOG_ERROR("Android fmod does not have access to this storage location.");
                    return false;
            }
            
			return true;
#else
            std::string strFilePath;
            static_cast<iOS::CFileSystem*>(Core::Application::GetFileSystemPtr())->GetBestPathToFile(ineStorageLocation, inFilePath, strFilePath);
            
            if(strFilePath.empty())
            {
                CS_LOG_ERROR("Cannot load audio from the given Storage Location.");
                return false;
            }
			
            mpFMODSystem->Create3DSound(strFilePath, (Audio::AudioResource*)outpResource.get());
			return true;
#endif
		}
		//----------------------------------------------------------------------------
		/// Stream Resource From File
		///
        /// @param The storage location to load from
		/// @param Filename
		/// @param Out: Resource object
		/// @return Success
		//----------------------------------------------------------------------------
		bool CFMODAudioLoader::StreamResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
            switch (ineStorageLocation)
			{
                case Core::StorageLocation::k_package:
                    CS_LOG_WARNING("Android fmod cannot load or stream audio from the package so trying cache instead.");
                    mpFMODSystem->CreateStream(m_cacheDirectory + inFilePath, (Audio::AudioResource*)outpResource.get());
                    break;
                case Core::StorageLocation::k_cache:
                    mpFMODSystem->CreateStream(m_cacheDirectory + inFilePath, (Audio::AudioResource*)outpResource.get());
                    break;
                case Core::StorageLocation::k_DLC:
                    CS_LOG_WARNING("Android fmod cannot fall back on audio contained in the package DLC Directory.");
                    mpFMODSystem->CreateStream(m_DLCDirectory + inFilePath, (Audio::AudioResource*)outpResource.get());
                    break;
                case Core::StorageLocation::k_saveData:
                    mpFMODSystem->CreateStream(m_saveDataDirectory + inFilePath, (Audio::AudioResource*)outpResource.get());
                    break;
                default:
                    CS_LOG_ERROR("Android fmod does not have access to this storage location.");
                    return false;
            }
            
			return true;
#else
            std::string strFilePath;
            static_cast<iOS::CFileSystem*>(Core::Application::GetFileSystemPtr())->GetBestPathToFile(ineStorageLocation, inFilePath, strFilePath);
            
            if(strFilePath.empty())
            {
                CS_LOG_ERROR("Cannot load audio from the given Storage Location.");
                return false;
            }
			
			
            mpFMODSystem->CreateStream(strFilePath, (Audio::AudioResource*)outpResource.get());
			return true;
#endif
		}
		//----------------------------------------------------------------------------
		/// Create Audio Listener
		///
		/// @return Audio listener
		//----------------------------------------------------------------------------
		Audio::AudioListenerSPtr CFMODAudioLoader::CreateAudioListener()
		{
			return mpFMODSystem->CreateAudioListener();
		}
	}
}
