//
//  State.h
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

#ifndef _CHILLISOURCE_CORE_STATE_STATE_H_
#define _CHILLISOURCE_CORE_STATE_STATE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/StateSystem.h>

#include <vector>

namespace ChilliSource
{
    //----------------------------------------------------------------------
    /// Manages the lifecycle of a user defined state. A state has its own
    /// scene that is rendered while the state is at the top of the stack.
    ///
    /// Applications should inherit from states and override the lifecycle
    /// delegates to load/display resources and manage game logic.
    ///
    /// States also have state systems thats lifecycles are governed by
    /// the state.
    ///
    /// @author S Downie
    //----------------------------------------------------------------------
    class State
    {
    public:
        CS_DECLARE_NOCOPY(State);
        //----------------------------------------------------
        /// Constructor
        ///
        /// @author S Downie
        //----------------------------------------------------
        State() = default;
        //----------------------------------------------------
        /// Destructor
        ///
        /// @author S Downie
        //----------------------------------------------------
        virtual ~State(){}
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @return A pointer to the main scene (that renders to screen).
        //----------------------------------------------------
        Scene* GetMainScene() noexcept;
        //----------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return A const pointer to the main scene (that renders to screen).
        //----------------------------------------------------
        const Scene* GetMainScene() const noexcept;
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @return List of pointers to the scenes (main and offscreen).
        //----------------------------------------------------
        const std::vector<Scene*>& GetScenes() const noexcept { return m_scenes; }
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @return A pointer to the UI canvas
        //----------------------------------------------------
        Canvas* GetUICanvas();
        //----------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return A const pointer to the UI canvas.
        //----------------------------------------------------
        const Canvas* GetUICanvas() const;
        //----------------------------------------------------
        /// Searches the state systems and returns the first
        /// one that implements the named interface
        ///
        /// @author S Downie
        ///
        /// @return A pointer to the system of type TNamedType
        //----------------------------------------------------
        template <typename TNamedType> TNamedType* GetSystem();
        //----------------------------------------------------
        /// Searches the state systems and returns the first
        /// one that implements the named interface
        ///
        /// @author Ian Copland
        ///
        /// @return A const pointer to the system of type
        /// TNamedType
        //----------------------------------------------------
        template <typename TNamedType> const TNamedType* GetSystem() const;
        //----------------------------------------------------
        /// Searches the state systems and returns all systems
        /// that implements the named interface
        ///
        /// @author HMcLaughlin
        ///
        /// @return A vector of pointers to the systems of type
        /// TNamedType
        //----------------------------------------------------
        template <typename TNamedType> std::vector<TNamedType*> GetSystems();
        //----------------------------------------------------
        /// Searches the state systems and returns all systems
        /// that implements the named interface
        ///
        /// @author Ian Copland
        ///
        /// @return A vector of const pointers to the systems
        /// of type TNamedType
        //----------------------------------------------------
        template <typename TNamedType> std::vector<const TNamedType*> GetSystems() const;
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @return Whether the state is the top of the
        /// hierarchy
        //----------------------------------------------------
        bool IsActiveState() const;
        
    protected:
        
        //----------------------------------------------------
        /// Factory method that ensures systems are created
        /// and added at the same time to prevent ordering
        /// issues
        ///
        /// @author S Downie
        ///
        /// @param System creation arguments
        ///
        /// @return New system owned by state
        //----------------------------------------------------
        template <typename TSystem, typename... TArgs> TSystem* CreateSystem(TArgs&&... in_args);
        
    private:

