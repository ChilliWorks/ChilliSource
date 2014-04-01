//
//  GooglePlayRemoteNotificationSystem.cpp
//  MoFlow
//
//  Created by Robert Henning on 23/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayRemoteNotificationSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayRemoteNotificationJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Notification/Notification.h>

namespace ChilliSource
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
    		m_javaInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<GooglePlayRemoteNotificationJavaInterface>();
    		if(nullptr == m_javaInterface)
    		{
    			m_javaInterface = GooglePlayRemoteNotificationJavaInterfaceSPtr(new GooglePlayRemoteNotificationJavaInterface());
				ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
    		}
    	}
        //--------------------------------------------------
        //--------------------------------------------------
        bool GooglePlayRemoteNotificationSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (GooglePlayRemoteNotificationSystem::InterfaceID == in_interfaceId || Core::RemoteNotificationSystem::InterfaceID == in_interfaceId);
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
        Core::IConnectableEvent<Core::RemoteNotificationSystem::NotificationReceivedDelegate>& GooglePlayRemoteNotificationSystem::GetRecievedEvent()
		{
        	return m_receivedEvent;
		}
        //--------------------------------------------------
        //--------------------------------------------------
		void GooglePlayRemoteNotificationSystem::OnRemoteTokenReceived(const std::string& in_token)
		{
			m_token = Core::BaseEncoding::Base64Encode(in_token);
			if(m_delegate != nullptr)
			{
				m_delegate(m_token);
				m_delegate = nullptr;
			}
		}
        //--------------------------------------------------
        //--------------------------------------------------
		void GooglePlayRemoteNotificationSystem::OnRemoteNotificationReceived(const Core::ParamDictionary& in_params)
		{
			if (m_enabled == true)
			{
				Core::NotificationSPtr notification(std::make_shared<CSCore::Notification>());
				notification->m_id = 0;
				notification->m_params = in_params;
				notification->m_priority = Core::Notification::Priority::k_standard;
				m_receivedEvent.NotifyConnections(notification);
			}
		}
    }
}
