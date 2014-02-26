//
//  WorkerQueue.h
//  ChilliSource
//
//  Created by Scott Downie on 29/03/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_CONTAINER_WORKERQUEUE_H_
#define _CHILLISOURCE_CORE_CONTAINER_WORKERQUEUE_H_

#include <ChilliSource/ChilliSource.h>

#include <queue>
#include <thread>

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
            /// Thread safe method that locks the queue while it returns
            /// the top object. If the flag is set then
            /// The method will force the requesting thread to sleep
            /// until the queue has data in it
            ///
            /// @author Scott Downie
            ///
            /// @return Object at front of queue of type T
            //-----------------------------------------------------------
            T front()
            {
                std::unique_lock<std::mutex> QueueLock(mQueueMutex);
                
                while(mQueue.empty())
                {
                    //Sleepy time
                    mEmptyWaitCondition.wait(QueueLock);
                }
                
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
            
        private:
            
            std::queue<T> mQueue;
            
            mutable std::mutex mQueueMutex;
            std::condition_variable mEmptyWaitCondition;
        };
    }
}

#endif