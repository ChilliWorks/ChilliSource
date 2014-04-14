//
//  State.h
//  ChilliSource
//
//  Created by Scott Downie on 21/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_STATE_STATE_H_
#define _CHILLISOURCE_CORE_STATE_STATE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/StateSystem.h>

#include <vector>

namespace ChilliSource
{
	namespace Core
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
            //--------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //--------------------------------------------
			State();
            //--------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //--------------------------------------------
			virtual ~State(){}
            //------------------------------------------
			/// @author S Downie
			///
			/// @return States scene
			//------------------------------------------
			Scene* GetScene();
            //----------------------------------------------------
            /// Searches the state systems and returns the first
            /// one that implements the named interface
            ///
            /// @author S Downie
            ///
            /// @return System of type TNamedType
            //----------------------------------------------------
            template <typename TNamedType> TNamedType* GetSystem();
            //------------------------------------------
            /// @author S Downie
			///
            /// @return Whether the state is the top of
            /// the hierarchy
            //-----------------------------------------
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
            template <typename TSystem, typename... TArgs> TSystem* CreateSystem(TArgs... in_args);
            
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
			virtual void OnUpdate(f32){};
            //-----------------------------------------
            /// Triggered each update loop at a fixed
            /// interval while the state is the active
            /// state.
            ///
            /// @author S Downie
            ///
            /// @param Fixed time since last update (Secs)
			//-----------------------------------------
			virtual void OnFixedUpdate(f32){};
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
            
			Scene* m_scene;
			
            bool m_canAddSystems;
			
			friend class StateManager;
		};
        
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TSystem, typename... TArgs> TSystem* State::CreateSystem(TArgs... in_args)
        {
            CS_ASSERT(m_canAddSystems == true, "Cannot add systems outwith the creation phase");
            
            std::unique_ptr<TSystem> newSystem = TSystem::Create(in_args...);
			TSystem* output = newSystem.get();
			if (newSystem != nullptr)
			{
				m_systems.push_back(std::move(newSystem));
			}
            return output;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TNamedType> TNamedType* State::GetSystem()
        {
            for (std::vector<StateSystemUPtr>::const_iterator it = m_systems.begin(); it != m_systems.end(); ++it)
            {
                if ((*it)->IsA(TNamedType::InterfaceID))
                {
                    return static_cast<TNamedType*>(it->get());
                }
            }
            
            CS_LOG_WARNING("State cannot find implementing systems");
            return nullptr;
        }
	}
}

#endif
