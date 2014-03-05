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
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>

namespace ChilliSource
{
	namespace Android
    {
		namespace
		{
			const std::string kstrProviderID = "Google";
		}
		//---------------------------------------------------------------
		/// Constructor
		//---------------------------------------------------------------
    	GooglePlayRemoteNotificationSystem::GooglePlayRemoteNotificationSystem()
    	{
    		if(NULL == (mpJavaInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<GooglePlayRemoteNotificationJavaInterface>()))
    		{
    			mpJavaInterface = GooglePlayRemoteNotificationJavaInterfacePtr(new GooglePlayRemoteNotificationJavaInterface());
				ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
    		}
    	}
		//-------------------------------------------------------------------------
		/// Request Remote Token
		//-------------------------------------------------------------------------
		void GooglePlayRemoteNotificationSystem::RequestRemoteToken(const Core::RemoteTokenReceivedDelegate& inDelegate)
		{
			mDelegate = inDelegate;
			mpJavaInterface->RequestRemoteToken();
		}
		//-------------------------------------------------------------------------
		/// Get Remote Token
		//-------------------------------------------------------------------------
		const std::string& GooglePlayRemoteNotificationSystem::GetRemoteToken() const
		{
			return mstrToken;
		}
		//-------------------------------------------------------------------------
		/// Get Provider ID
		//-------------------------------------------------------------------------
		const std::string& GooglePlayRemoteNotificationSystem::GetProviderID() const
		{
			return kstrProviderID;
		}
		//-------------------------------------------------------------------------
		/// On Remote Token Received
		//-------------------------------------------------------------------------
		void GooglePlayRemoteNotificationSystem::OnRemoteTokenReceived(const std::string& instrToken)
		{
			mstrToken = Core::BaseEncoding::Base64Encode(instrToken);
			if(mDelegate != NULL)
			{
				mDelegate(mstrToken);
				mDelegate = NULL;
			}
		}
		//-------------------------------------------------------------------------
		/// On Remote Notification Received
		//-------------------------------------------------------------------------
		void GooglePlayRemoteNotificationSystem::OnRemoteNotificationReceived(const Core::ParamDictionary& insParams)
		{
			Core::NotificationSPtr notification(std::make_shared<CSCore::Notification>());

			notification->bDismissed = false;
			notification->eType = Core::NotificationType::k_push;
			notification->TriggerTime = Core::Application::Get()->GetSystemTime();
			notification->ID = 0;
			notification->ePriority = Core::NotificationPriority::k_standard;
			notification->sParams = insParams;

			Core::NotificationScheduler::OnNotificationReceived(notification);
		}
    }
}
