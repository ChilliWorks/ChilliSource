/**
 * File: JavaInterface.h
 * Date: 1 Oct 2010
 * Description: Interface functions for calls to and from java.
 */

/**
 *
 * Author: Ian Copland
 * Version 1.0 - moFlo
 * Copyright �2010 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLOW_PLATFORM_ANDROID_CORE_JAVA_INTERFACE_JAVA_INTERFACE_
#define _MOFLOW_PLATFORM_ANDROID_CORE_JAVA_INTERFACE_JAVA_INTERFACE_

#include <jni.h>
#include <map>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>

extern ChilliSource::Core::Application* (*BootFunctionPtr)();

namespace ChilliSource
{
	namespace Android
	{
		class CCoreJavaInterface : public IJavaInterface
		{
		public:
			DECLARE_NAMED_INTERFACE();
			//--------------------------------------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------------------------------------
			CCoreJavaInterface();
			//--------------------------------------------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------------------------------------------
			/// Set Application
			///
			/// Sets the MoFlow CApplication pointer.
			///
			/// @param the application pointer.
			//--------------------------------------------------------------------------------------
			void SetApplication(ChilliSource::Core::Application* inApplication);
			//--------------------------------------------------------------------------------------
			/// Destroy Application
			///
			/// Deletes the MoFlow CApplication pointer.
			//--------------------------------------------------------------------------------------
			void DestroyApplication();
			//--------------------------------------------------------------------------------------
			/// Get Application
			///
			/// @return returns the global instance of the MoFlow application.
			//--------------------------------------------------------------------------------------
			ChilliSource::Core::Application* GetApplication();
			//-----------------------------------------
			/// Set Max FPS
			///
			/// @param The maximum frames per second
			/// to clamp to. This should be in multiples
			/// of 15 (15, 30, 60)
			//-----------------------------------------
			void SetMaxFPS(u32 inudwFPS);
			//--------------------------------------------------------------------------------------
			/// Get External Storage Directory
			///
			/// @return returns the External Storage Directory as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetExternalStorageDirectory();
			//--------------------------------------------------------------------------------------
			/// Get Application Name
			///
			/// @return returns the Application name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetApplicationName();
			//--------------------------------------------------------------------------------------
			/// Get Application Version Code
			///
			/// @return returns the Application version code as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			u32 GetApplicationVersionCode();
			//--------------------------------------------------------------------------------------
			/// Get Application Version Name
			///
			/// @return returns the Application version name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetApplicationVersionName();
			//--------------------------------------------------------------------------------------
			/// Get Package Name
			///
			/// @return returns the package name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetPackageName();
			//--------------------------------------------------------------------------------------
			/// Get APK Directory
			///
			/// @return returns the APK directory as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetAPKDirectory();
			//--------------------------------------------------------------------------------------
			/// Get Device Orientation
			///
			/// The value returns is moFlow orientation flag. The result is not cached
			///
			/// @return Orientation as reported by the OS returned as a moFlow orientation flag
			//--------------------------------------------------------------------------------------
			s32 GetOrientation();
			//--------------------------------------------------------------------------------------
			/// Get Screen Width
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the screen width as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetScreenWidth();
			//--------------------------------------------------------------------------------------
			/// Get Screen Height
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the screen height as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetScreenHeight();
			//--------------------------------------------------------------------------------------
			/// Get Default Locale Code
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the default locale code as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDefaultLocaleCode();
			//--------------------------------------------------------------------------------------
			/// Get Device Model
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the device model as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceModel();
			//--------------------------------------------------------------------------------------
			/// Get Device Model Type
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the device model type as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceModelType();
			//--------------------------------------------------------------------------------------
			/// Get Device Manufacturer
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the device manufacturer as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceManufacturer();
			//--------------------------------------------------------------------------------------
			/// Get OS Version Code
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the os version number as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetOSVersionCode();
			//--------------------------------------------------------------------------------------
			/// Get Number Of Cores
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the number of cores as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetNumberOfCores();
			//--------------------------------------------------------------------------------------
			/// Get Screen Density
			///
			/// This value is cached, so repeated calls wont result in extra c-to-java calls.
			///
			/// @return returns the screen density as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			f32 GetScreenDensity();
			//--------------------------------------------------------------
			/// Get Telephony Device ID
			///
			/// This returns the telephony device ID acquired from the
			/// CoreNativeInterface Java class.
			///
			/// This id is not accessable if the device does not contain a
			/// sim card, or if the sim is unavailable (flight mode). If the
			/// id is not accessable an empty string will be returned. This
			/// Should not change on factory reset of a device, but it may
			/// change if the sim card is changed.
			///
			/// @return the telephony device ID or an empty string.
			//--------------------------------------------------------------
			std::string GetTelephonyDeviceID();
			//--------------------------------------------------------------
			/// Get Mac Address
			///
			/// This returns the Mac Address acquired from the CoreNativeInterface
			/// Java class.
			///
			/// This may not be available on some devices while wifi is turned
			/// off. This should not change on factory reset of a device.
			///
			/// @return the Mac Address or an empty string.
			//--------------------------------------------------------------
			std::string GetMacAddress();
			//--------------------------------------------------------------
			/// Get Android ID
			///
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
			/// @return the unique Android ID or an empty string.
			//--------------------------------------------------------------
			std::string GetAndroidID();
            //--------------------------------------------------------------------------------------------------
            /// Make Toast
            ///
            /// Display a toast notification with the given text
            ///
            /// @param Text
            //--------------------------------------------------------------------------------------------------
            void MakeToast(const Core::UTF8String& instrText);
            //--------------------------------------------------------------------------------------------------
            /// Show System Confirm Dialog
            ///
            /// Display the systems confirmation dialog
            ///
            /// @param ID that is returned in pressed delegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            /// @param Cancel text
            //--------------------------------------------------------------------------------------------------
            void ShowSystemConfirmDialog(s32 indwDialogID, const Core::UTF8String& instrTitle, const Core::UTF8String& instrMessage, const Core::UTF8String& instrConfirm, const Core::UTF8String& instrCancel);
            //--------------------------------------------------------------------------------------------------
            /// Show System Dialog
            ///
            /// Display the systems confirmation dialog
            ///
            /// @param ID that is returned in pressed delegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            //--------------------------------------------------------------------------------------------------
            void ShowSystemDialog(s32 indwDialogID, const Core::UTF8String& instrTitle, const Core::UTF8String& instrMessage, const Core::UTF8String& instrConfirm);
            //-----------------------------------------------------------------------------------------------------
            /// Force Quit
            ///
            /// Kill the current process
            //-----------------------------------------------------------------------------------------------------
            void ForceQuit();
            //-----------------------------------------------------------------------------------------------------
            /// Get System Time In Milliseconds
            ///
            /// @return the system time in milliseconds.
            //-----------------------------------------------------------------------------------------------------
            TimeIntervalMs GetSystemTimeInMilliseconds();
            //-----------------------------------------------------------------------------------------------------
            /// Get Physical Screen Size
            ///
            /// @return the physical screen size in inches.
            //-----------------------------------------------------------------------------------------------------
            f32 GetPhysicalScreenSize();
            
		private:
			ChilliSource::Core::Application* mApplication;
			s32 mdwScreenWidth;
			s32 mdwScreenHeight;
			std::string mstrDeviceModel;
			std::string mstrDeviceModelType;
			std::string mstrDeviceManufacturer;
			std::string mstrLocaleCode;
			s32 mdwOSVersionCode;
			s32 mdwNumberOfCores;
			f32 mfScreenDensity;
			f32 mfPhysicalScreenSize;
		};
	}
}

#endif
