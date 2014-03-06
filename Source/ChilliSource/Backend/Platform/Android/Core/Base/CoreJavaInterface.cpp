/*
 *  CoreJavaInterface.h
 *  moFlow
 *
 *  Created by Ian Copland on 17/03/2011.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Core/Base/CoreJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/Core/DialogueBox/DialogueBoxSystem.h>
#include <ChilliSource/Backend/Platform/Android/Core/File/SharedPreferencesJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpConnectionJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProviderJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/SMSCompositionJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivityJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Web/Base/WebViewJavaInterface.h>

#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/LaunchingActions/LaunchingActions.h>

//--------------------------------------------------------------------------------------
/// Globals
//--------------------------------------------------------------------------------------
ChilliSource::Core::Application* (*BootFunctionPtr)() = nullptr;
//--------------------------------------------------------------------------------------
/// C function declarations
//--------------------------------------------------------------------------------------
extern "C"
{
	void Java_com_chillisource_core_CoreNativeInterface_SetupCoreJavaNativeInterface(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_core_CoreNativeInterface_Initialise(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_core_CoreNativeInterface_Resume(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_core_CoreNativeInterface_Suspend(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_core_CoreNativeInterface_DestroyApplication(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_core_CoreNativeInterface_FrameBegin(JNIEnv* inpEnv, jobject inThis, f32 infDeltaTime, s64 inddwTimestamp);
	void Java_com_chillisource_core_CoreNativeInterface_MemoryWarning(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_core_CoreNativeInterface_OrientationChanged(JNIEnv* inpEnv, jobject inThis, s32 indwOrientation);
	void Java_com_chillisource_core_CoreNativeInterface_OnBackPressed(JNIEnv* inpEnv, jobject inThis);
    void Java_com_chillisource_core_CoreNativeInterface_ApplicationDidReceiveLaunchingURL(JNIEnv* inpEnv, jobject inThis, jstring instrURL);
}
//--------------------------------------------------------------------------------------
/// Setup Core Java Native Interface
///
/// Setups the Core Java and Native Interfaces.
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_SetupCoreJavaNativeInterface(JNIEnv* inpEnv, jobject inThis)
{
	//get the java VM and init the Java Interface Manager
	JavaVM * pJavaVM;
	inpEnv->GetJavaVM(&pJavaVM);
	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->Initialise(pJavaVM);

	//add the core native interface
	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(ChilliSource::Android::IJavaInterfaceSPtr(new ChilliSource::Android::CoreJavaInterface()));
}
//--------------------------------------------------------------------------------------
/// Initialise
///
/// Interface function called from java. This is called when the application is started.
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_Initialise(JNIEnv* inpEnv, jobject inThis)
{
	//create the application
	ChilliSource::Core::Application* pApplication = BootFunctionPtr();
	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CoreJavaInterface>()->SetApplication(pApplication);

	//setup other interfaces
	JavaVM * pJavaVM;
	inpEnv->GetJavaVM(&pJavaVM);
	ChilliSource::Android::TouchInputJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::HttpConnectionJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::SharedPreferencesJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::WebViewJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::SMSCompositionJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::ContactInformationProviderJavaInterface::SetupJavaInterface(pJavaVM);
	ChilliSource::Android::TwitterAuthenticationActivityJavaInterface::SetupJavaInterface(pJavaVM);
    
	//run the application
    pApplication->OnInitialise();
}

//--------------------------------------------------------------------------------------
/// Resume
///
/// Interface function called from java. This is called when the application is resumed.
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_Resume(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Core::Application::Get()->OnResume();
}
//--------------------------------------------------------------------------------------
/// Suspend
///
/// Interface function called from java. This is called when the application is suspended
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_Suspend(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Core::Application::Get()->OnSuspend();
}
//--------------------------------------------------------------------------------------
/// Destroy
///
/// Interface function called from java. This is called when the application is destroyed
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_DestroyApplication(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CoreJavaInterface>()->DestroyApplication();
}
//--------------------------------------------------------------------------------------
/// Frame Begin
///
/// Interface function called from java. This is called when the application is updated
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
/// @param the delta time.
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_FrameBegin(JNIEnv* inpEnv, jobject inThis, f32 infDeltaTime, s64 inddwTimestamp)
{
	//Create the message with the time between frames
	ChilliSource::Core::Application::Get()->OnUpdate((f32)infDeltaTime, (u64)inddwTimestamp);
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
void Java_com_chillisource_core_CoreNativeInterface_MemoryWarning(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Core::Application::Get()->OnApplicationMemoryWarning();
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
void Java_com_chillisource_core_CoreNativeInterface_OrientationChanged(JNIEnv* inpEnv, jobject thiz, s32 indwOrientation)
{
	ChilliSource::Core::ScreenOrientation eScreenOrientation;

	if (indwOrientation == 1)
	{
		eScreenOrientation = ChilliSource::Core::ScreenOrientation::k_portraitUp;
		CS_LOG_DEBUG("Changing orientation to portrait");
	}
	else
	{
		eScreenOrientation = ChilliSource::Core::ScreenOrientation::k_landscapeLeft;
		CS_LOG_DEBUG("Changing orientation to landscape");
	}

	ChilliSource::Core::Application::Get()->OnScreenChangedOrientation(eScreenOrientation);
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
void Java_com_chillisource_core_CoreNativeInterface_OnBackPressed(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Core::Application::Get()->OnGoBack();
}
//--------------------------------------------------------------------------------------
/// Application Did Receive Launching URL
///
///
/// @param JNIEnv - The jni environment.
/// @param jobject - the java object calling the function
/// @param URL
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_ApplicationDidReceiveLaunchingURL(JNIEnv* inpEnv, jobject inThis, jstring instrURL)
{
	CSCore::LaunchingActions::ApplicationDidReceiveLaunchingURL(ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrURL));
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(CoreJavaInterface);

		//--------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------
		CoreJavaInterface::CoreJavaInterface()
		: mApplication(nullptr), mdwScreenWidth(0), mdwScreenHeight(0), mdwOSVersionCode(-1), mdwNumberOfCores(-1), mfScreenDensity(-0.1f), mfPhysicalScreenSize(-0.1f)
		{
			CreateNativeInterface("com/chillisource/core/CoreNativeInterface");
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
			CreateMethodReference("ForceQuit", "()V");
			CreateMethodReference("GetSystemTimeInMilliseconds", "()J");
			CreateMethodReference("GetPhysicalScreenSize", "()F");
			CreateMethodReference("SetMaxFPS", "(I)V");
		}
		//--------------------------------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------------------------------
		bool CoreJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CoreJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------------------------------
		/// Set Application
		//--------------------------------------------------------------------------------------
		void CoreJavaInterface::SetApplication(ChilliSource::Core::Application* inApplication)
		{
			mApplication = inApplication;
		}
		//--------------------------------------------------------------------------------------
		/// Destroy Application
		//--------------------------------------------------------------------------------------
		void CoreJavaInterface::DestroyApplication()
		{
			mApplication->OnDestroy();
			CS_SAFEDELETE(mApplication);
		}
		//--------------------------------------------------------------------------------------
		/// Get Application
		//--------------------------------------------------------------------------------------
		ChilliSource::Core::Application* CoreJavaInterface::GetApplication()
		{
			return mApplication;
		}
		//-----------------------------------------
		/// Set Max FPS
		//-----------------------------------------
		void CoreJavaInterface::SetMaxFPS(u32 inudwFPS)
		{
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("SetMaxFPS"), inudwFPS);
		}
		//--------------------------------------------------------------------------------------
		/// Get External Storage Directory
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetExternalStorageDirectory()
		{
			std::string sdCardRootPath = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrExternalStorageDir = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetExternalStorageDirectory")));
			sdCardRootPath = JavaInterfaceUtils::CreateSTDStringFromJString(jstrExternalStorageDir);
			env->DeleteLocalRef(jstrExternalStorageDir);
			return sdCardRootPath;
		}
		//--------------------------------------------------------------------------------------
		/// Get Application Name
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetApplicationName()
		{
			std::string appName = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
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
		u32 CoreJavaInterface::GetApplicationVersionCode()
		{
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallIntMethod(GetJavaObject(), GetMethodID("GetApplicationVersionCode"));
		}
		//--------------------------------------------------------------------------------------
		/// Get Application Version Name
		///
		/// @return returns the Application version name as acquired from the CoreNativeInterface
		/// Java class.
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetApplicationVersionName()
		{
			std::string appVersion = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrName = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetApplicationVersionName")));
			appVersion = JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);
			env->DeleteLocalRef(jstrName);
			return appVersion;
		}
		//--------------------------------------------------------------------------------------
		/// Get Package Name
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetPackageName()
		{
			std::string packageName = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrName = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetPackageName")));
			packageName = JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);
			env->DeleteLocalRef(jstrName);
			return packageName;
		}
		//--------------------------------------------------------------------------------------
		/// Get APK Directory
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetAPKDirectory()
		{
			std::string apkRootPath = " ";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrAkpPath = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetAPKDirectory")));
			apkRootPath = JavaInterfaceUtils::CreateSTDStringFromJString(jstrAkpPath);
			env->DeleteLocalRef(jstrAkpPath);
			return apkRootPath;
		}
		//--------------------------------------------------------------------------------------
		/// Get Device Orientation
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetOrientation()
		{
			s32 dwResult = -1;
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			s32 dwOSOrientation = env->CallIntMethod(GetJavaObject(), GetMethodID("GetOrientation"));
			s32 dwOSConstOrientationLandscape = env->CallIntMethod(GetJavaObject(), GetMethodID("GetOrientationLandscapeConstant"));
			s32 dwOSConstOrientationPortrait = env->CallIntMethod(GetJavaObject(), GetMethodID("GetOrientationPortraitConstant"));

			// Here we need to map the orientation reported back by Android to
			// moFlow orientation
			if(dwOSOrientation == dwOSConstOrientationPortrait)
			{
				dwResult = (s32)Core::ScreenOrientation::k_portraitUp;
			}
			else if(dwOSOrientation == dwOSConstOrientationLandscape)
			{
				dwResult = (s32)Core::ScreenOrientation::k_landscapeRight;
			}

			return dwResult;
		}
		//--------------------------------------------------------------------------------------
		/// Get Screen Width
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetScreenWidth()
		{
			if (mdwScreenWidth == 0)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mdwScreenWidth = env->CallIntMethod(GetJavaObject(), GetMethodID("GetScreenWidth"));
			}
			return mdwScreenWidth;
		}
		//--------------------------------------------------------------------------------------
		/// Get Screen Height
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetScreenHeight()
		{
			if (mdwScreenHeight == 0)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mdwScreenHeight = env->CallIntMethod(GetJavaObject(), GetMethodID("GetScreenHeight"));
			}
			return mdwScreenHeight;
		}
		//--------------------------------------------------------------------------------------
		/// Get Default Locale Code
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetDefaultLocaleCode()
		{
			if (mstrLocaleCode == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrLocalCode = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetDefaultLocaleCode")));
				mstrLocaleCode = JavaInterfaceUtils::CreateSTDStringFromJString(jstrLocalCode);
				env->DeleteLocalRef(jstrLocalCode);
			}
			return mstrLocaleCode;
		}
		//--------------------------------------------------------------------------------------
		/// Get Device Model
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetDeviceModel()
		{
			if (mstrDeviceModel == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceModel = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetDeviceModel")));
				mstrDeviceModel = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceModel);
				env->DeleteLocalRef(jstrDeviceModel);
			}
			return mstrDeviceModel;
		}
		//--------------------------------------------------------------------------------------
		/// Get Device Model Type
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetDeviceModelType()
		{
			if (mstrDeviceModelType == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceModelType = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetDeviceModelType")));
				mstrDeviceModelType = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceModelType);
				env->DeleteLocalRef(jstrDeviceModelType);
			}
			return mstrDeviceModelType;
		}
		//--------------------------------------------------------------------------------------
		/// Get Device Manufacturer
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetDeviceManufacturer()
		{
			if (mstrDeviceManufacturer == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceManufacturer = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetDeviceManufacturer")));
				mstrDeviceManufacturer = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceManufacturer);
				env->DeleteLocalRef(jstrDeviceManufacturer);
			}
			return mstrDeviceManufacturer;
		}
		//--------------------------------------------------------------------------------------
		/// Get OS Version Code
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetOSVersionCode()
		{
			if (mdwOSVersionCode == -1)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mdwOSVersionCode = env->CallIntMethod(GetJavaObject(), GetMethodID("GetOSVersion"));
			}
			return mdwOSVersionCode;
		}
		//--------------------------------------------------------------------------------------
		/// Get Number Of Cores
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetNumberOfCores()
		{
			if (mdwNumberOfCores == -1)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mdwNumberOfCores = env->CallIntMethod(GetJavaObject(), GetMethodID("GetNumberOfCores"));
			}
			return mdwNumberOfCores;
		}
		//--------------------------------------------------------------------------------------
		/// Get Screen Density
		//--------------------------------------------------------------------------------------
		f32 CoreJavaInterface::GetScreenDensity()
		{
			if (mfScreenDensity < 0.0f)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mfScreenDensity = env->CallFloatMethod(GetJavaObject(), GetMethodID("GetScreenDensity"));
			}
			return mfScreenDensity;
		}
		//--------------------------------------------------------------
		/// Get Telephony Device ID
		//--------------------------------------------------------------
		std::string CoreJavaInterface::GetTelephonyDeviceID()
		{
			std::string strOutput = "None";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrTelephonyDeviceID = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetTelephonyDeviceID")));
			strOutput = JavaInterfaceUtils::CreateSTDStringFromJString(jstrTelephonyDeviceID);
			env->DeleteLocalRef(jstrTelephonyDeviceID);
			return strOutput;
		}
		//--------------------------------------------------------------
		/// Get Mac Address
		//--------------------------------------------------------------
		std::string CoreJavaInterface::GetMacAddress()
		{
			std::string strOutput = "None";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrMacAddress = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetMacAddress")));
			strOutput = JavaInterfaceUtils::CreateSTDStringFromJString(jstrMacAddress);
			env->DeleteLocalRef(jstrMacAddress);
			return strOutput;
		}
		//--------------------------------------------------------------
		/// Get Android ID
		//--------------------------------------------------------------
		std::string CoreJavaInterface::GetAndroidID()
		{
			std::string strOutput = "None";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrAndroidID = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("GetAndroidID")));
			strOutput = JavaInterfaceUtils::CreateSTDStringFromJString(jstrAndroidID);
			env->DeleteLocalRef(jstrAndroidID);
			return strOutput;
		}
        //-----------------------------------------------------------------------------------------------------
        /// Force Quit
        //-----------------------------------------------------------------------------------------------------
        void CoreJavaInterface::ForceQuit()
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("ForceQuit"));
        }
        //-----------------------------------------------------------------------------------------------------
        /// Get System Time In Milliseconds
        //-----------------------------------------------------------------------------------------------------
        TimeIntervalMs CoreJavaInterface::GetSystemTimeInMilliseconds()
        {
        	TimeIntervalMs uqwOutput = 0;
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			uqwOutput = (TimeIntervalMs)env->CallLongMethod(GetJavaObject(), GetMethodID("GetSystemTimeInMilliseconds"));
			return uqwOutput;
        }
        //-----------------------------------------------------------------------------------------------------
        /// Get Physical creen Size
        //-----------------------------------------------------------------------------------------------------
        f32 CoreJavaInterface::GetPhysicalScreenSize()
        {
        	if (mfPhysicalScreenSize < 0.0f)
        	{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				mfPhysicalScreenSize = env->CallFloatMethod(GetJavaObject(), GetMethodID("GetPhysicalScreenSize"));
        	}
        	return mfPhysicalScreenSize;
        }
	}
}
