/*
 *  CoreJavaInterface.h
 *  moFlow
 *
 *  Created by Ian Copland on 17/03/2011.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/CoreJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Base/LaunchingActions.h>

#include <ChilliSource/Backend/Platform/Android/JavaInterface/TouchInputJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/HttpConnectionJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/SharedPreferencesJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/WebViewJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/ContactInformationProviderJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/SMSCompositionJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/TwitterAuthenticationActivityJavaInterface.h>

//--------------------------------------------------------------------------------------
/// Globals
//--------------------------------------------------------------------------------------
ChilliSource::Core::Application* (*BootFunctionPtr)() = NULL;
//--------------------------------------------------------------------------------------
/// C function declarations
//--------------------------------------------------------------------------------------
extern "C"
{
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_SetupCoreJavaNativeInterface(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_Initialise(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_Resume(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_Suspend(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_DestroyApplication(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_FrameBegin(JNIEnv* inpEnv, jobject inThis, f32 infDeltaTime, s64 inddwTimestamp);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_MemoryWarning(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_OrientationChanged(JNIEnv* inpEnv, jobject inThis, s32 indwOrientation);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_OnBackPressed(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_OnDialogConfirmPressed(JNIEnv* inpEnv, jobject inThis, s32 indwID);
	void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_OnDialogCancelPressed(JNIEnv* inpEnv, jobject inThis, s32 indwID);
    void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_ApplicationDidReceiveLaunchingURL(JNIEnv* inpEnv, jobject inThis, jstring instrURL);
}
//--------------------------------------------------------------------------------------
/// Setup Core Java Native Interface
///
/// Setups the Core Java and Native Interfaces.
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_SetupCoreJavaNativeInterface(JNIEnv* inpEnv, jobject inThis)
{
	//get the java VM and init the Java Interface Manager
	JavaVM * pJavaVM;
	inpEnv->GetJavaVM(&pJavaVM);
	ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->Initialise(pJavaVM);

	//add the core native interface
	ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(ChilliSource::Android::JavaInterfacePtr(new ChilliSource::Android::CCoreJavaInterface()));
}
//--------------------------------------------------------------------------------------
/// Initialise
///
/// Interface function called from java. This is called when the application is started.
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_Initialise(JNIEnv* inpEnv, jobject inThis)
{
	//create the application
	ChilliSource::Core::Application* pApplication = BootFunctionPtr();
	ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CCoreJavaInterface>()->SetApplication(pApplication);

	//setup other interfaces
	JavaVM * pJavaVM;
	inpEnv->GetJavaVM(&pJavaVM);
	ChilliSource::Android::SCTouchInputJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::SCHttpConnectionJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::SCSharedPreferencesJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::SCWebViewJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::SCSMSCompositionJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::SCContactInformationProviderJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::SCTwitterAuthenticationActivityJavaInterface::SetupJavaInterface(pJavaVM);
    
	//run the application
    pApplication->Run();
}

//--------------------------------------------------------------------------------------
/// Resume
///
/// Interface function called from java. This is called when the application is resumed.
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_Resume(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Core::Application::Resume();
}
//--------------------------------------------------------------------------------------
/// Suspend
///
/// Interface function called from java. This is called when the application is suspended
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_Suspend(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Core::Application::Suspend();
}
//--------------------------------------------------------------------------------------
/// Destroy
///
/// Interface function called from java. This is called when the application is destroyed
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_DestroyApplication(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CCoreJavaInterface>()->DestroyApplication();
}
//--------------------------------------------------------------------------------------
/// Frame Begin
///
/// Interface function called from java. This is called when the application is updated
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
/// @param the delta time.
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_FrameBegin(JNIEnv* inpEnv, jobject inThis, f32 infDeltaTime, s64 inddwTimestamp)
{
	//Create the message with the time between frames
	ChilliSource::Core::Application::OnFrameBegin((f32)infDeltaTime, (u64)inddwTimestamp);
}
//--------------------------------------------------------------------------------------
/// Memory Warning
///
/// Interface function called from java. This is called when the application is almost
/// Out of memory.
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_MemoryWarning(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Core::Application::OnApplicationMemoryWarning();
}
//--------------------------------------------------------------------------------------
/// Orientation Changed
///
/// Interface function called from java. This is called when the orientation has changed.
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
/// @param the new orientation as an int that maps to the orientation enum.
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_OrientationChanged(JNIEnv* inpEnv, jobject thiz, s32 indwOrientation)
{
	ChilliSource::Core::ScreenOrientation eScreenOrientation;

	if (indwOrientation == 1)
	{
		eScreenOrientation = ChilliSource::Core::PORTRAIT_UP;
		DEBUG_LOG("Changing orientation to portrait");
	}
	else
	{
		eScreenOrientation = ChilliSource::Core::LANDSCAPE_LEFT;
		DEBUG_LOG("Changing orientation to landscape");
	}

	ChilliSource::Core::Application::OnScreenChangedOrientation(eScreenOrientation);
}
//--------------------------------------------------------------------------------------
/// On Back Pressed
///
/// Interface function called from java. This is called when the back hardware button is
/// pressed
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_OnBackPressed(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Core::Application::OnGoBack();
}
//--------------------------------------------------------------------------------------
/// On Dialog Confirm Pressed
///
/// Interface function called from java. This is called when a system dialog has it's
/// confirm option pressed
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
/// @param Dialog ID
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_OnDialogConfirmPressed(JNIEnv* inpEnv, jobject inThis, s32 indwID)
{
	ChilliSource::Core::Application::OnSystemConfirmDialogResult((u32)indwID, ChilliSource::SystemConfirmDialog::CONFIRM);
}
//--------------------------------------------------------------------------------------
/// On Dialog Cancel Pressed
///
/// Interface function called from java. This is called when a system dialog has it's
/// cancel option pressed
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
/// @param Dialog ID
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_OnDialogCancelPressed(JNIEnv* inpEnv, jobject inThis, s32 indwID)
{
	ChilliSource::Core::Application::OnSystemConfirmDialogResult((u32)indwID, ChilliSource::SystemConfirmDialog::CANCEL);
}
//--------------------------------------------------------------------------------------
/// Application Did Receive Launching URL
///
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
/// @param URL
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CCoreNativeInterface_ApplicationDidReceiveLaunchingURL(JNIEnv* inpEnv, jobject inThis, jstring instrURL)
{
	ChilliSource::CLaunchingActions::ApplicationDidReceiveLaunchingURL(ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrURL));
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(CCoreJavaInterface);

		//--------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------
		CCoreJavaInterface::CCoreJavaInterface()
		: mApplication(NULL), mdwScreenWidth(0), mdwScreenHeight(0), mdwOSVersionCode(-1), mdwNumberOfCores(-1), mfScreenDensity(-0.1f), mfPhysicalScreenSize(-0.1f)
		{
			CreateNativeInterface("com/taggames/moflow/nativeinterface/CCoreNativeInterface");
			CreateMethodReference("GetExternalStorageDirectory", "()Ljava/lang/String;");
			CreateMethodReference("GetApplicationName", "()Ljava/lang/String;");
			CreateMethodReference("GetApplicationVersionCode", "()I");
			CreateMethodReference("GetApplicationVersionName", "()Ljava/lang/String;");
			CreateMethodReference("GetPackageName", "()Ljava/lang/String;");
			CreateMethodReference("GetAPKDirectory", "()Ljava/lang/String;");
			CreateMethodReference("GetOrientation", "()I");
			CreateMethodReference("GetOrientationLandscapeConstant", "()I");
			CreateMethodReference("GetOrientationPortraitConstant", "()I");
			CreateMethodReference("GetScreenWidth", "()I");
			CreateMethodReference("GetScreenHeight", "()I");
			CreateMethodReference("GetDefaultLocaleCode", "()Ljava/lang/String;");
			CreateMethodReference("GetDeviceModel", "()Ljava/lang/String;");
			CreateMethodReference("GetDeviceManufacturer", "()Ljava/lang/String;");
			CreateMethodReference("GetDeviceModelType", "()Ljava/lang/String;");
			CreateMethodReference("GetOSVersion", "()I");
			CreateMethodReference("GetNumberOfCores", "()I");
			CreateMethodReference("GetScreenDensity", "()F");
			CreateMethodReference("GetTelephonyDeviceID", "()Ljava/lang/String;");
			CreateMethodReference("GetMacAddress", "()Ljava/lang/String;");
			CreateMethodReference("GetAndroidID", "()Ljava/lang/String;");
			CreateMethodReference("MakeToast", "(Ljava/lang/String;)V");
			CreateMethodReference("ForceQuit", "()V");
			CreateMethodReference("ShowSystemConfirmDialog", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			CreateMethodReference("ShowSystemDialog", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			CreateMethodReference("GetSystemTimeInMilliseconds", "()J");
			CreateMethodReference("GetPhysicalScreenSize", "()F");
			CreateMethodReference("SetMaxFPS", "(I)V");
		}
		//--------------------------------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------------------------------
		bool CCoreJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CCoreJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------------------------------
		/// Set Application
		//--------------------------------------------------------------------------------------
		void CCoreJavaInterface::SetApplication(ChilliSource::Core::Application* inApplication)
		{
			mApplication = inApplication;
		}
		//--------------------------------------------------------------------------------------
		/// Destroy Application
		//--------------------------------------------------------------------------------------
		void CCoreJavaInterface::DestroyApplication()
		{
			delete mApplication;
			mApplication = NULL;
		}
		//--------------------------------------------------------------------------------------
		/// Get Application
		//--------------------------------------------------------------------------------------
		ChilliSource::Core::Application* CCoreJavaInterface::GetApplication()
		{
			return mApplication;
		}
		//-----------------------------------------
		/// Set Max FPS
		//-----------------------------------------
		void CCoreJavaInterface::SetMaxFPS(u32 inudwFPS)
		{
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("SetMaxFPS"), inudwFPS);
		}
		//--------------------------------------------------------------------------------------
		/// Get External Storage Directory
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetExternalStorageDirectory()
		{
			std::string sdCardRootPath = "";
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrExternalStorageDir = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetExternalStorageDirectory")));
			sdCardRootPath = JavaInterfaceUtils::CreateSTDStringFromJString(jstrExternalStorageDir);
			env->DeleteLocalRef(jstrExternalStorageDir);
			return sdCardRootPath;
		}
		//--------------------------------------------------------------------------------------
		/// Get Application Name
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetApplicationName()
		{
			std::string appName = "";
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrName = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetApplicationName")));
			appName = JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);
			env->DeleteLocalRef(jstrName);
			return appName;
		}
		//--------------------------------------------------------------------------------------
		/// Get Application Version Code
		///
		/// @return returns the Application version code as acquired from the CoreNativeInterface
		/// Java class.
		//--------------------------------------------------------------------------------------
		u32 CCoreJavaInterface::GetApplicationVersionCode()
		{
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallIntMethod(GetJavaObject(), GetMethodID("GetApplicationVersionCode"));
		}
		//--------------------------------------------------------------------------------------
		/// Get Application Version Name
		///
		/// @return returns the Application version name as acquired from the CoreNativeInterface
		/// Java class.
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetApplicationVersionName()
		{
			std::string appVersion = "";
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrName = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetApplicationVersionName")));
			appVersion = JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);
			env->DeleteLocalRef(jstrName);
			return appVersion;
		}
		//--------------------------------------------------------------------------------------
		/// Get Package Name
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetPackageName()
		{
			std::string packageName = "";
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrName = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetPackageName")));
			packageName = JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);
			env->DeleteLocalRef(jstrName);
			return packageName;
		}
		//--------------------------------------------------------------------------------------
		/// Get APK Directory
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetAPKDirectory()
		{
			std::string apkRootPath = " ";
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrAkpPath = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetAPKDirectory")));
			apkRootPath = JavaInterfaceUtils::CreateSTDStringFromJString(jstrAkpPath);
			env->DeleteLocalRef(jstrAkpPath);
			return apkRootPath;
		}
		//--------------------------------------------------------------------------------------
		/// Get Device Orientation
		//--------------------------------------------------------------------------------------
		s32 CCoreJavaInterface::GetOrientation()
		{
			s32 dwResult = -1;
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			s32 dwOSOrientation = env->CallIntMethod(GetJavaObject(), GetMethodID("GetOrientation"));
			s32 dwOSConstOrientationLandscape = env->CallIntMethod(GetJavaObject(), GetMethodID("GetOrientationLandscapeConstant"));
			s32 dwOSConstOrientationProtrait = env->CallIntMethod(GetJavaObject(), GetMethodID("GetOrientationPortraitConstant"));

			// Here we need to map the orientation reported back by Android to
			// moFlow orientation
			if(dwOSOrientation == dwOSConstOrientationProtrait)
			{
				dwResult = ChilliSource::Core::PORTRAIT_UP;
			}
			else
			if(dwOSOrientation == dwOSConstOrientationLandscape)
			{
				dwResult = ChilliSource::Core::LANDSCAPE_RIGHT;
			}

			return dwResult;
		}
		//--------------------------------------------------------------------------------------
		/// Get Screen Width
		//--------------------------------------------------------------------------------------
		s32 CCoreJavaInterface::GetScreenWidth()
		{
			if (mdwScreenWidth == 0)
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mdwScreenWidth = env->CallIntMethod(GetJavaObject(), GetMethodID("GetScreenWidth"));
			}
			return mdwScreenWidth;
		}
		//--------------------------------------------------------------------------------------
		/// Get Screen Height
		//--------------------------------------------------------------------------------------
		s32 CCoreJavaInterface::GetScreenHeight()
		{
			if (mdwScreenHeight == 0)
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mdwScreenHeight = env->CallIntMethod(GetJavaObject(), GetMethodID("GetScreenHeight"));
			}
			return mdwScreenHeight;
		}
		//--------------------------------------------------------------------------------------
		/// Get Default Locale Code
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetDefaultLocaleCode()
		{
			if (mstrLocaleCode == "")
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrLocalCode = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetDefaultLocaleCode")));
				mstrLocaleCode = JavaInterfaceUtils::CreateSTDStringFromJString(jstrLocalCode);
				env->DeleteLocalRef(jstrLocalCode);
			}
			return mstrLocaleCode;
		}
		//--------------------------------------------------------------------------------------
		/// Get Device Model
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetDeviceModel()
		{
			if (mstrDeviceModel == "")
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceModel = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetDeviceModel")));
				mstrDeviceModel = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceModel);
				env->DeleteLocalRef(jstrDeviceModel);
			}
			return mstrDeviceModel;
		}
		//--------------------------------------------------------------------------------------
		/// Get Device Model Type
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetDeviceModelType()
		{
			if (mstrDeviceModelType == "")
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceModelType = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetDeviceModelType")));
				mstrDeviceModelType = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceModelType);
				env->DeleteLocalRef(jstrDeviceModelType);
			}
			return mstrDeviceModelType;
		}
		//--------------------------------------------------------------------------------------
		/// Get Device Manufacturer
		//--------------------------------------------------------------------------------------
		std::string CCoreJavaInterface::GetDeviceManufacturer()
		{
			if (mstrDeviceManufacturer == "")
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceManufacturer = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetDeviceManufacturer")));
				mstrDeviceManufacturer = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceManufacturer);
				env->DeleteLocalRef(jstrDeviceManufacturer);
			}
			return mstrDeviceManufacturer;
		}
		//--------------------------------------------------------------------------------------
		/// Get OS Version Code
		//--------------------------------------------------------------------------------------
		s32 CCoreJavaInterface::GetOSVersionCode()
		{
			if (mdwOSVersionCode == -1)
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mdwOSVersionCode = env->CallIntMethod(GetJavaObject(), GetMethodID("GetOSVersion"));
			}
			return mdwOSVersionCode;
		}
		//--------------------------------------------------------------------------------------
		/// Get Number Of Cores
		//--------------------------------------------------------------------------------------
		s32 CCoreJavaInterface::GetNumberOfCores()
		{
			if (mdwNumberOfCores == -1)
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mdwNumberOfCores = env->CallIntMethod(GetJavaObject(), GetMethodID("GetNumberOfCores"));
			}
			return mdwNumberOfCores;
		}
		//--------------------------------------------------------------------------------------
		/// Get Screen Density
		//--------------------------------------------------------------------------------------
		f32 CCoreJavaInterface::GetScreenDensity()
		{
			if (mfScreenDensity < 0.0f)
			{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mfScreenDensity = env->CallFloatMethod(GetJavaObject(), GetMethodID("GetScreenDensity"));
			}
			return mfScreenDensity;
		}
		//--------------------------------------------------------------
		/// Get Telephony Device ID
		//--------------------------------------------------------------
		std::string CCoreJavaInterface::GetTelephonyDeviceID()
		{
			std::string strOutput = "None";
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrTelephonyDeviceID = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetTelephonyDeviceID")));
			strOutput = JavaInterfaceUtils::CreateSTDStringFromJString(jstrTelephonyDeviceID);
			env->DeleteLocalRef(jstrTelephonyDeviceID);
			return strOutput;
		}
		//--------------------------------------------------------------
		/// Get Mac Address
		//--------------------------------------------------------------
		std::string CCoreJavaInterface::GetMacAddress()
		{
			std::string strOutput = "None";
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrMacAddress = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetMacAddress")));
			strOutput = JavaInterfaceUtils::CreateSTDStringFromJString(jstrMacAddress);
			env->DeleteLocalRef(jstrMacAddress);
			return strOutput;
		}
		//--------------------------------------------------------------
		/// Get Android ID
		//--------------------------------------------------------------
		std::string CCoreJavaInterface::GetAndroidID()
		{
			std::string strOutput = "None";
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrAndroidID = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetAndroidID")));
			strOutput = JavaInterfaceUtils::CreateSTDStringFromJString(jstrAndroidID);
			env->DeleteLocalRef(jstrAndroidID);
			return strOutput;
		}
        //--------------------------------------------------------------------------------------------------
        /// Make Toast
        //--------------------------------------------------------------------------------------------------
        void CCoreJavaInterface::MakeToast(const UTF8String& instrText)
        {
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrMessage = JavaInterfaceUtils::CreateJStringFromUTF8String(instrText);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("MakeToast"), jstrMessage);
			env->DeleteLocalRef(jstrMessage);
        }
        //--------------------------------------------------------------------------------------------------
        /// Show System Confirm Dialog
        //--------------------------------------------------------------------------------------------------
        void CCoreJavaInterface::ShowSystemConfirmDialog(s32 indwDialogID, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm, const UTF8String& instrCancel)
        {
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrTitle = JavaInterfaceUtils::CreateJStringFromUTF8String(instrTitle);
			jstring jstrMessage = JavaInterfaceUtils::CreateJStringFromUTF8String(instrMessage);
			jstring jstrConfirm = JavaInterfaceUtils::CreateJStringFromUTF8String(instrConfirm);
			jstring jstrCancel = JavaInterfaceUtils::CreateJStringFromUTF8String(instrCancel);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("ShowSystemConfirmDialog"), indwDialogID, jstrTitle, jstrMessage, jstrConfirm, jstrCancel);
			env->DeleteLocalRef(jstrTitle);
			env->DeleteLocalRef(jstrMessage);
			env->DeleteLocalRef(jstrConfirm);
			env->DeleteLocalRef(jstrCancel);
        }
        //--------------------------------------------------------------------------------------------------
        /// Show System Dialog
        //--------------------------------------------------------------------------------------------------
        void CCoreJavaInterface::ShowSystemDialog(s32 indwDialogID, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm)
        {
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrTitle = JavaInterfaceUtils::CreateJStringFromUTF8String(instrTitle);
			jstring jstrMessage = JavaInterfaceUtils::CreateJStringFromUTF8String(instrMessage);
			jstring jstrConfirm = JavaInterfaceUtils::CreateJStringFromUTF8String(instrConfirm);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("ShowSystemDialog"), indwDialogID, jstrTitle, jstrMessage, jstrConfirm);
			env->DeleteLocalRef(jstrTitle);
			env->DeleteLocalRef(jstrMessage);
			env->DeleteLocalRef(jstrConfirm);
        }
        //-----------------------------------------------------------------------------------------------------
        /// Force Quit
        //-----------------------------------------------------------------------------------------------------
        void CCoreJavaInterface::ForceQuit()
        {
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("ForceQuit"));
        }
        //-----------------------------------------------------------------------------------------------------
        /// Get System Time In Milliseconds
        //-----------------------------------------------------------------------------------------------------
        TimeIntervalMs CCoreJavaInterface::GetSystemTimeInMilliseconds()
        {
        	TimeIntervalMs uqwOutput = 0;
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			uqwOutput = (TimeIntervalMs)env->CallLongMethod(GetJavaObject(), GetMethodID("GetSystemTimeInMilliseconds"));
			return uqwOutput;
        }
        //-----------------------------------------------------------------------------------------------------
        /// Get Physical creen Size
        //-----------------------------------------------------------------------------------------------------
        f32 CCoreJavaInterface::GetPhysicalScreenSize()
        {
        	if (mfPhysicalScreenSize < 0.0f)
        	{
				JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mfPhysicalScreenSize = env->CallFloatMethod(GetJavaObject(), GetMethodID("GetPhysicalScreenSize"));
        	}
        	return mfPhysicalScreenSize;
        }
	}
}
