/*
 *  IConnectableEvent.h
 *  ChilliSource
 *
 *  Created by Scott Downie on 27/02/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#ifndef _CHILLISOURCE_CORE_EVENT_ICONNECTABLEEVENT_H_
#define _CHILLISOURCE_CORE_EVENT_ICONNECTABLEEVENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/ForwardDeclarations.h>

namespace ChilliSource
{
    namespace Core
    {
        //-----------------------------------------------------------------
        /// An IConnectableEvent represents an object which can have multiple listeners
        /// (connections) with the given DelegateType. The subclass Event is what
        /// should be instanced by objects as they have an NotifyConnections method.
        /// Objects should though expose the IConnectableEvent interface to prevent
        /// other objects invoking the event.
        ///
        /// @author S Downie
        //-----------------------------------------------------------------
        template <typename TDelegateType> class IConnectableEvent
        {
        public:
            
            //-------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            IConnectableEvent(){}
            //-------------------------------------------------------------
            /// No copying of events is allowed. If you wish to make a
            /// shallow copy of an event then hold a pointer to it.
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            IConnectableEvent(const IConnectableEvent&) = delete;
            IConnectableEvent& operator= (const IConnectableEvent&) = delete;
            
            //-------------------------------------------------------------
            /// Opens a new connection to the event. While this connection
            /// remains in scope the delegate will be notified of events
            ///
            /// @author S Downie
            ///
            /// @param Delegate to notify
            ///
            /// @return Scoped connection
            //-------------------------------------------------------------
            virtual EventConnectionUPtr OpenConnection(const TDelegateType& in_delegate) = 0;
        };
    }
}

#endif