/**
 * File: TouchInputJavaInterface.h
 * Date: 05/05/2011
 * Description: Interface functions for calls to and from java.
 */

/**
 *
 * Author: Ian Copland
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLO_PLATFORM_ANDROID_TOUCH_INPUT_JAVA_INTERFACE_JAVA_INTERFACE_
#define _MOFLO_PLATFORM_ANDROID_TOUCH_INPUT_JAVA_INTERFACE_JAVA_INTERFACE_

#include <map>
#include <moFlo/Platform/Android/JavaInterface/_JavaInterface.h>
#include <moFlo/Platform/Android/Input/TouchScreen.h>
#include <jni.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		class SCTouchInputJavaInterface : public _IJavaInterface
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
			//--------------------------------------------------------------------------------------
			/// Register Touch Screen
			///
			/// Registers the current android touch screen that is to be used.
			///
			/// @param CTouchScreen* a pointer to the android touch screen.
			//--------------------------------------------------------------------------------------
			static void RegisterTouchScreen(CTouchScreen* inpTouchScreen);
			//--------------------------------------------------------------------------------------
			/// Get Touch Screen
			///
			/// @return CTouchScreen* returns the currently active touch screen or null.
			//--------------------------------------------------------------------------------------
			static CTouchScreen* GetTouchScreen();
		private:
			static CTouchScreen * mspTouchScreen;
		};
	}
}

#endif
