//
//  TwitterPostSystem.cpp
//  Chilli Source
//
//  Created by Robert Henning on 08/05/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Social/Twitter/TwitterPostSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Backend/Platform/iOS/Networking/Http/HttpRequestSystem.h>
#include <ChilliSource/Backend/Platform/iOS/Social/Twitter/TwitterAuthenticationActivity.h>

#include <UIKit/UIKit.h>

#ifdef __IPHONE_5_0
    #include <Twitter/Twitter.h>
#endif

namespace ChilliSource
{
    namespace iOS
    {
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		TwitterPostSystem::TwitterPostSystem(Networking::HttpRequestSystem* in_HttpRequestSystem, Core::OAuthSystem* in_oauthSystem)
        : Social::TwitterPostSystem(in_HttpRequestSystem, in_oauthSystem)
		{
			
		}
		//------------------------------------------------------------------------
        //------------------------------------------------------------------------
        bool TwitterPostSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == Social::TwitterPostSystem::InterfaceID || in_interfaceId == TwitterPostSystem::InterfaceID;
        }
        //------------------------------------------------------------------------
        //------------------------------------------------------------------------
        void TwitterPostSystem::Authenticate(const std::string& in_key, const std::string& in_secret, const AuthenticationResultDelegate& in_delegate)
        {
            CS_ASSERT(m_authDelegate == nullptr, "Twitter can only handle one auth request at a time");
            CS_ASSERT(in_key.empty() == false && in_secret.empty() == false, "Twitter must have a key and secret provided by the Twitter application");

            m_authDelegate = in_delegate;
            
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
        bool TwitterPostSystem::SupportedByDevice()
        {
			NSString *reqSysVer = @"5.0";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			return ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		bool TwitterPostSystem::IsImagePostSupported() const
		{
			NSString *reqSysVer = @"5.0";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			return ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
		}
        //------------------------------------------------------------------------
        //------------------------------------------------------------------------
        void TwitterPostSystem::Post(const Social::TwitterPostSystem::PostDesc& in_desc, const Social::TwitterPostSystem::PostResultDelegate& in_delegate)
        {
            CS_ASSERT(m_postDelegate == nullptr, "Cannot send tweet when one is already in progress");
            
			if(SupportedByDevice())
            {
                PostUsingiOS(in_desc, in_delegate);
            }
			else
            {
                PostUsingChilliSource(in_desc, in_delegate);
            }
        }
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::PostUsingiOS(const Social::TwitterPostSystem::PostDesc& in_desc, const Social::TwitterPostSystem::PostResultDelegate& in_delegate)
		{
			if([TWTweetComposeViewController canSendTweet])
            {
                TWTweetComposeViewController* pComposeViewController = [[TWTweetComposeViewController alloc] init];
                
                m_postDelegate = in_delegate;
                
                //Set the text
                if(in_desc.m_text.length() > 0)
                {
                    [pComposeViewController setInitialText:Core::StringUtils::UTF8StringToNSString(in_desc.m_text)];
                }
                
                bool bImageAttached = true;
                if(in_desc.m_localImagePath.length() > 0)
                {
                    std::string strPath = Core::Application::Get()->GetFileSystem()->GetStorageLocationDirectory(in_desc.m_localImageStorageLocation) + in_desc.m_localImagePath;
                    
                    NSString* pImagePath = Core::StringUtils::StringToNSString(strPath);
                    UIImage* pImage = [UIImage imageWithContentsOfFile:pImagePath];
                    
                    if(pImage != nil)
                    {
                        bImageAttached = [pComposeViewController addImage:pImage];
                        if(!bImageAttached)
                        {
                            CS_LOG_ERROR("TwitterPostSystem::TryPostUsingiOS - Failed to attach image to tweet, most likely because limit has been reached");
                        }
                    }
                }
                
                //Add a url if available
                if(in_desc.m_url.length() > 0)
                {
                    [pComposeViewController addURL:[NSURL URLWithString:Core::StringUtils::UTF8StringToNSString(in_desc.m_url)]];
                }
                
                
                //Set the completion handler to call our completion delegate
                pComposeViewController.completionHandler = ^(TWTweetComposeViewControllerResult inResult)
                {
					if(m_postDelegate)
					{
						switch(inResult)
						{
							case TWTweetComposeViewControllerResultDone:
								m_postDelegate(Social::TwitterPostSystem::PostResult::k_success);
								break;
							case TWTweetComposeViewControllerResultCancelled:
								m_postDelegate(Social::TwitterPostSystem::PostResult::k_cancelled);
								break;
							default:
								m_postDelegate(Social::TwitterPostSystem::PostResult::k_failed);
								break;
						};
                        
                        m_postDelegate = nullptr;
					}
                    
                    [[EAGLView sharedInstance].viewController dismissModalViewControllerAnimated:YES];
                };
                
                [[EAGLView sharedInstance].viewController presentModalViewController:pComposeViewController animated:YES];
                [pComposeViewController release];
            }
            else
            {
                UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"No Twitter account" message:@"Please sign in to your Twitter account from the device settings" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                [alertView show];
                [alertView release];
            }
		}
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
		void TwitterPostSystem::PostUsingChilliSource(const Social::TwitterPostSystem::PostDesc& in_desc, const Social::TwitterPostSystem::PostResultDelegate& in_delegate)
		{
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
		void TwitterPostSystem::OnPINComplete(const ChilliSource::Social::TwitterAuthenticationActivity::AuthenticationPINResult &in_result)
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
