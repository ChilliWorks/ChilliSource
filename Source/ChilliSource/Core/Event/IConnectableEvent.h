//
//  IConnectableEvent.h
//  Chilli Source
//  Created by Scott Downie on 27/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

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