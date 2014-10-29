//
//  concurrent_worker_queue.h
//  Chilli Source
//  Created by Scott Downie on 29/03/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_CONCURRENTWORKERQUEUE_H_
#define _CHILLISOURCE_CORE_CONTAINER_CONCURRENTWORKERQUEUE_H_

#include <ChilliSource/ChilliSource.h>

#include <condition_variable>
#include <mutex>
#include <queue>

namespace ChilliSource
{
    namespace Core
    {
		//------------------------------------------------------------------
		/// A worker queue which can safely be accessed from multiple
		/// threads. 
		/// 
		/// @author Scott Downie
		//------------------------------------------------------------------
        template <typename TType> class concurrent_worker_queue final
        {
        public:
			CS_DECLARE_NOCOPY(concurrent_worker_queue);
			//---------------------------------------------------------
			/// Default constructor.
			///
			/// @author Ian Copland
			//---------------------------------------------------------
			concurrent_worker_queue() = default;
			//---------------------------------------------------------
			/// @author Scott Downie
			///
			/// @return Whether the queue has no objects in it
			//---------------------------------------------------------
			bool empty() const;
			//---------------------------------------------------------
			/// @author Scott Downie
			///
			/// @return Size of queue
			//---------------------------------------------------------
			u32 size() const;
            //---------------------------------------------------------
            /// Thread safe method that will push an object onto the
            /// back of the queue
            ///
            /// @author Scott Downie
            ///
            /// @param Object of type TType
            //---------------------------------------------------------
			void push(TType in_object);
			//---------------------------------------------------------
			/// Pops and returns the front of the queue. If the queue
			/// is empty the current thread will block until something 
			/// is added. If the queue is deleted while a thread is 
			/// still waiting it will return unsuccessfully.
			///
			/// @author Ian Copland
			///
			/// @param [Out] The popped object. This will only be set
			/// if an object was successfully retreived from the queue.
			///
			/// @return Whether or not a value was successfully 
			/// retreived.
			//---------------------------------------------------------
			bool pop_or_wait(TType& out_poppedObject);
			//---------------------------------------------------------
			/// Clears the queue of all objects.
			///
			/// @author Ian Copland
			//---------------------------------------------------------
			void clear();
			//---------------------------------------------------------
			/// Destructor awakes all waiting threads
			///
			/// @author S Downie
			//---------------------------------------------------------
			~concurrent_worker_queue();
        private:
			//---------------------------------------------------------
			/// Awakes all waiting threads regardless of whether 
			/// objects have been pushed
			///
			/// @author S Downie
			//---------------------------------------------------------
			void awake_all();

			std::queue<TType> m_queue;
			bool m_finished = false;
            mutable std::mutex m_queueMutex;
            std::condition_variable m_emptyWaitCondition;
        };
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		template <typename TType> bool concurrent_worker_queue<TType>::empty() const
		{
			std::unique_lock<std::mutex> queueLock(m_queueMutex);
			return m_queue.empty();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		template <typename TType> u32 concurrent_worker_queue<TType>::size() const
		{
			std::unique_lock<std::mutex> queueLock(m_queueMutex);
			return m_queue.size();
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		template <typename TType> void concurrent_worker_queue<TType>::push(TType in_object)
		{
			//Lock the queue for writing
			std::unique_lock<std::mutex> queueLock(m_queueMutex);

			//Push the object onto the queue
			m_queue.push(in_object);

			//Wakey wakey!!!!
			m_emptyWaitCondition.notify_one();
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		template <typename TType> bool concurrent_worker_queue<TType>::pop_or_wait(TType& out_poppedObject)
		{
			std::unique_lock<std::mutex> queueLock(m_queueMutex);

			while (m_queue.empty() == true && m_finished == false)
			{
				m_emptyWaitCondition.wait(queueLock);
			}

			if (m_finished == false)
			{
				out_poppedObject = std::move(m_queue.front());
				m_queue.pop();
				return true;
			}

			return false;
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		template <typename TType> void concurrent_worker_queue<TType>::clear()
		{
			std::unique_lock<std::mutex> queueLock(m_queueMutex);

			while (m_queue.empty() == false)
			{
				m_queue.pop();
			}
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		template <typename TType> void concurrent_worker_queue<TType>::awake_all()
		{
			std::unique_lock<std::mutex> queueLock(m_queueMutex);
			m_finished = true;
			m_emptyWaitCondition.notify_all();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		template <typename TType> concurrent_worker_queue<TType>::~concurrent_worker_queue()
		{
			awake_all();
		}
    }
}

#endif
