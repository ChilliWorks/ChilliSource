/*
 *  ApplicationDelegates.h
 *  MoFlow
 *
 *  Created by Ian Copland on 10/01/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_CORE_APPLICATIONDELEGATES_H_
#define _MOFLOW_CORE_APPLICATIONDELEGATES_H_

#include <ChilliSource/Core/Base/CustomCreator.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace moFlo
{
	class IApplicationDelegates : public ICustomCreator<IApplicationDelegates>
	{
	public:
		virtual ~IApplicationDelegates(){};
		//----------------------------------------------
		/// SetInvokeResumeEvent
		///
		/// This tells the application to call "On
		/// Resume" during the next update.
		//----------------------------------------------
		virtual void SetInvokeResumeEvent(bool inbResume) = 0;
		//----------------------------------------------
		/// On Application Resumed
		///
		/// This is called whenever the application is
		/// resumed.
		//----------------------------------------------
		virtual void OnApplicationResumed() = 0;
		//----------------------------------------------
		/// On Application Suspended
		///
		/// This is called whenever the application is
		/// suspended.
		//----------------------------------------------
		virtual void OnApplicationSuspended() = 0;
		//----------------------------------------------
		/// On Frame Begin
		///
		/// This is called whenever a frame starts
		///
		/// @param the delta time
		/// @param the timestamp
		//----------------------------------------------
		virtual void OnFrameBegin(f32 infDt, TimeIntervalSecs inuddwTimestamp) = 0;
		//----------------------------------------------
		/// On Screen Changed Orientation
		///
		/// This is called whenever the orientation
		/// changes
		///
		/// @param the orientation
		//----------------------------------------------
		virtual void OnScreenChangedOrientation(Core::ScreenOrientation ineOrientation) = 0;
		//----------------------------------------------
		/// On Screen Resized
		///
		/// This is called whenever the screen is resized
		///
		/// @param the new width
		/// @param the new height
		//----------------------------------------------
		virtual void OnScreenResized(u32 inudwWidth, u32 inudwHeight) = 0;
		//----------------------------------------------
		/// On Application Memory Warning
		///
		/// This is called whenever the application
		/// received a memory warning.
		//----------------------------------------------
		virtual void OnApplicationMemoryWarning() = 0;
		//----------------------------------------------
		/// On Go Back
		///
		/// This is called whenever the application
		/// is told to "go back".
		//----------------------------------------------
		virtual void OnGoBack() = 0;
		//----------------------------------------------
		/// On Set Orientiation
		///
		/// This is called whenever the application
		/// orientation is set.
		///
		/// @param the new orientation
		//----------------------------------------------
		virtual void OnSetOrientation(Core::ScreenOrientation inOrientation) = 0;
	};
}

#endif
