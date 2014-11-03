//
//  concurrent_blocking_queue.h
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_CONCURRENTBLOCKINGQUEUE_H_
#define _CHILLISOURCE_CORE_CONTAINER_CONCURRENTBLOCKINGQUEUE_H_

#include <ChilliSource/ChilliSource.h>

#include <condition_variable>
#include <mutex>
#include <queue>

namespace ChilliSource
{
    namespace Core
    {
		//------------------------------------------------------------------
		/// A queue which can safely be accessed from multiple threads. If
        /// pop is called when there is nothing in the queue it will block
        /// until something is available.
		/// 
		/// @author Scott Downie
		//------------------------------------------------------------------
        template <typename TType> class concurrent_blocking_queue final
        {
        public:
            //---------------------------------------------------------
            /// Default constructor.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------
            concurrent_blocking_queue() = default;
            //---------------------------------------------------------
            /// Copy constructor. Copies the queue without copying the
            /// underlying mutex.
            ///
            /// @author Ian Copland
            ///
            /// @param The queue to copy.
            //---------------------------------------------------------
            concurrent_blocking_queue(const concurrent_blocking_queue<TType>& in_toCopy);
            //---------------------------------------------------------
            /// Deleted move constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param The queue to move.
            //---------------------------------------------------------
            concurrent_blocking_queue(concurrent_blocking_queue<TType>&& in_toMove) = delete;
            //---------------------------------------------------------
            /// Copy assignment.Copies the queue without copying the
            /// underlying mutex.
            ///
            /// @author Ian Copland
            ///
            /// @param The queue to copy.
            ///
            /// @return a reference to this.
            //---------------------------------------------------------
            concurrent_blocking_queue<TType>& operator=(const concurrent_blocking_queue<TType>& in_toCopy);
            //---------------------------------------------------------
            /// Deleted Copy assignment.
            ///
            /// @author Ian Copland
            ///
            /// @param The queue to move.
            ///
            /// @return a reference to this.
            //---------------------------------------------------------
            concurrent_blocking_queue<TType>& operator=(concurrent_blocking_queue<TType>&& in_toMove) = delete;
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
            /// Aborts the queue, awakening any threads that are
            /// currently waiting for a push. These threads should be
            /// joined (or at least confirmed to no longer be executing
            /// inside the queue) before the queue is deleted. This
            /// must be called before the queue is deleted. After
            /// this is called the queue is in a "Finished" state and
            /// can no longer be used.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------
            void abort();
			//---------------------------------------------------------
			/// Destructor awakes all waiting threads
			///
			/// @author S Downie
			//---------------------------------------------------------
			~concurrent_blocking_queue();
        private:

			std::queue<TType> m_queue;
			bool m_finished = false;
            mutable std::mutex m_queueMutex;
            std::condition_variable m_emptyWaitCondition;
        };
        //---------------------------------------------------------
        //---------------------------------------------------------
        template <typename TType> concurrent_blocking_queue<TType>::concurrent_blocking_queue(const concurrent_blocking_queue<TType>& in_toCopy)
        {
            std::unique_lock<std::mutex> copyQueueLock(in_toCopy.m_queueMutex);
            m_queue = in_toCopy.m_queue;
            m_finished = in_toCopy.m_finished;
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        template <typename TType> concurrent_blocking_queue<TType>& concurrent_blocking_queue<TType>::operator=(const concurrent_blocking_queue<TType>& in_toCopy)
        {
            std::unique_lock<std::mutex> copyQueueLock(in_toCopy.m_queueMutex);
            std::queue<TType> queue = in_toCopy.m_queue;
            bool finished = in_toCopy.m_finished;
            copyQueueLock.unlock();
            
            std::unique_lock<std::mutex> thisQueueLock(m_queueMutex);
            m_queue = queue;
            m_finished = finished;
            return *this;
        }
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		template <typename TType> bool concurrent_blocking_queue<TType>::empty() const
		{
            std::unique_lock<std::mutex> queueLock(m_queueMutex);
            CS_ASSERT(m_finished == false, "Concurrent blocking queue is being queried after calling abort().");
            
			return m_queue.empty();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		template <typename TType> u32 concurrent_blocking_queue<TType>::size() const
		{
            std::unique_lock<std::mutex> queueLock(m_queueMutex);
            CS_ASSERT(m_finished == false, "Concurrent blocking queue is being queried after calling abort().");
            
			return m_queue.size();
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		template <typename TType> void concurrent_blocking_queue<TType>::push(TType in_object)
		{
			//Lock the queue for writing
            std::unique_lock<std::mutex> queueLock(m_queueMutex);
            CS_ASSERT(m_finished == false, "Concurrent blocking queue is being pushed after calling abort().");

			//Push the object onto the queue
			m_queue.push(in_object);

			//Wakey wakey!!!!
			m_emptyWaitCondition.notify_one();
		}
		//---------------------------------------------------------
		//---------------------------------------------------------
		template <typename TType> bool concurrent_blocking_queue<TType>::pop_or_wait(TType& out_poppedObject)
		{
			std::unique_lock<std::mutex> queueLock(m_queueMutex);
            CS_ASSERT(m_finished == false, "Concurrent blocking queue is being popped after calling abort().");
            
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
		template <typename TType> void concurrent_blocking_queue<TType>::clear()
		{
			std::unique_lock<std::mutex> queueLock(m_queueMutex);
            CS_ASSERT(m_finished == false, "Concurrent blocking queue is being cleared after calling abort().");
            
			while (m_queue.empty() == false)
			{
				m_queue.pop();
			}
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		template <typename TType> void concurrent_blocking_queue<TType>::abort()
		{
			std::unique_lock<std::mutex> queueLock(m_queueMutex);
            CS_ASSERT(m_finished == false, "Cannot abort() a concurrent blocking queue twice.");
            
            while (m_queue.empty() == false)
            {
                m_queue.pop();
            }
            
			m_finished = true;
			m_emptyWaitCondition.notify_all();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		template <typename TType> concurrent_blocking_queue<TType>::~concurrent_blocking_queue()
		{
#ifdef CS_ENABLE_DEBUG
            std::unique_lock<std::mutex> queueLock(m_queueMutex);
            CS_ASSERT(m_finished == true, "Concurrent blocking queue is being deleted without first calling abort().");
#endif
		}
    }
}

#endif
