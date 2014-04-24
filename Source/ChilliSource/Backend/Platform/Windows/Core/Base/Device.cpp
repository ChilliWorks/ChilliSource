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

#include <ChilliSource/Backend/Platform/Windows/Core/Base/Device.h>

#include <windows.h>

namespace ChilliSource
{
    namespace Windows
    {
        namespace
        {
            //----------------------------------------------
            //----------------------------------------------
            std::string GetDeviceModel()
            {
				return "Windows";
            }
            //----------------------------------------------
            //----------------------------------------------
            std::string GetDeviceModelType()
            {
				return "PC";
            }
            //---------------------------------------------
            //---------------------------------------------
            std::string GetDeviceManufacturer()
            {
				return "Microsoft";
            }
            //---------------------------------------------
            //---------------------------------------------
            std::string GetOSVersion()
            {
				OSVERSIONINFOEX osvi;
				ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX)); osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
				GetVersionEx((OSVERSIONINFO*)&osvi);
				return std::string(Core::ToString((u32)osvi.dwMajorVersion) + "." + Core::ToString((u32)osvi.dwMinorVersion));
            }
            //---------------------------------------------
            //---------------------------------------------
            Core::Locale GetLocale()
            {
				wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = { 0 };

				if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
				{
					if (wcscmp(localeName, L"en") == 0)	return Core::Locale("en");
					if (wcscmp(localeName, L"fr") == 0)	return Core::Locale("fr");
					if (wcscmp(localeName, L"it") == 0)  return Core::Locale("it");
					if (wcscmp(localeName, L"de") == 0)  return Core::Locale("de");
					if (wcscmp(localeName, L"es") == 0)  return Core::Locale("es");
					if (wcscmp(localeName, L"jp") == 0)  return Core::Locale("jp");
				}

				//Just default to english
				return Core::Locale("en");
            }
            //---------------------------------------------
            //---------------------------------------------
            Core::Locale GetLanguage()
            {
				wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = { 0 };

				if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
				{
					if (wcscmp(localeName, L"en") == 0)	return Core::Locale("en");
					if (wcscmp(localeName, L"fr") == 0)	return Core::Locale("fr");
					if (wcscmp(localeName, L"it") == 0)  return Core::Locale("it");
					if (wcscmp(localeName, L"de") == 0)  return Core::Locale("de");
					if (wcscmp(localeName, L"es") == 0)  return Core::Locale("es");
					if (wcscmp(localeName, L"jp") == 0)  return Core::Locale("jp");
				}

				//Just default to english
				return Core::Locale("en");
            }
            //-----------------------------------------------
            //-----------------------------------------------
            std::string GetUDID()
            {
				CS_LOG_ERROR("PlatformSystem::GetDeviceID() has not been implemented!");
				return "FAKE ID";
            }
            //-------------------------------------------------
            //-------------------------------------------------
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
            : m_locale(Core::kUnknownLocale), m_language(Core::kUnknownLocale)
        {
			m_model = Windows::GetDeviceModel();
			m_modelType = Windows::GetDeviceModelType();
			m_manufacturer = Windows::GetDeviceManufacturer();
			m_locale = Windows::GetLocale();
			m_language = Windows::GetLanguage();
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
        const std::string& Device::GetModel()
        {
            return m_model;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModelType()
        {
            return m_modelType;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetManufacturer()
        {
            return m_manufacturer;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        Core::Locale& Device::GetLocale()
        {
            return m_locale;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        Core::Locale& Device::GetLanguage()
        {
            return m_language;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetOSVersion()
        {
            return m_osVersion;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetUDID()
        {
            return m_udid;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        u32 Device::GetNumberOfCPUCores()
        {
            return m_numCPUCores;
        }
    }
}


