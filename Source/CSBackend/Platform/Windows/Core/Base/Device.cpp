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

#include <CSBackend/Platform/Windows/Core/Base/Device.h>

#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>

#include <algorithm>

//this needs to be included last
#include <windows.h>

#pragma comment(lib, "version.lib" )

namespace CSBackend
{
    namespace Windows
    {
        namespace
        {
			const std::string k_defaultLocale = "en_US";
			const std::string k_defaultLanguage = "en";

            //----------------------------------------------
			/// @author Ian Copland
			///
			/// @return The device model name.
            //----------------------------------------------
            std::string GetDeviceModel()
            {
				return "Windows";
            }
			//----------------------------------------------
			/// @author Ian Copland
			///
			/// @return The device model type name.
			//----------------------------------------------
            std::string GetDeviceModelType()
            {
				return "PC";
            }
			//----------------------------------------------
			/// @author Ian Copland
			///
			/// @return The device manufacturer name.
			//----------------------------------------------
            std::string GetDeviceManufacturer()
            {
				return "Microsoft";
            }
			//----------------------------------------------
			/// This reads the version info of the 
			/// kernel32.dll system library and pulls the
			/// OS version from it, as recommended by
			/// MSDN:
			///
			/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724429(v=vs.85).aspx
			///
			/// @author Ian Copland
			///
			/// @return The OS version number string.
			//----------------------------------------------
            std::string GetOSVersion() noexcept
            {
				// This is based off the answer given by Chuck Walbourn here: http://stackoverflow.com/questions/22303824/warning-c4996-getversionexw-was-declared-deprecated

				WCHAR path[_MAX_PATH];
				if (!GetSystemDirectoryW(path, _MAX_PATH))
				{
					CS_LOG_FATAL("Failed to get the system directory path.");
				}

				wcscat_s(path, L"\\kernel32.dll");

				DWORD handle;
				DWORD len = GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, path, &handle);
				CS_ASSERT(len > 0, "Failed to get the kernel32.dll file version info size.");

				std::unique_ptr<uint8_t> buff(new uint8_t[len]);
				if (!GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, path, 0, len, buff.get()))
				{
					CS_LOG_FATAL("Failed to get the kernel32.dll file version info.");
				}

				VS_FIXEDFILEINFO *vInfo = nullptr;
				UINT infoSize;

				if (!VerQueryValueW(buff.get(), L"\\", reinterpret_cast<LPVOID*>(&vInfo), &infoSize))
				{
					CS_LOG_FATAL("Failed to query version info.");
				}

				if (infoSize == 0)
				{
					CS_LOG_FATAL("Info size is 0.");
				}

				return ChilliSource::ToString(HIWORD(vInfo->dwFileVersionMS)) + "." +
					ChilliSource::ToString(LOWORD(vInfo->dwFileVersionMS)) + "." +
					ChilliSource::ToString(HIWORD(vInfo->dwFileVersionLS)) + "." +
					ChilliSource::ToString(LOWORD(vInfo->dwFileVersionLS));
            }
			//----------------------------------------------
			/// @author Ian Copland
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
			/// @author Ian Copland
			///
			/// @param The locale code.
			///
			/// @return The language code.
			//----------------------------------------------------
			std::string ParseLanguageFromLocale(const std::string& in_locale)
			{
				std::vector<std::string> strLocaleBrokenUp = ChilliSource::StringUtils::Split(in_locale, "_", 0);

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
			/// @author Ian Copland
			///
			/// @return The UDID.
			//----------------------------------------------
            std::string GetUDID()
            {
				CS_LOG_ERROR("PlatformSystem::GetDeviceID() has not been implemented!");
				return "FAKE ID";
            }
			//----------------------------------------------
			/// @author Ian Copland
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
			m_model = CSBackend::Windows::GetDeviceModel();
			m_modelType = CSBackend::Windows::GetDeviceModelType();
			m_manufacturer = CSBackend::Windows::GetDeviceManufacturer();
			m_locale = CSBackend::Windows::GetLocale();
			m_language = CSBackend::Windows::ParseLanguageFromLocale(m_locale);
			m_osVersion = CSBackend::Windows::GetOSVersion();
			m_udid = CSBackend::Windows::GetUDID();
            m_numCPUCores = CSBackend::Windows::GetNumberOfCPUCores();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Device::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
            return (ChilliSource::Device::InterfaceID == in_interfaceId || Device::InterfaceID == in_interfaceId);
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
