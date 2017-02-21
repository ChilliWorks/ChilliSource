//
//  Device.h
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

#ifndef _CHILLISOURCE_CORE_BASE_DEVICE_H_
#define _CHILLISOURCE_CORE_BASE_DEVICE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/DeviceInfo.h>
#include <ChilliSource/Core/System/AppSystem.h>


namespace ChilliSource
{
    //---------------------------------------------------------
    /// An application system for providing cross platform
    /// access to information about the current device. This
    /// includes information like the current model name and
    /// the number of CPU cores. It is safe to use this during
    /// the OnInit() and OnDestroy() lifecycle events.
    ///
    /// @author S Downie
    //---------------------------------------------------------
    class Device final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(Device);
        //-------------------------------------------------------
        /// Queries whether or not this system implements the
        /// interface with the given Id.
        ///
        /// @author Ian Copland
        ///
        /// @param The interface Id.
        /// @param Whether system is of given type.
        //-------------------------------------------------------
        bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const noexcept override;
        //---------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The device model name.
        //---------------------------------------------------
        const std::string& GetModel() const noexcept;
        //---------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The name of the device model type.
        //---------------------------------------------------
        const std::string& GetModelType() const noexcept;
        //---------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The name of the device manufacturer.
        //---------------------------------------------------
        const std::string& GetManufacturer() const noexcept;
        //---------------------------------------------------
        /// @author S Downie
        ///
        /// @return The locale code registered with the
        /// device.
        //---------------------------------------------------
        const std::string& GetLocale() const noexcept;
        //---------------------------------------------------
        /// @author S Downie
        ///
        /// @return The language the device is set to.
        //---------------------------------------------------
        const std::string& GetLanguage() const noexcept;
        //---------------------------------------------------
        /// @author S Downie
        ///
        /// @return The version of the operating system.
        //---------------------------------------------------
        const std::string& GetOSVersion() const noexcept;
        //---------------------------------------------------
        /// @author S Downie
        ///
        /// @return An identifier that can be used to uniquely
        /// identify the device.
        //---------------------------------------------------
        const std::string& GetUDID() const noexcept;
        //---------------------------------------------------
        /// @author S Downie
        ///
        /// @return The number of CPU cores available on the
        /// device.
        //---------------------------------------------------
        u32 GetNumberOfCPUCores() const noexcept;

    protected:
        friend class Application;
        //---------------------------------------------------
        /// Creates the system.
        ///
        /// @author Ian Copland
        ///
        /// @return The new instance.
        //---------------------------------------------------
        static DeviceUPtr Create(const ChilliSource::DeviceInfo& deviceInfo);
        //----------------------------------------------------
        /// Constructor. Declared private to force the use of
        /// the factory method.
        ///
        /// @author Ian Copland
        //----------------------------------------------------
        Device(const ChilliSource::DeviceInfo& deviceInfo) noexcept;

        DeviceInfo m_deviceInfo;
    };
}

#endif
