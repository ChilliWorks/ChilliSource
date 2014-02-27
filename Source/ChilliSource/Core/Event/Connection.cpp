/*
 *  Connection.cpp
 *  ChilliSource
 *
 *  Created by Scott Downie on 27/02/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Event/Connection.h>

#include <ChilliSource/Core/Event/IDisconnectableEvent.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void Connection::SetOwningEvent(IDisconnectableEvent* in_owningEvent)
        {
            CS_ASSERT(m_owningEvent == nullptr || in_owningEvent == nullptr, "Connection already has an owning event");
            m_owningEvent = in_owningEvent;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void Connection::Close()
        {
            if(m_owningEvent != nullptr)
            {
                m_owningEvent->CloseConnection(this);
            }
        }
        //----------------------------------------------------------------
        // Destructor closes the connection automatically when the
        // connection object is destroyed
        //----------------------------------------------------------------
        Connection::~Connection()
        {
            Close();
        }
    }
}
