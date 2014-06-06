//
//  Device.cpp
//  Chilli Source
//  Created by Ian Copland on 24/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <ChilliSource/Backend/Platform/Windows/Core/Base/Device.h>

#include <ChilliSource/Backend/Platform/Windows/Core/String/WindowsStringUtils.h>

#include <algorithm>

//this needs to be included last
#include <windows.h>

namespace ChilliSource
{
    namespace Windows
    {
        namespace
        {
			const std::string k_defaultLocale = "en_US";
			const std::string k_defaultLanguage = "en";

            //----------------------------------------------
			/// @author I Copland
			///
			/// @return The device model name.
            //----------------------------------------------
            std::string GetDeviceModel()
            {
				return "Windows";
            }
			//----------------------------------------------
			/// @author I Copland
			///
			/// @return The device model type name.
			//----------------------------------------------
            std::string GetDeviceModelType()
            {
				return "PC";
            }
			//----------------------------------------------
			/// @author I Copland
			///
			/// @return The device manufacturer name.
			//----------------------------------------------
            std::string GetDeviceManufacturer()
            {
				return "Microsoft";
            }
			//----------------------------------------------
			/// @author I Copland
			///
			/// @return The OS version number string.
			//----------------------------------------------
            std::string GetOSVersion()
            {
				OSVERSIONINFOEX osvi;
				ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX)); osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
				GetVersionEx((OSVERSIONINFO*)&osvi);
				return std::string(Core::ToString((u32)osvi.dwMajorVersion) + "." + Core::ToString((u32)osvi.dwMinorVersion));
            }
			//----------------------------------------------
			/// @author I Copland
			///
			/// @return The current locale.
			//----------------------------------------------
            std::string GetLocale()
            {
				wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = { 0 };

				if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
				{
					std::wstring wideLocale(localeName);
					std::string locale = WindowsStringUtils::UTF16ToUTF8(wideLocale);
					std::replace(locale.begin(), locale.end(), '-', '_');
					return locale;
				}

				return k_defaultLocale;
            }
			//----------------------------------------------------
			/// Returns the language portion of a locale code.
			///
			/// @author I Copland
			///
			/// @param The locale code.
			///
			/// @return The language code.
			//----------------------------------------------------
			std::string ParseLanguageFromLocale(const std::string& in_locale)
			{
				std::vector<std::string> strLocaleBrokenUp = ChilliSource::Core::StringUtils::Split(in_locale, "_", 0);

				if (strLocaleBrokenUp.size() > 0)
				{
					return strLocaleBrokenUp[0];
				}
				else
				{
					return k_defaultLanguage;
				}
			}
			//----------------------------------------------
			/// @author I Copland
			///
			/// @return The UDID.
			//----------------------------------------------
            std::string GetUDID()
            {
				CS_LOG_ERROR("PlatformSystem::GetDeviceID() has not been implemented!");
				return "FAKE ID";
            }
			//----------------------------------------------
			/// @author I Copland
			///
			/// @return The number of cores.
			//----------------------------------------------
            u32 GetNumberOfCPUCores()
            {
				SYSTEM_INFO SysInfo;
				GetSystemInfo(&SysInfo);
				return SysInfo.dwNumberOfProcessors;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(Device);
        //----------------------------------------------------
        //----------------------------------------------------
        Device::Device()
			: m_locale(k_defaultLocale), m_language(k_defaultLanguage)
        {
			m_model = Windows::GetDeviceModel();
			m_modelType = Windows::GetDeviceModelType();
			m_manufacturer = Windows::GetDeviceManufacturer();
			m_locale = Windows::GetLocale();
			m_language = Windows::ParseLanguageFromLocale(m_locale);
			m_osVersion = Windows::GetOSVersion();
			m_udid = Windows::GetUDID();
            m_numCPUCores = Windows::GetNumberOfCPUCores();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Device::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Core::Device::InterfaceID == in_interfaceId || Device::InterfaceID == in_interfaceId);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModel() const
        {
            return m_model;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetModelType() const
        {
            return m_modelType;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetManufacturer() const
        {
            return m_manufacturer;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetLocale() const
        {
            return m_locale;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetLanguage() const
        {
            return m_language;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetOSVersion() const
        {
            return m_osVersion;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetUDID() const
        {
            return m_udid;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		u32 Device::GetNumberOfCPUCores() const
        {
            return m_numCPUCores;
        }
    }
}

#endif
