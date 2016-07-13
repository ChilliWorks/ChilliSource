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

#include <ChilliSource/Core/Base/DeviceInfo.h>

namespace ChilliSource
{
    //----------------------------------------------------------------
    DeviceInfo::DeviceInfo(const std::string& model, const std::string& modelType, const std::string& manufacturer, const std::string& udid, const std::string& locale, const std::string& language, const std::string& osVersion, u32 numCPUCores) noexcept
        : m_model(model), m_modelType(modelType), m_manufacturer(manufacturer), m_udid(udid), m_locale(locale), m_language(language), m_osVersion(osVersion), m_numCPUCores(numCPUCores) 
    {
    }

    //----------------------------------------------------------------
    const std::string& DeviceInfo::GetModel() const noexcept
    {
        return m_model;
    }
    
    //----------------------------------------------------------------
    const std::string& DeviceInfo::GetModelType() const noexcept
    {
        return m_modelType;
    }

    //----------------------------------------------------------------
    const std::string& DeviceInfo::GetManufacturer() const noexcept
    {
        return m_manufacturer;
    }

    //----------------------------------------------------------------
    const std::string& DeviceInfo::GetUDID() const noexcept
    {
        return m_udid;
    }

    //----------------------------------------------------------------
    const std::string& DeviceInfo::GetLocale() const noexcept
    {
        return m_locale;
    }

    //----------------------------------------------------------------
    const std::string& DeviceInfo::GetLanguage() const noexcept
    {
        return m_language;
    }

    //----------------------------------------------------------------
    const std::string& DeviceInfo::GetOSVersion() const noexcept
    {
        return m_osVersion;
    }

    //----------------------------------------------------------------
    u32 DeviceInfo::GetNumCPUCores() const noexcept
    {
        return m_numCPUCores;
    }
}