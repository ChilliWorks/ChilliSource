/**
 * Author: Ian Copland
 * Date: 17/03/2011
 * Description: A interface class for
 */

//includes
#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Input/Pointer/PointerSystem.h>

extern "C"
{
	//-------------------------------------------------------
	/// Interface function called from java. This is called
	/// when a touch is first registered.
	///
	/// @author I Copland
	///
	/// @param The jni environment.
	/// @param the java object calling the function
	/// @param the touch id
	/// @param the x coordinate
	/// @param the y coordinate
	//-------------------------------------------------------
	void Java_com_chillisource_input_TouchInputNativeInterface_TouchDown(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y);
	//-------------------------------------------------------
	/// Interface function called from java. This is called
	/// when a touch has moved.
	///
	/// @author I Copland
	///
	/// @param The jni environment.
	/// @param the java object calling the function
	/// @param the touch id
	/// @param the x coordinate
	/// @param the y coordinate
	//-------------------------------------------------------
	void Java_com_chillisource_input_TouchInputNativeInterface_TouchMoved(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y);
	//-------------------------------------------------------
	/// Interface function called from java. This is called
	/// when a touch has finished.
	///
	/// @author I Copland
	///
	/// @param The jni environment.
	/// @param the java object calling the function
	/// @param the touch id
	/// @param the x coordinate
	/// @param the y coordinate
	//-------------------------------------------------------
	void Java_com_chillisource_input_TouchInputNativeInterface_TouchUp(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y);
}
//-------------------------------------------------------
//-------------------------------------------------------
void Java_com_chillisource_input_TouchInputNativeInterface_TouchDown(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y)
{
	ChilliSource::Android::PointerSystem* pointerSystem = ChilliSource::Android::TouchInputJavaInterface::GetPointerSystem();
	if (pointerSystem != nullptr)
	{
		pointerSystem->OnTouchDown(in_systemId, ChilliSource::Core::Vector2(in_x, in_y));
	}
}
//-------------------------------------------------------
//-------------------------------------------------------
void Java_com_chillisource_input_TouchInputNativeInterface_TouchMoved(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y)
{
	ChilliSource::Android::PointerSystem* pointerSystem = ChilliSource::Android::TouchInputJavaInterface::GetPointerSystem();
	if (pointerSystem != nullptr)
	{
		pointerSystem->OnTouchMoved(in_systemId, ChilliSource::Core::Vector2(in_x, in_y));
	}
}
//-------------------------------------------------------
//-------------------------------------------------------
void Java_com_chillisource_input_TouchInputNativeInterface_TouchUp(JNIEnv* in_environment, jobject in_this, s32 in_systemId, f32 in_x, f32 in_y)
{
	ChilliSource::Android::PointerSystem* pointerSystem = ChilliSource::Android::TouchInputJavaInterface::GetPointerSystem();
	if (pointerSystem != nullptr)
	{
		pointerSystem->OnTouchUp(in_systemId);
	}
}

namespace ChilliSource
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
