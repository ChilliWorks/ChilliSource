//
//  Timer.cpp
//  Chilli Source
//  Created by Scott Downie on 23/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Core/Time/Timer.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Event/EventConnection.h>

namespace ChilliSource
{
	namespace Core 
	{
		//--------------------------------------------
		//--------------------------------------------
		Timer::Timer()
		{
            m_coreTimerUpdateConnection = CoreTimer::GetTimerUpdateEvent().OpenConnection(MakeDelegate(this, &Timer::Update));
		}
		//--------------------------------------------
		/// Start
		///
		/// Begin the timer 
		//--------------------------------------------
		void Timer::Start()
		{
			m_isActive = true;
		}
		//--------------------------------------------
		/// Reset 
		///
		/// Reset the elapsed time
		//--------------------------------------------
		void Timer::Reset()
		{
			m_elapsedTime = 0.0f;
		}
		//--------------------------------------------
		/// Stop 
		///
		/// Stop the timer
		//--------------------------------------------
		void Timer::Stop()
		{
			m_isActive = false;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        EventConnectionUPtr Timer::OpenConnection(f32 in_periodSecs, Delegate in_delegate)
        {
            EventConnectionUPtr connection(new EventConnection());
            connection->SetOwningEvent(this);
            
            ConnectionDesc desc;
            desc.m_delegate = in_delegate;
            desc.m_connection = connection.get();
            desc.m_elapsedSinceLastUpdate = 0.0f;
            desc.m_updatePeriod = in_periodSecs;
            m_connections.push_back(desc);
            
            return connection;
        }
		//----------------------------------------------------
		//----------------------------------------------------
		void Timer::Update(const f32 in_dt)
		{
			if(m_isActive)
			{
				m_elapsedTime += in_dt;
                
                m_isNotifying = true;
                
                //Take a snapshot of the number of delegates so any new ones added
                //during the notify loop aren't notified themseleves.
                u32 numConnections = m_connections.size();
                for(u32 i=0; i<numConnections; ++i)
                {
                    if(m_connections[i].m_connection != nullptr && (m_connections[i].m_elapsedSinceLastUpdate += in_dt) >= m_connections[i].m_updatePeriod)
                    {
                        m_connections[i].m_elapsedSinceLastUpdate = 0.0f;
                        m_connections[i].m_delegate();
                    }
                }
                
                m_isNotifying = false;
                
                RemoveClosedConnections();
			}
		}
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        void Timer::CloseConnection(EventConnection* in_connection)
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
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        void Timer::RemoveClosedConnections()
        {
            for(ConnectionList::iterator it = m_connections.begin(); it != m_connections.end(); )
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
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        void Timer::CloseAllConnections()
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
		//--------------------------------------------
		//--------------------------------------------
		Timer::~Timer()
		{
            CloseAllConnections();
		}
	}
}
