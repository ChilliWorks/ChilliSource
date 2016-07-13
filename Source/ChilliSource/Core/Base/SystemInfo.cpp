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

#include <ChilliSource/Core/Base/SystemInfo.h>

namespace ChilliSource
{
    //----------------------------------------------------------------
    SystemInfo::SystemInfo(const std::string& model, const std::string& modelType, const std::string& manufacturer, const std::string& udid, const std::string& locale, const std::string& language, const std::string& osVersion, u32 numCPUCores) noexcept
        : m_deviceInfo(DeviceInfo(model, modelType, manufacturer, udid, locale, language, osVersion, numCPUCores))
    {   
    }

    //----------------------------------------------------------------
    const DeviceInfo& SystemInfo::GetDeviceInfo() const noexcept
    {
        return m_deviceInfo;
    }
}