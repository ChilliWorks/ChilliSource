//
//  CoreJavaInterface.cpp
//  Chilli Source
//  Created by Ian Copland on 17/03/2011.
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

#include <CSBackend/Platform/Android/Core/Base/CoreJavaInterface.h>

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/Base/Screen.h>
#include <CSBackend/Platform/Android/Core/DialogueBox/DialogueBoxSystem.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <CSBackend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>
#include <CSBackend/Platform/Android/Networking/Http/HttpRequestJavaInterface.h>
#include <CSBackend/Platform/Android/Web/Base/WebViewJavaInterface.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

//--------------------------------------------------------------------------------------
/// C function declarations
//--------------------------------------------------------------------------------------
extern "C"
{
	//--------------------------------------------------------------------------------------
	/// Interface function called from java that forces creation of the core interface
	///
	/// @author Ian Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_create(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is launched.
	///
	/// @author Ian Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_init(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is resumed.
	///
	/// @author Ian Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_resume(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is pushed
	/// to the foreground
	///
	/// @author S Downie
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_foreground(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is pushed
	/// to the background
	///
	/// @author S Downie
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_background(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is suspended
	///
	/// @author Ian Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_suspend(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is destroyed
	///
	/// @author Ian Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_destroy(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is updated
	///
	/// @author Ian Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	/// @param the delta time.
	/// @param Elapsed app time
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_update(JNIEnv* in_env, jobject in_this, f32 in_deltaTime, s64 in_elapsedTime);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the application is almost
	/// Out of memory.
	///
	/// @author Ian Copland
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_memoryWarning(JNIEnv* in_env, jobject in_this);
	//--------------------------------------------------------------------------------------
	/// Interface function called from java. This is called when the resolution changes.
	///
	/// @author Ian Copland
	///
	/// @param The jni environment.
	/// @param the java object calling the function
	/// @param The new width.
	/// @param The new height.
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_core_CoreNativeInterface_onResolutionChanged(JNIEnv* in_env, jobject in_this, s32 in_width, s32 in_height);
	//--------------------------------------------------------------------------------------
	/// Application Did Receive Launching URL
	///
	///
	/// @param JNIEnv - The jni environment.
	/// @param jobject - the java object calling the function
	/// @param URL
	//--------------------------------------------------------------------------------------
    void Java_com_chilliworks_chillisource_core_CoreNativeInterface_applicationDidReceiveLaunchingURL(JNIEnv* in_env, jobject in_this, jstring in_url);
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_create(JNIEnv* in_env, jobject in_this)
{
	//get the java VM and init the Java Interface Manager
	JavaVM* javaVM;
	in_env->GetJavaVM(&javaVM);

	CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->Initialise(javaVM);

	//add the core native interface
	CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(CSBackend::Android::IJavaInterfaceSPtr(new CSBackend::Android::CoreJavaInterface()));
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_init(JNIEnv* in_env, jobject in_this)
{
	//get the java VM and init the Java Interface Manager
	JavaVM* javaVM;
	in_env->GetJavaVM(&javaVM);

	//create the application
	CSCore::Application* pApplication = CreateApplication();
	CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::CoreJavaInterface>()->SetApplication(pApplication);

	//setup other interfaces
	CSBackend::Android::TouchInputJavaInterface::SetupJavaInterface(javaVM);
	CSBackend::Android::HttpRequestJavaInterface::SetupJavaInterface(javaVM);
	CSBackend::Android::WebViewJavaInterface::SetupJavaInterface(javaVM);
    
	//run the application
    pApplication->Init();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_resume(JNIEnv* in_env, jobject in_this)
{
	CSCore::Application::Get()->Resume();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_foreground(JNIEnv* in_env, jobject in_this)
{
	CSCore::Application::Get()->Foreground();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_background(JNIEnv* in_env, jobject in_this)
{
	CSCore::Application::Get()->Background();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_suspend(JNIEnv* in_env, jobject in_this)
{
	CSCore::Application::Get()->Suspend();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_destroy(JNIEnv* in_env, jobject in_this)
{
	CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::CoreJavaInterface>()->DestroyApplication();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_update(JNIEnv* in_env, jobject in_this, f32 in_deltaTime, s64 in_elaspedTime)
{
	//Create the message with the time between frames
	CSCore::Application::Get()->Update(in_deltaTime, (u64)in_elaspedTime);
	CSCore::Application::Get()->Render();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_memoryWarning(JNIEnv* in_env, jobject in_this)
{
	CSCore::Application::Get()->ApplicationMemoryWarning();
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_core_CoreNativeInterface_onResolutionChanged(JNIEnv* in_env, jobject in_this, s32 in_width, s32 in_height)
{
	CSBackend::Android::Screen* screen = CSCore::Application::Get()->GetSystem<CSBackend::Android::Screen>();
	CS_ASSERT(screen != nullptr, "Cannot find required system: Screen.");
	screen->OnResolutionChanged(CSCore::Vector2((f32)in_width, (f32)in_height));
}

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(CoreJavaInterface);

		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		CoreJavaInterface::CoreJavaInterface()
		: m_application(nullptr), m_screenWidth(0), m_screenHeight(0), m_screenDensity(-0.1f), m_physicalScreenSize(-0.1f)
		{
			CreateNativeInterface("com/chilliworks/chillisource/core/CoreNativeInterface");
			CreateMethodReference("getExternalStorageDirectory", "()Ljava/lang/String;");
			CreateMethodReference("getApplicationName", "()Ljava/lang/String;");
			CreateMethodReference("getApplicationVersionCode", "()I");
			CreateMethodReference("getApplicationVersionName", "()Ljava/lang/String;");
			CreateMethodReference("getPackageName", "()Ljava/lang/String;");
			CreateMethodReference("getAPKDirectory", "()Ljava/lang/String;");
			CreateMethodReference("getScreenWidth", "()I");
			CreateMethodReference("getScreenHeight", "()I");
			CreateMethodReference("getScreenDensity", "()F");
			CreateMethodReference("forceQuit", "()V");
			CreateMethodReference("getSystemTimeInMilliseconds", "()J");
			CreateMethodReference("setPreferredFPS", "(I)V");
			CreateMethodReference("getActivity", "()Landroid/app/Activity;");
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		bool CoreJavaInterface::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == CoreJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		void CoreJavaInterface::SetApplication(CSCore::Application* in_application)
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
		CSCore::Application* CoreJavaInterface::GetApplication()
		{
			return m_application;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		jobject CoreJavaInterface::GetActivity() const
		{
			JNIEnv* pEnv = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallObjectMethod(GetJavaObject(), GetMethodID("getActivity"));
		}
		//-----------------------------------------
		//-----------------------------------------
		void CoreJavaInterface::SetPreferredFPS(u32 in_maxFPS)
		{
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("setPreferredFPS"), in_maxFPS);
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
		f32 CoreJavaInterface::GetScreenDensity()
		{
			if (m_screenDensity < 0.0f)
			{
				JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
				m_screenDensity = env->CallFloatMethod(GetJavaObject(), GetMethodID("getScreenDensity"));
			}
			return m_screenDensity;
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
	}
}

#endif
