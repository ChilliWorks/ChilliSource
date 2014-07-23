//
//  FacebookAuthenticationSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 01/06/2011.
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

#include <CSBackend/Platform/iOS/Social/Facebook/FacebookAuthenticationSystem.h>

#include <CSBackend/Platform/iOS/Social/Facebook/FacebookAppDelegate.h>
#include <CSBackend/Platform/iOS/Core/Base/CSAppDelegate.h>
#include <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

#ifdef __IPHONE_6_0
#	include <Accounts/Accounts.h>
#endif

namespace CSBackend
{
	namespace iOS
	{
        namespace
        {
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Array of std strings
            ///
            /// @return Ownership of NSArray of NSStrings
            //----------------------------------------------------
            NSArray* CreateNSArrayFromStringArray(const std::vector<std::string> & in_strings)
            {
                NSMutableArray * result = [[NSMutableArray alloc] initWithCapacity:in_strings.size()];
                
                for (u32 i=0; i<in_strings.size(); ++i)
                {
                    NSString* string = [NSStringUtils newNSStringWithUTF8String:in_strings[i]];
                    [result addObject: string];
                    [string release];
                }
                
                return result;
            }
        }
        
		CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);
        
		//----------------------------------------------------
        //----------------------------------------------------
		bool FacebookAuthenticationSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == FacebookAuthenticationSystem::InterfaceID) || (in_interfaceId == CSSocial::FacebookAuthenticationSystem::InterfaceID);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::OnInit()
        {
            m_fbAppDelegate = [[FacebookAppDelegate alloc] init];
            [[CSAppDelegate sharedInstance] addAppDelegateListener:m_fbAppDelegate];
        }
		//----------------------------------------------------
        //----------------------------------------------------
		void FacebookAuthenticationSystem::Authenticate(const std::vector<std::string>& in_readPermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection)
		{
            CS_ASSERT(m_authenticateDelegateConnection == nullptr, "Cannot authenticate more than once at the same time");
            
            m_authenticateDelegateConnection = std::move(in_delegateConnection);

            if(IsSignedIn())
            {
                //If we already have an open session then just return a success
                if(m_authenticateDelegateConnection)
                {
                    AuthenticateResponse response;
                    response.m_token = GetActiveToken();
                    response.m_result = AuthenticateResult::k_success;

                    auto delegateConnection = std::move(m_authenticateDelegateConnection);
                    m_authenticateDelegateConnection = nullptr;
                    delegateConnection->Call(response);
                }
                
                return;
            }
            
            bool hasExistingSession = TryResumeExisitingSession(in_readPermissions);
            if(!hasExistingSession)
            {
                CreateNewSession(in_readPermissions);
            }
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::CreateNewSession(const std::vector<std::string>& in_readPermissions)
        {
            OpenSession(in_readPermissions, true);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool FacebookAuthenticationSystem::TryResumeExisitingSession(const std::vector<std::string>& in_readPermissions)
        {
            if(FBSession.activeSession.state != FBSessionStateCreatedTokenLoaded)
                return false;
            
            return OpenSession(in_readPermissions, false);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool FacebookAuthenticationSystem::OpenSession(const std::vector<std::string>& in_readPermissions, bool in_shouldPresentLogin)
        {
            NSArray* permissionsArray = in_readPermissions.empty() ? nil : CreateNSArrayFromStringArray(in_readPermissions);
            bool hasExistingSession = [FBSession openActiveSessionWithReadPermissions:permissionsArray
                                                                         allowLoginUI:in_shouldPresentLogin
                                                                    completionHandler:^(FBSession* session, FBSessionState state, NSError* error)
             {
                 if(!error)
                 {
                     OnSessionStateChanged(session, state);
                 }
                 else
                 {
                     NSLog(@"%@", error.localizedDescription);
                     
                     if(m_authenticateDelegateConnection)
                     {
                         AuthenticateResponse response;
                         response.m_result = AuthenticateResult::k_failed;
           
                         auto delegateConnection = std::move(m_authenticateDelegateConnection);
                         m_authenticateDelegateConnection = nullptr;
                         delegateConnection->Call(response);
                     }
                 }
             }];
            
            if(permissionsArray)
            {
                [permissionsArray release];
            }
            
            return hasExistingSession;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::OnSessionStateChanged(FBSession* in_session, FBSessionState in_state)
        {
            switch (in_state)
            {
                case FBSessionStateOpen:
                {
                    if(m_authenticateDelegateConnection)
                    {
                        AuthenticateResponse response;
                        response.m_token = [NSStringUtils newUTF8StringWithNSString:[[in_session accessTokenData] accessToken]];
                        response.m_result = AuthenticateResult::k_success;
  
                        auto delegateConnection = std::move(m_authenticateDelegateConnection);
                        m_authenticateDelegateConnection = nullptr;
                        delegateConnection->Call(response);
                    }
                    break;
                }
                case FBSessionStateClosed:
                {
                    [in_session closeAndClearTokenInformation];
                    break;
                }
                case FBSessionStateClosedLoginFailed:
                default:
                {
                    if(m_authenticateDelegateConnection)
                    {
                        AuthenticateResponse response;
                        response.m_result = AuthenticateResult::k_failed;
     
                        auto delegateConnection = std::move(m_authenticateDelegateConnection);
                        m_authenticateDelegateConnection = nullptr;
                        delegateConnection->Call(response);
                    }
                    break;
                }
            }
        }
		//----------------------------------------------------
        //----------------------------------------------------
		bool FacebookAuthenticationSystem::IsSignedIn() const
		{
			return FBSession.activeSession.isOpen;
		}
		//----------------------------------------------------
        //----------------------------------------------------
        std::string FacebookAuthenticationSystem::GetActiveToken() const
		{
			return [NSStringUtils newUTF8StringWithNSString:FBSession.activeSession.accessTokenData.accessToken];
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::AuthoriseReadPermissions(const std::vector<std::string>& in_readPermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection)
        {
            CS_ASSERT(m_authoriseReadDelegateConnection == nullptr, "Only one read permission request can be active at a time");
            CS_ASSERT(IsSignedIn() == true, "Must be authenticated");
            
            m_authoriseReadDelegateConnection = std::move(in_delegateConnection);
            
            NSArray * permissionsArray = CreateNSArrayFromStringArray(in_readPermissions);
            
            [FBSession.activeSession requestNewReadPermissions:permissionsArray
                                             completionHandler:^(FBSession* session, NSError* error)
            {
                 if(error)
                 {
                     NSLog(@"%@", error.localizedDescription);
             
                     if(m_authoriseReadDelegateConnection)
                     {
                         AuthenticateResponse response;
                         response.m_result = AuthenticateResult::k_failed;

                         auto delegateConnection = std::move(m_authoriseReadDelegateConnection);
                         m_authoriseReadDelegateConnection = nullptr;
                         delegateConnection->Call(response);
                     }
                 }
                 else if(m_authoriseReadDelegateConnection)
                 {
                     AuthenticateResponse response;
                     response.m_result = AuthenticateResult::k_success;
                     response.m_token = GetActiveToken();
  
                     auto delegateConnection = std::move(m_authoriseReadDelegateConnection);
                     m_authoriseReadDelegateConnection = nullptr;
                     delegateConnection->Call(response);
                 }
            }];
            
            [permissionsArray release];
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::AuthoriseWritePermissions(const std::vector<std::string>& in_writePermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection)
        {
            CS_ASSERT(m_authoriseWriteDelegateConnection == nullptr, "Only one write permission request can be active at a time");
            CS_ASSERT(IsSignedIn() == true, "Must be authenticated");
            
            m_authoriseWriteDelegateConnection = std::move(in_delegateConnection);
            
            //We store these so we can check that the user granted all our permissions
            m_currentRequestedWritePermissions = in_writePermissions;
            
            NSArray * permissionsArray = CreateNSArrayFromStringArray(m_currentRequestedWritePermissions);
            
            [FBSession.activeSession requestNewPublishPermissions:permissionsArray
                                                  defaultAudience:FBSessionDefaultAudienceFriends
                                                completionHandler:^(FBSession* session, NSError* error)
            {
                if(error)
                {
                    NSLog(@"%@", error.localizedDescription);
             
                    if(m_authoriseWriteDelegateConnection)
                    {
                         AuthenticateResponse response;
                         response.m_result = AuthenticateResult::k_failed;
     
                         auto delegateConnection = std::move(m_authoriseWriteDelegateConnection);
                         m_authoriseWriteDelegateConnection = nullptr;
                         delegateConnection->Call(response);
                    }
                }
                else if(m_authoriseWriteDelegateConnection)
                {
                    //Check to see if the user granted the permissions we asked for.
                    bool permissionMismatch = false;
                    
                    for(u32 i=0; i<m_currentRequestedWritePermissions.size(); ++i)
                    {
                        NSString* permissionName = [NSString stringWithCString:m_currentRequestedWritePermissions.at(i).c_str() encoding:[NSString defaultCStringEncoding]];
                        
                        permissionMismatch = ![[FBSession activeSession].permissions containsObject:permissionName];
                        
                        if(permissionMismatch)
                        {
                            break;
                        }
                    }
                    
                    //Free unused memory.
                    m_currentRequestedWritePermissions.clear();
                    m_currentRequestedWritePermissions.shrink_to_fit();
             
                    AuthenticateResponse response;
                    if(permissionMismatch == false)
                    {
                        response.m_result = AuthenticateResult::k_success;
                    }
                    else
                    {
                        response.m_result = AuthenticateResult::k_permissionMismatch;
                    }
             
                     auto delegateConnection = std::move(m_authoriseWriteDelegateConnection);
                     m_authoriseWriteDelegateConnection = nullptr;
                     delegateConnection->Call(response);
                }
            }];
            
            [permissionsArray release];
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool FacebookAuthenticationSystem::HasPermission(const std::string& in_permission) const
        {
            NSString* permission = [NSStringUtils newNSStringWithUTF8String:in_permission];
            NSInteger result = [FBSession.activeSession.permissions indexOfObject:permission];
            [permission release];
            return (result != NSNotFound);
        }
		//----------------------------------------------------
        //----------------------------------------------------
		void FacebookAuthenticationSystem::SignOut()
		{
            if(IsSignedIn())
            {
                [FBSession.activeSession closeAndClearTokenInformation];
            }
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::OnDestroy()
        {
            [FBSession.activeSession close];
            [[CSAppDelegate sharedInstance] removeAppDelegateListener:m_fbAppDelegate];
            [m_fbAppDelegate release];
        }
	}
}

#endif
