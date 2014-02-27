/*
 *  IDisconnectableEvent.h
 *  ChilliSource
 *
 *  Created by Scott Downie on 27/02/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#ifndef _CHILLISOURCE_CORE_EVENT_IDISCONNECTABLEEVENT_H_
#define _CHILLISOURCE_CORE_EVENT_IDISCONNECTABLEEVENT_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        class Connection;
        
        //-----------------------------------------------------------------
        /// An interface that allows connections to be closed on an event
        ///
        /// @author S Downie
        //-----------------------------------------------------------------
        class IDisconnectableEvent
        {
        public:
            //-------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            IDisconnectableEvent(){}
            //-------------------------------------------------------------
            /// No copying of events is allowed. If you wish to make a
            /// shallow copy of an event then hold a pointer to it.
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            IDisconnectableEvent(const IDisconnectableEvent&) = delete;
            IDisconnectableEvent& operator= (const IDisconnectableEvent&) = delete;
            
            //-------------------------------------------------------------
            /// Close connection to the event. The connection will
            /// no longer be notified of the event
            ///
            /// @author S Downie
            ///
            /// @param Connection to close
            //-------------------------------------------------------------
            virtual void CloseConnection(Connection* in_connection) = 0;
        };
    }
}

#endif