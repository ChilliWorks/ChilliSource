//
//  CoreJavaInterface.cpp
//  Chilli Source
//
//  Created by Ian Copland on 17/03/2011.
//  Copyright 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Core/Base/CoreJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/Core/DialogueBox/DialogueBoxSystem.h>
#include <ChilliSource/Backend/Platform/Android/Core/File/SharedPreferencesJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpRequestJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProviderJavaInterface.h>
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
	//--------------------------------------------------------------------------------------
	/// Interface function called from java that forces creation of the core interface
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_create(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is launched.
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_init(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is resumed.
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_resume(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is pushed
	/// to the foreground
	///
	/// @author S Downie
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_foreground(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is pushed
	/// to the background
	///
	/// @author S Downie
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_background(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is suspended
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_suspend(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is destroyed
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_destroy(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is updated
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	/// @param the delta time.
	/// @param Elapsed app time
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_update(JNIEnv* in_env, jobject in_this, f32 in_deltaTime, s64 in_elapsedTime);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is almost
	/// Out of memory.
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_memoryWarning(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the orientation has changed.
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	/// @param the new orientation as an int that maps to the orientation enum.
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_orientationChanged(JNIEnv* in_env, jobject in_this, s32 in_orientationID);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the back hardware button is
	/// pressed
	///
	/// @author I Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_core_CoreNativeInterface_onBackPressed(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Application Did Receive Launching URL
	///
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	/// @param URL
	//--------------------------------------------------------------------------------------
    void Java_com_chillisource_core_CoreNativeInterface_applicationDidReceiveLaunchingURL(JNIEnv* in_env, jobject in_this, jstring in_url);
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_create(JNIEnv* in_env, jobject in_this)
{
	//get the java VM and init the Java Interface Manager
	JavaVM* javaVM;
	in_env->GetJavaVM(&javaVM);

	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->Initialise(javaVM);

	//add the core native interface
	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(ChilliSource::Android::IJavaInterfaceSPtr(new ChilliSource::Android::CoreJavaInterface()));
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_init(JNIEnv* in_env, jobject in_this)
{
	//get the java VM and init the Java Interface Manager
	JavaVM* javaVM;
	in_env->GetJavaVM(&javaVM);

	//create the application
	ChilliSource::Core::Application* pApplication = BootFunctionPtr();
	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CoreJavaInterface>()->SetApplication(pApplication);

	//setup other interfaces
	ChilliSource::Android::TouchInputJavaInterface::SetupJavaInterface(javaVM);
	ChilliSource::Android::HttpRequestJavaInterface::SetupJavaInterface(javaVM);
	ChilliSource::Android::SharedPreferencesJavaInterface::SetupJavaInterface(javaVM);
	ChilliSource::Android::WebViewJavaInterface::SetupJavaInterface(javaVM);
	ChilliSource::Android::ContactInformationProviderJavaInterface::SetupJavaInterface(javaVM);
	ChilliSource::Android::TwitterAuthenticationActivityJavaInterface::SetupJavaInterface(javaVM);
    
	//run the application
    pApplication->Init();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_resume(JNIEnv* in_env, jobject in_this)
{
	ChilliSource::Core::Application::Get()->Resume();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_foreground(JNIEnv* in_env, jobject in_this)
{
	ChilliSource::Core::Application::Get()->Foreground();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_background(JNIEnv* in_env, jobject in_this)
{
	ChilliSource::Core::Application::Get()->Background();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_suspend(JNIEnv* in_env, jobject in_this)
{
	ChilliSource::Core::Application::Get()->Suspend();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_destroy(JNIEnv* in_env, jobject in_this)
{
	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CoreJavaInterface>()->DestroyApplication();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_update(JNIEnv* in_env, jobject in_this, f32 in_deltaTime, s64 in_elaspedTime)
{
	//Create the message with the time between frames
	ChilliSource::Core::Application::Get()->Update(in_deltaTime, (u64)in_elaspedTime);
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_memoryWarning(JNIEnv* in_env, jobject in_this)
{
	ChilliSource::Core::Application::Get()->ApplicationMemoryWarning();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_orientationChanged(JNIEnv* in_env, jobject thiz, s32 in_orientationID)
{
	ChilliSource::Core::ScreenOrientation screenOrientation;

	if (in_orientationID == 1)
	{
		screenOrientation = ChilliSource::Core::ScreenOrientation::k_portraitUp;
		CS_LOG_VERBOSE("Changing orientation to portrait");
	}
	else
	{
		screenOrientation = ChilliSource::Core::ScreenOrientation::k_landscapeLeft;
		CS_LOG_VERBOSE("Changing orientation to landscape");
	}

	ChilliSource::Core::Application::Get()->ScreenChangedOrientation(screenOrientation);
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_onBackPressed(JNIEnv* in_env, jobject in_this)
{
	ChilliSource::Core::Application::Get()->GoBack();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_core_CoreNativeInterface_applicationDidReceiveLaunchingURL(JNIEnv* in_env, jobject in_this, jstring in_url)
{
	CSCore::LaunchingActions::ApplicationDidReceiveLaunchingURL(ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(in_url));
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(CoreJavaInterface);

		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		CoreJavaInterface::CoreJavaInterface()
		: m_application(nullptr), m_screenWidth(0), m_screenHeight(0), m_OSVersionCode(-1), m_numberOfCores(-1), m_screenDensity(-0.1f), m_physicalScreenSize(-0.1f)
		{
			CreateNativeInterface("com/chillisource/core/CoreNativeInterface");
			CreateMethodReference("getExternalStorageDirectory", "()Ljava/lang/String;");
			CreateMethodReference("getApplicationName", "()Ljava/lang/String;");
			CreateMethodReference("getApplicationVersionCode", "()I");
			CreateMethodReference("getApplicationVersionName", "()Ljava/lang/String;");
			CreateMethodReference("getPackageName", "()Ljava/lang/String;");
			CreateMethodReference("getAPKDirectory", "()Ljava/lang/String;");
			CreateMethodReference("getOrientation", "()I");
			CreateMethodReference("getOrientationLandscapeConstant", "()I");
			CreateMethodReference("getOrientationPortraitConstant", "()I");
			CreateMethodReference("getScreenWidth", "()I");
			CreateMethodReference("getScreenHeight", "()I");
			CreateMethodReference("getDefaultLocaleCode", "()Ljava/lang/String;");
			CreateMethodReference("getDeviceModel", "()Ljava/lang/String;");
			CreateMethodReference("getDeviceManufacturer", "()Ljava/lang/String;");
			CreateMethodReference("getDeviceModelType", "()Ljava/lang/String;");
			CreateMethodReference("getOSVersion", "()I");
			CreateMethodReference("getNumberOfCores", "()I");
			CreateMethodReference("getScreenDensity", "()F");
			CreateMethodReference("getTelephonyDeviceID", "()Ljava/lang/String;");
			CreateMethodReference("getMacAddress", "()Ljava/lang/String;");
			CreateMethodReference("getAndroidID", "()Ljava/lang/String;");
			CreateMethodReference("forceQuit", "()V");
			CreateMethodReference("getSystemTimeInMilliseconds", "()J");
			CreateMethodReference("getPhysicalScreenSize", "()F");
			CreateMethodReference("setMaxFPS", "(I)V");
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		bool CoreJavaInterface::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == CoreJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void CoreJavaInterface::SetApplication(ChilliSource::Core::Application* in_application)
		{
			m_application = in_application;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void CoreJavaInterface::DestroyApplication()
		{
			m_application->Destroy();
			CS_SAFEDELETE(m_application);
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		ChilliSource::Core::Application* CoreJavaInterface::GetApplication()
		{
			return m_application;
		}
		//-----------------------------------------
		//-----------------------------------------
		void CoreJavaInterface::SetMaxFPS(u32 in_maxFPS)
		{
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("setMaxFPS"), in_maxFPS);
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetExternalStorageDirectory()
		{
			std::string sdCardRootPath = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring externalStorageDir = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getExternalStorageDirectory")));
			sdCardRootPath = JavaInterfaceUtils::CreateSTDStringFromJString(externalStorageDir);
			env->DeleteLocalRef(externalStorageDir);
			return sdCardRootPath;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetApplicationName()
		{
			std::string appName = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrName = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getApplicationName")));
			appName = JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);
			env->DeleteLocalRef(jstrName);
			return appName;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		u32 CoreJavaInterface::GetApplicationVersionCode()
		{
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallIntMethod(GetJavaObject(), GetMethodID("getApplicationVersionCode"));
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetApplicationVersionName()
		{
			std::string appVersion = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrName = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getApplicationVersionName")));
			appVersion = JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);
			env->DeleteLocalRef(jstrName);
			return appVersion;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetPackageName()
		{
			std::string packageName = "";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrName = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getPackageName")));
			packageName = JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);
			env->DeleteLocalRef(jstrName);
			return packageName;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetAPKDirectory()
		{
			std::string apkRootPath = " ";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrAkpPath = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getAPKDirectory")));
			apkRootPath = JavaInterfaceUtils::CreateSTDStringFromJString(jstrAkpPath);
			env->DeleteLocalRef(jstrAkpPath);
			return apkRootPath;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetOrientation()
		{
			s32 result = -1;
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			s32 OSOrientation = env->CallIntMethod(GetJavaObject(), GetMethodID("getOrientation"));
			s32 OSConstOrientationLandscape = env->CallIntMethod(GetJavaObject(), GetMethodID("getOrientationLandscapeConstant"));
			s32 OSConstOrientationPortrait = env->CallIntMethod(GetJavaObject(), GetMethodID("getOrientationPortraitConstant"));

			// Here we need to map the orientation reported back by Android to
			// moFlow orientation
			if(OSOrientation == OSConstOrientationPortrait)
			{
				result = (s32)Core::ScreenOrientation::k_portraitUp;
			}
			else if(OSOrientation == OSConstOrientationLandscape)
			{
				result = (s32)Core::ScreenOrientation::k_landscapeRight;
			}

			return result;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetScreenWidth()
		{
			if (m_screenWidth == 0)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_screenWidth = env->CallIntMethod(GetJavaObject(), GetMethodID("getScreenWidth"));
			}
			return m_screenWidth;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetScreenHeight()
		{
			if (m_screenHeight == 0)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_screenHeight = env->CallIntMethod(GetJavaObject(), GetMethodID("getScreenHeight"));
			}
			return m_screenHeight;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetDefaultLocaleCode()
		{
			if (m_localeCode == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrLocalCode = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getDefaultLocaleCode")));
				m_localeCode = JavaInterfaceUtils::CreateSTDStringFromJString(jstrLocalCode);
				env->DeleteLocalRef(jstrLocalCode);
			}
			return m_localeCode;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetDeviceModel()
		{
			if (m_deviceModel == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceModel = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getDeviceModel")));
				m_deviceModel = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceModel);
				env->DeleteLocalRef(jstrDeviceModel);
			}
			return m_deviceModel;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetDeviceModelType()
		{
			if (m_deviceModelType == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceModelType = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getDeviceModelType")));
				m_deviceModelType = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceModelType);
				env->DeleteLocalRef(jstrDeviceModelType);
			}
			return m_deviceModelType;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		std::string CoreJavaInterface::GetDeviceManufacturer()
		{
			if (m_deviceManufacturer == "")
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				jstring jstrDeviceManufacturer = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getDeviceManufacturer")));
				m_deviceManufacturer = JavaInterfaceUtils::CreateSTDStringFromJString(jstrDeviceManufacturer);
				env->DeleteLocalRef(jstrDeviceManufacturer);
			}
			return m_deviceManufacturer;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetOSVersionCode()
		{
			if (m_OSVersionCode == -1)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_OSVersionCode = env->CallIntMethod(GetJavaObject(), GetMethodID("getOSVersion"));
			}
			return m_OSVersionCode;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		s32 CoreJavaInterface::GetNumberOfCores()
		{
			if (m_numberOfCores == -1)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_numberOfCores = env->CallIntMethod(GetJavaObject(), GetMethodID("getNumberOfCores"));
			}
			return m_numberOfCores;
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		f32 CoreJavaInterface::GetScreenDensity()
		{
			if (m_screenDensity < 0.0f)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_screenDensity = env->CallFloatMethod(GetJavaObject(), GetMethodID("getScreenDensity"));
			}
			return m_screenDensity;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string CoreJavaInterface::GetTelephonyDeviceID()
		{
			std::string output = "None";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring telephonyDeviceID = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getTelephonyDeviceID")));
			output = JavaInterfaceUtils::CreateSTDStringFromJString(telephonyDeviceID);
			env->DeleteLocalRef(telephonyDeviceID);
			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string CoreJavaInterface::GetMacAddress()
		{
			std::string output = "None";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring macAddress = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getMacAddress")));
			output = JavaInterfaceUtils::CreateSTDStringFromJString(macAddress);
			env->DeleteLocalRef(macAddress);
			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string CoreJavaInterface::GetAndroidID()
		{
			std::string output = "None";
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring androidID = static_cast<jstring>(env->CallObjectMethod(GetJavaObject(), GetMethodID("getAndroidID")));
			output = JavaInterfaceUtils::CreateSTDStringFromJString(androidID);
			env->DeleteLocalRef(androidID);
			return output;
		}
        //-----------------------------------------------------------------------------------------------------
        //-----------------------------------------------------------------------------------------------------
        void CoreJavaInterface::ForceQuit()
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("forceQuit"));
        }
        //-----------------------------------------------------------------------------------------------------
        //-----------------------------------------------------------------------------------------------------
        TimeIntervalMs CoreJavaInterface::GetSystemTimeInMilliseconds()
        {
        	TimeIntervalMs output = 0;
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			output = (TimeIntervalMs)env->CallLongMethod(GetJavaObject(), GetMethodID("getSystemTimeInMilliseconds"));
			return output;
        }
        //-----------------------------------------------------------------------------------------------------
        //-----------------------------------------------------------------------------------------------------
        f32 CoreJavaInterface::GetPhysicalScreenSize()
        {
        	if (m_physicalScreenSize < 0.0f)
        	{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_physicalScreenSize = env->CallFloatMethod(GetJavaObject(), GetMethodID("getPhysicalScreenSize"));
        	}
        	return m_physicalScreenSize;
        }
	}
}
