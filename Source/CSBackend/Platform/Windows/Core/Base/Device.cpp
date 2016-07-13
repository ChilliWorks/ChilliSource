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

#include <ChilliSource/Core/Base/SystemInfo.h>

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
        CS_DEFINE_NAMEDTYPE(Device);
        //----------------------------------------------------
        //----------------------------------------------------
        Device::Device(const ChilliSource::DeviceInfo& deviceInfo) noexcept
        {
			m_model = deviceInfo.GetModel();
            m_modelType = deviceInfo.GetModelType();
            m_manufacturer = deviceInfo.GetManufacturer();
			m_locale = deviceInfo.GetLocale();
            m_language = deviceInfo.GetLanguage();
            m_osVersion = deviceInfo.GetOSVersion();
            m_udid = deviceInfo.GetUDID();
            m_numCPUCores = deviceInfo.GetNumCPUCores();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Device::IsA(ChilliSource::InterfaceIDType in_interfaceId) const noexcept
        {
            return (ChilliSource::Device::InterfaceID == in_interfaceId || Device::InterfaceID == in_interfaceId);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModel() const noexcept
        {
            return m_model;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetModelType() const noexcept
        {
            return m_modelType;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetManufacturer() const noexcept
        {
            return m_manufacturer;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetLocale() const noexcept
        {
            return m_locale;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetLanguage() const noexcept
        {
            return m_language;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetOSVersion() const noexcept
        {
            return m_osVersion;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		const std::string& Device::GetUDID() const noexcept
        {
            return m_udid;
        }
        //---------------------------------------------------
        //---------------------------------------------------
		u32 Device::GetNumberOfCPUCores() const noexcept
        {
            return m_numCPUCores;
        }
    }
}

#endif
