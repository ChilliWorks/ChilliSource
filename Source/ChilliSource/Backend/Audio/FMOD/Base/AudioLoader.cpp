//
//  AudioLoader.cpp
//  Chilli Source
//
//  Created by Scott Downie on 16/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Audio/FMOD/Base/AudioLoader.h>

#include <ChilliSource/Backend/Audio/FMOD/3D/AudioListener.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/AudioResource.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace FMOD
	{
        namespace
        {
            const std::string k_WAVExtension("wav");
            const std::string k_AACExtension("aac");
            const std::string k_OGGExtension("ogg");
            const std::string k_AIFFExtension("aiff");
            
            //-------------------------------------------------------
            /// Log any FMOD errors and exit
            ///
            /// @author S Downie
            ///
            /// @param Result of FMOD function return
            //-------------------------------------------------------
            void ErrorCheck(FMOD_RESULT in_result)
            {
#ifdef CS_ENABLE_DEBUG
                if(in_result != FMOD_OK)
                {
                    CS_LOG_FATAL("FMOD error: " + std::string(FMOD_ErrorString(in_result)));
                }
#endif
            }
        }
		
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		AudioLoader::AudioLoader(FMOD::FMODSystem* in_system)
		: m_FMODSystem(in_system)
		{

		}
        //-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		bool AudioLoader::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == ResourceProviderOld::InterfaceID || in_interfaceId == Audio::AudioLoader::InterfaceID || in_interfaceId == AudioLoader::InterfaceID);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool AudioLoader::CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == Audio::AudioResource::InterfaceID || in_interfaceId == AudioResource::InterfaceID);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool AudioLoader::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
		{
			return
            (in_extension == k_WAVExtension) ||
            (in_extension == k_AACExtension) ||
            (in_extension == k_OGGExtension) ||
            (in_extension == k_AIFFExtension);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool AudioLoader::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::ResourceOldSPtr& outpResource)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
            std::string strFilePath;
            Core::FileSystem* fileSystem = Core::Application::Get()->GetFileSystem();
            
            switch (in_storageLocation)
			{
                case Core::StorageLocation::k_package:
                    CS_LOG_WARNING("Android fmod cannot load or stream audio from the package so trying cache instead.");
                    strFilePath = fileSystem->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_cache) + in_filePath;
                    break;
                case Core::StorageLocation::k_cache:
                    strFilePath = fileSystem->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_cache) + in_filePath;
                    break;
                case Core::StorageLocation::k_DLC:
                    CS_LOG_WARNING("Android fmod cannot fall back on audio contained in the package DLC Directory.");
                    strFilePath = fileSystem->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_filePath;
                    break;
                case Core::StorageLocation::k_saveData:
                    strFilePath = fileSystem->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_saveData) + in_filePath;
                    break;
                default:
                    CS_LOG_ERROR("Android fmod does not have access to this storage location.");
                    return false;
            }
            
            ErrorCheck(m_FMODSystem->GetFMODSystem()->createSound(strFilePath.c_str(), FMOD_SOFTWARE|FMOD_3D, nullptr, &static_cast<AudioResource*>(outpResource.get())->mpFMODSound));
			return true;
#else
            std::string strFilePath = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToFile(in_storageLocation, in_filePath);
            
            if(strFilePath.empty())
            {
                CS_LOG_ERROR("Cannot load audio from the given Storage Location.");
                return false;
            }
			
            ErrorCheck(m_FMODSystem->GetFMODSystem()->createSound(strFilePath.c_str(), FMOD_SOFTWARE|FMOD_3D, nullptr, &static_cast<AudioResource*>(outpResource.get())->mpFMODSound));
			return true;
#endif
		}
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        bool AudioLoader::AsyncCreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource)
        {
            CS_LOG_FATAL("AudioLoader::AsyncCreateResourceFromFile - Not yet implemented");
            return false;
        }
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool AudioLoader::StreamResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::ResourceOldSPtr& outpResource)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
            std::string strFilePath;
            Core::FileSystem* fileSystem = Core::Application::Get()->GetFileSystem();
            
            switch (in_storageLocation)
			{
                case Core::StorageLocation::k_package:
                    CS_LOG_WARNING("Android fmod cannot load or stream audio from the package so trying cache instead.");
                    strFilePath = fileSystem->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_cache) + in_filePath;
                    break;
                case Core::StorageLocation::k_cache:
                    strFilePath = fileSystem->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_cache) + in_filePath;
                    break;
                case Core::StorageLocation::k_DLC:
                    CS_LOG_WARNING("Android fmod cannot fall back on audio contained in the package DLC Directory.");
                    strFilePath = fileSystem->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_filePath;
                    break;
                case Core::StorageLocation::k_saveData:
                    strFilePath = fileSystem->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_saveData) + in_filePath;
                    break;
                default:
                    CS_LOG_ERROR("Android fmod does not have access to this storage location.");
                    return false;
            }
            
            ErrorCheck(m_FMODSystem->GetFMODSystem()->createStream(strFilePath.c_str(), FMOD_SOFTWARE|FMOD_LOOP_NORMAL, nullptr, &static_cast<AudioResource*>(outpResource.get())->mpFMODSound));
			return true;
#else
            std::string strFilePath = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToFile(in_storageLocation, in_filePath);
            
            if(strFilePath.empty())
            {
                CS_LOG_ERROR("Cannot load audio from the given Storage Location.");
                return false;
            }
			
            ErrorCheck(m_FMODSystem->GetFMODSystem()->createStream(strFilePath.c_str(), FMOD_SOFTWARE|FMOD_LOOP_NORMAL, nullptr, &static_cast<AudioResource*>(outpResource.get())->mpFMODSound));
			return true;
#endif
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		Audio::AudioListenerUPtr AudioLoader::CreateAudioListener()
		{
			return Audio::AudioListenerUPtr(new AudioListener(m_FMODSystem->GetFMODSystem()));
		}
        //-------------------------------------------------------
		//-------------------------------------------------------
		void AudioLoader::LoadEventData(Core::StorageLocation in_storageLocation, const std::string& in_filePath)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			CS_ASSERT(in_storageLocation != Core::StorageLocation::k_package, "FMOD Android cannot load from package");
#endif
            
            std::string strFilePath = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToFile(in_storageLocation, in_filePath);
            ::FMOD::EventProject* project = m_FMODSystem->GetFMODEventProject();
            ErrorCheck(m_FMODSystem->GetFMODEventSystem()->load(strFilePath.c_str(), nullptr, &project));
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void AudioLoader::UnloadEventData(const std::string& in_groupName)
		{
            if(m_FMODSystem->GetFMODEventProject())
            {
                ::FMOD::EventGroup * pcEventRelease = nullptr;
                
                // get groups and free it
                if(m_FMODSystem->GetFMODEventProject()->getGroup(in_groupName.c_str(), false, &pcEventRelease) == FMOD_OK)
                {
                    ErrorCheck(pcEventRelease->freeEventData(nullptr, true));
                }
            }
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void AudioLoader::PreloadEventGroup(const std::string& in_groupName)
		{
			if(m_FMODSystem->GetFMODEventProject() != nullptr)
			{
				::FMOD::EventGroup * pPreloadGroup = nullptr;
				m_FMODSystem->GetFMODEventProject()->getGroup(in_groupName.c_str(), true, &pPreloadGroup);
			}
		}
	}
}
