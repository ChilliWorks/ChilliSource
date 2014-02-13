/*
 *  IState.h
 *  moFlo
 *
 *  Created by Scott Downie on 21/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_STATE_
#define _MO_FLO_STATE_

#include <moFlo/Core/ForwardDeclarations.h>
#include <moFlo/Core/Notifications/NotificationScheduler.h>

namespace moFlo
{
	namespace Core
	{
		//======================================================
		/// Description:
		///
		/// Abstract state from which all other states derive.
		/// Each application must have at least one state.
		///
		/// Holds the states handle to its scene manager
		//======================================================
		class IState
		{
		public:
			IState(CScene* inpScene = NULL);
			virtual ~IState();
			
			//-----------------------------------------
			/// On Init
			///
			/// Called at the start of the application
			/// and is first responder after a state
			/// has been destroyed and recreated.
			//-----------------------------------------
			virtual void OnInit(){};
			//-----------------------------------------
			/// On Start
			///
			/// Called whenever the state becomes active
			//-----------------------------------------
			virtual void OnStart(){};
			//-----------------------------------------
			/// On Resume
			///
			/// Called whenever the state becomes active
			/// from a system pause 
			//-----------------------------------------
			virtual void OnResume(){};
			//-----------------------------------------
			/// On Update
			///
			/// Called at the start of each frame is
			/// the state is active. It passes the time
			/// elapsed since the last frame.
			//-----------------------------------------
			virtual void OnUpdate(f32){};
            //-----------------------------------------
			/// On Fixed Update
			///
            /// Can be called multiple times per frame
            /// depending on the update frequency
            ///
            /// @param Time since last update
			//-----------------------------------------
			virtual void OnFixedUpdate(f32){};
			//-----------------------------------------
			/// On Pause
			///
			/// Called whenever the state becomes 
			/// inactive due to a system pause
			//-----------------------------------------
			virtual void OnPause(){};
			//-----------------------------------------
			/// On Stop
			///
			/// Called when a state becomes in-active
			//-----------------------------------------
			virtual void OnStop(){};
			//-----------------------------------------
			/// On Destroy
			///
			/// Final call to state to release all it's
			/// assets.
			//-----------------------------------------
			virtual void OnDestroy(){};
			//-----------------------------------------
			/// On Go Back
			///
			/// Called when a state is told to go back.
			//-----------------------------------------
			virtual void OnGoBack(){};
            //-----------------------------------------
            /// Should Receive Notifications
            ///
            /// Delegate call used to decide whether
            /// the state should be notified of any
            /// notifications
            ///
            /// @return Whether to receive notices
            //-----------------------------------------
            virtual bool ShouldReceiveNotifications() const;
            //---------------------------------------------------------------------
            /// On Receive Notification
            ///
            /// Delegate call for when notification is triggered and the state has 
            /// notifed the system it is prepared to receive notifications via the 
            /// ShouldReceiveNotifications() delegate call
            ///
            /// @param Notification
            //---------------------------------------------------------------------
            virtual bool OnReceiveNotification(moFlo::Notification* inpsNotification){return true;}
            
            //---------------------------------------------------------------------
            /// Get Name
            ///
            /// Return a name for the state. (added for debug purposes)
            ///
            //---------------------------------------------------------------------            
            virtual std::string GetName() {return std::string();}
            
            //---------------------------------------------------------------------
            /// DebugPrint
            ///
            /// Print current state debug information to console with message
            ///
            //---------------------------------------------------------------------
            virtual void DebugPrint(std::string instrMessage);
            
			//-----------------------------------------
			/// Get Application
			///
			/// Handy accessors for the main application
			/// @return Application handle
			//-----------------------------------------
			CApplication& GetApplication();
			//-----------------------------------------
			/// Get Application
			///
			/// Handy accessors for the main application
			/// @return Application handle
			//-----------------------------------------
			CApplication* GetApplicationPtr();
			//------------------------------------------
			/// Get Scene
			///
			/// @return Scene to present modally
			//------------------------------------------
			CScene* GetScenePtr();
			//------------------------------------------
			/// Get State Manager
			///
			/// @return Pointer to state manager
			//------------------------------------------
			CStateManager* GetStateManagerPtr() const;
            //------------------------------------------
            /// Is Active State
            ///
            /// @return Whether the state is the top of 
            /// the hierarchy
            //-----------------------------------------
			bool IsActiveState() const;
            
            
		protected:
			
			CStateManager* mpStateMgr;
			CScene* mpScene;
			
			bool mbOwnsScene;
			
			friend class CStateManager;
		};
		
		typedef SHARED_PTR<IState> StatePtr;
	}
}

#endif
