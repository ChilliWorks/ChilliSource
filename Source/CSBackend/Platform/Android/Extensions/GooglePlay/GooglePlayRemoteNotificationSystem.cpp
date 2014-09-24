//
//  GooglePlayRemoteNotificationSystem.cpp
//  Chilli Source
//  Created by Robert Henning on 23/01/2014.
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

#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY

#include <CSBackend/Platform/Android/Extensions/GooglePlay/GooglePlayRemoteNotificationSystem.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Extensions/GooglePlay/GooglePlayRemoteNotificationJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Notification/Notification.h>

namespace CSBackend
{
	namespace Android
    {
		namespace
		{
			const std::string k_providerID = "Google";
		}

		CS_DEFINE_NAMEDTYPE(GooglePlayRemoteNotificationSystem);
        //--------------------------------------------------
        //--------------------------------------------------
    	GooglePlayRemoteNotificationSystem::GooglePlayRemoteNotificationSystem()
    	{
    		m_javaInterface = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<GooglePlayRemoteNotificationJavaInterface>();
    		if(nullptr == m_javaInterface)
    		{
    			m_javaInterface = GooglePlayRemoteNotificationJavaInterfaceSPtr(new GooglePlayRemoteNotificationJavaInterface());
				CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
    		}
    	}
        //--------------------------------------------------
        //--------------------------------------------------
        bool GooglePlayRemoteNotificationSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (GooglePlayRemoteNotificationSystem::InterfaceID == in_interfaceId || CSCore::RemoteNotificationSystem::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void GooglePlayRemoteNotificationSystem::SetEnabled(bool in_enabled)
        {
            m_enabled = in_enabled;
        }
        //--------------------------------------------------
        //--------------------------------------------------
		void GooglePlayRemoteNotificationSystem::RequestRemoteToken(const TokenReceivedDelegate& in_delegate)
		{
			m_delegate = in_delegate;
			m_javaInterface->RequestRemoteToken();
		}
        //--------------------------------------------------
        //--------------------------------------------------
		const std::string& GooglePlayRemoteNotificationSystem::GetRemoteToken() const
		{
			return m_token;
		}
        //--------------------------------------------------
        //--------------------------------------------------
		const std::string& GooglePlayRemoteNotificationSystem::GetProviderID() const
		{
			return k_providerID;
		}
        //--------------------------------------------------
        //---------------------------------------------------
        CSCore::IConnectableEvent<CSCore::RemoteNotificationSystem::NotificationReceivedDelegate>& GooglePlayRemoteNotificationSystem::GetReceivedEvent()
		{
        	return m_receivedEvent;
		}
        //--------------------------------------------------
        //--------------------------------------------------
		void GooglePlayRemoteNotificationSystem::OnRemoteTokenReceived(const std::string& in_token)
		{
			m_token = CSCore::BaseEncoding::Base64Encode(in_token);
			if(m_delegate != nullptr)
			{
				m_delegate(m_token);
				m_delegate = nullptr;
			}
		}
        //--------------------------------------------------
        //--------------------------------------------------
		void GooglePlayRemoteNotificationSystem::OnRemoteNotificationReceived(const CSCore::ParamDictionary& in_params)
		{
			if (m_enabled == true)
			{
				CSCore::NotificationSPtr notification(std::make_shared<CSCore::Notification>());
				notification->m_id = 0;
				notification->m_params = in_params;
				notification->m_priority = CSCore::Notification::Priority::k_standard;
				m_receivedEvent.NotifyConnections(notification);
			}
		}
    }
}

#endif

#endif
