//
//  GooglePlayRemoteNotificationSystem.cpp
//  MoFlow
//
//  Created by Robert Henning on 23/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Platform/Android/GooglePlay/GooglePlayRemoteNotificationSystem.h>

#include <ChilliSource/Core/Main/Application.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>

#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Platform/Android/GooglePlay/GooglePlayRemoteNotificationJavaInterface.h>

namespace moFlo
{
	namespace AndroidPlatform
    {
		namespace
		{
			const std::string kstrProviderID = "Google";
		}
		//---------------------------------------------------------------
		/// Constructor
		//---------------------------------------------------------------
    	CGooglePlayRemoteNotificationSystem::CGooglePlayRemoteNotificationSystem()
    	{
    		if(NULL == (mpJavaInterface = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CGooglePlayRemoteNotificationJavaInterface>()))
    		{
    			mpJavaInterface = GooglePlayRemoteNotificationJavaInterfacePtr(new CGooglePlayRemoteNotificationJavaInterface());
				moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
    		}
    	}
		//-------------------------------------------------------------------------
		/// Request Remote Token
		//-------------------------------------------------------------------------
		void CGooglePlayRemoteNotificationSystem::RequestRemoteToken(const Core::RemoteTokenReceivedDelegate& inDelegate)
		{
			mDelegate = inDelegate;
			mpJavaInterface->RequestRemoteToken();
		}
		//-------------------------------------------------------------------------
		/// Get Remote Token
		//-------------------------------------------------------------------------
		const std::string& CGooglePlayRemoteNotificationSystem::GetRemoteToken() const
		{
			return mstrToken;
		}
		//-------------------------------------------------------------------------
		/// Get Provider ID
		//-------------------------------------------------------------------------
		const std::string& CGooglePlayRemoteNotificationSystem::GetProviderID() const
		{
			return kstrProviderID;
		}
		//-------------------------------------------------------------------------
		/// On Remote Token Received
		//-------------------------------------------------------------------------
		void CGooglePlayRemoteNotificationSystem::OnRemoteTokenReceived(const std::string& instrToken)
		{
			mstrToken = moFlo::CBaseEncoding::Base64Encode(instrToken);
			if(mDelegate != NULL)
			{
				mDelegate(mstrToken);
				mDelegate = NULL;
			}
		}
		//-------------------------------------------------------------------------
		/// On Remote Notification Received
		//-------------------------------------------------------------------------
		void CGooglePlayRemoteNotificationSystem::OnRemoteNotificationReceived(const Core::ParamDictionary& insParams)
		{
			moFlo::Notification sNotification;

			sNotification.bDismissed = false;
			sNotification.eType = NOTICE_PUSH;
			sNotification.TriggerTime = Core::CApplication::GetSystemTime();
			sNotification.ID = 0;
			sNotification.ePriority = NOTICE_STANDARD;
			sNotification.sParams = insParams;

			WARNING_LOG("Remote notification received: " + sNotification.sParams.ToString());
			CNotificationScheduler::OnNotificationReceived(sNotification);
		}
    }
}
