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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/_JavaInterface.h>

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
			//---------------------------------------------------
			/// Sets the Pointer system that should be used. If
			/// null is passed to this, no pointer system will
			/// be used.
			///
			/// @author I Copland
			///
			/// @param The pointer system.
			//---------------------------------------------------
			static void SetPointerSystem(PointerSystem* in_pointerSystem);
			//---------------------------------------------------
			/// @author I Copland
			///
			/// @return The pointer system.
			//---------------------------------------------------
			static PointerSystem* GetPointerSystem();
		private:
			static PointerSystem* s_pointerSystem;
		};
	}
}

#endif
