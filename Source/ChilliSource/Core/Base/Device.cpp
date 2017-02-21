//
//  Device.cpp
//  ChilliSource
//  Created by Scott Downie on 03/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/DeviceInfo.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Device);
    //--------------------------------------------------
    DeviceUPtr Device::Create(const ChilliSource::DeviceInfo& deviceInfo)
    {
        return DeviceUPtr(new Device(deviceInfo));
    }
    //----------------------------------------------------
    //----------------------------------------------------
    Device::Device(const ChilliSource::DeviceInfo& deviceInfo) noexcept
        : m_deviceInfo(deviceInfo)
    {
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    bool Device::IsA(ChilliSource::InterfaceIDType in_interfaceId) const noexcept
    {
        return (Device::InterfaceID == in_interfaceId);
    }
    //---------------------------------------------------
    //---------------------------------------------------
    const std::string& Device::GetModel() const noexcept
    {
        return m_deviceInfo.GetModel();
    }
    //---------------------------------------------------
    //---------------------------------------------------
    const std::string& Device::GetModelType() const noexcept
    {
        return m_deviceInfo.GetModelType();
    }
    //---------------------------------------------------
    //---------------------------------------------------
    const std::string& Device::GetManufacturer() const noexcept
    {
        return m_deviceInfo.GetManufacturer();
    }
    //---------------------------------------------------
    //---------------------------------------------------
    const std::string& Device::GetLocale() const noexcept
    {
        return m_deviceInfo.GetLocale();
    }
    //---------------------------------------------------
    //---------------------------------------------------
    const std::string& Device::GetLanguage() const noexcept
    {
        return m_deviceInfo.GetLanguage();
    }
    //---------------------------------------------------
    //---------------------------------------------------
    const std::string& Device::GetOSVersion() const noexcept
    {
        return m_deviceInfo.GetOSVersion();
    }
    //---------------------------------------------------
    //---------------------------------------------------
    const std::string& Device::GetUDID() const noexcept
    {
        return m_deviceInfo.GetUDID();
    }
    //---------------------------------------------------
    //---------------------------------------------------
    u32 Device::GetNumberOfCPUCores() const noexcept
    {
        return m_deviceInfo.GetNumCPUCores();
    }

}
