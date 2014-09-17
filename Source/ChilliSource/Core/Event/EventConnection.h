//
//  EventConnection.h
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

#ifndef _CHILLISOURCE_CORE_EVENT_EVENTCONNECTION_H_
#define _CHILLISOURCE_CORE_EVENT_EVENTCONNECTION_H_

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
        class EventConnection
        {
        public:
            
            //----------------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------
            ~EventConnection();
            //----------------------------------------------------------------
			/// Sets the event this is connected to. This should only be called
			/// by Event itself, don't call manually.
			///
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