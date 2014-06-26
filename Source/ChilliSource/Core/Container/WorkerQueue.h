//
//  WorkerQueue.h
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_WORKERQUEUE_H_
#define _CHILLISOURCE_CORE_CONTAINER_WORKERQUEUE_H_

#include <ChilliSource/ChilliSource.h>

#include <condition_variable>
#include <mutex>
#include <queue>

namespace ChilliSource
{
    namespace Core
    {
        template <typename T> class WorkerQueue
        {
        public:
            
            //---------------------------------------------------------
            /// Thread safe method that will push an object onto the
            /// back of the queue
            ///
            /// @author Scott Downie
            ///
            /// @param Object of type T
            //---------------------------------------------------------
            void push(T inObject)
            {
                //Lock the queue for writing
                std::unique_lock<std::mutex> QueueLock(mQueueMutex);
                
                //Push the object onto the queue
                mQueue.push(inObject);
                
                //Make sure when we wake the thread up it's not blocked
                QueueLock.unlock();
                
                //Wakey wakey!!!!
                mEmptyWaitCondition.notify_one();
            }
			//-----------------------------------------------------------
			/// Causes the calling thread to sleep until the queue
			/// gets an object pushed onto it or is destroyed
			///
			/// @author Scott Downie
			//-----------------------------------------------------------
			void wait_for_push_or_destroy()
			{
				std::unique_lock<std::mutex> QueueLock(mQueueMutex);

				if (mQueue.empty())
				{
					//Sleepy time
					mEmptyWaitCondition.wait(QueueLock);
				}
			}
            //-----------------------------------------------------------
            /// Thread safe method that locks the queue while it returns
            /// the top object. Ensure the queue is not empty before 
			/// calling
            ///
            /// @author Scott Downie
            ///
            /// @return Object at front of queue of type T
            //-----------------------------------------------------------
            T front()
            {
                std::unique_lock<std::mutex> QueueLock(mQueueMutex);
                return mQueue.front();
            }
            //-----------------------------------------------------------
            /// Thread safe method that locks the queue while it pops
            /// the top object.
            ///
            /// @author Scott Downie
            //-----------------------------------------------------------
            void pop()
            {
                std::unique_lock<std::mutex> QueueLock(mQueueMutex);
                mQueue.pop();
            }
            //-----------------------------------------------------------
            /// @author Scott Downie
            ///
            /// @return Whether the queue has no objects in it
            //-----------------------------------------------------------
            bool empty() const
            {
                std::unique_lock<std::mutex> QueueLock(mQueueMutex);
                return mQueue.empty();
            }
            //-----------------------------------------------------------
            /// @author Scott Downie
            ///
            /// @return Size of queue
            //-----------------------------------------------------------
            u32 size() const
            {
                std::unique_lock<std::mutex> QueueLock(mQueueMutex);
                return mQueue.size();
            }
			//-----------------------------------------------------------
			/// Awakes all waiting threads regardless of whether 
			/// objects have been pushed
			///
			/// @author S Downie
			//-----------------------------------------------------------
			void awake_all()
			{
				mEmptyWaitCondition.notify_all();
			}
			//-----------------------------------------------------------
			/// Destructor awakes all waiting threads
			///
			/// @author S Downie
			//-----------------------------------------------------------
			~WorkerQueue()
			{
				awake_all();
			}
            
        private:
            
            std::queue<T> mQueue;
            
            mutable std::mutex mQueueMutex;
            std::condition_variable mEmptyWaitCondition;
        };
    }
}

#endif
