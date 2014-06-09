//
//  GooglePlayRemoteNotificationSystem.cpp
//  MoFlow
//
//  Created by Robert Henning on 23/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_ANDROID

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
        CSCore::IConnectableEvent<CSCore::RemoteNotificationSystem::NotificationReceivedDelegate>& GooglePlayRemoteNotificationSystem::GetRecievedEvent()
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