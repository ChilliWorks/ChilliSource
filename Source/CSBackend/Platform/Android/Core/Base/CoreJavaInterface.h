//
//  CoreJavaInterface.h
//  Chilli Source
//
//  Created by Ian Copland on 17/03/2011.
//  Copyright 2012 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_CORE_BASE_COREJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_CORE_BASE_COREJAVAINTERFACE_H_

#include <ChilliSource/Chillisource.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>

#include <jni.h>

namespace CSBackend
{
	namespace Android
	{
		class CoreJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(CoreJavaInterface);
			//--------------------------------------------------------------------------------------
			/// Constructor
			///
			/// @author I Copland
			//--------------------------------------------------------------------------------------
			CoreJavaInterface();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Interface ID
			///
			/// @return Whether or not this object implements the given interface.
			//--------------------------------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//--------------------------------------------------------------------------------------
			/// Sets the Chilli Source Application pointer.
			///
			/// @author I Copland
			///
			/// @param the application pointer.
			//--------------------------------------------------------------------------------------
			void SetApplication(CSCore::Application* in_application);
			//--------------------------------------------------------------------------------------
			/// Deletes the Chilli Source Application pointer.
			///
			/// @author I Copland
			//--------------------------------------------------------------------------------------
			void DestroyApplication();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the global instance of the MoFlow application.
			//--------------------------------------------------------------------------------------
			CSCore::Application* GetApplication();
			//-----------------------------------------
			/// @author I Copland
			///
			/// @param The maximum frames per second
			/// to clamp to. This should be in multiples
			/// of 15 (15, 30, 60)
			//-----------------------------------------
			void SetPreferredFPS(u32 in_maxFPS);
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the External Storage Directory as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetExternalStorageDirectory();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the Application name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetApplicationName();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the Application version code as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			u32 GetApplicationVersionCode();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the Application version name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetApplicationVersionName();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the package name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetPackageName();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the APK directory as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetAPKDirectory();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the screen width as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetScreenWidth();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the screen height as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetScreenHeight();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the default locale code as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDefaultLocaleCode();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the device model as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceModel();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the device model type as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceModelType();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the device manufacturer as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceManufacturer();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the os version number as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetOSVersionCode();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the number of cores as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetNumberOfCores();
			//--------------------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @return returns the screen density as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			f32 GetScreenDensity();
			//--------------------------------------------------------------
			/// This returns the telephony device ID acquired from the
			/// CoreNativeInterface Java class.
			///
			/// This id is not accessable if the device does not contain a
			/// sim card, or if the sim is unavailable (flight mode). If the
			/// id is not accessable an empty string will be returned. This
			/// Should not change on factory reset of a device, but it may
			/// change if the sim card is changed.
			///
			/// @author I Copland
			///
			/// @return the telephony device ID or an empty string.
			//--------------------------------------------------------------
			std::string GetTelephonyDeviceID();
			//--------------------------------------------------------------
			/// This returns the Mac Address acquired from the CoreNativeInterface
			/// Java class.
			///
			/// This may not be available on some devices while wifi is turned
			/// off. This should not change on factory reset of a device.
			///
			/// @author I Copland
			///
			/// @return the Mac Address or an empty string.
			//--------------------------------------------------------------
			std::string GetMacAddress();
			//--------------------------------------------------------------
			/// This returns the Android ID as acquired from the
			/// CoreNativeInterface Java class.
			///
			/// This should be unique however a bug in 2.2 has lead to a large
			/// amount of devices on 2.2 sharing the same ID (9774d56d682e549c).
			/// This ID is checked for and, if found, is discarded. If the id is
			/// unavailable or the duplicate ID is found, an empty string
			/// will be returned. This value may change if the device is
			/// factory reset.
			///
			/// @author I Copland
			///
			/// @return the unique Android ID or an empty string.
			//--------------------------------------------------------------
			std::string GetAndroidID();
            //-----------------------------------------------------------------------------------------------------
            /// @author I Copland
            ///
            /// Kill the application process
            //-----------------------------------------------------------------------------------------------------
            void ForceQuit();
            //-----------------------------------------------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return the system time in milliseconds.
            //-----------------------------------------------------------------------------------------------------
            TimeIntervalMs GetSystemTimeInMilliseconds();
            
		private:

			CSCore::Application* m_application;
			s32 m_screenWidth;
			s32 m_screenHeight;
			std::string m_deviceModel;
			std::string m_deviceModelType;
			std::string m_deviceManufacturer;
			std::string m_localeCode;
			s32 m_OSVersionCode;
			s32 m_numberOfCores;
			f32 m_screenDensity;
			f32 m_physicalScreenSize;
		};
	}
}

#endif

#endif