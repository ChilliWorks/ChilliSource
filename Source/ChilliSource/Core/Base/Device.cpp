//
//  Device.cpp
//  moFloTest
//
//  Created by Scott Downie on 03/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Device.h>

namespace moFlo
{
    namespace Core
    {
        std::string CDevice::mstrDeviceModel;
		std::string CDevice::mstrDeviceModelType;
		std::string CDevice::mstrDeviceManufacturer;
		CLocale CDevice::mLocale = kUnknownLocale;
		CLocale CDevice::mLanguage = kUnknownLocale;
        std::string CDevice::mstrOSVersion;
        std::string CDevice::mstrDeviceID;
		u32 CDevice::mudwNumCores = 0;
		f32 CDevice::mfPhysicalScreenSize = 0.0f;
        
        //---------------------------------------------
        /// Init
        ///
        /// @param Platform system
        //---------------------------------------------
        void CDevice::Init(IPlatformSystem* inpPlatformSystem)
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
        const std::string& CDevice::GetModelName()
        {
        	return mstrDeviceModel;
        }
        //---------------------------------------------
        /// Get Model Type Name
        ///
        /// @return Device model type Name
        //---------------------------------------------
        const std::string& CDevice::GetModelTypeName()
        {
        	return mstrDeviceModelType;
        }
        //---------------------------------------------
        /// Get Manufacturer Name
        ///
        /// @return Device manufacturer Name
        //---------------------------------------------
        const std::string& CDevice::GetManufacturerName()
        {
        	return mstrDeviceManufacturer;
        }
        //---------------------------------------------
        /// Get Locale
        ///
        /// @return the locale
        //---------------------------------------------
        CLocale& CDevice::GetLocale()
        {
            return mLocale;
        }
        //---------------------------------------------
        /// Get Language
        ///
        /// @return the language in locale format
        //---------------------------------------------
        CLocale& CDevice::GetLanguage()
        {
            return mLanguage;
        }
        //---------------------------------------------
        /// Get OS Version
        ///
        /// @return String containing OS version
        //---------------------------------------------
        const std::string& CDevice::GetOSVersion()
        {
            return mstrOSVersion;
        }
        //---------------------------------------------
        /// Get UDID
        ///
        /// @return String containing unique device ID
        //---------------------------------------------
        const std::string& CDevice::GetUDID()
        {
            return mstrDeviceID;
        }
		//---------------------------------------------
		/// Get Num CPU Cores
		///
		/// @return The number of cores available on device
		//--------------------------------------------
		u32 CDevice::GetNumCPUCores()
		{
			return mudwNumCores;
		}
		//--------------------------------------------
		/// Get Physical Screen Size
		///
		/// @param The physical size of the screen in
		/// inches.
		//--------------------------------------------
		f32 CDevice::GetPhysicalScreenSize()
		{
			return mfPhysicalScreenSize;
		}
    }
}
