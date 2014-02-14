/*
 *  ApplicationDelegates.h
 *  MoFlow
 *
 *  Created by Ian Copland on 10/01/2012.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */


#ifndef _MOFLOW_ANDROIDPLATFORM_APPLICATIONDELEGATES_H_
#define _MOFLOW_ANDROIDPLATFORM_APPLICATIONDELEGATES_H_

#include <ChilliSource/Core/Main/ApplicationDelegates.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		class CApplicationDelegates : public IApplicationDelegates
		{
			DECLARE_CREATABLE(IApplicationDelegates, CApplicationDelegates);
		public:
			//----------------------------------------------
			/// SetInvokeResumeEvent
			///
			/// This tells the application to call "On
			/// Resume" during the next update.
			//----------------------------------------------
			void SetInvokeResumeEvent(bool inbResume);
			//----------------------------------------------
			/// On Application Resumed
			///
			/// This is called whenever the application is
			/// resumed.
			//----------------------------------------------
			void OnApplicationResumed();
			//----------------------------------------------
			/// On Application Suspended
			///
			/// This is called whenever the application is
			/// suspended.
			//----------------------------------------------
			void OnApplicationSuspended();
			//----------------------------------------------
			/// On Frame Begin
			///
			/// This is called whenever a frame starts
			///
			/// @param the delta time
			/// @param the timestamp
			//----------------------------------------------
			void OnFrameBegin(f32 infDt, TimeIntervalSecs inuddwTimestamp);
			//----------------------------------------------
			/// On Screen Changed Orientation
			///
			/// This is called whenever the orientation
			/// changes
			///
			/// @param the orientation
			//----------------------------------------------
			void OnScreenChangedOrientation(Core::ScreenOrientation ineOrientation);
			//----------------------------------------------
			/// On Screen Resized
			///
			/// This is called whenever the screen is resized
			///
			/// @param the new width
			/// @param the new height
			//----------------------------------------------
			void OnScreenResized(u32 inudwWidth, u32 inudwHeight);
			//----------------------------------------------
			/// On Application Memory Warning
			///
			/// This is called whenever the application
			/// received a memory warning.
			//----------------------------------------------
			void OnApplicationMemoryWarning();
			//----------------------------------------------
			/// On Go Back
			///
			/// This is called whenever the application
			/// is told to "go back".
			//----------------------------------------------
			void OnGoBack();
			//----------------------------------------------
			/// On Set Orientiation
			///
			/// This is called whenever the application
			/// orientation is set.
			///
			/// @param the new orientation
			//----------------------------------------------
			void OnSetOrientation(Core::ScreenOrientation inOrientation);
			//----------------------------------------------
			/// Destructor
			//----------------------------------------------
			virtual ~CApplicationDelegates();
		protected:
			//----------------------------------------------
			/// Constructor
			//----------------------------------------------
			CApplicationDelegates();
		private:

			f32 mfUpdateIntervalRemainder;
			bool mbShouldInvokeResumeEvent;
			bool mbIsFirstFrame;
            bool mbIsSuspending;
		};
	}
}

#endif
