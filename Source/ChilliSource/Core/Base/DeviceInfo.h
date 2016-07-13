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

#ifndef _CHILLISOURCE_CORE_BASE_DEVICEINFO_H_
#define _CHILLISOURCE_CORE_BASE_DEVICEINFO_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// This class contains information necessary for building
    /// the Device class.
    ///
    /// This class is immutable, and therefore thread-safe.
    ///
    class DeviceInfo final
    {
        public:
            /// Constructs the DeviceInfo class.
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
            DeviceInfo(const std::string& model, const std::string& modelType, const std::string& manufacturer, const std::string& udid, const std::string& locale, const std::string& language, const std::string& osVersion, u32 numCPUCores) noexcept;

            /// @return The device model name.
            ///
            const std::string& GetModel() const noexcept;

            /// @return The name of the device model type.
            ///
            const std::string& GetModelType() const noexcept;

            /// @return The name of the device manufacturer.
            ///
            const std::string& GetManufacturer() const noexcept;

            /// @return An identifier that can be used to uniquely
            /// identify the device.
            ///
            const std::string& GetUDID() const noexcept;

            /// @return The locale code registered with the
            /// device.
            ///
            const std::string& GetLocale() const noexcept;

            /// @return The language the device is set to.
            ///
            const std::string& GetLanguage() const noexcept;

            /// @return The version of the operating system.
            ///
            const std::string& GetOSVersion() const noexcept;

            /// @return The number of CPU cores available on the
            /// device.
            ///
            u32 GetNumCPUCores() const noexcept;

        private:
            std::string m_model;
            std::string m_modelType;
            std::string m_manufacturer;
            std::string m_udid;
            std::string m_locale;
            std::string m_language;
            std::string m_osVersion;
            u32 m_numCPUCores;
    };
}

#endif