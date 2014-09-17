//
//  Event.h
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

#ifndef _CHILLISOURCE_CORE_EVENT_EVENT_H_
#define _CHILLISOURCE_CORE_EVENT_EVENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/Event/IConnectableEvent.h>
#include <ChilliSource/Core/Event/IDisconnectableEvent.h>

#include <vector>

namespace ChilliSource
{
    namespace Core
    {
        //-----------------------------------------------------------------
        /// An Event represents an object which can have multiple listeners
        /// (connections) with the given DelegateType.
        /// Objects should though expose the IConnectableEvent interface
        /// to prevent other objects invoking the event.
        ///
        /// @author S Downie
        //-----------------------------------------------------------------
        template <typename TDelegateType> class Event final : public IConnectableEvent<TDelegateType>, public IDisconnectableEvent
        {
        public:
            //-------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            Event(){}
            //-------------------------------------------------------------
            /// Destructor closes all open connections
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            ~Event()
            {
                CloseAllConnections();
            }
            //-------------------------------------------------------------
            /// No copying of events is allowed. If you wish to make a
            /// shallow copy of an event then hold a pointer to it.
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            Event(const Event&) = delete;
            Event& operator= (const Event&) = delete;
			//-------------------------------------------------------------
			/// Although we don't want events to be copyable, we do want
			/// them to be moveable. Connection references are updated to
            /// point to the new event.
			///
			/// @author Ian Copland
			/// 
			/// @param The event to move into this.
			//-------------------------------------------------------------
			Event(Event&& in_moveFrom)
			{
				CloseAllConnections();

				m_isNotifying = in_moveFrom.m_isNotifying;
				m_connections = std::move(in_moveFrom.m_connections);
				for (auto& connectionContainer : m_connections)
				{
					connectionContainer.m_connection->SetOwningEvent(this);
				}
			}
			//-------------------------------------------------------------
			/// Although we don't want events to be copyable, we do want
			/// them to be moveable. Connection references are updated to
            /// point to the new event.
			///
			/// @author Ian Copland
			/// 
			/// @param The event to move into this.
			///
			/// @param A reference to this after the move.
			//-------------------------------------------------------------
			Event& operator= (Event&& in_moveFrom)
			{
				CloseAllConnections();

				m_isNotifying = in_moveFrom.m_isNotifying;
				m_connections = std::move(in_moveFrom.m_connections);
				for (auto& connectionContainer : m_connections)
				{
					connectionContainer.m_connection->SetOwningEvent(this);
				}

				return *this;
			}
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
            EventConnectionUPtr OpenConnection(const TDelegateType& in_delegate) override
            {
                EventConnectionUPtr connection(new EventConnection());
                connection->SetOwningEvent(this);
            
                ConnectionDesc desc;
                desc.m_delegate = in_delegate;
                desc.m_connection = connection.get();
                m_connections.push_back(desc);
            
                return connection;
            }
            //-------------------------------------------------------------
            /// Close connection to the event. The connection will
            /// no longer be notified of the event
            ///
            /// @author S Downie
            ///
            /// @param Connection to close
            //-------------------------------------------------------------
            void CloseConnection(EventConnection* in_connection) override
            {
                for(u32 i=0; i<m_connections.size(); ++i)
                {
                    ConnectionDesc& desc = m_connections[i];
                    if(desc.m_connection == in_connection)
                    {
                        if(m_isNotifying == false)
                        {
                            m_connections.erase(m_connections.begin() + i);
                        }
                        else
                        {
                            desc.m_connection = nullptr;
                        }
                        
                        return;
                    }
                }
            }
            //-------------------------------------------------------------
            /// Notify connections that the event has occurred
            ///
            /// @author S Downie
            ///
            /// @param Arguments to pass to the connection delegate
            //-------------------------------------------------------------
            template <typename... TArgTypes> void NotifyConnections(TArgTypes&&... in_args)
            {
                m_isNotifying = true;
                
                //Take a snapshot of the number of delegates so any new ones added
                //during the notify loop aren't notified themseleves.
                u32 numConnections = m_connections.size();
                for(u32 i=0; i<numConnections; ++i)
                {
                    if(m_connections[i].m_connection != nullptr)
                    {
                        m_connections[i].m_delegate(std::forward<TArgTypes>(in_args)...);
                    }
                }
                
                m_isNotifying = false;
                
                RemoveClosedConnections();
            }
            //-------------------------------------------------------------
            /// Closes all the currently open connections
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            void CloseAllConnections()
            {
                for(u32 i=0; i<m_connections.size(); ++i)
                {
                    if(m_connections[i].m_connection != nullptr)
                    {
                        m_connections[i].m_connection->SetOwningEvent(nullptr);
                    }
                }
                
                m_connections.clear();
            }
    
        private:
    
            //-------------------------------------------------------------------------
            /// Remove from the list any connections that have been flagged as closed
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------
            void RemoveClosedConnections()
            {
                for(typename ConnectionList::iterator it = m_connections.begin(); it != m_connections.end(); )
                {
                    if(it->m_connection == nullptr)
                    {
                        it = m_connections.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
    
        private:
                                    
            struct ConnectionDesc
            {
                TDelegateType m_delegate;
                EventConnection* m_connection = nullptr;
            };
    
            typedef std::vector<ConnectionDesc> ConnectionList;
            ConnectionList m_connections;
        
            bool m_isNotifying = false;
        };
    }
}

#endif