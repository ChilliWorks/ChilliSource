//
//  IDisconnectableEvent.h
//  ChilliSource
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

#ifndef _CHILLISOURCE_CORE_EVENT_IDISCONNECTABLEEVENT_H_
#define _CHILLISOURCE_CORE_EVENT_IDISCONNECTABLEEVENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/ForwardDeclarations.h>

namespace ChilliSource
{
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
        virtual void CloseConnection(EventConnection* in_connection) = 0;
        //-------------------------------------------------------------
        /// Virtual destructor
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------
        virtual ~IDisconnectableEvent() {};
    };
}

#endif
