/*
 *  Task.h
 *  Chilli Source
 *
 *  Created by Scott Downie on 15/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _CHILLISOURCE_CORE_THREADING_TASK_H_
#define _CHILLISOURCE_CORE_THREADING_TASK_H_

#include <thread>
#include <type_traits>

namespace ChilliSource
{
    namespace Core
    {
        namespace ExpansionHelpers
        {
            //------------------------------------------------------------------
            // Expand tuples into individual arguments
            // http://stackoverflow.com/a/16868151/658622
            //------------------------------------------------------------------
            template <s32... TIdxs> struct Index {};
            template <s32 TNum, s32... TIdxs> struct GenSeq : GenSeq<TNum - 1, TNum - 1, TIdxs...> {};
            template <s32... TIdxs> struct GenSeq<0, TIdxs...> : Index<TIdxs...> {};
        }
        
        class WaitCondition
        {
        public:
            
            //------------------------------------------------
            /// Constructor 
            ///
            /// @author S Downie
            ///
            /// @param [Optional] Num times before notify
            //------------------------------------------------
            WaitCondition(u32 in_numTilNotify = 1)
            : m_notfiedCounter(in_numTilNotify)
            {
            }
            //------------------------------------------------
            /// Block the calling thread until the condition
            /// has been met
            ///
            /// @author S Downie
            //------------------------------------------------
            void Wait()
            {
                //The task thread should have locked the mutex by this point
                //and the thread that scheduled the task should be the
                //once calling this method
                std::unique_lock<std::mutex> Lock(m_mutex);
                
                while(m_notfiedCounter)
                {
                    //Wait on the scheduler mutex to become available
                    m_condition.wait(Lock);
                }
            }
            //------------------------------------------------
            /// Called by the thread executing the task when
            /// the task is complete. Once all the dependent
            /// tasks are complete then the any waiters
            /// are notified
            ///
            /// @author S Downie
            //------------------------------------------------
            void NotifyCondition()
            {
                //The thread updating the counter needs
                //exculsive access. If we have dependent tasks
                //then multiple threads may need to access this
                std::unique_lock<std::mutex> Lock(m_mutex);
                
                if((--m_notfiedCounter) == 0)
                {
                    m_condition.notify_all();
                }
            }
            
        private:
            
            std::condition_variable m_condition;
            std::mutex m_mutex;
            
            u32 m_notfiedCounter;
        };

        template <typename... TArgTypes> class Task
        {
        public:
            
            typedef std::function<void(TArgTypes...)> TaskDelegate;
            
			//------------------------------------------------------------------
			/// Constructor (Delegate object)
			///
			/// @author S Downie
			///
			/// @param Delegate function that is called when the task is executed
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			Task(const TaskDelegate& in_taskDelegate, TArgTypes... in_args)
				: m_delegate(in_taskDelegate)
				, m_args(in_args...)
				, m_waitCondition(nullptr)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Delegate object)
			///
			/// @author I Copland
			///
			/// @param Delegate function that is called when the task is executed
			/// @param Wait condition that task will notify when it completes. 
			/// This can be used to block on the task.
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			Task(const TaskDelegate& in_taskDelegate, WaitCondition* in_waitCondition, TArgTypes... in_args)
				: m_delegate(in_taskDelegate)
				, m_args(in_args...)
				, m_waitCondition(in_waitCondition)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Free delegate)
			///
			/// @author S Downie
			///
			/// @param Free function that's called on execution
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			Task(void(*in_func)(TArgTypes...), TArgTypes... in_args)
				: m_delegate(in_func)
				, m_args(in_args...)
				, m_waitCondition(nullptr)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Free delegate)
			///
			/// @author I Copland
			///
			/// @param Free function that's called on execution
			/// @param Wait condition that task will notify when it completes. 
			/// This can be used to block on the task.
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			Task(void(*in_func)(TArgTypes...), WaitCondition* in_waitCondition, TArgTypes... in_args)
				: m_delegate(in_func)
				, m_args(in_args...)
				, m_waitCondition(in_waitCondition)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Static delegate)
			///
			/// @author S Downie
			///
			/// @param Static function that's called on execution
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			template <typename TDelegate> Task(void (TDelegate::*in_func)(TArgTypes...), TArgTypes... in_args)
				: m_delegate(in_func)
				, m_args(in_args...)
				, m_waitCondition(nullptr)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Static delegate)
			///
			/// @author I Copland
			///
			/// @param Static function that's called on execution
			/// @param Wait condition that task will notify when it completes. 
			/// This can be used to block on the task.
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			template <typename TDelegate> Task(void (TDelegate::*in_func)(TArgTypes...), WaitCondition* in_waitCondition, TArgTypes... in_args)
				: m_delegate(in_func)
				, m_args(in_args...)
				, m_waitCondition(in_waitCondition)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Non-const delegate)
			///
			/// @author S Downie
			///
			/// @param Instance that's delegate function is called on execution
			/// @param Function that's called on execution
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			template <typename TDelegate, typename TSender> Task(TSender* in_sender, void (TDelegate::*in_func)(TArgTypes...), TArgTypes... in_args)
				: m_delegate(std::bind(in_func, in_sender, in_args...))
				, m_args(in_args...)
				, m_waitCondition(nullptr)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Non-const delegate)
			///
			/// @author I Copland
			///
			/// @param Instance that's delegate function is called on execution
			/// @param Function that's called on execution
			/// @param Wait condition that task will notify when it completes. 
			/// This can be used to block on the task.
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			template <typename TDelegate, typename TSender> Task(TSender* in_sender, void (TDelegate::*in_func)(TArgTypes...), WaitCondition* in_waitCondition, TArgTypes... in_args)
				: m_delegate(std::bind(in_func, in_sender, in_args...))
				, m_args(in_args...)
				, m_waitCondition(in_waitCondition)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Const delegate)
			///
			/// @author S Downie
			///
			/// @param Instance that's delegate function is called on execution
			/// @param Function that's called on execution
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			template <typename TDelegate, typename TSender> Task(TSender* in_sender, void (TDelegate::*in_func)(TArgTypes...) const, TArgTypes... in_args)
				: m_delegate(std::bind(in_func, in_sender, in_args...))
				, m_args(in_args...)
				, m_waitCondition(nullptr)
			{
			}
			//------------------------------------------------------------------
			/// Constructor (Const delegate)
			///
			/// @author I Copland
			///
			/// @param Instance that's delegate function is called on execution
			/// @param Function that's called on execution
			/// @param Wait condition that task will notify when it completes. 
			/// This can be used to block on the task.
			/// @param [Variadic] Arguments to pass to the task on execution
			//------------------------------------------------------------------
			template <typename TDelegate, typename TSender> Task(TSender* in_sender, void (TDelegate::*in_func)(TArgTypes...) const, WaitCondition* in_waitCondition, TArgTypes... in_args)
				: m_delegate(std::bind(in_func, in_sender, in_args...))
				, m_args(in_args...)
				, m_waitCondition(in_waitCondition)
			{
			}
            //------------------------------------------------------------------
            /// Executes the task function with the stored arguments and
            /// notifies the wait condition on completion
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            void operator()()
            {
                Expand(m_args);
                
                if(m_waitCondition != nullptr)
                {
                    m_waitCondition->NotifyCondition();
                }
            }
            
        private:
            
            //------------------------------------------------------------------
            /// Expand the given tuple into individual arguments.
            /// http://stackoverflow.com/a/16868151/658622
            ///
            /// @author S Downie
            ///
            /// @param Tuple
            /// @param Indices
            //------------------------------------------------------------------
            template <typename... TArgs, int... TIdxs> void Expand(std::tuple<TArgs...>& in_args, ExpansionHelpers::Index<TIdxs...>)
            {
                m_delegate(std::get<TIdxs>(in_args)...);
            }
            //------------------------------------------------------------------
            /// Expand the given tuple into individual arguments.
            /// http://stackoverflow.com/a/16868151/658622
            ///
            /// @author S Downie
            ///
            /// @param Tuple
            //------------------------------------------------------------------
            template <typename... Args> void Expand(std::tuple<Args...>& in_args)
            {
                Expand(in_args, ExpansionHelpers::GenSeq<sizeof...(Args)>{});
            }
            
        private:
            
            TaskDelegate m_delegate;
            std::tuple<typename std::remove_reference<TArgTypes>::type...> m_args;
            WaitCondition* m_waitCondition;
        };
    }
}

#endif

