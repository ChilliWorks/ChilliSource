//
//  Device.h
//  moFloTest
//
//  Created by Scott Downie on 03/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_CORE_DEVICE_H_
#define _MO_FLO_CORE_DEVICE_H_

#include <ChilliSource/Core/Locales.h>

#include <ChilliSource/Core/PlatformSystem.h>

namespace moFlo
{
    namespace Core
    {
        class CDevice
        {
        public:
            //---------------------------------------------
            /// Init
            ///
            /// @param Platform system
            //---------------------------------------------
            static void Init(IPlatformSystem* inpPlatformSystem);
            //---------------------------------------------
            /// Get Model Name
            ///
            /// @return Device model Name
            //---------------------------------------------
            static const std::string& GetModelName();
            //---------------------------------------------
            /// Get Model Type Name
            ///
            /// @return Device model type Name
            //---------------------------------------------
            static const std::string& GetModelTypeName();
            //---------------------------------------------
            /// Get Manufacturer Name
            ///
            /// @return Device manufacturer Name
            //---------------------------------------------
            static const std::string& GetManufacturerName();
            //---------------------------------------------
            /// Get Locale
            ///
            /// @return the locale.
            //---------------------------------------------
            static CLocale& GetLocale();
            //---------------------------------------------
            /// Get Langauge
            ///
            /// @return the langauge in locale format
            //---------------------------------------------
            static CLocale& GetLanguage();
            //---------------------------------------------
            /// Get OS Version
            ///
            /// @return String containing OS version
            //---------------------------------------------
            static const std::string& GetOSVersion();
            //---------------------------------------------
            /// Get UDID
            ///
            /// @return String containing unique device ID
            //---------------------------------------------
            static const std::string& GetUDID();
			//---------------------------------------------
			/// Get Num CPU Cores
			///
			/// @return The number of cores available on device
			//--------------------------------------------
			static u32 GetNumCPUCores();
			//--------------------------------------------
			/// Get Physical Screen Size
			///
			/// @param The physical size of the screen in
			/// inches.
			//--------------------------------------------
			static f32 GetPhysicalScreenSize();
            
        private:
            
            static std::string mstrDeviceModel;
            static std::string mstrDeviceModelType;
            static std::string mstrDeviceManufacturer;
            static std::string mstrDeviceID;
            static CLocale mLocale;
            static CLocale mLanguage;
            static std::string mstrOSVersion;
			static u32 mudwNumCores;
			static f32 mfPhysicalScreenSize;
        };
    }
}

#endif
