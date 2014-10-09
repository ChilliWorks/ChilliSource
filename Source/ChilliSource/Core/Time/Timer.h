//
//  Timer.h
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

#ifndef _CHILLISOURCE_CORE_TIMER_H_
#define _CHILLISOURCE_CORE_TIMER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/IDisconnectableEvent.h>
#include <ChilliSource/Core/Time/CoreTimer.h>

#include <vector>

namespace ChilliSource
{
	namespace Core 
	{
		//---------------------------------------------------
        /// Event style class that objects can connect to
        /// and be notified periodically or after a set
        /// period of time
        ///
        /// @author S Downie
        //---------------------------------------------------
		class Timer final : public IDisconnectableEvent
		{
		public:
            
            typedef std::function<void()> Delegate;
            
            //--------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //--------------------------------------------
			Timer();
            //--------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //--------------------------------------------
			~Timer();
			//--------------------------------------------
			/// Begin the timer
            ///
            /// @author S Downie
			//--------------------------------------------
			void Start();
			//--------------------------------------------
			/// Reset the elapsed time
            ///
            /// @author S Downie
			//--------------------------------------------
			void Reset();
			//--------------------------------------------
			/// Stop the timer
            ///
            /// @author S Downie
			//--------------------------------------------
			void Stop();
			//--------------------------------------------
            /// @author S Downie
            ///
			/// @return Whether timer is active
			//--------------------------------------------
            inline bool IsTimerActive() const
            {
                return m_isActive;
            }
			//--------------------------------------------
            /// @author S Downie
            ///
			/// @return Time elapsed since first start
            /// or last reset
			//--------------------------------------------
            inline f32 GetElapsedTime() const
            {
                return m_elapsedTime;
            }
			//----------------------------------------------------
            /// Opens a scoped connection to the timers periodic update
            /// event. The listener will be notified after every
            /// period.
            ///
            /// @author S Downie
            ///
            /// @param Seconds after which connection is notified
			/// @param Delegate
            ///
            /// @return Scoped connection
			//----------------------------------------------------
			EventConnectionUPtr OpenConnection(f32 in_periodSecs, Delegate in_delegate);
            //-------------------------------------------------------------
            /// Close connection to the event. The connection will
            /// no longer be notified of the event
            ///
            /// @author S Downie
            ///
            /// @param Connection to close
            //-------------------------------------------------------------
            void CloseConnection(EventConnection* in_connection) override;
			
		private:
            
			//----------------------------------------------------
            /// Updates the elapsed time
            ///
            /// @author S Downie
            ///
			/// @param Time since last update
			//----------------------------------------------------
			void Update(const f32 in_dt);
            //-------------------------------------------------------------------------
            /// Remove from the list any connections that have been flagged as closed
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------
            void RemoveClosedConnections();
            //-------------------------------------------------------------
            /// Closes all the currently open connections
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            void CloseAllConnections();
			
		private:
			
            struct ConnectionDesc
            {
                Delegate m_delegate;
                EventConnection* m_connection = nullptr;
                f32 m_updatePeriod;
				f32 m_elapsedSinceLastUpdate = 0.0f;
            };
            
            typedef std::vector<ConnectionDesc> ConnectionList;
            ConnectionList m_connections;
            
            EventConnectionUPtr m_coreTimerUpdateConnection;
            
            f32 m_elapsedTime = 0.0f;
            
            bool m_isNotifying = false;
            bool m_isActive = false;;
		};
	}
}

#endif