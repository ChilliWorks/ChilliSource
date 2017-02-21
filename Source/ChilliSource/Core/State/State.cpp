//
//  State.cpp
//  ChilliSource
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
#include <ChilliSource/Rendering/Base/TargetType.h>
#include <ChilliSource/Rendering/Target/TargetGroup.h>
#include <ChilliSource/UI/Base/Canvas.h>

namespace ChilliSource
{
    //-----------------------------------------
    //-----------------------------------------
    void State::Init()
    {
        m_canAddSystems = true;
        
        //Create the default systems and main scene
        CreateSystem<Scene>(nullptr);
        m_canvas = CreateSystem<Canvas>();
        CreateSystem<GestureSystem>();
        
        //create user systems.
        CreateSystems();
        
        m_canAddSystems = false;
        
        for(auto& system : m_systems)
        {
           if(system->IsA<Scene>())
           {
               m_scenes.push_back(static_cast<Scene*>(system.get()));
           }
        }
        
        for(auto& system : m_systems)
        {
            system->OnInit();
        }
        
        OnInit();
    }
    //-----------------------------------------
    //-----------------------------------------
    void State::Resume()
    {
        for(auto& system : m_systems)
        {
            system->OnResume();
        }
        
        for(auto scene : m_scenes)
        {
            scene->ResumeEntities();
        }
        
        OnResume();
    }
    //-----------------------------------------
    //-----------------------------------------
    void State::Foreground()
    {
        for(auto& system : m_systems)
        {
            system->OnForeground();
        }
        
        for(auto scene : m_scenes)
        {
            scene->ForegroundEntities();
        }
        
        OnForeground();
    }
    //-----------------------------------------
    //-----------------------------------------
    void State::Update(f32 in_timeSinceLastUpdate)
    {
        for(auto& system : m_systems)
        {
            system->OnUpdate(in_timeSinceLastUpdate);
        }
        
        for(auto scene : m_scenes)
        {
            scene->UpdateEntities(in_timeSinceLastUpdate);
        }
        
        OnUpdate(in_timeSinceLastUpdate);
    }
    //-----------------------------------------
    //-----------------------------------------
    void State::FixedUpdate(f32 in_fixedTimeSinceLastUpdate)
    {
        for(auto& system : m_systems)
        {
            system->OnFixedUpdate(in_fixedTimeSinceLastUpdate);
        }
        
        for(auto scene : m_scenes)
        {
            scene->FixedUpdateEntities(in_fixedTimeSinceLastUpdate);
        }
        
        OnFixedUpdate(in_fixedTimeSinceLastUpdate);
    }
    //-----------------------------------------
    //-----------------------------------------
    void State::RenderSnapshot(TargetType targetType, class RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        for(auto& system : m_systems)
        {
            system->OnRenderSnapshot(targetType, renderSnapshot, frameAllocator);
        }
        
        OnRenderSnapshot(targetType, renderSnapshot, frameAllocator);
    }
    //-----------------------------------------
    //-----------------------------------------
    void State::Background()
    {
        OnBackground();
        
        for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); ++it)
        {
            (*it)->BackgroundEntities();
        }
        
        for (auto it = m_systems.rbegin(); it != m_systems.rend(); ++it)
        {
            (*it)->OnBackground();
        }
    }
    //-----------------------------------------
    //-----------------------------------------
    void State::Suspend()
    {
        OnSuspend();
        
        for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); ++it)
        {
            (*it)->SuspendEntities();
        }
        
        for(auto it = m_systems.rbegin(); it != m_systems.rend(); ++it)
        {
            (*it)->OnSuspend();
        }
    }
    //-----------------------------------------
    //-----------------------------------------
    void State::Destroy()
    {
        OnDestroy();
        
        for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); ++it)
        {
            (*it)->RemoveAllEntities();
        }
        
        for(auto it = m_systems.rbegin(); it != m_systems.rend(); ++it)
        {
            (*it)->OnDestroy();
        }
    }
    //------------------------------------------------
    //------------------------------------------------
    void State::MemoryWarning()
    {
        for(auto& system : m_systems)
        {
            system->OnMemoryWarning();
        }
    }
    //------------------------------------------
    //------------------------------------------
    Scene* State::GetMainScene() noexcept
    {
        return m_scenes[0];
    }
    //------------------------------------------
    //------------------------------------------
    const Scene* State::GetMainScene() const noexcept
    {
        return m_scenes[0];
    }
    //------------------------------------------
    //------------------------------------------
    Canvas* State::GetUICanvas()
    {
        return m_canvas;
    }
    //------------------------------------------
    //------------------------------------------
    const Canvas* State::GetUICanvas() const
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