        //-----------------------------------------
        /// Triggered whenever a state is added
        /// to the state manager.
        ///
        /// (Pairs with Destroy)
        ///
        /// @author S Downie
        //-----------------------------------------
        void Init();
        //-----------------------------------------
        /// Triggered when a state is the
        /// active state in the state manager and
        /// the application/state resumes.
        ///
        /// (Pairs with Suspend)
        ///
        /// @author S Downie
        //-----------------------------------------
        void Resume();
        //-----------------------------------------
        /// Triggered when a state is the
        /// active state in the state manager and
        /// the application becomes the foreground
        /// app.
        ///
        /// (Pairs with Background)
        ///
        /// @author S Downie
        //-----------------------------------------
        void Foreground();
        //-----------------------------------------
        /// Triggered each update loop while the
        /// state is the active state.
        ///
        /// @author S Downie
        ///
        /// @param Time since last update (Secs)
        //-----------------------------------------
        void Update(f32 in_timeSinceLastUpdate);
        //-----------------------------------------
        /// Triggered each update loop at a fixed
        /// interval while the state is the active
        /// state.
        ///
        /// @author S Downie
        ///
        /// @param Fixed time since last update (Secs)
        //-----------------------------------------
        void FixedUpdate(f32 in_fixedTimeSinceLastUpdate);
        //-----------------------------------------
        /// Sends the render snapshot event onto
        /// the state, all state systems and any
        /// entities currently in the scene.
        ///
        /// @author Ian Copland
        ///
        /// @param targetType - Whether the snapshot is
        /// for the main screen or an offscreen target
        /// @param renderSnapshot - The render
        /// snapshot object which contains all
        /// snapshotted data.
        /// @param frameAllocator - Allocate
        /// any data required for rendering this frame
        /// from here
        //-----------------------------------------
        void RenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept;
        //-----------------------------------------
        /// Triggered when a state is the
        /// active state in the state manager and
        /// the application is no longer at the
        /// foreground.
        ///
        /// (Pairs with OnForeground)
        ///
        /// @author S Downie
        //-----------------------------------------
        void Background();
        //-----------------------------------------
        /// Triggered when a state is the
        /// active state in the state manager and
        /// the application/state suspends.
        ///
        /// (Pairs with OnResume)
        ///
        /// @author S Downie
        //-----------------------------------------
        void Suspend();
        //-----------------------------------------
        /// Triggered whenever a state is removed
        /// from the state manager.
        ///
        /// (Pairs with Init)
        ///
        /// @author S Downie
        //-----------------------------------------
        void Destroy();
        //------------------------------------------------
        /// Called when the application receives a memory
        /// warning. This is filtered to all states
        ///
        /// @author S Downie
        //------------------------------------------------
        void MemoryWarning();
        //------------------------------------------------------
        /// The users state should override this to add
        /// desired systems. Systems can only be added in this
        /// method.
        ///
        /// @author S Downie
        //------------------------------------------------------
        virtual void CreateSystems(){};
        //-----------------------------------------
        /// Triggered whenever a state is added
        /// to the state manager.
        ///
        /// (Pairs with OnDestroy)
        ///
        /// @author S Downie
        //-----------------------------------------
        virtual void OnInit(){};
        //-----------------------------------------
        /// Triggered when a state is the
        /// active state in the state manager and
        /// the application/state resumes.
        ///
        /// (Pairs with OnSuspend)
        ///
        /// @author S Downie
        //-----------------------------------------
        virtual void OnResume(){};
        //-----------------------------------------
        /// Triggered when a state is the
        /// active state in the state manager and
        /// the application becomes the foreground
        /// app.
        ///
        /// (Pairs with OnBackground)
        ///
        /// @author S Downie
        //-----------------------------------------
        virtual void OnForeground(){};
        //-----------------------------------------
        /// Triggered each update loop while the
        /// state is the active state.
        ///
        /// @author S Downie
        ///
        /// @param Time since last update (Secs)
        //-----------------------------------------
        virtual void OnUpdate(f32 in_deltaTime){};
        //-----------------------------------------
        /// Triggered each update loop at a fixed
        /// interval while the state is the active
        /// state.
        ///
        /// @author S Downie
        ///
        /// @param Fixed time since last update (Secs)
        //-----------------------------------------
        virtual void OnFixedUpdate(f32 in_deltaTime){};
        
        /// The render snapshot event can be implemented by a state to allow it to
        /// snapshot any data which pertains to the renderer.
        ///
        /// @author Ian Copland
        ///
        /// @param targetType
        ///     Whether the snapshot is for the main screen or offscreen
        /// @param renderSnapshot
        ///     The render snapshot object which contains all snapshotted data.
        /// @param frameAllocator
        ///     Use this to allocate any memory required for this frame
        ///
        virtual void OnRenderSnapshot(TargetType targetType, class RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept {};
        
        //-----------------------------------------
        /// Triggered when a state is the
        /// active state in the state manager and
        /// the application is no longer at the
        /// foreground.
        ///
        /// (Pairs with OnForeground)
        ///
        /// @author S Downie
        //-----------------------------------------
        virtual void OnBackground(){};
        //-----------------------------------------
        /// Triggered when a state is the
        /// active state in the state manager and
        /// the application/state suspends.
        ///
        /// (Pairs with OnResume)
        ///
        /// @author S Downie
        //-----------------------------------------
        virtual void OnSuspend(){};
        //-----------------------------------------
        /// Triggered whenever a state is removed
        /// from the state manager.
        ///
        /// (Pairs with OnInit)
        ///
        /// @author S Downie
        //-----------------------------------------
        virtual void OnDestroy(){};
        
    private:
        
        std::vector<StateSystemUPtr> m_systems;
        
        std::vector<Scene*> m_scenes;
        Canvas* m_canvas = nullptr;
        bool m_canAddSystems = false;
        
        friend class StateManager;
    };
    
    //----------------------------------------------------
    //----------------------------------------------------
    template <typename TSystem, typename... TArgs> TSystem* State::CreateSystem(TArgs&&... in_args)
    {
        CS_ASSERT(m_canAddSystems == true, "Cannot add systems outwith the creation phase");
        
        std::unique_ptr<TSystem> newSystem = TSystem::Create(std::forward<TArgs>(in_args)...);
        TSystem* output = newSystem.get();
        if (newSystem != nullptr)
        {
            newSystem->SetState(this);
            m_systems.push_back(std::move(newSystem));
        }
        return output;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    template <typename TNamedType> TNamedType* State::GetSystem()
    {
        for (const auto& system : m_systems)
        {
            if (system->IsA<TNamedType>() == true)
            {
                return static_cast<TNamedType*>(system.get());
            }
        }
        
        return nullptr;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    template <typename TNamedType> const TNamedType* State::GetSystem() const
    {
        for (const auto& system : m_systems)
        {
            if (system->IsA<TNamedType>() == true)
            {
                return static_cast<TNamedType*>(system.get());
            }
        }
        
        return nullptr;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    template <typename TNamedType> std::vector<TNamedType*> State::GetSystems()
    {
        std::vector<TNamedType*> systems;
        
        for (const auto& stateSystem : m_systems)
        {
            if (stateSystem->IsA(TNamedType::InterfaceID))
            {
                systems.push_back(static_cast<TNamedType*>(stateSystem.get()));
            }
        }
        
        return systems;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    template <typename TNamedType> std::vector<const TNamedType*> State::GetSystems() const
    {
        std::vector<const TNamedType*> systems;
        
        for (const auto& stateSystem : m_systems)
        {
            if (stateSystem->IsA(TNamedType::InterfaceID))
            {
                systems.push_back(static_cast<TNamedType*>(stateSystem.get()));
            }
        }
        
        return systems;
    }
}

#endif
