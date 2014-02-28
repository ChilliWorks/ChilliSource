//
//  TwitterPostSystem.cpp
//  MoFlow
//
//  Created by Scott Downie on 12/09/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Backend/Platform/iOS/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/PlatformSystem.h>
#include <ChilliSource/Backend/Platform/iOS/Social/Twitter/TwitterPostSystem.h>
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
		/// Constructor
		//------------------------------------------------------------------------
		CTwitterPostSystem::CTwitterPostSystem(iOS::CHttpConnectionSystem* inpHttpConnectionSystem,
											   Core::OAuthSystem* inpOAuthSystem) : Social::TwitterPostSystem(inpHttpConnectionSystem, inpOAuthSystem)
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
        ///
        /// @param Interface ID
        /// @return Is of interface ID type
        //------------------------------------------------------------------------
        bool CTwitterPostSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == Social::TwitterPostSystem::InterfaceID;
        }
		//------------------------------------------------------------------------
		/// Run the OAuth process and, if successful, leave the system in state
		/// ready to communicate with Twitter
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
					if(Core::Application::GetPlatformSystemPtr()->CanCreateActivityWithInterface<ChilliSource::Social::TwitterAuthenticationActivity>())
					{
						mpAuthenticationView = Core::Application::GetPlatformSystemPtr()->CreateActivityWithInterface<ChilliSource::Social::TwitterAuthenticationActivity>();
						mpAuthenticationView->SetAuthenticationPINResultDelegate(Core::MakeDelegate(this, &TwitterPostSystem::OnPINComplete));
						m_authorisationDismissedConnection = mpAuthenticationView->GetDismissedEvent().OpenConnection(Core::MakeDelegate(this, &TwitterPostSystem::OnAuthorisationDismissed));
						mpAuthenticationView->Present();
					}
				}
				else
				{
					CS_LOG_DEBUG("CTwitterPostSystem::Authenticate() - Unable to get OAuth token!");
					return false;
				}
				CS_LOG_DEBUG("CTwitterPostSystem::Authenticate() - Got request token URL of \""+strAuthoiseURL+"\"");
				bResult = false;
			}
			else
			{
				// We have keys, so set them and rock on!
				mpOAuthSystem->SetOAuthTokenKey(mstrSavedOAuthTokenKey);
				mpOAuthSystem->SetOAuthTokenSecret(mstrSavedOAuthTokenSecret);
				CS_LOG_DEBUG("CTwitterPostSystem::Authenticate() - Set OAuth key token \""+mstrSavedOAuthTokenKey+"\" and OAuth secret token \""+mstrSavedOAuthTokenSecret+"\"");
				bResult = true;
			}
			
			return bResult;
		}
        //------------------------------------------------------------------------
        /// Supported By Device
        //------------------------------------------------------------------------
        bool CTwitterPostSystem::SupportedByDevice()
        {
			NSString *reqSysVer = @"5.0";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			return ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
		}
		//------------------------------------------------------------------------
		/// Is Image Post Supported
		//------------------------------------------------------------------------
		bool CTwitterPostSystem::IsImagePostSupported() const
		{
			NSString *reqSysVer = @"5.0";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			return ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
		}
        //------------------------------------------------------------------------
        /// Try Post
        //------------------------------------------------------------------------
        bool CTwitterPostSystem::TryPost(const Social::TwitterPostDesc & insDesc, const Social::TwitterPostSystem::PostResultDelegate & inResultCallback)
        {
			if(SupportedByDevice())
				return TryPostUsingiOS(insDesc, inResultCallback);
			else
				return TryPostUsingMoFlow(insDesc, inResultCallback);
        }
		
		//--- Private
		
		//------------------------------------------------------------------------
		/// Try Post Using iOS
		//------------------------------------------------------------------------
		bool CTwitterPostSystem::TryPostUsingiOS(const Social::TwitterPostDesc & insDesc, const Social::TwitterPostSystem::PostResultDelegate & inResultCallback)
		{
			if([TWTweetComposeViewController canSendTweet])
            {
                TWTweetComposeViewController* pComposeViewController = [[TWTweetComposeViewController alloc] init];
                
                mCompletionDelegate = inResultCallback;
                
                //Set the text
                bool bTextResult = true;
                if(insDesc.strText.length() > 0)
                {
                    bTextResult = [pComposeViewController setInitialText:Core::StringUtils::UTF8StringToNSString(insDesc.strText)];
                }
                
                bool bImageAttached = true;
                if(insDesc.strLocalImagePath.length() > 0)
                {
                    std::string strPath = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(insDesc.eLocalImageStorageLocation) + insDesc.strLocalImagePath;
                    
                    NSString* pImagePath = Core::StringUtils::StringToNSString(strPath);
                    UIImage* pImage = [UIImage imageWithContentsOfFile:pImagePath];
                    
                    if(pImage != nil)
                    {
                        bImageAttached = [pComposeViewController addImage:pImage];
                        if(!bImageAttached)
                        {
                            CS_LOG_ERROR("CTwitterPostSystem::TryPostUsingiOS - Failed to attach image to tweet, most likely because limit has been reached");
                        }
                    }
                }
                
                //Add a url if available
                bool bUrlResult = true;
                if(insDesc.strUrl.length() > 0)
                {
                    bUrlResult = [pComposeViewController addURL:[NSURL URLWithString:Core::StringUtils::UTF8StringToNSString(insDesc.strUrl)]];
                }
                
                
                //Set the completion handler to call our completion delegate
                pComposeViewController.completionHandler = ^(TWTweetComposeViewControllerResult inResult)
                {
					if(mCompletionDelegate)
					{
						switch(inResult)
						{
							case TWTweetComposeViewControllerResultDone:
								mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_success);
								break;
							case TWTweetComposeViewControllerResultCancelled:
								mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_cancelled);
								break;
							default:
								mCompletionDelegate(Social::TwitterPostSystem::PostResult::k_failed);
								break;
						};
					}
                    
                    [[EAGLView sharedInstance].viewController dismissModalViewControllerAnimated:YES];
                };
                
                [[EAGLView sharedInstance].viewController presentModalViewController:pComposeViewController animated:YES];
                [pComposeViewController release];
                
                return (bTextResult && bUrlResult);
            }
            else
            {
                UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"No Twitter account" message:@"Please sign in to your Twitter account from the device settings" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                [alertView show];
                [alertView release];
                return false;
            }
		}
		//------------------------------------------------------------------------
		/// Try Post Using moFlow
		//------------------------------------------------------------------------
		bool CTwitterPostSystem::TryPostUsingMoFlow(const Social::TwitterPostDesc & insDesc, const Social::TwitterPostSystem::PostResultDelegate & inResultCallback)
		{
			bool bResult = false;
			
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
		///
		/// @param PIN entered by user
		//------------------------------------------------------------------------
		void CTwitterPostSystem::OnPINComplete(const ChilliSource::Social::TwitterAuthenticationActivity::AuthenticationPINResult &insResult)
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
		void CTwitterPostSystem::OnAuthorisationDismissed(Core::Activity* inpActivity)
		{
			// User has cancelled
			if(mpAuthenticationView)
			{
				CS_SAFEDELETE(mpAuthenticationView);
			}
		}
    }
}
