/*
 *  TwitterPostSystem.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 05/03/2012.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterPostSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/Base/PlatformSystem.h>
#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivity.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>

namespace ChilliSource
{
	namespace Android
	{
		//------------------------------------------------------------------------
		/// Constructor
		//------------------------------------------------------------------------
		TwitterPostSystem::TwitterPostSystem(Networking::HttpConnectionSystem* inpHttpConnectionSystem, Core::OAuthSystem* inpOAuthSystem)
		: Social::TwitterPostSystem(inpHttpConnectionSystem, inpOAuthSystem)
		{
		}
		//------------------------------------------------------------------------
		/// Destructor
		//------------------------------------------------------------------------
		TwitterPostSystem::~TwitterPostSystem()
		{
		}
		//------------------------------------------------------------------------
        /// Is A
        //------------------------------------------------------------------------
        bool TwitterPostSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == Social::TwitterPostSystem::InterfaceID;
        }
		//------------------------------------------------------------------------
		/// Authenticate
		//------------------------------------------------------------------------
		bool TwitterPostSystem::Authenticate()
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
					m_authenticationView = Social::TwitterAuthenticationActivity::Create();
					if(m_authenticationView != nullptr)
					{
						m_authenticationView->SetAuthenticationPINResultDelegate(Core::MakeDelegate(this, &TwitterPostSystem::OnPINComplete));
						m_authorisationDismissedConnection = m_authenticationView->GetDismissedEvent().OpenConnection(Core::MakeDelegate(this, &TwitterPostSystem::OnAuthorisationDismissed));
						m_authenticationView->Present();
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
		bool TwitterPostSystem::IsImagePostSupported() const
		{
			return false;
		}
        //------------------------------------------------------------------------
		/// Try Post
		//------------------------------------------------------------------------
		bool TwitterPostSystem::TryPost(const Social::TwitterPostDesc & insDesc, const Social::TwitterPostSystem::PostResultDelegate & inResultCallback)
		{
			bool bResult = false;

			CS_ASSERT(insDesc.strLocalImagePath.size() == 0, "The android twitter post system does not support image post.");

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
					inResultCallback(Social::TwitterPostSystem::PostResult::k_notAuthenticated);
				}
				bResult = false;
			}

			return bResult;
		}
		//------------------------------------------------------------------------
		/// Delegate called when the user confirms entry of the PIN
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnPINComplete(const Social::TwitterAuthenticationActivity::AuthenticationPINResult &insResult)
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
		void TwitterPostSystem::OnAuthorisationDismissed(Core::Activity* inpActivity)
		{
			// User has cancelled
			m_authorisationDismissedConnection = nullptr;
			m_authenticationView = nullptr;
		}
	}
}
