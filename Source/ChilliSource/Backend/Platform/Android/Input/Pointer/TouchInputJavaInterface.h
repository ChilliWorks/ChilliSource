/**
 * File: TouchInputJavaInterface.h
 * Date: 05/05/2011
 * Description: Interface functions for calls to and from java.
 */

/**
 *
 * Author: Ian Copland
 * Version 1.0 - moFlo
 * Copyright �2010 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLO_PLATFORM_ANDROID_TOUCH_INPUT_JAVA_INTERFACE_JAVA_INTERFACE_
#define _MOFLO_PLATFORM_ANDROID_TOUCH_INPUT_JAVA_INTERFACE_JAVA_INTERFACE_

#include <ChilliSource/Backend/Platform/Android/Core/JNI/_JavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchScreen.h>

#include <jni.h>

#include <map>

namespace ChilliSource
{
	namespace Android
	{
		class TouchInputJavaInterface : public _IJavaInterface
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
			static void RegisterTouchScreen(TouchScreen* inpTouchScreen);
			//--------------------------------------------------------------------------------------
			/// Get Touch Screen
			///
			/// @return CTouchScreen* returns the currently active touch screen or null.
			//--------------------------------------------------------------------------------------
			static TouchScreen* GetTouchScreen();
		private:
			static TouchScreen * mspTouchScreen;
		};
	}
}

#endif
