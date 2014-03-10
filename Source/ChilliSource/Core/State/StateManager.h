/*
 *  StateManager.h
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_STATE_MANAGER_H_
#define _MO_FLO_STATE_MANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/State/State.h>

#include <list>
#include <queue>
#include <vector>

namespace ChilliSource
{
	namespace Core
	{
		//---------------------------------------------------------
		/// Description:
		///
		/// Handles the calling of active state functions
		/// and the changes or layering of states.
		///
		/// Also creates and owns the scene graph for the app.
		//---------------------------------------------------------
		class StateManager
		{
		public:
			StateManager();
			
            //---------------------------------------------------------
            /// Set Owning Application
            ///
            /// @param Moflow application
            //---------------------------------------------------------
            void SetOwningApplication(Application* inpApp);
            //---------------------------------------------------------
            /// Get Application
            ///
            /// @return Moflow application
            //---------------------------------------------------------
			Application& GetApplication();
            //---------------------------------------------------------
            /// Get Application Pointer
            ///
            /// @return Moflow application pointer
            //---------------------------------------------------------
			Application* GetApplicationPtr();
			//---------------------------------------------------------
			/// Get all States
			///
			/// @return all States
			//---------------------------------------------------------
            const std::vector<StateSPtr>& GetStates() const;
			//---------------------------------------------------------
			/// Get Active State
			///
			/// @return State on top of the hierarchy
			//---------------------------------------------------------
			const StateSPtr& GetActiveState() const;
			//---------------------------------------------------------
            /// Get Is State In Hierarchy
			///
			/// @return returns true if inpcState is in state stack, otherwise false
			//---------------------------------------------------------
            bool GetIsStateInHierarchy(State* inpState) const;
            //---------------------------------------------------------
            /// Get Num Instances of State In Hierarchy
            ///
            /// @return The number of times the state appears in the stack
            //---------------------------------------------------------
            u32 GetNumInstancesOfStateInHierarchy(State* inpState) const;
			//---------------------------------------------------------
            /// Get Is State Pending
			///
			/// @return returns true if inpcState is pending to be pushed in state operation stack
			//---------------------------------------------------------
            bool GetIsStatePending(State* inpState) const;
			//---------------------------------------------------------
            /// Get Child State
			///
			/// @return If this state exists within hierarchy then returns child state, otherwise returns StateSPtr() (nullptr)
			//---------------------------------------------------------
            const StateSPtr& GetChildState(State* inpState) const;
			//---------------------------------------------------------
            /// Get Parent State
			///
			/// @return If this state exists within hierarchy then returns parent state, otherwise returns StateSPtr() (nullptr)
			//---------------------------------------------------------
            const StateSPtr& GetParentState(State* inpState) const;
            //---------------------------------------------------------
            /// Find State With Pointer
            ///
            /// @param Raw pointer
            /// @return Equivalent shared pointer if found in hierarchy
            //---------------------------------------------------------
            const StateSPtr& FindStateWithPointer(State* inpState) const;
			//---------------------------------------------------------
			/// Get Active Scene Ptr
			///
			/// @return State on top of the hierarchys scene
			//---------------------------------------------------------
			Scene* GetActiveScenePtr();
			//---------------------------------------------------------
			/// Push
			///
			/// Add this state to the hierarchy and make it the active
			/// state.
			///
			/// @param New active state
			//---------------------------------------------------------
			void Push(const StateSPtr &inpState);
			//---------------------------------------------------------
			/// Pop
			///
			/// Remove the active state from the hierarchy
			/// and resume the state beneath it. This
			/// state will now be the active state
			//---------------------------------------------------------
			void Pop();
			//---------------------------------------------------------
			/// Pop To State
			///
			/// Removes states from the hierarchy until the
			/// specified state is topmost. The given state
			/// will become active. If the given state is now on the
			/// stack the
			//---------------------------------------------------------
			void PopToState(State * inpTarget);
			//---------------------------------------------------------
			/// Pop To State Unique
			///
			/// Removes states from the hierarchy until the
			/// specified state is topmost and is unique in the stack.
			/// The given state will become active.
			//---------------------------------------------------------
			void PopToStateUnique(State * inpTarget);
			//---------------------------------------------------------
			/// ClearStack
			///
			/// Remove all states from the stack. ALL OF THEM!
			//---------------------------------------------------------
			void ClearStack();
			//---------------------------------------------------------
			/// Get Stack Count
			///
			/// @return the number of states on the stack.
			//---------------------------------------------------------
			u32 GetStackCount();
			//---------------------------------------------------------
			/// Change
			///
			/// Destroy the current state and set the given
			/// state to active
			///
			/// @param State to become active
			//---------------------------------------------------------
			void Change(const StateSPtr &inpState);
			//------------------------------------------------------------------
			/// Get Factory Producing
			///
			/// @return a handle to a factory that can create object of type ID
			//------------------------------------------------------------------
			ComponentFactory* GetFactoryProducing(InterfaceIDType inInterfaceID);
			//-------------------------------------------------------------------------
			/// On Notification Received
			///
			/// Called by the notification scheduler a new notice is triggered
			/// The manager will then forward this to the state delegates
			///
			/// @param Notification
			//-------------------------------------------------------------------------
			bool OnNotificationReceived(Notification* inpsNotification);
			//---------------------------------------------------------
			/// Resume
			///
			/// Called by the application on receiving a resume
			/// notification
			//---------------------------------------------------------
			void Resume();
            //---------------------------------------------------------
			/// Fixed Update
			///
			/// Updates the current state with the time
			/// since last update
			///
			/// @param Time since last update
			//---------------------------------------------------------
			void FixedUpdate(f32 dt);
			//---------------------------------------------------------
			/// Pause
			///
			/// Called by the application on receiving a suspend
			/// notification
			//---------------------------------------------------------
			void Pause();
            //---------------------------------------------------------
			/// Update
			///
			/// Updates the current state with the time
			/// since last update
			///
			/// @param Time since last update
			//---------------------------------------------------------
			void Update(f32 dt);
            
		private:

			//---------------------------------------------------------
			/// Init
			///
			/// Initialises the current state
			//---------------------------------------------------------
			void Init();
			//---------------------------------------------------------
			/// Destroy
			///
			/// Calls destroy on the current state
			/// and removes it from the state hierarchy
			//---------------------------------------------------------
			void Destroy();
			//---------------------------------------------------------
			/// Destroy All
			///
			/// Remove all states from the stack and destroy each
			//---------------------------------------------------------
			void DestroyAll();
            //---------------------------------------------------------
            /// Pop Hierarchy
            ///
            /// Manages the hierarchy list and the setting of
            /// the state as active or inactive
            //---------------------------------------------------------
            void PopHierarchy();
            //---------------------------------------------------------
            /// Push Hierarchy
            ///
            /// Manages the hierarchy list and the setting of
            /// the state as active or inactive
            ///
            /// @param State to push
            //---------------------------------------------------------
            void PushHierarchy(const StateSPtr& inpState);
			
			enum class StateOperationAction
            {
                k_push,
                k_pop,
                k_popTo,
                k_popToUnique,
                k_clear
            };
            
			struct StateOperation
			{
				StateOperation(StateOperationAction ineAction) : eAction(ineAction), pRawState(nullptr){}
				StateOperation(StateOperationAction ineAction, const StateSPtr& inpState) : eAction(ineAction), pState(inpState), pRawState(nullptr){}
				StateOperation(StateOperationAction ineAction, State * inpState) : eAction(ineAction), pRawState(inpState){}

				StateOperationAction eAction;
				StateSPtr pState;
				State * pRawState;
			};
			
		private:
		
			Application* mpApp;
			
			std::vector<StateSPtr> mStateHierarchy;              //The alive states. A state can be pushed onto the stack to become the active state. Yet the other states will not be destroyed. Hierarchy is now a vector, so that states within can be inspected, since std::stack is a little opaque
			std::list<StateOperation> mStateOperationQueue;
            
            bool mbStartState;
			
			friend class Application;
			friend class IApplicationDelegates;
		};
	}
}


#endif
