/*
 *  Tasks.hpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 15/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_TASKS_H_
#define _MO_FLO_CORE_TASKS_H_

#include <ChilliSource/Core/FastDelegate.h>

#include <ChilliSource/Core/boost/thread/condition.hpp>
#include <ChilliSource/Core/boost/type_traits/remove_reference.hpp>

namespace moFlo
{
	//==========================================================
	/// Wait Condition
	///
	/// Holds a mutex and a condition which a can be used to
	/// wait on the execution of a task
	//==========================================================
	class CWaitCondition
	{
	public:
		
		//------------------------------------
		/// Constructor 
		///
		/// Default - Waiting on one task
		//------------------------------------
		CWaitCondition(u32 inudwNumTasks = 1) : mudwCounter(inudwNumTasks)
		{
		}
		//------------------------------------
		/// Wait 
		///
		/// Block the calling thread until
		/// the condition has been met
		//------------------------------------
		void Wait()
		{
			//The task thread should have locked the mutex by this point
			//and the thread that scheduled the task should be the
			//once calling this method
			boost::mutex::scoped_lock Lock(mMutex);
			
			while(mudwCounter)
			{
				//Wait on the scheduler mutex to become available
				mCondition.wait(Lock);
			}
		}
		//------------------------------------
		/// Update Condition 
		///
		/// Called by the thread executing
		/// the task when the task is complete
		/// Once all the dependent tasks are 
		/// complete then the any waiters
		/// are notified
		//------------------------------------
		void UpdateCondition()
		{
			//The thread updating the counter needs
			//exculsive access. If we have dependent tasks
			//then multiple threads may need to access this
			boost::mutex::scoped_lock Lock(mMutex);
			
			if((--mudwCounter) == 0)
			{
				mCondition.notify_all();
			}
		}
		
	private:
		
		boost::condition mCondition;
		boost::mutex mMutex;
		
		u32 mudwCounter;
	};
	//==========================================================
	/// Tasks
	///
	/// Holds a delegate and parameters on which execution
	/// can be deferred
	//==========================================================
	//---Task 0
	struct Task0
	{
		fastdelegate::FastDelegate0<> TaskDelegate;
		CWaitCondition* pWaitCondition;
		
		Task0(const fastdelegate::FastDelegate0<>& inTaskDelegate, CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inTaskDelegate), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task0(TSender* inpSender, void (TDelegate::*func)(), CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inpSender, func), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task0(TSender* inpSender, void (TDelegate::*func)() const, CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inpSender, func), pWaitCondition(inpWaitCondition)
		{
		}
		
		void operator()()
		{
			TaskDelegate();
			
			if(pWaitCondition)
			{
				pWaitCondition->UpdateCondition();
			}
		}
	};
	//---Task 1
	template <typename T> struct Task1
	{
		fastdelegate::FastDelegate1<T> TaskDelegate;
		typename boost::remove_reference<T>::type Arg1;
		CWaitCondition* pWaitCondition;
		
		Task1(const fastdelegate::FastDelegate1<T>& inTaskDelegate, 
			  T inArg1, 
			  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inTaskDelegate), Arg1(inArg1), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task1(TSender* inpSender, void (TDelegate::*func)(T inArg1), 
															  T inArg1, 
															  CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inpSender, func), Arg1(inArg1), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task1(TSender* inpSender, void (TDelegate::*func)(T inArg1) const, 
															  T inArg1, 
															  CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inpSender, func), Arg1(inArg1), pWaitCondition(inpWaitCondition)
		{
		}
		
		void operator()()
		{
			TaskDelegate(Arg1);
			
			if(pWaitCondition)
			{
				pWaitCondition->UpdateCondition();
			}
		}
	};
	//---Task 2
	template <typename T1, typename T2> struct Task2
	{
		fastdelegate::FastDelegate2<T1, T2> TaskDelegate;
		typename boost::remove_reference<T1>::type Arg1;
		typename boost::remove_reference<T2>::type Arg2;
		CWaitCondition* pWaitCondition;
		
		Task2(const fastdelegate::FastDelegate2<T1, T2>& inTaskDelegate, 
			  T1 inArg1, T2 inArg2, 
			  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inTaskDelegate), Arg1(inArg1), Arg2(inArg2), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task2(TSender* inpSender, void (TDelegate::*func)(T1 inArg1, T2 inArg2), 
															  T1 inArg1, T2 inArg2, 
															  CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inpSender, func), Arg1(inArg1), Arg2(inArg2), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task2(TSender* inpSender, void (TDelegate::*func)(T1 inArg1, T2 inArg2) const, 
															  T1 inArg1, T2 inArg2, 
															  CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inpSender, func), Arg1(inArg1), Arg2(inArg2), pWaitCondition(inpWaitCondition)
		{
		}
		
		void operator()()
		{
			TaskDelegate(Arg1, Arg2);
			
			if(pWaitCondition)
			{
				pWaitCondition->UpdateCondition();
			}
		}
	};
	//---Task 3
	template <typename T1, typename T2, typename T3> struct Task3
	{
		fastdelegate::FastDelegate3<T1, T2, T3> TaskDelegate;
		typename boost::remove_reference<T1>::type Arg1;
		typename boost::remove_reference<T2>::type Arg2;
		typename boost::remove_reference<T3>::type Arg3;
		CWaitCondition* pWaitCondition;
		
		Task3(const fastdelegate::FastDelegate3<T1, T2, T3>& inTaskDelegate, 
			  T1 inArg1, T2 inArg2, T3 inArg3, 
			  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inTaskDelegate), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task3(TSender* inpSender, void (TDelegate::*func)(T1 inArg1, T2 inArg2, T3 inArg3), 
															  T1 inArg1, T2 inArg2, T3 inArg3, 
															  CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inpSender, func), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task3(TSender* inpSender, void (TDelegate::*func)(T1 inArg1, T2 inArg2, T3 inArg3) const, 
															  T1 inArg1, T2 inArg2, T3 inArg3, 
															  CWaitCondition* inpWaitCondition = NULL) 
		: TaskDelegate(inpSender, func), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), pWaitCondition(inpWaitCondition)
		{
		}
		
		void operator()()
		{
			TaskDelegate(Arg1, Arg2, Arg3);
			
			if(pWaitCondition)
			{
				pWaitCondition->UpdateCondition();
			}
		}
	};
	//---Task 4
	template <typename T1, typename T2, typename T3, typename T4> struct Task4
	{
		fastdelegate::FastDelegate4<T1, T2, T3, T4> TaskDelegate;
		typename boost::remove_reference<T1>::type Arg1;
		typename boost::remove_reference<T2>::type Arg2;
		typename boost::remove_reference<T3>::type Arg3;
		typename boost::remove_reference<T4>::type Arg4;
		CWaitCondition* pWaitCondition;
		
		Task4(const fastdelegate::FastDelegate4<T1, T2, T3, T4>& inTaskDelegate,
			  T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4,
			  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inTaskDelegate), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), Arg4(inArg4), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task4(TSender* inpSender, void (TDelegate::*func)(T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4),
															  T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4,
															  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inpSender, func), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), Arg4(inArg4), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task4(TSender* inpSender, void (TDelegate::*func)(T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4) const,
															  T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4,
															  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inpSender, func), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), Arg4(inArg4), pWaitCondition(inpWaitCondition)
		{
		}
		
		void operator()()
		{
			TaskDelegate(Arg1, Arg2, Arg3, Arg4);
			
			if(pWaitCondition)
			{
				pWaitCondition->UpdateCondition();
			}
		}
	};
    //---Task5
	template <typename T1, typename T2, typename T3, typename T4, typename T5> struct Task5
	{
		fastdelegate::FastDelegate5<T1, T2, T3, T4, T5> TaskDelegate;
		typename boost::remove_reference<T1>::type Arg1;
		typename boost::remove_reference<T2>::type Arg2;
		typename boost::remove_reference<T3>::type Arg3;
		typename boost::remove_reference<T4>::type Arg4;
        typename boost::remove_reference<T5>::type Arg5;
		CWaitCondition* pWaitCondition;
		
		Task5(const fastdelegate::FastDelegate4<T1, T2, T3, T4, T5>& inTaskDelegate,
			  T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4, T5 inArg5,
			  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inTaskDelegate), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), Arg4(inArg4), Arg5(inArg5), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task5(TSender* inpSender, void (TDelegate::*func)(T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4, T5 inArg5),
															  T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4, T5 inArg5,
															  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inpSender, func), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), Arg4(inArg4), Arg5(inArg5), pWaitCondition(inpWaitCondition)
		{
		}
		
		template <typename TDelegate, typename TSender> Task5(TSender* inpSender, void (TDelegate::*func)(T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4, T5 inArg5) const,
															  T1 inArg1, T2 inArg2, T3 inArg3, T4 inArg4, T5 inArg5,
															  CWaitCondition* inpWaitCondition = NULL)
		: TaskDelegate(inpSender, func), Arg1(inArg1), Arg2(inArg2), Arg3(inArg3), Arg4(inArg4), Arg5(inArg5), pWaitCondition(inpWaitCondition)
		{
		}
		
		void operator()()
		{
			TaskDelegate(Arg1, Arg2, Arg3, Arg4, Arg5);
			
			if(pWaitCondition)
			{
				pWaitCondition->UpdateCondition();
			}
		}
	};
}

#endif

