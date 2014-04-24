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

#include <ChilliSource/Backend/Platform/Android/Core/Base/Device.h>

#include <ChilliSource/Backend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace ChilliSource
{
    namespace Android
    {
		namespace
		{
			//----------------------------------------------------
			/// Parses the locale code string received from the
			/// Android API and converts it to a Locale.
			///
			/// @author I Copland
			///
			/// @param The locale string.
			///
			/// @return The locale.
			//----------------------------------------------------
			Core::Locale ParseLocaleString(const std::string& in_locale)
			{
				std::vector<std::string> strLocaleBrokenUp = ChilliSource::Core::StringUtils::Split(in_locale, "_", 0);

				if (strLocaleBrokenUp.size() > 1)
				{
					return Core::Locale(strLocaleBrokenUp[0],strLocaleBrokenUp[1]);
				}
				else if (strLocaleBrokenUp.size() == 1)
				{
					return Core::Locale(strLocaleBrokenUp[0]);
				}
				else
				{
					return Core::kUnknownLocale;
				}
			}
		}

        CS_DEFINE_NAMEDTYPE(Device);
        //----------------------------------------------------
        //----------------------------------------------------
        Device::Device()
            : m_locale(Core::kUnknownLocale), m_language(Core::kUnknownLocale)
        {
        	CoreJavaInterfaceSPtr javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();

            m_model = javaInterface->GetDeviceModel();
            m_modelType = javaInterface->GetDeviceModelType();
            m_manufacturer = javaInterface->GetDeviceManufacturer();
            m_osVersion = javaInterface->GetOSVersionCode();
            m_locale = ParseLocaleString(javaInterface->GetDefaultLocaleCode());
            m_language = m_locale;
            m_numCPUCores = javaInterface->GetNumberOfCores();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Device::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Core::Device::InterfaceID == in_interfaceId || Device::InterfaceID == in_interfaceId);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModel()
        {
            return m_model;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModelType()
        {
            return m_modelType;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetManufacturer()
        {
            return m_manufacturer;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        Core::Locale& Device::GetLocale()
        {
            return m_locale;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        Core::Locale& Device::GetLanguage()
        {
            return m_language;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetOSVersion()
        {
            return m_osVersion;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetUDID()
        {
            return m_udidManager.GetUDID();
        }
        //---------------------------------------------------
        //---------------------------------------------------
        u32 Device::GetNumberOfCPUCores()
        {
            return m_numCPUCores;
        }
    }
}


