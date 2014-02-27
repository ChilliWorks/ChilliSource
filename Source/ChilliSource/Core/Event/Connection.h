/*
 *  Connection.h
 *  ChilliSource
 *
 *  Created by Scott Downie on 27/02/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#ifndef _CHILLISOURCE_CORE_EVENT_CONNECTION_H_
#define _CHILLISOURCE_CORE_EVENT_CONNECTION_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------
        /// A connection is a channel to an event. When a listener
        /// is registered with an event a scoped connection is created
        /// when this connection object is destroyed then the connection
        /// is automatically closed. (See Event.h)
        ///
        /// @author S Downie
        //----------------------------------------------------------------
        class Connection
        {
        public:
            
            //----------------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------
            ~Connection();
            //----------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Event that owns the connection
            //----------------------------------------------------------------
            void SetOwningEvent(IDisconnectableEvent* in_owningEvent);
            //----------------------------------------------------------------
            /// Manually close the connection so that it no longer receieves
            /// any events
            ///
            /// @author S Downie
            //----------------------------------------------------------------
            void Close();

        private:
            
            IDisconnectableEvent* m_owningEvent = nullptr;
        };
    }
}

#endif