/*
 *  StateManager.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>

namespace ChilliSource
{
	namespace Core
	{
        static StateSPtr NullStatePtr;
        
		//---------------------------------------------------------
		/// Constructor
		///
		/// Default
		//---------------------------------------------------------
		StateManager::StateManager() : mpApp(nullptr), mbStartState(false)
		{
		}
        //---------------------------------------------------------
		/// Set Owning Application
		///
        /// @param Moflow application
		//---------------------------------------------------------
        void StateManager::SetOwningApplication(Application* inpApp)
        {
            mpApp = inpApp;
        }
        //---------------------------------------------------------
        /// Get Application
        ///
        /// @return Moflow application
        //---------------------------------------------------------
        Application& StateManager::GetApplication()
        {
            return *mpApp;
        }
        //---------------------------------------------------------
        /// Get Application Pointer
        ///
        /// @return Moflow application pointer
        //---------------------------------------------------------
        Application* StateManager::GetApplicationPtr()
        {
            return mpApp;
        }
		//---------------------------------------------------------
		/// Resume
		///
		/// Called by the application on receiving a resume
		/// notification
		//---------------------------------------------------------
		void StateManager::Resume()
		{
			if(!mStateHierarchy.empty()) 
			{
				mStateHierarchy.back().get()->OnResume();
			}
		}
		//---------------------------------------------------------
		/// Pause
		///
		/// Called by the application on receiving a suspend
		/// notification
		//---------------------------------------------------------
		void StateManager::Pause()
		{
			if(!mStateHierarchy.empty()) 
			{
				mStateHierarchy.back().get()->OnPause();
			}
		}
		//---------------------------------------------------------
		/// Update
		///
		/// Updates the current state with the time
		/// since last update
		///
		/// @param Time since last update
		//---------------------------------------------------------
		void StateManager::Update(f32 dt)
		{
			while(!mStateOperationQueue.empty()) 
			{
				switch(mStateOperationQueue.front().eAction) 
				{
					case StateOperationAction::k_push:
                    {
                        StateSPtr pPushed = mStateOperationQueue.front().pState;
                        StateSPtr pTop;
                        
                        if(!mStateHierarchy.empty())
                        {
                            pTop = mStateHierarchy.back();
                            CS_ASSERT(pTop != pPushed, "You cannot push a copy of a state onto itself");
                        }
                        
                        //Only the top state will have been started so this
                        //is the only one we need to stop
						if(!mbStartState && !mStateHierarchy.empty()) 
						{
                            pTop->OnStop();
						}
                        
                        //Check that the state we have pushed is not already in the hierarchy
                        bool bAlreadyInHierarchy = GetIsStateInHierarchy(pPushed.get());
                        
                        PushHierarchy(pPushed);
                        
                        if(!bAlreadyInHierarchy)
                        {
                            pPushed->OnInit();
                        }
                        
                        mbStartState = true;
						break;
                    }
					case StateOperationAction::k_popTo:
                    {
						State * pTargetState = mStateOperationQueue.front().pRawState;
						bool bTopStateStopped = false;
						while (!mStateHierarchy.empty() && (mStateHierarchy.back().get() != pTargetState))
                        {
                            StateSPtr pPopped = mStateHierarchy.back();
							
							// Stop the top state only
							if(!bTopStateStopped)
							{
								bTopStateStopped = true;
								pPopped->OnStop();
							}
							
                            if(GetNumInstancesOfStateInHierarchy(pPopped.get()) == 1)
                            {
                                pPopped->OnDestroy();
                            }
                            
                            PopHierarchy();
                            
                            // Only start the state if something has been popped
                            mbStartState = true;
						}
                        
                        break;
                    }
					case StateOperationAction::k_popToUnique:
                    {
						State * pTargetState = mStateOperationQueue.front().pRawState;
						bool bTopStateStopped = false;
						while (!mStateHierarchy.empty())
                        {
							// If top state is the target and is unique
							if(mStateHierarchy.back().get() == pTargetState && GetNumInstancesOfStateInHierarchy(pTargetState) == 1)
								break;
							
                            StateSPtr pPopped = mStateHierarchy.back();
							
							// Stop the top state only
							if(!bTopStateStopped)
							{
								bTopStateStopped = true;
								pPopped->OnStop();
							}
							
                            if(GetNumInstancesOfStateInHierarchy(pPopped.get()) == 1)
                            {
                                pPopped->OnDestroy();
                            }
                            
                            PopHierarchy();
						}
						
						mbStartState = true;
                        
                        break;
                    }
					case StateOperationAction::k_clear:
                        
                        if(!mStateHierarchy.empty())
                        {
                            StateSPtr pTop = mStateHierarchy.back();
                            pTop->OnStop();
                        }
                        
						while (!mStateHierarchy.empty())
                        {
                            StateSPtr pTop = mStateHierarchy.back();
                            
                            if(GetNumInstancesOfStateInHierarchy(pTop.get()) == 1)
                            {
                                pTop->OnDestroy();
                            }
                            
                            PopHierarchy();
						}
						break;
					case StateOperationAction::k_pop:
                    {
                        if(mStateHierarchy.empty())
                        {
                            break;
                        }
                        
                        //Only the top state will have been started so this
                        //is the only one we need to stop
                        StateSPtr pPopped = mStateHierarchy.back();
                        
						if(!mbStartState) 
						{
                            pPopped->OnStop();
						}
                        
                        if(GetNumInstancesOfStateInHierarchy(pPopped.get()) == 1)
                        {
                            pPopped->OnDestroy();
                        }
                        
                        PopHierarchy();
						
                        mbStartState = true;
						break;
                    }
					default:
						break;
				}
				mStateOperationQueue.pop_front();
			}
            
			if(!mStateHierarchy.empty()) 
			{
                //We only want to start the state we intend to finish on (i.e. pushing 3 states in a single frame)
                if(mbStartState)
                {
                    mbStartState = false;
                    mStateHierarchy.back()->OnStart();
                }
                
				mStateHierarchy.back()->OnUpdate(dt);
				mStateHierarchy.back()->GetScenePtr()->Update(dt);
			}
		}
        //---------------------------------------------------------
        /// Pop Hierarchy
        ///
        /// Manages the hierarchy list and the setting of
        /// the state as active or inactive
        //---------------------------------------------------------
        void StateManager::PopHierarchy()
        {
            bool bOldStateOwnsScene = mStateHierarchy.back()->mbOwnsScene;
            
            if(mStateHierarchy.back()->mpScene && bOldStateOwnsScene)
            {
                mStateHierarchy.back()->mpScene->BecomeInactive();
            }
            
            mStateHierarchy.pop_back();
            
            if(!mStateHierarchy.empty() && mStateHierarchy.back()->mpScene && bOldStateOwnsScene)
            {
                mStateHierarchy.back()->mpScene->BecomeActive();
            }
        }
        //---------------------------------------------------------
        /// Push Hierarchy
        ///
        /// Manages the hierarchy list and the setting of
        /// the state as active or inactive
        /// @param State to push
        //---------------------------------------------------------
        void StateManager::PushHierarchy(const StateSPtr& inpState)
        {
            if(!mStateHierarchy.empty() && mStateHierarchy.back()->mpScene && inpState->mbOwnsScene)
                mStateHierarchy.back()->mpScene->BecomeInactive();
            
            mStateHierarchy.push_back(inpState);
            
            if(mStateHierarchy.back()->mpScene && mStateHierarchy.back()->mbOwnsScene)
                mStateHierarchy.back()->mpScene->BecomeActive();
        }
        //---------------------------------------------------------
        /// Fixed Update
        ///
        /// Updates the current state with the time
        /// since last update
        ///
        /// @param Time since last update
        //---------------------------------------------------------
        void StateManager::FixedUpdate(f32 dt)
        {
            if(!mStateHierarchy.empty()) 
			{
                mStateHierarchy.back()->OnFixedUpdate(dt);
            }
        }
		//---------------------------------------------------------
		/// Destroy All
		///
		/// Remove all states from the stack and destroy each
		//---------------------------------------------------------
		void StateManager::DestroyAll()
		{
			while(!mStateHierarchy.empty())
			{
                StateSPtr pState = mStateHierarchy.back();
                mStateHierarchy.pop_back();
                
                if(!GetIsStateInHierarchy(pState.get()))
                {
                    pState->OnStop();
                    pState->OnDestroy();
				}
			}
		}
		//---------------------------------------------------------
		/// Push
		///
		/// Add this state to the hierarchy and make it the active
		/// state.
		///
		/// @param New active state
		//---------------------------------------------------------
		void StateManager::Push(const StateSPtr &inpState)
		{
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_push, inpState));
		}
		//---------------------------------------------------------
		/// Pop
		///
		/// Remove the active state from the hierarchy
		/// and resume the state beneath it. This
		/// state will now be the active state
		//---------------------------------------------------------
		void StateManager::Pop()
		{
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_pop));
		}
		//---------------------------------------------------------
		/// PopToState
		///
		/// Removes states from the hierarchy until the 
		/// specified state is topmost. The given state
		/// will become active. If the given state is now on the
		/// stack the 
		//---------------------------------------------------------
		void StateManager::PopToState(State * inpTarget)
        {
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_popTo, inpTarget));
		}
		//---------------------------------------------------------
		/// Pop To State Unique
		///
		/// Removes states from the hierarchy until the
		/// specified state is topmost and is unique in the stack.
		/// The given state will become active.
		//---------------------------------------------------------
		void StateManager::PopToStateUnique(State * inpTarget)
		{
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_popToUnique, inpTarget));
		}
		//---------------------------------------------------------
		/// ClearStack
		///
		/// Remove all states from the stack. ALL OF THEM!
		//---------------------------------------------------------
		void StateManager::ClearStack()
		{
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_clear));
		}
		//---------------------------------------------------------
		/// Get Stack Count
		///
		/// @return the number of states on the stack.
		//---------------------------------------------------------
		u32 StateManager::GetStackCount()
		{
			return mStateHierarchy.size();
		}
		//---------------------------------------------------------
		/// Change
		///
		/// Destroy the current state and set the given
		/// state to active
		///
		/// @param State to become active
		//---------------------------------------------------------
		void StateManager::Change(const StateSPtr &inpState)
		{
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_pop));
            mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_push, inpState));
		}
		//------------------------------------------------------------------
		/// Get Factory Producing
		///
		/// @return a handle to a factory that can create object of type ID
		//------------------------------------------------------------------
		ComponentFactory* StateManager::GetFactoryProducing(InterfaceIDType inInterfaceID)
		{
			return Core::ComponentFactoryDispenser::GetSingletonPtr()->GetFactoryProducing(inInterfaceID);
		}
        //---------------------------------------------------------
        /// Get all States
        ///
        /// @return all States
        //---------------------------------------------------------
        const std::vector<StateSPtr>& StateManager::GetStates() const
        {
            return mStateHierarchy;
        }
		//---------------------------------------------------------
		/// Get Active State
		///
		/// @return State on top of the hierarchy
		//---------------------------------------------------------
		const StateSPtr& StateManager::GetActiveState() const
		{
			if(!mStateHierarchy.empty())
			{
				return mStateHierarchy.back();
			}
			else
			{
				return NullStatePtr;
			}
		}
        //---------------------------------------------------------
        /// Get Num Instances of State In Hierarchy
        ///
        /// @return The number of times the state appears in the stack
        //---------------------------------------------------------
        u32 StateManager::GetNumInstancesOfStateInHierarchy(State* inpState) const
        {
            u32 udwCount = 0;
            
            for (std::vector<StateSPtr>::const_iterator it = mStateHierarchy.begin(); it != mStateHierarchy.end(); ++it)
            {
                if ((*it).get() == inpState)
                {
                    ++udwCount;
                }
            }
            
            return udwCount;
        }
        //---------------------------------------------------------
        /// Get Is State In Hierarchy
        ///
        /// @return returns true if inpcState is in state stack, otherwise false
        //---------------------------------------------------------
        bool StateManager::GetIsStateInHierarchy(State* inpState) const
        {
            bool bIsActive = false;
            
            for (std::vector<StateSPtr>::const_iterator it = mStateHierarchy.begin(); it != mStateHierarchy.end(); ++it)
            {
                if ((*it).get() == inpState)
                {
                    bIsActive = true;
                    break;
                }
            }
            
            return bIsActive;
        }
        //---------------------------------------------------------
        /// Get Is State Pending
        ///
        /// @return returns true if inpcState is in state stack, otherwise false
        //---------------------------------------------------------
        bool StateManager::GetIsStatePending(State* inpState) const
        {
            bool bIsPending = false;
            
            for (std::list<StateOperation>::const_iterator it = mStateOperationQueue.begin(); it != mStateOperationQueue.end(); ++it)
            {
                if (((*it).eAction == StateOperationAction::k_push || (*it).eAction == StateOperationAction::k_popTo) && (*it).pState.get() == inpState)
                {
                    bIsPending = true;
                    break;
                }
            }
            
            return bIsPending;
        }
        //---------------------------------------------------------
        /// Get Parent State
        ///
        /// @return If this state exists within hierarchy then returns parent state, otherwise returns StateSPtr() (nullptr)
        //---------------------------------------------------------
        const StateSPtr& StateManager::GetParentState(State* inpState) const 
        {
            StateSPtr pcState = StateSPtr();
            
            for (s32 i = mStateHierarchy.size() - 1; i >= 0; --i)
            {
                if (mStateHierarchy[i].get() == inpState)
                {
                    if (i > 0)
                    {
                        return mStateHierarchy[i - 1];
                    }
                }
            }
			
            return NullStatePtr;
        }
        //---------------------------------------------------------
        /// Get Child State
        ///
        /// @return If this state exists within hierarchy then returns child state, otherwise returns StateSPtr() (nullptr)
        //---------------------------------------------------------
        const StateSPtr& StateManager::GetChildState(State* inpState) const
        {
            for (s32 i = mStateHierarchy.size() - 1; i >= 0; --i)
            {
                if (mStateHierarchy[i].get() == inpState)
                {
                    if (i < mStateHierarchy.size() - 1)
                    {
                        return mStateHierarchy[i + 1];
                    }
                }
            }
			
            return NullStatePtr;
        }
        //---------------------------------------------------------
        /// Find State With Pointer
        ///
        /// @param Raw pointer
        /// @return Equivalent shared pointer if found in hierarchy
        //---------------------------------------------------------
        const StateSPtr& StateManager::FindStateWithPointer(State* inpState) const
        {
            for (s32 i = mStateHierarchy.size() - 1; i >= 0; --i)
            {
                if (mStateHierarchy[i].get() == inpState)
                {
                    return mStateHierarchy[i];
                }
            }
            
            return NullStatePtr;
        }
		//---------------------------------------------------------
		/// Get Active Scene Ptr
		///
		/// @return State on top of the hierarchys scene
		//---------------------------------------------------------
		Scene* StateManager::GetActiveScenePtr()
		{
			if(!mStateHierarchy.empty())
			{
				return mStateHierarchy.back()->mpScene;
			}
			else
			{
				return nullptr;
			}
		}
        //-------------------------------------------------------------------------
        /// On Notification Received
        ///
        /// Called by the notification scheduler a new notice is triggered
        /// The manager will then forward this to the state delegates
        ///
        /// @param Notification
        //-------------------------------------------------------------------------
        bool StateManager::OnNotificationReceived(Core::Notification* inpsNotification)
        {
            if(mStateHierarchy.back()->ShouldReceiveNotifications())
            {
                return mStateHierarchy.back()->OnReceiveNotification(inpsNotification);
            }
            else
            {
                 //   CS_LOG_DEBUG("StateManager state not accepting notifications. for notification "+ToString(inpsNotification->ID)+" priority "+ToString(inpsNotification->ePriority)+" type "+ToString(inpsNotification->eType));
            }
			return false;
        }
        
        void StateManager::DebugPrint(std::string instrMessage){
            CS_LOG_DEBUG("StateManager "+ToString((u32)this)+" - "+instrMessage);
            CS_LOG_DEBUG("mpApp = "+ToString((u32)mpApp));
            CS_LOG_DEBUG("mStateOperationQueue size:"+ToString(mStateOperationQueue.size()));
    
            
            CS_LOG_DEBUG("mStateHierarchy size:"+ToString(mStateHierarchy.size()));

            
            for(s32 i=0;i<mStateHierarchy.size();i++){
                CS_LOG_DEBUG("("+ToString(i)+") "+ToString((u32)mStateHierarchy[i].get())+" name:"+mStateHierarchy[i]->GetName());
            }
            
            CS_LOG_DEBUG("mStateOperationQueue size:"+ToString(mStateOperationQueue.size()));

            s32 udwCount=0;
            std::list<StateOperation>::iterator it=mStateOperationQueue.begin();
            while(it!=mStateOperationQueue.end()){
                CS_LOG_DEBUG("("+ToString(udwCount)+") eAction:"+ToString((u32)(*it).eAction)+" pState:"+ToString((u32)(*it).pState.get())+" pRawState:"+ToString((u32)(*it).pRawState));
                udwCount++;
                it++;
            }
        }
        
	}
}
