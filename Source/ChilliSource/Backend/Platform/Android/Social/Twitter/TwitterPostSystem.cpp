/*
 *  TwitterPostSystem.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 05/03/2012.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Application.h>
#include <ChilliSource/Platform/Android/HttpConnectionSystem.h>
#include <ChilliSource/Platform/Android/PlatformSystem.h>
#include <ChilliSource/Platform/Android/Social/Twitter/TwitterPostSystem.h>
#include <ChilliSource/Platform/Android/Social/Twitter/TwitterAuthenticationActivity.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		//------------------------------------------------------------------------
		/// Constructor
		//------------------------------------------------------------------------
		CTwitterPostSystem::CTwitterPostSystem(AndroidPlatform::CHttpConnectionSystem* inpHttpConnectionSystem,
											   Networking::COAuthSystem* inpOAuthSystem) : Social::ITwitterPostSystem(inpHttpConnectionSystem, inpOAuthSystem)
		{
		}
		//------------------------------------------------------------------------
		/// Destructor
		//------------------------------------------------------------------------
		CTwitterPostSystem::~CTwitterPostSystem()
		{
		}
		//------------------------------------------------------------------------
        /// Is A
        //------------------------------------------------------------------------
        bool CTwitterPostSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == Social::ITwitterPostSystem::InterfaceID;
        }
		//------------------------------------------------------------------------
		/// Authenticate
		//------------------------------------------------------------------------
		bool CTwitterPostSystem::Authenticate()
		{
			bool bResult = false;

			// Try and load saved keys
			TryLoadAuthenticationKeys();

			if(mstrSavedOAuthTokenKey.empty() || mstrSavedOAuthTokenSecret.empty())
			{
				// We don't have any keys to go though the authentication process
				std::string strAuthoiseURL;
				if(RequestOAuthToken(strAuthoiseURL))
				{
					// Show authentication view
					if(Core::CApplication::GetPlatformSystemPtr()->CanCreateActivityWithInterface<moFlo::Social::ITwitterAuthenticationActivity>())
					{
						mpAuthenticationView = Core::CApplication::GetPlatformSystemPtr()->CreateActivityWithInterface<moFlo::Social::ITwitterAuthenticationActivity>();
						mpAuthenticationView->SetAuthenticationPINResultDelegate(moFlo::Social::ITwitterAuthenticationActivity::AuthenticationPINResultDelegate(this, &CTwitterPostSystem::OnPINComplete));
						mpAuthenticationView->GetDismissedEvent() += moFlo::ActivityDismissedEvent(this, &CTwitterPostSystem::OnAuthorisationDismissed);
						mpAuthenticationView->Present();
					}
				}
				else
				{
					return false;
				}
				bResult = false;
			}
			else
			{
				// We have keys, so set them and rock on!
				mpOAuthSystem->SetOAuthTokenKey(mstrSavedOAuthTokenKey);
				mpOAuthSystem->SetOAuthTokenSecret(mstrSavedOAuthTokenSecret);
				bResult = true;
			}

			return bResult;
		}
		//------------------------------------------------------------------------
		/// Is Image Post Supported
		//------------------------------------------------------------------------
		bool CTwitterPostSystem::IsImagePostSupported() const
		{
			return false;
		}
        //------------------------------------------------------------------------
		/// Try Post
		//------------------------------------------------------------------------
		bool CTwitterPostSystem::TryPost(const Social::TwitterPostDesc & insDesc, const Social::ITwitterPostSystem::PostResultDelegate & inResultCallback)
		{
			bool bResult = false;

			MOFLOW_ASSERT(insDesc.strLocalImagePath.size() == 0, "The android twitter post system does not support image post.");

			// Set our callback
			mCompletionDelegate = inResultCallback;

			if(Authenticate())
			{
				// Authentication has already occured and our token and secret keys are set
				// so we are go for Twitter posing action...
				PostUsingMoFlow(insDesc);
			}
			else
			{
				// Remember our post description
				msDesc.strUrl = insDesc.strUrl;
				msDesc.strText = insDesc.strText;
				if(mCompletionDelegate)
				{
					inResultCallback(Social::ITwitterPostSystem::PR_NOT_AUTHENTICATED);
				}
				bResult = false;
			}

			return bResult;
		}
		//------------------------------------------------------------------------
		/// Delegate called when the user confirms entry of the PIN
		//------------------------------------------------------------------------
		void CTwitterPostSystem::OnPINComplete(const moFlo::Social::ITwitterAuthenticationActivity::AuthenticationPINResult &insResult)
		{
			if(Social::TwitterPIN::kudwTwitterPINLength == insResult.strPIN.size())
			{
				mpOAuthSystem->SetOAuthPin(insResult.strPIN);
				RequestOAuthAccessToken();
			}
		}
		//------------------------------------------------------------------------
		/// Delegate called with the authorisation view is dismissed.
		//------------------------------------------------------------------------
		void CTwitterPostSystem::OnAuthorisationDismissed(moFlo::IActivity* inpActivity)
		{
			// User has cancelled
			if(mpAuthenticationView)
			{
				mpAuthenticationView->GetDismissedEvent() -= moFlo::ActivityDismissedEvent(this, &CTwitterPostSystem::OnAuthorisationDismissed);
				SAFE_DELETE(mpAuthenticationView);
			}
		}
	}
}
