//
//  Device.cpp
//  moFloTest
//
//  Created by Scott Downie on 03/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Device.h>

namespace ChilliSource
{
    namespace Core
    {
        std::string Device::mstrDeviceModel;
		std::string Device::mstrDeviceModelType;
		std::string Device::mstrDeviceManufacturer;
		Locale Device::mLocale = kUnknownLocale;
		Locale Device::mLanguage = kUnknownLocale;
        std::string Device::mstrOSVersion;
        std::string Device::mstrDeviceID;
		u32 Device::mudwNumCores = 0;
		f32 Device::mfPhysicalScreenSize = 0.0f;
        
        //---------------------------------------------
        /// Init
        ///
        /// @param Platform system
        //---------------------------------------------
        void Device::Init(PlatformSystem* inpPlatformSystem)
        {
            mstrDeviceModel = inpPlatformSystem->GetDeviceModelName();
            mstrDeviceModelType = inpPlatformSystem->GetDeviceModelTypeName();
            mstrDeviceManufacturer = inpPlatformSystem->GetDeviceManufacturerName();
            mLocale = inpPlatformSystem->GetLocale();
            mLanguage = inpPlatformSystem->GetLanguage();
            mstrOSVersion = inpPlatformSystem->GetOSVersion();
            mstrDeviceID = inpPlatformSystem->GetDeviceID();
			mudwNumCores = inpPlatformSystem->GetNumberOfCPUCores();
			mfPhysicalScreenSize = inpPlatformSystem->GetPhysicalScreenSize();
        }
        //---------------------------------------------
        /// Get Model Name
        ///
        /// @return Device model Name
        //---------------------------------------------
        const std::string& Device::GetModelName()
        {
        	return mstrDeviceModel;
        }
        //---------------------------------------------
        /// Get Model Type Name
        ///
        /// @return Device model type Name
        //---------------------------------------------
        const std::string& Device::GetModelTypeName()
        {
        	return mstrDeviceModelType;
        }
        //---------------------------------------------
        /// Get Manufacturer Name
        ///
        /// @return Device manufacturer Name
        //---------------------------------------------
        const std::string& Device::GetManufacturerName()
        {
        	return mstrDeviceManufacturer;
        }
        //---------------------------------------------
        /// Get Locale
        ///
        /// @return the locale
        //---------------------------------------------
        Locale& Device::GetLocale()
        {
            return mLocale;
        }
        //---------------------------------------------
        /// Get Language
        ///
        /// @return the language in locale format
        //---------------------------------------------
        Locale& Device::GetLanguage()
        {
            return mLanguage;
        }
        //---------------------------------------------
        /// Get OS Version
        ///
        /// @return String containing OS version
        //---------------------------------------------
        const std::string& Device::GetOSVersion()
        {
            return mstrOSVersion;
        }
        //---------------------------------------------
        /// Get UDID
        ///
        /// @return String containing unique device ID
        //---------------------------------------------
        const std::string& Device::GetUDID()
        {
            return mstrDeviceID;
        }
		//---------------------------------------------
		/// Get Num CPU Cores
		///
		/// @return The number of cores available on device
		//--------------------------------------------
		u32 Device::GetNumCPUCores()
		{
			return mudwNumCores;
		}
		//--------------------------------------------
		/// Get Physical Screen Size
		///
		/// @param The physical size of the screen in
		/// inches.
		//--------------------------------------------
		f32 Device::GetPhysicalScreenSize()
		{
			return mfPhysicalScreenSize;
		}
    }
}
