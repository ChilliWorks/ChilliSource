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

namespace moFlo
{
	using namespace Social;
	
	namespace iOSPlatform
	{
		CFacebookPostSystem::CFacebookPostSystem(Social::IFacebookAuthenticationSystem* inpAuthSystem)
        : mpAuthSystem(inpAuthSystem)
		{
			mCompletionDelegate = NULL;
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
                                                                                       Core::CStringUtils::StringToNSString(insDesc.strURL), @"link",
                                                                                       Core::CStringUtils::StringToNSString(insDesc.strPictureURL), @"picture",
                                                                                       Core::CStringUtils::StringToNSString(insDesc.strName), @"name",
                                                                                       Core::CStringUtils::StringToNSString(insDesc.strCaption), @"caption",
                                                                                       Core::CStringUtils::StringToNSString(insDesc.strDescription), @"description",
                                                                                       Core::CStringUtils::StringToNSString(insDesc.strTo), @"to",
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
                    DYNAMIC_ARRAY<std::string> aWritePerms;
                    aWritePerms.push_back("publish_actions");
                    if(insDesc.strTo != "")
                        aWritePerms.push_back("publish_stream");
                    mpAuthSystem->AuthoriseWritePermissions(aWritePerms, fastdelegate::MakeDelegate(this, &CFacebookPostSystem::OnPublishPermissionAuthorised));
                }
            }
            else
            {
                ERROR_LOG("Facebook Post: User must be authenticated");
            }
        }
        
        void CFacebookPostSystem::TrySendRequest(const Social::FacebookPostDesc& insDesc, const PostResultDelegate& insResultCallback, DYNAMIC_ARRAY<std::string>& inastrRecommendedFriends)
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
                params = [NSMutableDictionary dictionaryWithObjectsAndKeys:Core::CStringUtils::StringToNSString(insDesc.strTo), @"to", nil];
            }
            else
            {
                params = [NSMutableDictionary dictionaryWithObjectsAndKeys:Core::CStringUtils::StringToNSString(strFriends), @"suggestions", nil];
            }
            
            [FBWebDialogs presentRequestsDialogModallyWithSession:nil
                                                          message:Core::CStringUtils::StringToNSString(insDesc.strDescription)
                                                            title:Core::CStringUtils::StringToNSString(insDesc.strCaption)
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
        
        void CFacebookPostSystem::OnPublishPermissionAuthorised(const IFacebookAuthenticationSystem::AuthenticateResponse& insResponse)
        {
            switch(insResponse.eResult)
            {
                case IFacebookAuthenticationSystem::AuthenticateResult::k_success:
                {
                    Post(msPostDesc);
                    break;
                }
                case IFacebookAuthenticationSystem::AuthenticateResult::k_failed:
                {
                    if(mCompletionDelegate)
                    {
                        mCompletionDelegate(Social::IFacebookPostSystem::PostResult::k_failed);
                    }
                    break;
                }
                case IFacebookAuthenticationSystem::AuthenticateResult::k_permissionMismatch:
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
                                                            initialText:Core::CStringUtils::StringToNSString(insDesc.strName + " " + insDesc.strDescription)
                                                                  image:nil
                                                                    url:[NSURL URLWithString:Core::CStringUtils::StringToNSString(insDesc.strURL)]
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
