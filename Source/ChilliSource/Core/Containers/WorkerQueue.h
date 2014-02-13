//
//  WorkerQueue.h
//  moFloTest
//
//  Created by Scott Downie on 29/03/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_WORKER_QUEUE_H_
#define _MO_FLO_CORE_WORKER_QUEUE_H_

#include <queue>

#include <moFlo/Core/Thread.h>

namespace moFlo
{
    namespace Core
    {
        template <typename T> class CWorkerQueue
        {
        public:
            
            //---------------------------------------------------------
            /// Push
            ///
            /// Thread safe method that will push an object into the
            /// back of the queue
            ///
            /// @param Object of type T
            //---------------------------------------------------------
            void Push(T inObject, bool inbWakeup)
            {
                //Lock the queue for writing
                CThread::ScopedLock QueueLock(mQueueMutex);
                
                //Push the object onto the queue
                bool bWasQueueEmpty = mQueue.empty();
                mQueue.push(inObject);
                
                //Make sure when we wake the thread up it's not blocked
                QueueLock.unlock();
                
                //If the request queue was empty then the thread is asleep. Let's wake him up
                if(bWasQueueEmpty && inbWakeup)
                {
                    //Wakey wakey!!!!
                    mEmptyWaitCondition.notify_one();
                }
            }
            //-----------------------------------------------------------
            /// Pop
            ///
            /// Thread safe method that locks the queue while it pops
            /// and returns the top object. If the flag is set then
            /// the method will force the requesting thread to sleep
            /// until the queue has data in it
            ///
            /// @return Object at front of queue of type T
            //-----------------------------------------------------------
            T Pop(bool inbSleepIfEmpty)
            {
                CThread::ScopedLock QueueLock(mQueueMutex);
                
                if(inbSleepIfEmpty)
                {
                    while(mQueue.empty())
                    {
                        //Sleepy time
                        mEmptyWaitCondition.wait(QueueLock);
                    }
                }
                
                T FrontObject = mQueue.front();
                mQueue.pop();
                
                return FrontObject;
            }
            //-----------------------------------------------------------
            /// Is Empty
            ///
            /// @return Whether the queue has no objects in it
            //-----------------------------------------------------------
            bool IsEmpty() const
            {
                CThread::ScopedLock QueueLock(mQueueMutex);
                return mQueue.empty();
            }
            
        private:
            
            std::queue<T> mQueue;
            
            CThread::Mutex mQueueMutex;
            CThread::Condition mEmptyWaitCondition;
        };
    }
}

#endif