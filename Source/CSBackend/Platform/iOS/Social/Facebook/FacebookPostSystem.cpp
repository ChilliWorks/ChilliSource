//
//  FacebookPostSystem.cpp
//  Chilli Source
//  Created by Stuart McGaw on 06/06/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_IOS

#include <CSBackend/Platform/iOS/Social/Facebook/FacebookPostSystem.h>

#include <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Social/Facebook/FacebookAuthenticationSystem.h>

#include <FacebookSDK/FacebookSDK.h>

#ifdef __IPHONE_6_0
#include <UIKit/UIKit.h>
#include <Social/Social.h>
#include <CSBackend/Platform/iOS/Social/Facebook/FacebookUtils.h>
#endif

namespace CSBackend
{
	namespace iOS
	{
        namespace
        {
            //----------------------------------------------------
            /// Convert the post description to a newly allocated
            /// NSDictionary which is what the iOS framework deals
            /// with
            ///
            /// @author S Downie
            ///
            /// @param Post description
            /// @return Ownership of new dictionary
            //----------------------------------------------------
            NSDictionary* CreateNSDisctionaryFromPostDesc(const CSSocial::FacebookPostSystem::PostDesc& in_desc)
            {
                NSString* url = [NSStringUtils newNSStringWithUTF8String:in_desc.m_url];
                NSString* picUrl = [NSStringUtils newNSStringWithUTF8String:in_desc.m_picUrl];
                NSString* name = [NSStringUtils newNSStringWithUTF8String:in_desc.m_name];
                NSString* caption = [NSStringUtils newNSStringWithUTF8String:in_desc.m_caption];
                NSString* description = [NSStringUtils newNSStringWithUTF8String:in_desc.m_description];
                NSString* to = [NSStringUtils newNSStringWithUTF8String:in_desc.m_to];
                
                NSDictionary* postParams = [[NSMutableDictionary alloc] initWithObjectsAndKeys: url, @"link", picUrl, @"picture", name, @"name", caption, @"caption", description, @"description", to, @"to", nil];
                
                [url release];
                [picUrl release];
                [name release];
                [caption release];
                [description release];
                [to release];
                
                return postParams;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(FacebookPostSystem);
        
        //----------------------------------------------------
        //----------------------------------------------------
		FacebookPostSystem::FacebookPostSystem(CSSocial::FacebookAuthenticationSystem* in_authSystem)
        : m_authSystem(in_authSystem)
		{

		}
		//----------------------------------------------------
        //----------------------------------------------------
		bool FacebookPostSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return FacebookPostSystem::InterfaceID == in_interfaceId || CSSocial::FacebookPostSystem::InterfaceID == in_interfaceId;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void FacebookPostSystem::Post(const PostDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection)
		{
            CS_ASSERT(m_postCompleteDelegateConnection == nullptr, "Cannot post more than once at a time");
            CS_ASSERT(m_authSystem->IsSignedIn() == true, "User must be authenticated to post");
            
            m_postCompleteDelegateConnection = std::move(in_delegateConnection);
			
            PostWeb(in_desc);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookPostSystem::SendRequest(const RequestDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection)
        {
            CS_ASSERT(m_requestCompleteDelegateConnection == nullptr, "Cannot request more than once at a time");
            CS_ASSERT(m_authSystem->IsSignedIn() == true, "User must be authenticated to request");
            
            @autoreleasepool
            {
                //Construct a list of comma separated IDs
                std::string recipients;
                CSCore::StringUtils::ToCSV(in_desc.m_recipients, recipients);
                
                m_requestCompleteDelegateConnection = std::move(in_delegateConnection);
                
                NSString* requestType = @"to";
                
                switch (in_desc.m_type)
                {
                    case RequestRecipientMode::k_fixed:
                        requestType = @"to";
                        break;
                    case RequestRecipientMode::k_optional:
                        requestType = @"suggestions";
                        break;
                }
                
                NSString* nsReciptients = [NSStringUtils newNSStringWithUTF8String:recipients];
                
                NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys:nsReciptients, requestType, nil];
                
                [nsReciptients release];
                
                NSString* description = [NSStringUtils newNSStringWithUTF8String:in_desc.m_description];
                NSString* caption = [NSStringUtils newNSStringWithUTF8String:in_desc.m_caption];
                
                [FBWebDialogs presentRequestsDialogModallyWithSession:nil message:description title:caption parameters:params handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error)
                 {
                     if(!error)
                     {
                         if(m_requestCompleteDelegateConnection)
                         {
                             if(result == FBWebDialogResultDialogCompleted)
                             {
                                 NSDictionary* urlParams = [URLParser parseURLParams:[resultURL query]];
                                 if (![urlParams valueForKey:@"request"])
                                 {
                                     // User clicked the Cancel button
                                     auto delegateConnection = std::move(m_requestCompleteDelegateConnection);
                                     m_requestCompleteDelegateConnection = nullptr;
                                     delegateConnection->Call(CSSocial::FacebookPostSystem::PostResult::k_cancelled);
                                 }
                                 else
                                 {
                                     // User clicked the Share button
                                     auto delegateConnection = std::move(m_requestCompleteDelegateConnection);
                                     m_requestCompleteDelegateConnection = nullptr;
                                     delegateConnection->Call(CSSocial::FacebookPostSystem::PostResult::k_success);
                                 }
                 
                                 [urlParams release];
                             }
                             else
                             {
                                 // User clicked the X button
                                 auto delegateConnection = std::move(m_requestCompleteDelegateConnection);
                                 m_requestCompleteDelegateConnection = nullptr;
                                 delegateConnection->Call(CSSocial::FacebookPostSystem::PostResult::k_cancelled);
                             }
                         }
                     }
                     else
                     {
                         NSLog(@"%@", error.localizedDescription);
                         NSLog(@"%@", error.description);
                 
                         if(m_requestCompleteDelegateConnection)
                         {
                             auto delegateConnection = std::move(m_requestCompleteDelegateConnection);
                             m_requestCompleteDelegateConnection = nullptr;
                             delegateConnection->Call(CSSocial::FacebookPostSystem::PostResult::k_failed);
                         }
                     }
                 }];
                
                [description release];
                [caption release];
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookPostSystem::PostWeb(const PostDesc& in_desc)
        {
            //If the user is posting to nobody then they are posting to
            //their own wall.
            std::string toUser = "me";
            if(!in_desc.m_to.empty())
            {
                toUser = in_desc.m_to;
            }
            
            NSDictionary* postParams = CreateNSDisctionaryFromPostDesc(in_desc);
            
            [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                                   parameters:postParams
                                                      handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error)
            {
                if(!error)
                {
                    if(m_postCompleteDelegateConnection)
                    {
                        if(result == FBWebDialogResultDialogCompleted)
                        {
                            NSDictionary* urlParams = [URLParser parseURLParams:[resultURL query]];
                            if (![urlParams valueForKey:@"post_id"])
                            {
                                // User clicked the Cancel button
                                 auto delegateConnection = std::move(m_postCompleteDelegateConnection);
                                 m_postCompleteDelegateConnection = nullptr;
                                 delegateConnection->Call(CSSocial::FacebookPostSystem::PostResult::k_cancelled);
                            }
                            else
                            {
                                // User clicked the Share button
                                 auto delegateConnection = std::move(m_postCompleteDelegateConnection);
                                 m_postCompleteDelegateConnection = nullptr;
                                 delegateConnection->Call(CSSocial::FacebookPostSystem::PostResult::k_success);
                            }
                            [urlParams release];
                        }
                        else
                        {
                            // User clicked the X button
                             auto delegateConnection = std::move(m_postCompleteDelegateConnection);
                             m_postCompleteDelegateConnection = nullptr;
                             delegateConnection->Call(CSSocial::FacebookPostSystem::PostResult::k_cancelled);
                        }
                    }
                }
                else
                {
                    NSLog(@"%@", error.localizedDescription);
                    NSLog(@"%@", error.description);
             
                    if(m_postCompleteDelegateConnection)
                    {
                         auto delegateConnection = std::move(m_postCompleteDelegateConnection);
                         m_postCompleteDelegateConnection = nullptr;
                         delegateConnection->Call(CSSocial::FacebookPostSystem::PostResult::k_failed);
                    }
                }
            }];
            
            [postParams release];
        }
	}
}

#endif