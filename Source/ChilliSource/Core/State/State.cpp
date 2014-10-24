//
//  State.cpp
//  Chilli Source
//  Created by Scott Downie on 21/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Core/State/State.h>

#include <ChilliSource/Core/Scene/Scene.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Input/Gesture/GestureSystem.h>
#include <ChilliSource/UI/Base/Canvas.h>

namespace ChilliSource
{
	namespace Core 
	{
        //-----------------------------------------
        //-----------------------------------------
        void State::Init()
        {
            m_canAddSystems = true;
            
            //Create the default systems
            m_scene = CreateSystem<Scene>();
            m_canvas = CreateSystem<UI::Canvas>();
            CreateSystem<CSInput::GestureSystem>();
            
            //create user systems.
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
        void State::Resume()
        {
            u32 numSystems = m_systems.size();
            for(u32 i=0; i<numSystems; ++i)
            {
                m_systems[i]->OnResume();
            }
            
            m_scene->ResumeEntities();
            
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
            
            m_scene->ForegroundEntities();
            
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
            
            m_scene->UpdateEntities(in_timeSinceLastUpdate);
            
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
            
            m_scene->FixedUpdateEntities(in_fixedTimeSinceLastUpdate);
            
            OnFixedUpdate(in_fixedTimeSinceLastUpdate);
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Background()
        {
            OnBackground();
            
            m_scene->BackgroundEntities();
            
            s32 numSystems = m_systems.size();
            for(s32 i=numSystems-1; i>=0; --i)
            {
                m_systems[i]->OnBackground();
            }
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Suspend()
        {
            OnSuspend();
            
            m_scene->SuspendEntities();
            
            s32 numSystems = m_systems.size();
            for(s32 i=numSystems-1; i>=0; --i)
            {
                m_systems[i]->OnSuspend();
            }
        }
        //-----------------------------------------
        //-----------------------------------------
        void State::Destroy()
        {
            OnDestroy();
            
            m_scene->RemoveAllEntities();
            
            s32 numSystems = m_systems.size();
            for(s32 i=numSystems-1; i>=0; --i)
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
        const Scene* State::GetScene() const
        {
            return m_scene;
        }
        //------------------------------------------
        //------------------------------------------
        UI::Canvas* State::GetUICanvas()
        {
            return m_canvas;
        }
        //------------------------------------------
        //------------------------------------------
        const UI::Canvas* State::GetUICanvas() const
        {
            return m_canvas;
        }
        //------------------------------------------
        //------------------------------------------
		bool State::IsActiveState() const 
        { 
			return Application::Get()->GetStateManager()->GetActiveState().get() == this;
		}
	}
}
