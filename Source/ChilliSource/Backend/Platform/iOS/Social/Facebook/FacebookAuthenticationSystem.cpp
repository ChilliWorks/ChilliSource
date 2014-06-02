//
//  FacebookAuthenticationSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 01/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookAuthenticationSystem.h>

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>

#ifdef __IPHONE_6_0
#	include <Accounts/Accounts.h>
#endif

namespace ChilliSource
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
		bool FacebookAuthenticationSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == FacebookAuthenticationSystem::InterfaceID) || (in_interfaceId == Social::FacebookAuthenticationSystem::InterfaceID);
		}
		//----------------------------------------------------
        //----------------------------------------------------
		void FacebookAuthenticationSystem::Authenticate(const std::vector<std::string>& in_readPermissions, const AuthenticationCompleteDelegate& in_delegate)
		{
            CS_ASSERT(m_authenticateDelegate == nullptr, "Cannot authenticate more than once at the same time");
            
            m_authenticateDelegate = in_delegate;

            if(IsSignedIn())
            {
                //If we already have an open session then just return a success
                if(m_authenticateDelegate)
                {
                    AuthenticateResponse response;
                    response.m_token = GetActiveToken();
                    response.m_result = AuthenticateResult::k_success;
                    m_authenticateDelegate(response);
                    m_authenticateDelegate = nullptr;
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
                     
                     if(m_authenticateDelegate)
                     {
                         AuthenticateResponse response;
                         response.m_result = AuthenticateResult::k_failed;
                         m_authenticateDelegate(response);
                         m_authenticateDelegate = nullptr;
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
                    if(m_authenticateDelegate)
                    {
                        AuthenticateResponse response;
                        response.m_token = [NSStringUtils newUTF8StringWithNSString:[[in_session accessTokenData] accessToken]];
                        response.m_result = AuthenticateResult::k_success;
                        m_authenticateDelegate(response);
                        m_authenticateDelegate = nullptr;
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
                    if(m_authenticateDelegate)
                    {
                        AuthenticateResponse response;
                        response.m_result = AuthenticateResult::k_failed;
                        m_authenticateDelegate(response);
                        m_authenticateDelegate = nullptr;
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
        void FacebookAuthenticationSystem::AuthoriseReadPermissions(const std::vector<std::string>& in_readPermissions, const AuthenticationCompleteDelegate& in_delegate)
        {
            CS_ASSERT(m_authoriseReadDelegate == nullptr, "Only one read permission request can be active at a time");
            CS_ASSERT(IsSignedIn() == true, "Must be authenticated");
            
            m_authoriseReadDelegate = in_delegate;
            
            NSArray * permissionsArray = CreateNSArrayFromStringArray(in_readPermissions);
            
            [FBSession.activeSession requestNewReadPermissions:permissionsArray
                                             completionHandler:^(FBSession* session, NSError* error)
            {
                 if(error)
                 {
                     NSLog(@"%@", error.localizedDescription);
             
                     if(m_authoriseReadDelegate)
                     {
                         AuthenticateResponse response;
                         response.m_result = AuthenticateResult::k_failed;
                         m_authoriseReadDelegate(response);
                         m_authoriseReadDelegate = nullptr;
                     }
                 }
                 else if(m_authoriseReadDelegate)
                 {
                     AuthenticateResponse response;
                     response.m_result = AuthenticateResult::k_success;
                     response.m_token = GetActiveToken();
                     m_authoriseReadDelegate(response);
                     m_authoriseReadDelegate = nullptr;
                 }
            }];
            
            [permissionsArray release];
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::AuthoriseWritePermissions(const std::vector<std::string>& in_writePermissions, const AuthenticationCompleteDelegate& in_delegate)
        {
            CS_ASSERT(m_authoriseWriteDelegate == nullptr, "Only one write permission request can be active at a time");
            CS_ASSERT(IsSignedIn() == true, "Must be authenticated");
            
            m_authoriseWriteDelegate = in_delegate;
            
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
             
                    if(m_authoriseWriteDelegate)
                    {
                        AuthenticateResponse response;
                        response.m_result = AuthenticateResult::k_failed;
                        m_authoriseWriteDelegate(response);
                        m_authoriseWriteDelegate = nullptr;
                    }
                }
                else if(m_authoriseWriteDelegate)
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
                    
                    response.m_token = GetActiveToken();
                    m_authoriseWriteDelegate(response);
                    m_authoriseWriteDelegate = nullptr;
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
        }
	}
}

#endif
