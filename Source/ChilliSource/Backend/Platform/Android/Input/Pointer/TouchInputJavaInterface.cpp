/**
 * Author: Ian Copland
 * Date: 17/03/2011
 * Description: A interface class for
 */

//includes
#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/Base/CoreJavaInterface.h>

//function definitions
extern "C"
{
	void Java_com_chillisource_input_TouchInputNativeInterface_TouchDown(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY);
	void Java_com_chillisource_input_TouchInputNativeInterface_TouchUp(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY);
	void Java_com_chillisource_input_TouchInputNativeInterface_TouchMoved(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY);
}

//--------------------------------------------------------------------------------------
/// Touch Down
///
/// Interface function called from java. This is called when a touch is first registered
///
/// @param The jni environment.
/// @param the java object calling the function
/// @param the touch id
/// @param the x coordinate
/// @param the y coordinate
//--------------------------------------------------------------------------------------
void Java_com_chillisource_input_TouchInputNativeInterface_TouchDown(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY)
{
	ChilliSource::Android::TouchScreen* touchScreen = ChilliSource::Android::TouchInputJavaInterface::GetTouchScreen();
	if (touchScreen != nullptr)
	{
		touchScreen->OnTouchBegan(indwID, infX, infY);
	}
}
//--------------------------------------------------------------------------------------
/// Touch Up
///
/// Interface function called from java. This is called when a touch has finished
///
/// @param The jni environment.
/// @param the java object calling the function
/// @param the touch id
/// @param the x coordinate
/// @param the y coordinate
//--------------------------------------------------------------------------------------
void Java_com_chillisource_input_TouchInputNativeInterface_TouchUp(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY)
{
	ChilliSource::Android::TouchScreen* touchScreen = ChilliSource::Android::TouchInputJavaInterface::GetTouchScreen();
	if (touchScreen != nullptr)
	{
		touchScreen->OnTouchEnded(indwID, infX, infY);
	}
}
//--------------------------------------------------------------------------------------
/// Touch Moved
///
/// Interface function called from java. This is called when a touch has moved.
///
/// @param The jni environment.
/// @param the java object calling the function
/// @param the touch id
/// @param the x coordinate
/// @param the y coordinate
//--------------------------------------------------------------------------------------
void Java_com_chillisource_input_TouchInputNativeInterface_TouchMoved(JNIEnv* inpEnv, jobject inThis, s32 indwID, f32 infX, f32 infY)
{
	ChilliSource::Android::TouchScreen* touchScreen = ChilliSource::Android::TouchInputJavaInterface::GetTouchScreen();
	if (touchScreen != nullptr)
	{
		touchScreen->OnTouchMoved(indwID, infX, infY);
	}
}


namespace ChilliSource
{
	namespace Android
	{
		//create static member
		ChilliSource::Android::TouchScreen * TouchInputJavaInterface::mspTouchScreen = nullptr;

		//--------------------------------------------------------------------------------------
		/// Setup Java Interface
		//--------------------------------------------------------------------------------------
		void TouchInputJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;
		}
		//--------------------------------------------------------------------------------------
		/// Register Touch Screen
		//--------------------------------------------------------------------------------------
		void TouchInputJavaInterface::RegisterTouchScreen(ChilliSource::Android::TouchScreen* inpTouchScreen)
		{
			mspTouchScreen = inpTouchScreen;
		}
		//--------------------------------------------------------------------------------------
		/// Get Touch Screen
		//--------------------------------------------------------------------------------------
		ChilliSource::Android::TouchScreen* TouchInputJavaInterface::GetTouchScreen()
		{
			return mspTouchScreen;
		}
	}
}
