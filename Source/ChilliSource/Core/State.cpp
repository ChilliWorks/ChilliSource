/*
 *  IState.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 07/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */


#include <moFlo/Core/State.h>
#include <moFlo/Core/StateManager.h>
#include <moFlo/Core/Application.h>

namespace moFlo
{
	namespace Core 
	{
		//-----------------------------------------
		/// Constructor
		///
		/// Create a new scene for this state
		//-----------------------------------------
		IState::IState(CScene* inpScene) : mbOwnsScene(true)
		{
            mpStateMgr = CApplication::GetStateManagerPtr();
            
			if(!inpScene)
			{
				mpScene = new CScene(mpStateMgr->GetApplicationPtr()->GetInputSystemPtr());
			}
			else
			{
				mbOwnsScene = false;
				mpScene = inpScene;
			}
		}
		//-----------------------------------------
		/// Get Application
		///
		/// Handy accessors for the main application
		/// @return Application handle
		//-----------------------------------------
		CApplication& IState::GetApplication()
		{
			return mpStateMgr->GetApplication();
		}
		//-----------------------------------------
		/// Get Application
		///
		/// Handy accessors for the main application
		/// @return Application handle
		//-----------------------------------------
		CApplication* IState::GetApplicationPtr()
		{
			return mpStateMgr->GetApplicationPtr();
		}
		//------------------------------------------
		/// Get Scene
		///
		/// @return Scene to present modally
		//------------------------------------------
		CScene* IState::GetScenePtr()
		{
			return mpScene;
		}
		//------------------------------------------
		/// Get State Manager
		///
		/// @return Pointer to state manager
		//------------------------------------------
		CStateManager* IState::GetStateManagerPtr() const 
		{ 
			return mpStateMgr; 
		}
        //------------------------------------------
        /// Is Active State
        ///
        /// @return Whether the state is the top of 
        /// the hierarchy
        //------------------------------------------
		bool IState::IsActiveState() const 
        { 
			return mpStateMgr->GetActiveState().get() == this;
		}
        //-----------------------------------------
        /// Should Receive Notifications
        ///
        /// Delegate call used to decide whether
        /// the state should be notified of any
        /// notifications
        ///
        /// @return Whether to receive notices
        //-----------------------------------------
        bool IState::ShouldReceiveNotifications() const
        {
            return false;
        }
		//-----------------------------------------
		/// Destructor
		//-----------------------------------------
		IState::~IState() 
		{
			if(mbOwnsScene)
			{
				SAFE_DELETE(mpScene);
			}
		}
        
        void IState::DebugPrint(std::string instrMessage)
        {
            DEBUG_LOG("IState "+CStringConverter::ToString((u32)this)+" - "+instrMessage);
            DEBUG_LOG("name = "+GetName());
            DEBUG_LOG("mpScene = "+CStringConverter::ToString((u32)mpScene));
            DEBUG_LOG("mbOwnsScene = "+CStringConverter::ToString(mbOwnsScene));
            DEBUG_LOG("mpStateMgr = "+CStringConverter::ToString((u32)mpStateMgr));
        }
        
	}
}
