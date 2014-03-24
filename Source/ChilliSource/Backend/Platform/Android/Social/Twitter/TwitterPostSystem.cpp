//
//  TwitterPostSystem.cpp
//  Chilli Source
//
//  Created by Robert Henning on 11/05/2012.
//  Copyright 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterPostSystem.h>

#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivity.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>

namespace ChilliSource
{
	namespace Android
	{
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		TwitterPostSystem::TwitterPostSystem(Networking::HttpConnectionSystem* in_httpConnectionSystem, Core::OAuthSystem* in_oauthSystem)
		: Social::TwitterPostSystem(in_httpConnectionSystem, in_oauthSystem)
		{
		}
		//------------------------------------------------------------------------
        //------------------------------------------------------------------------
        bool TwitterPostSystem::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return in_interfaceID == Social::TwitterPostSystem::InterfaceID;
        }
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
        void TwitterPostSystem::Authenticate(const std::string& in_key, const std::string& in_secret, const AuthenticationResultDelegate& in_delegate)
		{
            CS_ASSERT(m_authDelegate == nullptr, "Twitter can only handle one auth request at a time");
            CS_ASSERT(in_key.empty() == false && in_secret.empty() == false, "Twitter must have a key and secret provided by the Twitter application");

            m_authDelegate = in_delegate;

			// Try and load saved keys
			LoadAuthenticationKeys();

            if(m_savedOAuthTokenKey.empty() == false && m_savedOAuthTokenSecret.empty() == false)
            {
                //We have cached Auth tokens so don't need to prompt
                m_isAuthenticated = true;
                m_oauthSystem->SetOAuthTokenKey(m_savedOAuthTokenKey);
				m_oauthSystem->SetOAuthTokenSecret(m_savedOAuthTokenSecret);

                if(m_authDelegate)
                {
                    m_authDelegate(AuthenticationResult::k_success);
                    m_authDelegate = nullptr;
                }
            }
            else
            {
                //We need to prompt the user to login
                if(RequestOAuthToken())
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
                    CS_LOG_ERROR("TwitterPostSystem::Authenticate - Unable to get OAuth token!");
                }
            }
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		bool TwitterPostSystem::IsImagePostSupported() const
		{
			return false;
		}
        //------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::Post(const Social::TwitterPostSystem::PostDesc& in_desc, const Social::TwitterPostSystem::PostResultDelegate& in_delegate)
		{
			CS_ASSERT(in_desc.m_localImagePath.size() == 0, "The Android twitter post system does not support image post.");

			// Set our callback
			m_postDelegate = in_delegate;

			if(IsAuthenticated())
			{
				// Authentication has already occured and our token and secret keys are set
				// so we are go for Twitter posing action...
				Social::TwitterPostSystem::PostUsingChilliSource(in_desc);
			}
			else
			{
				if(m_postDelegate)
				{
					m_postDelegate(Social::TwitterPostSystem::PostResult::k_notAuthenticated);
                    m_postDelegate = nullptr;
				}
			}
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnPINComplete(const Social::TwitterAuthenticationActivity::AuthenticationPINResult& in_result)
		{
			if(in_result.strPIN.empty() == false)
			{
				m_oauthSystem->SetOAuthPin(in_result.strPIN);
				RequestOAuthAccessToken();
			}
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::OnAuthorisationDismissed(Core::Activity* in_activity)
		{
			// User has cancelled
			m_authorisationDismissedConnection = nullptr;
			m_authenticationView = nullptr;
		}
	}
}
