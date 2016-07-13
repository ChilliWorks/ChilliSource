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

namespace ChilliSource
{
    /// This class contains all information necessary for building
    /// the Device, PlatformSystem, and Screen classes.
    ///
    class SystemInfo final
    {
        public:
            /// @return The device info for building the Device class.
            ///
            const DeviceInfo& GetDeviceInfo() const noexcept;

            /// Constructs the SystemInfo class.
            ///
            /// @param model
            ///         The device model name.
            /// @param modelType
            ///         The device model type.
            /// @param manufacturer
            ///         The name of the device manufacturer.
            /// @param udid
            ///         An identifier that can be used to uniquely
            ///         identify the device.
            /// @param locale
            ///         The locale code registered with the device.
            /// @param language
            ///         The language the device is set to.
            /// @param osVersion
            ///         The version of the operating system.
            /// @param numCPUCores
            ///         The number of CPU cores available on the device.
            ///
            SystemInfo(const std::string& model, const std::string& modelType, const std::string& manufacturer, const std::string& udid, const std::string& locale, const std::string& language, const std::string& osVersion, u32 numCPUCores) noexcept;

        private:
            DeviceInfo m_deviceInfo;
    };
}

#endif