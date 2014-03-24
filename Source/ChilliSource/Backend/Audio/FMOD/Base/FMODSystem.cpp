//
//  FMODSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 17/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>

#include <ChilliSource/Backend/Audio/FMOD/Base/AudioManager.h>
#include <ChilliSource/Backend/Audio/FMOD/3D/AudioComponent.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/AudioResource.h>
#include <ChilliSource/Backend/Audio/FMOD/3D/AudioComponentFactory.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <fmodiphone.h>
#endif

namespace ChilliSource
{
	namespace FMOD
	{
        CS_DEFINE_NAMEDTYPE(FMODSystem);
        
        namespace
        {
            const u32 k_maxChannels = 32;
            
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
        
		//-------------------------------------------------------
		//-------------------------------------------------------
		FMODSystem::FMODSystem()
        : m_FMODSystem(nullptr), m_FMODEventSystem(nullptr), m_FMODEventProject(nullptr)
		{
            //TODO: These will go when all the systems are created
            m_audioManager = new AudioManager();
            m_audioComponentFactory = new AudioComponentFactory(this, m_audioManager);
            
            Core::ResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(m_audioManager);
		}
        //------------------------------------------------
        //------------------------------------------------
        void FMODSystem::OnInit()
        {
			//Create the FMOD event system
			ErrorCheck(::FMOD::EventSystem_Create(&m_FMODEventSystem));
			
			//Get the FMOD system
			ErrorCheck(m_FMODEventSystem->getSystemObject(&m_FMODSystem));
			
			//Initialise the system with the number of virtual channels
#ifdef CS_ENABLE_FMODIOSPLAYANDRECORD
            FMOD_IPHONE_EXTRADRIVERDATA driverData;
            driverData.forceSpeakerOutput = true;
            driverData.forceMixWithOthers = false;
            driverData.sessionCategory = FMOD_IPHONE_SESSIONCATEGORY_PLAYANDRECORD;
			ErrorCheck(m_FMODEventSystem->init(kudwMaxFMODChannels, FMOD_INIT_NORMAL, &driverData));
#else
            ErrorCheck(m_FMODEventSystem->init(k_maxChannels, FMOD_INIT_NORMAL, nullptr));
#endif
            
#ifdef CS_ENABLE_FMODANDROIDOPENSL
			m_FMODSystem->setOutput(FMOD_OUTPUTTYPE_OPENSL);
#else
			m_FMODSystem->setOutput(FMOD_OUTPUTTYPE_AUDIOTRACK);
#endif
			
			//Set defaults
            SetMasterEffectVolume(m_masterEffectVolume);
			SetMasterStreamVolume(m_masterStreamVolume);
			m_FMODSystem->set3DSettings(m_dopplerFactor, m_distanceFactor, m_rolloffFactor);
        }
        //-------------------------------------------------------
		//-------------------------------------------------------
		bool FMODSystem::IsA(Core::InterfaceIDType in_interfaceID) const
		{
			return
            in_interfaceID == Core::IUpdateable::InterfaceID ||
            in_interfaceID == Core::IComponentProducer::InterfaceID ||
            in_interfaceID == AudioSystem::InterfaceID ||
            in_interfaceID == FMODSystem::InterfaceID;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void FMODSystem::PlaySound(Audio::AudioComponent* in_audioComponent)
		{
			//We let FMOD manages the channels 
			::FMOD::Channel* activeChannel = nullptr;
			ErrorCheck(m_FMODSystem->playSound(FMOD_CHANNEL_FREE, std::static_pointer_cast<AudioResource>(in_audioComponent->GetAudioSource())->mpFMODSound, false, &activeChannel));
			
			//Give the sound it's channel so we can query the state
			static_cast<AudioComponent*>(in_audioComponent)->SetChannel(activeChannel);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		::FMOD::Event* FMODSystem::PlayEvent(const std::string& instrEventName)
		{
			if(m_FMODEventProject != nullptr)
			{
				::FMOD::Event * pEvent = nullptr;
				ErrorCheck(m_FMODEventProject->getEvent(instrEventName.c_str(), FMOD_EVENT_DEFAULT, &pEvent));
                
                if(pEvent)
                {
                    ErrorCheck(pEvent->start());
                }
                
				return pEvent; 
			}
			return nullptr;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void FMODSystem::Update(f32 dt)
		{
			ErrorCheck(m_FMODEventSystem->update());
		}
		//-------------------------------------------------------
        //-------------------------------------------------------
		void FMODSystem::OnMemoryWarning()
		{
			if(m_FMODEventProject!= nullptr)
			{
				m_FMODEventProject->stopAllEvents();
                
                if(m_FMODEventProject)
                {
                    ::FMOD::EventGroup* eventRelease = nullptr;
                    u32 groupID = 0;
                    
                    // get all groups and free them
                    while(m_FMODEventProject->getGroupByIndex(groupID++, false, &eventRelease) == FMOD_OK)
                    {
                        ErrorCheck(eventRelease->freeEventData(nullptr, true));
                    }
                }
			}
		}
        //------------------------------------------------
        //------------------------------------------------
        void FMODSystem::OnDestroy()
        {
            CS_SAFEDELETE(m_audioComponentFactory);
			CS_SAFEDELETE(m_audioManager);
            
            ErrorCheck(m_FMODEventSystem->release());
			m_FMODEventSystem = nullptr;
        }
	}
}

