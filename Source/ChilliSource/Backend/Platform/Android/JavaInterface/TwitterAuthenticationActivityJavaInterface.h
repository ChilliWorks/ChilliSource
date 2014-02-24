/*
 *  TwitterAuthenticationActivityJavaInterface.h
 *  moFlow
 *
 *  Created by Robert Henning on 14/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */
#ifndef _MOFLOW_PLATFORM_ANDROID_JAVA_INTERFACE_TWITTER_AUTHENTICATION_ACTIVITY_JAVA_INTERFACE_
#define _MOFLOW_PLATFORM_ANDROID_JAVA_INTERFACE_TWITTER_AUTHENTICATION_ACTIVITY_JAVA_INTERFACE_

#include <ChilliSource/Platform/Android/JavaInterface/_JavaInterface.h>
#include <ChilliSource/Platform/Android/Social/Twitter/TwitterAuthenticationActivity.h>

namespace ChilliSource
{
	namespace Android
	{
		class SCTwitterAuthenticationActivityJavaInterface : public moFlo::AndroidPlatform::_IJavaInterface
		{
		public:
			//--------------------------------------------------------------------------------------
			/// Setup Java Interface
			///
			/// Sets up the java interface: stores the Virtual Machine, and Initialises any java
			/// methods callable from native.
			///
			/// @param a pointer to the virtual machine
			//--------------------------------------------------------------------------------------
			static void SetupJavaInterface(JavaVM* inpJavaVM);
			//-----------------------------------------------
			/// Register Activity
			///
			/// @param The Activity we want to talk to
			//-----------------------------------------------
			static void RegisterActivity(moFlo::AndroidPlatform::CTwitterAuthenticationActivity* inpActivity);
			//-----------------------------------------------
			/// Present
			///
			/// Starts the activity by presenting a view with
			/// a 'busy, please wait' message
			//-----------------------------------------------
			static void Present();
			//-----------------------------------------------
			/// Presents the authorise URL
			///
			/// Set the URL received from Twitter to visit
			/// in order to see PIN
			///
			/// @param URL
			//-----------------------------------------------
			static void PresentURL(const std::string& instrURL);
			//-----------------------------------------------
			/// On PIN Complete
			///
			/// Give the register activity the enter PIN
			///
			/// @param PIN entered by user
			//-----------------------------------------------
			static void OnPINComplete(const std::string instrPIN);

		private:
			static moFlo::AndroidPlatform::CTwitterAuthenticationActivity*		mpActivity;
		};
	}
}

#endif
