//
//  StateManager.h
//  ChilliSource
//
//  Created by Scott Downie on 23/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_STATE_STATEMANAGER_H_
#define _CHILLISOURCE_CORE_STATE_STATEMANAGER_H_

#include <ChilliSource/ChilliSource.h>

#include <list>
#include <vector>

namespace ChilliSource
{
	namespace Core
	{
		//---------------------------------------------------------
        /// Manages the state stack ensuring that only one state
        /// is active at a time. Funnels application lifecycle
        /// events to the active state.
        ///
        /// @author S Downie
		//---------------------------------------------------------
		class StateManager
		{
		public:
			StateManager();
			
			//---------------------------------------------------------
            /// @author T Kane
            ///
			/// @return all States
			//---------------------------------------------------------
            const std::vector<StateSPtr>& GetStates() const;
			//---------------------------------------------------------
            /// @author N Tanda
            ///
			/// @return State on top of the hierarchy or null
			//---------------------------------------------------------
			const StateSPtr& GetActiveState() const;
			//---------------------------------------------------------
            /// @author N Tanda
            ///
			/// @return returns true if state is in stack, otherwise false
			//---------------------------------------------------------
            bool DoesStateExist(State* in_state) const;
            //---------------------------------------------------------
            /// @author N Tanda
            ///
            /// @return The number of times the state appears in the stack
            //---------------------------------------------------------
            u32 GetNumInstancesOfState(State* in_state) const;
			//---------------------------------------------------------
            /// @author N Tanda
            ///
			/// @return returns true if state is pending to be pushed
            /// in state operation stack
			//---------------------------------------------------------
            bool IsStatePending(State* in_state) const;
			//---------------------------------------------------------
			/// Add this state to the stack and make it the active
			/// state.
            ///
            /// @author S Downie
			///
			/// @param New active state
			//---------------------------------------------------------
			void Push(const StateSPtr& in_state);
			//---------------------------------------------------------
			/// Remove the active state from the hierarchy
			/// and resume the state beneath it. This
			/// state will now be the active state
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void Pop();
			//---------------------------------------------------------
			/// Removes states from the hierarchy until the
			/// specified state is topmost. The given state
			/// will become active. Will pop to empty if state doesn't
            /// exist.
            ///
            /// @author N Tanda
            ///
            /// @param Target state
			//---------------------------------------------------------
			void PopToState(State* in_target);
			//---------------------------------------------------------
			/// Removes states from the hierarchy until the
			/// specified state is topmost and is unique in the stack.
			/// The given state will become active. Will pop to empty
            /// if state doesn't exist.
            ///
            /// @author N Tanda
            ///
            /// @param Target state
			//---------------------------------------------------------
			void PopToStateUnique(State* in_target);
			//---------------------------------------------------------
			/// Remove all states from the stack.
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void Clear();
			//---------------------------------------------------------
            /// @author N Tanda
            ///
			/// @return the number of states on the stack.
			//---------------------------------------------------------
			u32 GetNumStates() const;
			//---------------------------------------------------------
			/// Change
			///
			/// Pop the current state and push the given state
            ///
            /// @author S Downie
			///
			/// @param State to become active
			//---------------------------------------------------------
			void Change(const StateSPtr& in_state);
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
				StateOperation(StateOperationAction in_action) : m_action(in_action), m_rawState(nullptr){}
				StateOperation(StateOperationAction in_action, const StateSPtr& in_state) : m_action(in_action), m_state(in_state), m_rawState(nullptr){}
				StateOperation(StateOperationAction in_action, State* in_state) : m_action(in_action), m_rawState(in_state){}

				StateOperationAction m_action;
				StateSPtr m_state;
				State * m_rawState;
			};
			
		private:
		
			std::vector<StateSPtr> m_states;
			std::list<StateOperation> m_operations;
            
            bool m_isStartState;
			
			friend class Application;
		};
	}
}


#endif
