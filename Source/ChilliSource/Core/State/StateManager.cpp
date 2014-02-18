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

namespace moFlo
{
	namespace Core
	{
        static StatePtr NullStatePtr;
        
		//---------------------------------------------------------
		/// Constructor
		///
		/// Default
		//---------------------------------------------------------
		CStateManager::CStateManager() : mpApp(NULL), mbStartState(false)
		{
		}
        //---------------------------------------------------------
		/// Set Owning Application
		///
        /// @param Moflow application
		//---------------------------------------------------------
        void CStateManager::SetOwningApplication(CApplication* inpApp)
        {
            mpApp = inpApp;
        }
        //---------------------------------------------------------
        /// Get Application
        ///
        /// @return Moflow application
        //---------------------------------------------------------
        CApplication& CStateManager::GetApplication()
        {
            return *mpApp;
        }
        //---------------------------------------------------------
        /// Get Application Pointer
        ///
        /// @return Moflow application pointer
        //---------------------------------------------------------
        CApplication* CStateManager::GetApplicationPtr()
        {
            return mpApp;
        }
		//---------------------------------------------------------
		/// Resume
		///
		/// Called by the application on receiving a resume
		/// notification
		//---------------------------------------------------------
		void CStateManager::Resume()
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
		void CStateManager::Pause()
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
		void CStateManager::Update(f32 dt)
		{
			while(!mStateOperationQueue.empty()) 
			{
				switch(mStateOperationQueue.front().eAction) 
				{
					case StateOperationAction::k_push:
                    {
                        StatePtr pPushed = mStateOperationQueue.front().pState;
                        StatePtr pTop;
                        
                        if(!mStateHierarchy.empty())
                        {
                            pTop = mStateHierarchy.back();
                            MOFLOW_ASSERT(pTop != pPushed, "You cannot push a copy of a state onto itself");
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
						IState * pTargetState = mStateOperationQueue.front().pRawState;
						bool bTopStateStopped = false;
						while (!mStateHierarchy.empty() && (mStateHierarchy.back().get() != pTargetState))
                        {
                            StatePtr pPopped = mStateHierarchy.back();
							
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
						IState * pTargetState = mStateOperationQueue.front().pRawState;
						bool bTopStateStopped = false;
						while (!mStateHierarchy.empty())
                        {
							// If top state is the target and is unique
							if(mStateHierarchy.back().get() == pTargetState && GetNumInstancesOfStateInHierarchy(pTargetState) == 1)
								break;
							
                            StatePtr pPopped = mStateHierarchy.back();
							
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
                            StatePtr pTop = mStateHierarchy.back();
                            pTop->OnStop();
                        }
                        
						while (!mStateHierarchy.empty())
                        {
                            StatePtr pTop = mStateHierarchy.back();
                            
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
                        StatePtr pPopped = mStateHierarchy.back();
                        
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
        void CStateManager::PopHierarchy()
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
        void CStateManager::PushHierarchy(const StatePtr& inpState)
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
        void CStateManager::FixedUpdate(f32 dt)
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
		void CStateManager::DestroyAll()
		{
			while(!mStateHierarchy.empty())
			{
                StatePtr pState = mStateHierarchy.back();
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
		void CStateManager::Push(const StatePtr &inpState)
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
		void CStateManager::Pop()
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
		void CStateManager::PopToState(IState * inpTarget)
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
		void CStateManager::PopToStateUnique(IState * inpTarget)
		{
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_popToUnique, inpTarget));
		}
		//---------------------------------------------------------
		/// ClearStack
		///
		/// Remove all states from the stack. ALL OF THEM!
		//---------------------------------------------------------
		void CStateManager::ClearStack()
		{
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_clear));
		}
		//---------------------------------------------------------
		/// Get Stack Count
		///
		/// @return the number of states on the stack.
		//---------------------------------------------------------
		u32 CStateManager::GetStackCount()
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
		void CStateManager::Change(const StatePtr &inpState)
		{
			mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_pop));
            mStateOperationQueue.push_back(StateOperation(StateOperationAction::k_push, inpState));
		}
		//------------------------------------------------------------------
		/// Get Factory Producing
		///
		/// @return a handle to a factory that can create object of type ID
		//------------------------------------------------------------------
		IComponentFactory* CStateManager::GetFactoryProducing(InterfaceIDType inInterfaceID)
		{
			return Core::CComponentFactoryDispenser::GetSingletonPtr()->GetFactoryProducing(inInterfaceID);
		}
        //---------------------------------------------------------
        /// Get all States
        ///
        /// @return all States
        //---------------------------------------------------------
        const DYNAMIC_ARRAY<StatePtr>& CStateManager::GetStates() const
        {
            return mStateHierarchy;
        }
		//---------------------------------------------------------
		/// Get Active State
		///
		/// @return State on top of the hierarchy
		//---------------------------------------------------------
		const StatePtr& CStateManager::GetActiveState() const
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
        u32 CStateManager::GetNumInstancesOfStateInHierarchy(IState* inpState) const
        {
            u32 udwCount = 0;
            
            for (DYNAMIC_ARRAY<StatePtr>::const_iterator it = mStateHierarchy.begin(); it != mStateHierarchy.end(); ++it)
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
        bool CStateManager::GetIsStateInHierarchy(IState* inpState) const
        {
            bool bIsActive = false;
            
            for (DYNAMIC_ARRAY<StatePtr>::const_iterator it = mStateHierarchy.begin(); it != mStateHierarchy.end(); ++it)
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
        bool CStateManager::GetIsStatePending(IState* inpState) const
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
        /// @return If this state exists within hierarchy then returns parent state, otherwise returns StatePtr() (NULL)
        //---------------------------------------------------------
        const StatePtr& CStateManager::GetParentState(IState* inpState) const 
        {
            StatePtr pcState = StatePtr();
            
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
        /// @return If this state exists within hierarchy then returns child state, otherwise returns StatePtr() (NULL)
        //---------------------------------------------------------
        const StatePtr& CStateManager::GetChildState(IState* inpState) const
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
        const StatePtr& CStateManager::FindStateWithPointer(IState* inpState) const
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
		CScene* CStateManager::GetActiveScenePtr()
		{
			if(!mStateHierarchy.empty())
			{
				return mStateHierarchy.back()->mpScene;
			}
			else
			{
				return NULL;
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
        bool CStateManager::OnNotificationReceived(moFlo::Notification* inpsNotification)
        {
            if(mStateHierarchy.back()->ShouldReceiveNotifications())
            {
                return mStateHierarchy.back()->OnReceiveNotification(inpsNotification);
            }
            else
            {
                 //   DEBUG_LOG("StateManager state not accepting notifications. for notification "+STRING_CAST(inpsNotification->ID)+" priority "+STRING_CAST(inpsNotification->ePriority)+" type "+STRING_CAST(inpsNotification->eType));
            }
			return false;
        }
        
        void CStateManager::DebugPrint(std::string instrMessage){
            DEBUG_LOG("CStateManager "+CStringConverter::ToString((u32)this)+" - "+instrMessage);
            DEBUG_LOG("mpApp = "+CStringConverter::ToString((u32)mpApp));
            DEBUG_LOG("mStateOperationQueue size:"+CStringConverter::ToString(mStateOperationQueue.size()));
    
            
            DEBUG_LOG("mStateHierarchy size:"+CStringConverter::ToString(mStateHierarchy.size()));

            
            for(s32 i=0;i<mStateHierarchy.size();i++){
                DEBUG_LOG("("+CStringConverter::ToString(i)+") "+CStringConverter::ToString((u32)mStateHierarchy[i].get())+" name:"+mStateHierarchy[i]->GetName());
            }
            
            DEBUG_LOG("mStateOperationQueue size:"+CStringConverter::ToString(mStateOperationQueue.size()));

            s32 udwCount=0;
            std::list<StateOperation>::iterator it=mStateOperationQueue.begin();
            while(it!=mStateOperationQueue.end()){
                DEBUG_LOG("("+CStringConverter::ToString(udwCount)+") eAction:"+CStringConverter::ToString((u32)(*it).eAction)+" pState:"+CStringConverter::ToString((u32)(*it).pState.get())+" pRawState:"+CStringConverter::ToString((u32)(*it).pRawState));
                udwCount++;
                it++;
            }
        }
        
	}
}
