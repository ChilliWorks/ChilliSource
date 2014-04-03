//
//  StateManager.cpp
//  ChilliSource
//
//  Created by Scott Downie on 23/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/State/StateManager.h>

#include <ChilliSource/Core/State/State.h>

namespace ChilliSource
{
	namespace Core
	{
        namespace
        {
            const StateSPtr NullStatePtr;
        }
        
        CS_DEFINE_NAMEDTYPE(StateManager);
        
        //---------------------------------------------------------
        //---------------------------------------------------------
        StateManagerUPtr StateManager::Create()
        {
            return StateManagerUPtr(new StateManager());
        }
		//---------------------------------------------------------
		//---------------------------------------------------------
		StateManager::StateManager()
        : m_isStartState(false)
		{
		}
        //---------------------------------------------------------
        //---------------------------------------------------------
        bool StateManager::IsA(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == StateManager::InterfaceID;
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::PopHierarchy()
        {
            m_states.pop_back();
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::PushHierarchy(const StateSPtr& in_state)
        {
            m_states.push_back(in_state);
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::OnResume()
        {
            if(!m_states.empty())
			{
				m_states.back()->Resume();
			}
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::OnForeground()
        {
            if(!m_states.empty())
			{
				m_states.back()->Foreground();
			}
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::OnUpdate(f32 in_timeSinceLastUpdate)
        {
            while(!m_operations.empty())
			{
				switch(m_operations.front().m_action)
				{
					case StateOperationAction::k_push:
                    {
                        StateSPtr pushed = m_operations.front().m_state;
                        StateSPtr top;
                        
                        if(!m_states.empty())
                        {
                            top = m_states.back();
                            CS_ASSERT(top != pushed, "You cannot push a copy of a state onto itself");
                        }
                        
                        //Only the top state will have been started so this
                        //is the only one we need to stop
						if(!m_isStartState && !m_states.empty())
						{
                            top->Background();
                            top->Suspend();
						}
                        
                        //Check that the state we have pushed is not already in the hierarchy
                        bool bAlreadyInHierarchy = DoesStateExist(pushed.get());
                        
                        PushHierarchy(pushed);
                        
                        if(!bAlreadyInHierarchy)
                        {
                            pushed->Init();
                        }
                        
                        m_isStartState = true;
						break;
                    }
					case StateOperationAction::k_popTo:
                    {
						State * targetState = m_operations.front().m_rawState;
						bool isTopStateStopped = false;
						while (!m_states.empty() && (m_states.back().get() != targetState))
                        {
                            StateSPtr popped = m_states.back();
							
							// Stop the top state only
							if(!isTopStateStopped)
							{
								isTopStateStopped = true;
                                popped->Background();
                                popped->Suspend();
							}
							
                            if(GetNumInstancesOfState(popped.get()) == 1)
                            {
                                popped->Destroy();
                            }
                            
                            PopHierarchy();
                            
                            // Only start the state if something has been popped
                            m_isStartState = true;
						}
                        
                        break;
                    }
					case StateOperationAction::k_popToUnique:
                    {
						State * targetState = m_operations.front().m_rawState;
						bool isTopStateStopped = false;
						while (!m_states.empty())
                        {
							// If top state is the target and is unique
							if(m_states.back().get() == targetState && GetNumInstancesOfState(targetState) == 1)
								break;
							
                            StateSPtr popped = m_states.back();
							
							// Stop the top state only
							if(!isTopStateStopped)
							{
								isTopStateStopped = true;
                                popped->Background();
                                popped->Suspend();
							}
							
                            if(GetNumInstancesOfState(popped.get()) == 1)
                            {
                                popped->Destroy();
                            }
                            
                            PopHierarchy();
						}
						
						m_isStartState = true;
                        
                        break;
                    }
					case StateOperationAction::k_clear:
                        
                        if(!m_states.empty())
                        {
                            StateSPtr top = m_states.back();
                            top->Background();
                            top->Suspend();
                        }
                        
						while (!m_states.empty())
                        {
                            StateSPtr top = m_states.back();
                            
                            if(GetNumInstancesOfState(top.get()) == 1)
                            {
                                top->Destroy();
                            }
                            
                            PopHierarchy();
						}
						break;
					case StateOperationAction::k_pop:
                    {
                        if(m_states.empty())
                        {
                            break;
                        }
                        
                        //Only the top state will have been started so this
                        //is the only one we need to stop
                        StateSPtr popped = m_states.back();
                        
						if(!m_isStartState)
						{
                            popped->Background();
                            popped->Suspend();
						}
                        
                        if(GetNumInstancesOfState(popped.get()) == 1)
                        {
                            popped->Destroy();
                        }
                        
                        PopHierarchy();
						
                        m_isStartState = true;
						break;
                    }
					default:
						break;
				}
				m_operations.pop_front();
			}
            
			if(!m_states.empty())
			{
                //We only want to start the state we intend to finish on (i.e. pushing 3 states in a single frame)
                if(m_isStartState)
                {
                    m_isStartState = false;
                    m_states.back()->Resume();
                    m_states.back()->Foreground();
                }
                
				m_states.back()->Update(in_timeSinceLastUpdate);
			}

        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::OnFixedUpdate(f32 in_fixedTimeSinceLastUpdate)
        {
            if(!m_states.empty())
			{
                m_states.back()->FixedUpdate(in_fixedTimeSinceLastUpdate);
            }
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::OnBackground()
        {
            if(!m_states.empty())
			{
				m_states.back()->Background();
			}
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::OnSuspend()
        {
            if(!m_states.empty())
			{
				m_states.back()->OnSuspend();
			}
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::DestroyStates()
        {
            while(!m_states.empty())
			{
                StateSPtr state = m_states.back();
                m_states.pop_back();
                
                if(!DoesStateExist(state.get()))
                {
                    state->Destroy();
				}
			}
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void StateManager::OnMemoryWarning()
        {
            for(std::vector<StateSPtr>::const_iterator it = m_states.begin(); it != m_states.end(); ++it)
            {
                (*it)->MemoryWarning();
            }
        }
		//---------------------------------------------------------
		//---------------------------------------------------------
		void StateManager::Push(const StateSPtr &in_state)
		{
			m_operations.push_back(StateOperation(StateOperationAction::k_push, in_state));
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		void StateManager::Pop()
		{
			m_operations.push_back(StateOperation(StateOperationAction::k_pop));
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		void StateManager::PopToState(State* in_target)
        {
			m_operations.push_back(StateOperation(StateOperationAction::k_popTo, in_target));
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		void StateManager::PopToStateUnique(State * in_target)
		{
			m_operations.push_back(StateOperation(StateOperationAction::k_popToUnique, in_target));
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		void StateManager::Clear()
		{
			m_operations.push_back(StateOperation(StateOperationAction::k_clear));
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		u32 StateManager::GetNumStates() const
		{
			return m_states.size();
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		void StateManager::Change(const StateSPtr& in_state)
		{
			m_operations.push_back(StateOperation(StateOperationAction::k_pop));
            m_operations.push_back(StateOperation(StateOperationAction::k_push, in_state));
		}
        //---------------------------------------------------------
        //---------------------------------------------------------
        const std::vector<StateSPtr>& StateManager::GetStates() const
        {
            return m_states;
        }
		//---------------------------------------------------------
		//---------------------------------------------------------
		const StateSPtr& StateManager::GetActiveState() const
		{
			if(!m_states.empty())
			{
				return m_states.back();
			}
			else
			{
				return NullStatePtr;
			}
		}
        //---------------------------------------------------------
        //---------------------------------------------------------
        u32 StateManager::GetNumInstancesOfState(State* in_state) const
        {
            u32 numInstances = 0;
            
            for (std::vector<StateSPtr>::const_iterator it = m_states.begin(); it != m_states.end(); ++it)
            {
                if ((*it).get() == in_state)
                {
                    ++numInstances;
                }
            }
            
            return numInstances;
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        bool StateManager::DoesStateExist(State* in_state) const
        {
            for (std::vector<StateSPtr>::const_iterator it = m_states.begin(); it != m_states.end(); ++it)
            {
                if ((*it).get() == in_state)
                {
                    return true;
                }
            }
            
            return false;
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        bool StateManager::IsStatePending(State* in_state) const
        {
            for (std::list<StateOperation>::const_iterator it = m_operations.begin(); it != m_operations.end(); ++it)
            {
                if (((*it).m_action == StateOperationAction::k_push || (*it).m_action == StateOperationAction::k_popTo) && (*it).m_state.get() == in_state)
                {
                    return true;
                }
            }
            
            return false;
        }
	}
}
