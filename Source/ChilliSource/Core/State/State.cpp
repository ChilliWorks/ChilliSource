//
//  State.cpp
//  ChilliSource
//
//  Created by Scott Downie on 21/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//


#include <ChilliSource/Core/State/State.h>

#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
	namespace Core 
	{
		//-----------------------------------------
		//-----------------------------------------
		State::State()
        : m_canAddSystems(false)
        , m_scene(nullptr)
		{

		}
        //-----------------------------------------
        //-----------------------------------------
        void State::Init()
        {
            m_canAddSystems = true;
            //States will always have a scene by default
            m_scene = CreateSystem<Scene>(Application::Get()->GetInputSystem());
            CreateSystems();
            m_canAddSystems = false;
            
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnInit();
            }
            
            OnInit();
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Start()
        {
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnStart();
            }
            
            OnStart();
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Resume()
        {
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnResume();
            }
            
            OnResume();
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Foreground()
        {
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnForeground();
            }
            
            OnForeground();
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Update(f32 in_timeSinceLastUpdate)
        {
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnUpdate(in_timeSinceLastUpdate);
            }
            
            OnUpdate(in_timeSinceLastUpdate);
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::FixedUpdate(f32 in_fixedTimeSinceLastUpdate)
        {
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnFixedUpdate(in_fixedTimeSinceLastUpdate);
            }
            
            OnFixedUpdate(in_fixedTimeSinceLastUpdate);
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Background()
        {
            OnBackground();
            
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnBackground();
            }
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Suspend()
        {
            OnSuspend();
            
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnSuspend();
            }
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Stop()
        {
            OnStop();
            
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnStop();
            }
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Destroy()
        {
            OnDestroy();
            
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnDestroy();
            }
        }
        //------------------------------------------------
        //------------------------------------------------
        void State::MemoryWarning()
        {
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnMemoryWarning();
            }
        }
		//------------------------------------------
		//------------------------------------------
		Scene* State::GetScene()
		{
			return m_scene;
		}
        //------------------------------------------
        //------------------------------------------
		bool State::IsActiveState() const 
        { 
			return Application::Get()->GetStateManager()->GetActiveState().get() == this;
		}
	}
}
