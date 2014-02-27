/*
 *  FacebookPostSystem.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 06/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookPostSystem.h>

#include <FacebookSDK/FacebookSDK.h>

#ifdef __IPHONE_6_0
#include <UIKit/UIKit.h>
#include <Social/Social.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookUtils.h>
    #include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookUtils.h>
    #include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookUtils.h>
#endif

namespace ChilliSource
{
	using namespace Social;
	
	namespace iOS
	{
		CFacebookPostSystem::CFacebookPostSystem(Social::FacebookAuthenticationSystem* inpAuthSystem)
        : mpAuthSystem(inpAuthSystem)
		{
			mCompletionDelegate = nullptr;
#if DEBUG
            [FBSettings enableBetaFeature:FBBetaFeaturesShareDialog];
#endif
		}
		
		bool CFacebookPostSystem::IsA(Core::InterfaceIDType inID) const
		{
			return IFacebookPostSystem::InterfaceID == inID;
		}
        
        NSDictionary* CreateNSDisctionaryFromPostDesc(const FacebookPostDesc& insDesc)
        {
            NSDictionary* pPostParams = [[NSMutableDictionary alloc] initWithObjectsAndKeys:
                                                                                       Core::StringUtils::StringToNSString(insDesc.strURL), @"link",
                                                                                       Core::StringUtils::StringToNSString(insDesc.strPictureURL), @"picture",
                                                                                       Core::StringUtils::StringToNSString(insDesc.strName), @"name",
                                                                                       Core::StringUtils::StringToNSString(insDesc.strCaption), @"caption",
                                                                                       Core::StringUtils::StringToNSString(insDesc.strDescription), @"description",
                                                                                       Core::StringUtils::StringToNSString(insDesc.strTo), @"to",
                                         nil];
            
            return pPostParams;
        }
        
		void CFacebookPostSystem::TryPost(const FacebookPostDesc& insDesc, const PostResultDelegate& insResultCallback)
		{
			mCompletionDelegate = insResultCallback;
			
            if(![FBSession.activeSession isOpen])
                mpAuthSystem->Authenticate();
            
            if(mpAuthSystem->IsSignedIn())
            {
                if(mpAuthSystem->HasPermission("publish_actions") && (insDesc.strTo != "" || mpAuthSystem->HasPermission("publish_stream")))
                {
                    Post(insDesc);
                }
                else
                {
                    msPostDesc = insDesc;
                    std::vector<std::string> aWritePerms;
                    aWritePerms.push_back("publish_actions");
                    if(insDesc.strTo != "")
                        aWritePerms.push_back("publish_stream");
                    mpAuthSystem->AuthoriseWritePermissions(aWritePerms, fastdelegate::MakeDelegate(this, &CFacebookPostSystem::OnPublishPermissionAuthorised));
                }
            }
            else
            {
                CS_ERROR_LOG("Facebook Post: User must be authenticated");
            }
        }
        
        void CFacebookPostSystem::TrySendRequest(const Social::FacebookPostDesc& insDesc, const PostResultDelegate& insResultCallback, std::vector<std::string>& inastrRecommendedFriends)
        {
            std::string strFriends;
            
            for(u32 i = 0; i < inastrRecommendedFriends.size(); ++i)
            {
                strFriends += inastrRecommendedFriends[i];
                if(i != inastrRecommendedFriends.size() - 1)
                {
                    strFriends += ",";
                }
            }
            
            mRequestCompleteDelegate = insResultCallback;
            
            NSMutableDictionary* params;
            
            if(!insDesc.strTo.empty())
            {
                params = [NSMutableDictionary dictionaryWithObjectsAndKeys:Core::StringUtils::StringToNSString(insDesc.strTo), @"to", nil];
            }
            else
            {
                params = [NSMutableDictionary dictionaryWithObjectsAndKeys:Core::StringUtils::StringToNSString(strFriends), @"suggestions", nil];
            }
            
            [FBWebDialogs presentRequestsDialogModallyWithSession:nil
                                                          message:Core::StringUtils::StringToNSString(insDesc.strDescription)
                                                            title:Core::StringUtils::StringToNSString(insDesc.strCaption)
                                                       parameters:params
                                                          handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error)
             {
                 if(!error)
                 {
                     if(mRequestCompleteDelegate)
                     {
                         if(result == FBWebDialogResultDialogCompleted)
                         {
                             NSDictionary *urlParams = [URLParser parseURLParams:[resultURL query]];
                             if (![urlParams valueForKey:@"request"])
                             {
                                 // User clicked the Cancel button
                                 mRequestCompleteDelegate(Social::IFacebookPostSystem::PostResult::k_cancelled);
                             }
                             else
                             {
                                 // User clicked the Share button
                                 mRequestCompleteDelegate(Social::IFacebookPostSystem::PostResult::k_success);
                             }
                         }
                         else
                         {
                             // User clicked the X button
                             mRequestCompleteDelegate(Social::IFacebookPostSystem::PostResult::k_cancelled);
                         }
                     }
                 }
                 else
                 {
                     NSLog(@"%@", error.localizedDescription);
                     NSLog(@"%@", error.description);
                     if(mRequestCompleteDelegate)
                     {
                         mRequestCompleteDelegate(Social::IFacebookPostSystem::PostResult::k_failed);
                     }
                 }
             }];
        }
        
        void CFacebookPostSystem::OnPublishPermissionAuthorised(const FacebookAuthenticationSystem::AuthenticateResponse& insResponse)
        {
            switch(insResponse.eResult)
            {
                case FacebookAuthenticationSystem::AuthenticateResult::k_success:
                {
                    Post(msPostDesc);
                    break;
                }
                case FacebookAuthenticationSystem::AuthenticateResult::k_failed:
                {
                    if(mCompletionDelegate)
                    {
                        mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_failed);
                    }
                    break;
                }
                case FacebookAuthenticationSystem::AuthenticateResult::k_permissionMismatch:
                {
                    if(mCompletionDelegate)
                    {
                        mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_cancelled);
                    }
                    break;  
                }                    
            }
        }
        
        void CFacebookPostSystem::Post(const FacebookPostDesc& insDesc)
        {
            bool bDisplayedNative = TryPostNative(insDesc);
            if(!bDisplayedNative)
            {
                PostWebBased(insDesc);
            }
        }
        
        bool CFacebookPostSystem::TryPostNative(const Social::FacebookPostDesc& insDesc)
        {
            return false;
            
            bool bIsiOS6 = ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0f);
            
            if(!bIsiOS6)
            {
                return false;
            }
            
            return [FBDialogs presentOSIntegratedShareDialogModallyFrom:[UIApplication sharedApplication].keyWindow.rootViewController
                                                            initialText:Core::StringUtils::StringToNSString(insDesc.strName + " " + insDesc.strDescription)
                                                                  image:nil
                                                                    url:[NSURL URLWithString:Core::StringUtils::StringToNSString(insDesc.strURL)]
                                                                handler:^(FBOSIntegratedShareDialogResult result, NSError *error)
                    {
                        if (error)
                        {
                            NSLog(@"%@", error.localizedDescription);
                            if(mCompletionDelegate)
                            {
                                mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_failed);
                            }
                        }
                        else
                        {
                            if (result == FBNativeDialogResultSucceeded)
                            {
                                if(mCompletionDelegate)
                                {
                                    mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_success);
                                }
                            }
                            else
                            {
                                if(mCompletionDelegate)
                                {
                                    mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_cancelled);
                                }
                            }
                        }
                    }];
        }
        
        void CFacebookPostSystem::PostWebBased(const Social::FacebookPostDesc& insDesc)
        {
            std::string strToUser = "me";
            if(!insDesc.strTo.empty())
            {
                strToUser = insDesc.strTo;
            }
            
            NSDictionary* pPostParams = CreateNSDisctionaryFromPostDesc(insDesc);
            
            [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                                   parameters:pPostParams
                                                      handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error)
            {
                if(!error)
                {
                    if(mCompletionDelegate)
                    {
                        if(result == FBWebDialogResultDialogCompleted)
                        {
                            NSDictionary *urlParams = [URLParser parseURLParams:[resultURL query]];
                            if (![urlParams valueForKey:@"post_id"])
                            {
                                // User clicked the Cancel button
                                mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_cancelled);
                            }
                            else
                            {
                                // User clicked the Share button
                                mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_success);
                            }
                        }
                        else
                        {
                            // User clicked the X button
                            mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_cancelled);
                        }
                    }
                }
                else
                {
                    NSLog(@"%@", error.localizedDescription);
                    NSLog(@"%@", error.description);
                    if(mCompletionDelegate)
                    {
                        mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_failed);
                    }
                }
                
            }];
            
            [pPostParams release];
        }
	}
}
