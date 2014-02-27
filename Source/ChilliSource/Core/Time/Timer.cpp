/*
 *  Timer.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Time/Timer.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Event/Connection.h>

namespace ChilliSource
{
	namespace Core 
	{
		//--------------------------------------------
		//--------------------------------------------
		Timer::Timer()
		{
            m_coreTimerUpdateConnection = CCoreTimer::GetTimerUpdateEvent().OpenConnection(MakeDelegate(this, &Timer::Update));
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
        ConnectionUPtr Timer::OpenConnection(Delegate in_delegate, f32 in_periodSecs)
        {
            ConnectionUPtr connection(new Connection());
            connection->SetOwningEvent(this);
            
            ConnectionDesc desc;
            desc.m_delegate = in_delegate;
            desc.m_connection = connection.get();
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
        void Timer::CloseConnection(Connection* in_connection)
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
