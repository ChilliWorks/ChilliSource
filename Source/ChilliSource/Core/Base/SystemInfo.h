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

#ifndef _CHILLISOURCE_CORE_BASE_SYSTEMINFO_H_
#define _CHILLISOURCE_CORE_BASE_SYSTEMINFO_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/DeviceInfo.h>
#include <ChilliSource/Core/Base/PlatformInfo.h>
#include <ChilliSource/Core/Base/ScreenInfo.h>

namespace ChilliSource
{
    /// This class contains all information necessary for building
    /// the Device, PlatformSystem, and Screen classes.
    ///
    class SystemInfo final
    {
    public:
        /// Constructs the SystemInfo class.
        ///
        /// @param deviceInfo
        ///     The device info to populate device with.
        /// @param screenInfo
        ///     The screen info to populate screen with.
        /// @param platformInfo
        ///     The platform info to populate the app with.
        ///
        SystemInfo(const DeviceInfo& deviceInfo, const ScreenInfo& screenInfo, const PlatformInfo& platformInfo) noexcept;

        /// @return The device info for building the Device class.
        ///
        const DeviceInfo& GetDeviceInfo() const noexcept;

        /// @return The screen info for building the Screen class.
        ///
        const ScreenInfo& GetScreenInfo() const noexcept;
        
        /// @return The platform info.
        ///
        const PlatformInfo& GetPlatformInfo() const noexcept;

    private:
        DeviceInfo m_deviceInfo;
        ScreenInfo m_screenInfo;
        PlatformInfo m_platformInfo;
    };
}

#endif