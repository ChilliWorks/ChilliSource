/*
 * File: Thread.cpp
 * Date: 02/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Core/Thread.h>

namespace moFlo
{
	//----------------------------------------------------
	/// Constructor
	///
	/// Default
	//----------------------------------------------------
	CThread::CThread() : mpThread(NULL)
	{
	}
	//----------------------------------------------------
	/// Stop
	///
	/// Force the thread to suspend and rejoin the main
	/// thread. This will also destroy the thread
	//----------------------------------------------------
	void CThread::Stop()
	{
		if(mpThread)
		{
			//Join this thread back with the main one
			mpThread->join();
			
			SAFE_DELETE(mpThread);
		}
	}
	//----------------------------------------------------
	/// Yield
	///
	/// Causes the thread to give up its timeslice
	//----------------------------------------------------
	void CThread::Yield()
	{
		CThread::Thread::yield();
	}
    //----------------------------------------------------
    /// Sleep
    ///
    /// Causes the thread to suspend for a specified
    /// time
    ///
    /// @param Time to suspend in milliseconds
    //----------------------------------------------------
    void CThread::Sleep(const u32 inudwMS)
    {
        boost::xtime xt;
        boost::xtime_get(&xt, boost::TIME_UTC);
        
        xt.nsec += inudwMS * 1000;
        
        boost::thread::sleep(xt);
    }
	//----------------------------------------------------
	/// Is Active
	///
	/// @return Whether the thread is in progress
	//----------------------------------------------------
	bool CThread::IsActive() const
	{
		return (mpThread != NULL);
	}
	//----------------------------------------------------
	/// Destructor
	///
	//----------------------------------------------------
	CThread::~CThread()
	{
		Stop();
	}
}