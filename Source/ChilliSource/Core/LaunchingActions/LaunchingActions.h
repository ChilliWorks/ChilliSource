//
//  LaunchingActions.h
//  MoFlow
//
//  Created by Scott Downie on 26/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLO_CORE_LAUNCHING_ACTIONS_H_
#define _MOFLO_CORE_LAUNCHING_ACTIONS_H_

#include <ChilliSource/ChilliSource.h>

#include <functional>

namespace ChilliSource
{
    namespace Core
    {
        namespace LaunchingActions
        {
            typedef std::function<void(const std::string&)> ActionDelegate;
            
            //-----------------------------------------------------------------------
            /// Subscribe For Action Type
            ///
            /// Add a delegate as a listener to be notified when the system receives
            /// a launching action of a certain type. Only one delegate can
            /// be described per action
            ///
            /// @param Action type
            /// @param Delegate
            //-----------------------------------------------------------------------
            void SubscribeForActionType(const std::string& instrActionType, const ActionDelegate& inDelegate);
            //-----------------------------------------------------------------------
            /// Unsubscribe For Action Type
            ///
            /// Remove the listener for the given action type
            ///
            /// @param Action type
            //-----------------------------------------------------------------------
            void UnsubscribeForActionType(const std::string& instrActionType);
            //-----------------------------------------------------------------------
            /// Application Did Receive Launching URL
            ///
            /// Called by the OS upon launching the application via a URL
            ///
            /// @param Launching URL that must be decoded into an action
            //-----------------------------------------------------------------------
            void ApplicationDidReceiveLaunchingURL(const std::string& instrURL);
        };
    }
}

#endif

