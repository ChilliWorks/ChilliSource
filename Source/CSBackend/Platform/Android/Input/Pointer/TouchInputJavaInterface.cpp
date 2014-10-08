//
//  TouchInputJavaInterface.cpp
//  Chilli Source
//  Created by Ian Copland on 05/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>

#include <CSBackend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <CSBackend/Platform/Android/Input/Pointer/PointerSystem.h>

extern "C"
{
	//-------------------------------------------------------
	/// Interface function called from java. This is called
	/// when a touch is first registered.
	///
	/// @author Ian Copland
	///
	/// @param The jni environment.
	/// @param the java object calling the function
	/// @param the touch id
	/// @param the x coordinate
	/// @param the y coordinate
	//-------------------------------------------------------
	void Java_com_chilliworks_chillisource_input_TouchInputNativeInterface_TouchDown(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y);
	//-------------------------------------------------------
	/// Interface function called from java. This is called
	/// when a touch has moved.
	///
	/// @author Ian Copland
	///
	/// @param The jni environment.
	/// @param the java object calling the function
	/// @param the touch id
	/// @param the x coordinate
	/// @param the y coordinate
	//-------------------------------------------------------
	void Java_com_chilliworks_chillisource_input_TouchInputNativeInterface_TouchMoved(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y);
	//-------------------------------------------------------
	/// Interface function called from java. This is called
	/// when a touch has finished.
	///
	/// @author Ian Copland
	///
	/// @param The jni environment.
	/// @param the java object calling the function
	/// @param the touch id
	/// @param the x coordinate
	/// @param the y coordinate
	//-------------------------------------------------------
	void Java_com_chilliworks_chillisource_input_TouchInputNativeInterface_TouchUp(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y);
}
//-------------------------------------------------------
//-------------------------------------------------------
void Java_com_chilliworks_chillisource_input_TouchInputNativeInterface_TouchDown(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y)
{
	CSBackend::Android::PointerSystem* pointerSystem = CSBackend::Android::TouchInputJavaInterface::GetPointerSystem();
	CS_ASSERT(pointerSystem, "Touch Input Java Interface: No pointer system!");
	pointerSystem->OnTouchDown(in_systemId, CSCore::Vector2(in_x, in_y));
}
//-------------------------------------------------------
//-------------------------------------------------------
void Java_com_chilliworks_chillisource_input_TouchInputNativeInterface_TouchMoved(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y)
{
	CSBackend::Android::PointerSystem* pointerSystem = CSBackend::Android::TouchInputJavaInterface::GetPointerSystem();
	CS_ASSERT(pointerSystem, "Touch Input Java Interface: No pointer system!");
	pointerSystem->OnTouchMoved(in_systemId, CSCore::Vector2(in_x, in_y));
}
//-------------------------------------------------------
//-------------------------------------------------------
void Java_com_chilliworks_chillisource_input_TouchInputNativeInterface_TouchUp(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y)
{
	CSBackend::Android::PointerSystem* pointerSystem = CSBackend::Android::TouchInputJavaInterface::GetPointerSystem();
	CS_ASSERT(pointerSystem, "Touch Input Java Interface: No pointer system!");
	pointerSystem->OnTouchUp(in_systemId);
}

namespace CSBackend
{
	namespace Android
	{
		PointerSystem* TouchInputJavaInterface::s_pointerSystem = nullptr;
		//--------------------------------------------------------------------------------------
		/// Setup Java Interface
		//--------------------------------------------------------------------------------------
		void TouchInputJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void TouchInputJavaInterface::SetPointerSystem(PointerSystem* in_pointerSystem)
		{
			s_pointerSystem = in_pointerSystem;
		}
		//---------------------------------------------------
		//---------------------------------------------------
		PointerSystem* TouchInputJavaInterface::GetPointerSystem()
		{
			return s_pointerSystem;
		}
	}
}

#endif