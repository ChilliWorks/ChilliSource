//
//  DeviceButtonSystem.cpp
//  Chilli Source
//  Created by Ian Copland on 16/05/2014.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Input/DeviceButtons/DeviceButtonSystem.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Input/DeviceButtons/DeviceButtonJavaInterface.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			//----------------------------------------------------
			/// Converts from the integer received from java
			/// to the button enum.
			///
			/// @author Ian Copland
			///
			/// @param An integer representing the button.
			///
			/// @param The button.
			//----------------------------------------------------
			DeviceButtonSystem::DeviceButton IntegerToButton(s32 in_buttonInteger)
			{
				const s32 k_backButton = 0;

				switch (in_buttonInteger)
				{
				case k_backButton:
					return DeviceButtonSystem::DeviceButton::k_backButton;
				default:
					CS_LOG_ERROR("DeviceButtonSystem: Cannot convert from integer to button.");
					return DeviceButtonSystem::DeviceButton::k_backButton;
				}
			}
		}

        CS_DEFINE_NAMEDTYPE(DeviceButtonSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        bool DeviceButtonSystem::IsA(CSCore::InterfaceIDType in_interfaceID) const
        {
            return (in_interfaceID == CSInput::DeviceButtonSystem::InterfaceID || in_interfaceID == DeviceButtonSystem::InterfaceID);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        CSCore::IConnectableEvent<DeviceButtonSystem::TriggeredDelegate>& DeviceButtonSystem::GetTriggeredEvent()
        {
            return m_triggeredEvent;
        }
        //------------------------------------------------
        //------------------------------------------------
        void DeviceButtonSystem::OnInit()
        {
        	m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::DeviceButtonJavaInterface>();
        	if (m_javaInterface == nullptr)
        	{
        		m_javaInterface = DeviceButtonJavaInterfaceSPtr(new DeviceButtonJavaInterface());
        		JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
        	}

        	m_javaInterface->SetTriggeredDelegate(CSCore::MakeDelegate(this, &DeviceButtonSystem::OnTriggered));
        }
        //------------------------------------------------
        //------------------------------------------------
        void DeviceButtonSystem::OnTriggered(s32 in_button)
        {
        	m_triggeredEvent.NotifyConnections(IntegerToButton(in_button));
        }
        //------------------------------------------------
        //------------------------------------------------
        void DeviceButtonSystem::OnDestroy()
        {
        	m_javaInterface->SetTriggeredDelegate(nullptr);
        	m_javaInterface = nullptr;
        }
	}
}

#endif