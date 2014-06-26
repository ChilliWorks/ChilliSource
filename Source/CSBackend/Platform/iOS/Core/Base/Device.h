//
//  Device.h
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

#ifdef CS_TARGETPLATFORM_IOS

#import <ChilliSource/ChilliSource.h>
#import <CSBackend/Platform/iOS/ForwardDeclarations.h>
#import <ChilliSource/Core/Base/Device.h>

namespace CSBackend
{
    namespace iOS
    {
        //---------------------------------------------------------
        /// The iOS backend for the device system. This provides
        /// information on the specific iOS device.
        ///
        /// @author Ian Copland
        //---------------------------------------------------------
        class Device final : public CSCore::Device
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
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The device model name.
            //---------------------------------------------------
            const std::string& GetModel() const override;
            //---------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the device model type.
            //---------------------------------------------------
            const std::string& GetModelType() const override;
            //---------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the device manufacturer.
            //---------------------------------------------------
            const std::string& GetManufacturer() const override;
            //---------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The locale code registered with the
            /// device.
            //---------------------------------------------------
            const std::string& GetLocale() const override;
            //---------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The language the device is set to.
            //---------------------------------------------------
            const std::string& GetLanguage() const override;
            //---------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The version of the operating system.
            //---------------------------------------------------
            const std::string& GetOSVersion() const override;
            //---------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An identifier that can be used to uniquely
            /// identify the device.
            //---------------------------------------------------
            const std::string& GetUDID() const override;
			//---------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The number of CPU cores available on the
            /// device.
			//--------------------------------------------------
			u32 GetNumberOfCPUCores() const override;
        private:
            friend CSCore::DeviceUPtr CSCore::Device::Create();
            //----------------------------------------------------
			/// Constructor. Declared private to force the use of
            /// the factory method.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			Device();
            
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
}

#endif