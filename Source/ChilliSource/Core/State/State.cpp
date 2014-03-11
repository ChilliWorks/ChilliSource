/*
 *  State.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 07/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Core/State/State.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
	namespace Core 
	{
		//-----------------------------------------
		/// Constructor
		///
		/// Create a new scene for this state
		//-----------------------------------------
		State::State(Scene* inpScene) : mbOwnsScene(true)
		{
            mpStateMgr = Application::Get()->GetStateManager();
            
			if(!inpScene)
			{
				mpScene = new Scene(Application::Get()->GetInputSystem());
			}
			else
			{
				mbOwnsScene = false;
				mpScene = inpScene;
			}
		}
		//------------------------------------------
		/// Get Scene
		///
		/// @return Scene to present modally
		//------------------------------------------
		Scene* State::GetScenePtr()
		{
			return mpScene;
		}
		//------------------------------------------
		/// Get State Manager
		///
		/// @return Pointer to state manager
		//------------------------------------------
		StateManager* State::GetStateManagerPtr() const 
		{ 
			return mpStateMgr; 
		}
        //------------------------------------------
        /// Is Active State
        ///
        /// @return Whether the state is the top of 
        /// the hierarchy
        //------------------------------------------
		bool State::IsActiveState() const 
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
        bool State::ShouldReceiveNotifications() const
        {
            return false;
        }
		//-----------------------------------------
		/// Destructor
		//-----------------------------------------
		State::~State() 
		{
			if(mbOwnsScene)
			{
				CS_SAFEDELETE(mpScene);
			}
		}
        
        void State::DebugPrint(std::string instrMessage)
        {
            CS_LOG_DEBUG("State "+ToString((u32)this)+" - "+instrMessage);
            CS_LOG_DEBUG("name = "+GetName());
            CS_LOG_DEBUG("mpScene = "+ToString((u32)mpScene));
            CS_LOG_DEBUG("mbOwnsScene = "+ToString(mbOwnsScene));
            CS_LOG_DEBUG("mpStateMgr = "+ToString((u32)mpStateMgr));
        }
        
	}
}
