//
//  StateManager.h
//  Chilli Source
//  Created by Scott Downie on 23/09/2010.
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

#ifndef _CHILLISOURCE_CORE_STATE_STATEMANAGER_H_
#define _CHILLISOURCE_CORE_STATE_STATEMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

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
		class StateManager : public AppSystem
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(StateManager);
            
            //---------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New state manager instance
            //---------------------------------------------------------
            static StateManagerUPtr Create();
			//---------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID
            ///
            /// @return Whether the system implements the given interface
            //---------------------------------------------------------
            bool IsA(InterfaceIDType in_interfaceId) const;
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
            
		private:
            friend class Application;
            
            //---------------------------------------------------------
            /// Private to enforce use of factory method
            ///
            /// @author S Downie
            //---------------------------------------------------------
			StateManager();
            //---------------------------------------------------------
            /// Triggered by the application when the system is
            /// resumed. Forwards to states.
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void ResumeStates();
            //---------------------------------------------------------
            /// Triggered by the application when the system is
            /// pushed to the foreground. Forwards to states.
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void ForegroundStates();
            //---------------------------------------------------------
			/// Updates the current state with the time
			/// since last update
            ///
            /// @author S Downie
			///
			/// @param Time since last update in seconds
			//---------------------------------------------------------
			void UpdateStates(f32 in_timeSinceLastUpdate);
            //---------------------------------------------------------
			/// Updates the current state with the time
			/// since last update. This is called at a fixed interval
            ///
            /// @author S Downie
			///
			/// @param Fixed time since last update in seconds
			//---------------------------------------------------------
			void FixedUpdateStates(f32 in_fixedTimeSinceLastUpdate);
            //---------------------------------------------------------
            /// Triggered by the application when the system is
            /// pushed to the background. Forwards to states.
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void BackgroundStates();
            //---------------------------------------------------------
            /// Triggered by the application when the app is suspended.
            /// Forwards to states.
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void SuspendStates();
            //---------------------------------------------------------
            /// Triggered by the application when the system is
            /// about to be destroyed to ensure systems exist when
            /// states are destroyed.
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void DestroyStates();
            //---------------------------------------------------------
            /// Triggered by the application when it receives
            /// a low memory warning. Forwards to states.
            ///
            /// @author S Downie
			//---------------------------------------------------------
			void MemoryWarningStates();
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
			bool m_statesActive = false;
			bool m_statesForegrounded = false;
		};
	}
}


#endif
