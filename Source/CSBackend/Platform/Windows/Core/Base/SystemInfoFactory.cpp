//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#include <ChilliSource/Core/Base/DeviceInfo.h>
#include <ChilliSource/Core/Base/ScreenInfo.h>
#include <ChilliSource/Core/Base/SystemInfo.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <CSBackend/Platform/Windows/Core/Base/Screen.h>
#include <CSBackend/Platform/Windows/Core/Base/SystemInfoFactory.h>
#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <CSBackend/Rendering/OpenGL/Base/RenderInfoFactory.h>

#include <algorithm>
#include <vector>

//this needs to be included last
#include <Windows.h>

#pragma comment(lib, "version.lib" )

namespace CSBackend
{
    namespace Windows
    {
        namespace
        {
            const std::string k_defaultLocale = "en_US";
            const std::string k_defaultLanguage = "en";
            const std::string k_deviceModel = "Windows";
            const std::string k_deviceModelType = "PC";
            const std::string k_deviceManufacturer = "Microsoft";
            const std::string k_deviceUdid = "FAKE ID";

            /// This reads the version info of the 
            /// kernel32.dll system library and pulls the
            /// OS version from it, as recommended by
            /// MSDN:
            ///
            /// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724429(v=vs.85).aspx
            ///
            /// @return The OS version number string.
            ///
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

            /// @return The current locale.
            ///
            std::string GetLocale() noexcept
            {
                wchar_t localeName[LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)] = { 0 };

                if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
                {
                    std::wstring wideLocale(localeName);
                    std::string locale = WindowsStringUtils::UTF16ToUTF8(wideLocale);
                    std::replace(locale.begin(), locale.end(), '-', '_');
                    return locale;
                }

                return k_defaultLocale;
            }

            /// Returns the language portion of a locale code.
            ///
            /// @param The locale code.
            ///
            /// @return The language code.
            ///
            std::string ParseLanguageFromLocale(const std::string& in_locale) noexcept
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

            /// @return The number of cores.
            ///
            u32 GetNumberOfCPUCores() noexcept
            {
                SYSTEM_INFO SysInfo;
                GetSystemInfo(&SysInfo);
                return SysInfo.dwNumberOfProcessors;
            }

            /// @return The screen's current resolution.
            ///
            ChilliSource::Vector2 GetScreenResolution() noexcept
            {
                ChilliSource::Integer2 resolution = SFMLWindow::Get()->GetWindowSize();
                return ChilliSource::Vector2((f32)resolution.x, (f32)resolution.y);
            }

            /// @return A list of resolutions supported by the display
            ///
            std::vector<ChilliSource::Integer2> GetSupportedFullscreenResolutions() noexcept
            {
                return SFMLWindow::Get()->GetSupportedFullscreenResolutions();
            }

        }

        //--------------------------------------------------------------------------------
        ChilliSource::SystemInfoCUPtr SystemInfoFactory::CreateSystemInfo() noexcept
        {
            // Create DeviceInfo.
            ChilliSource::DeviceInfo deviceInfo(k_deviceModel, k_deviceModelType, k_deviceManufacturer, k_deviceUdid, GetLocale(), ParseLanguageFromLocale(GetLocale()), GetOSVersion(), GetNumberOfCPUCores());

            // Create ScreenInfo.
            ChilliSource::ScreenInfo screenInfo(GetScreenResolution(), 1.0f, 1.0f, GetSupportedFullscreenResolutions());

			//Create RenderInfo
			ChilliSource::RenderInfo renderInfo = OpenGL::RenderInfoFactory::CreateRenderInfo();

            // Create SystemInfo.
            ChilliSource::SystemInfoUPtr systemInfo(new ChilliSource::SystemInfo(deviceInfo, screenInfo, renderInfo, ""));

            return std::move(systemInfo);
        }
        
    }
}

#endif