/*
 *  FacebookAuthentication.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 02/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookAuthentication.h>

#include <ChilliSource/Core/File/LocalDataStore.h>

#ifdef __IPHONE_6_0
#	include <Accounts/Accounts.h>
#endif

NSArray* CreateNSArrayFromStringArray(const std::vector<std::string> & inaStrings)
{
    NSMutableArray * pResult = [[NSMutableArray alloc] initWithCapacity:inaStrings.size()];
    
    for (u32 i = 0; i < inaStrings.size(); ++i)
    {
        [pResult addObject: ChilliSource::Core::StringUtils::StringToNSString(inaStrings[i])];
    }
    
    return pResult;
}

namespace ChilliSource
{
	using namespace Social;
	using namespace Core;
    
	namespace iOS
	{
		CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);
		
		bool FacebookAuthenticationSystem::IsA(Core::InterfaceIDType inID) const
		{
			return (inID == FacebookAuthenticationSystem::InterfaceID) || (inID == FacebookAuthenticationSystem::InterfaceID);
		}
		
		void FacebookAuthenticationSystem::Authenticate(const std::vector<std::string>& inastrReadPermissions, const FacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate)
		{
            mAuthenticateDelegate = inDelegate;
            mastrPermissions = inastrReadPermissions;

            if(IsSignedIn())
            {
                if(mAuthenticateDelegate)
                {
                    AuthenticateResponse sResponse;
                    sResponse.strToken = GetActiveToken();
                    sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_success;
                    mAuthenticateDelegate(sResponse);
                }
                
                return;
            }
            
            bool bHasExistingSession = TryResumeExisitingSession();
            if(!bHasExistingSession)
            {
                CreateNewSession();
            }
		}
        
        void FacebookAuthenticationSystem::CreateNewSession()
        {
            OpenSession(true);
        }
        
        bool FacebookAuthenticationSystem::TryResumeExisitingSession()
        {
            return OpenSession(false);
        }
        
        bool FacebookAuthenticationSystem::OpenSession(bool bShowLogin)
        {
            NSArray* pPermissionsArray = mastrPermissions.empty() ? nil : CreateNSArrayFromStringArray(mastrPermissions);
            bool bHasExistingSession = [FBSession openActiveSessionWithReadPermissions:pPermissionsArray
                                               allowLoginUI:bShowLogin
                                          completionHandler:^(FBSession *session, FBSessionState state, NSError *error)
             {
                 if(!error)
                 {
                     OnSessionStateChanged(session, state);
                 }
                 else
                 {
                     NSLog(@"%@", error.localizedDescription);
                     
                     if(mAuthenticateDelegate)
                     {
                         AuthenticateResponse sResponse;
                         sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_failed;
                         mAuthenticateDelegate(sResponse);
                     }
                 }
             }];
            
            return bHasExistingSession;
        }
        
        void FacebookAuthenticationSystem::OnSessionStateChanged(FBSession* inpSession, FBSessionState ineState)
        {
            AuthenticateResponse sResponse;
            
            switch (ineState)
            {
                case FBSessionStateOpen:
                {
                    if(mAuthenticateDelegate)
                    {
                        sResponse.strToken = Core::StringUtils::NSStringToString([[inpSession accessTokenData] accessToken]);
                        sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_success;
                        mAuthenticateDelegate(sResponse);
                    }
                    break;
                }
                case FBSessionStateClosed:
                {
                    [inpSession closeAndClearTokenInformation];
                    break;
                }
                case FBSessionStateClosedLoginFailed:
                default:
                {
                    if(mAuthenticateDelegate)
                    {
                        sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_failed;
                        mAuthenticateDelegate(sResponse);
                    }
                    break;
                }
            }
        }
		
		bool FacebookAuthenticationSystem::IsSignedIn() const
		{
			return FBSession.activeSession.isOpen;
		}
		
        std::string FacebookAuthenticationSystem::GetActiveToken() const
		{
			return Core::StringUtils::NSStringToString(FBSession.activeSession.accessTokenData.accessToken);
		}
        
        void FacebookAuthenticationSystem::AuthoriseReadPermissions(const std::vector<std::string> & inaReadPerms, const FacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate)
        {
            mAuthoriseReadDelegate = inDelegate;
            
            NSArray * pPermissionsArray = CreateNSArrayFromStringArray(inaReadPerms);
            [FBSession.activeSession requestNewReadPermissions:pPermissionsArray
                        completionHandler:^(FBSession *session, NSError *error)
                         {
                 AuthenticateResponse sResponse;
                 if(error)
                 {
                     NSLog(@"%@", error.localizedDescription);
                     if(mAuthoriseReadDelegate)
                     {
                         sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_failed;
                         mAuthoriseReadDelegate(sResponse);
                     }
                 }
                 else if(mAuthoriseReadDelegate)
                 {
                     sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_success;
                     sResponse.strToken = GetActiveToken();
                     mAuthoriseReadDelegate(sResponse);
                 }
             }];
            [pPermissionsArray release];
        }
        
        void FacebookAuthenticationSystem::AuthoriseWritePermissions(const std::vector<std::string> & inaWritePerms, const FacebookAuthenticationSystem::AuthenticationCompleteDelegate& inDelegate)
        {
            mAuthoriseWriteDelegate = inDelegate;
            
            CS_ASSERT(maRequestedWritePermissions.empty(), "Previous authorise hasn't been completed yet!");
            maRequestedWritePermissions = inaWritePerms;
            
            NSArray * pPermissionsArray = CreateNSArrayFromStringArray(inaWritePerms);
             [FBSession.activeSession requestNewPublishPermissions:pPermissionsArray
                                     defaultAudience:FBSessionDefaultAudienceFriends
                                        completionHandler:^(FBSession *session, NSError *error)
                {
                    AuthenticateResponse sResponse;
                    if(error)
                    {
                        NSLog(@"%@", error.localizedDescription);
                        if(mAuthoriseWriteDelegate)
                        {
                            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_failed;
                            mAuthoriseWriteDelegate(sResponse);
                        }
                    }
                    else if(mAuthoriseWriteDelegate)
                    {
                        // Check to see if we got the permissions we asked for.
                        BOOL bPermissionMismatch = NO;
                        
                        for(u32 i = 0; i < maRequestedWritePermissions.size(); ++i)
                        {
                            NSString *permissionName = [NSString stringWithCString:maRequestedWritePermissions.at(i).c_str() encoding:[NSString defaultCStringEncoding]];
                            
                            bPermissionMismatch = ![[FBSession activeSession].permissions containsObject:permissionName];
                            
                            if(bPermissionMismatch)
                            {
                                break;
                            }
                        }
                        
                        // Free unused memory.
                        maRequestedWritePermissions.clear();
                        std::vector<std::string>(maRequestedWritePermissions).swap(maRequestedWritePermissions);
                        
                        if(bPermissionMismatch == NO)
                        {
                            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_success;
                        }
                        else
                        {
                            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_permissionMismatch;
                        }
                        
                        sResponse.strToken = GetActiveToken();
                        mAuthoriseWriteDelegate(sResponse);
                    }
                }];
            [pPermissionsArray release];
        }
        
        bool FacebookAuthenticationSystem::HasPermission(const std::string& instrPermission) const
        {
            return ([FBSession.activeSession.permissions indexOfObject:StringUtils::StringToNSString(instrPermission)] != NSNotFound);
        }
		
		void FacebookAuthenticationSystem::SignOut()
		{
            if(IsSignedIn())
            {
                [FBSession.activeSession closeAndClearTokenInformation];
            }
		}
        
        void FacebookAuthenticationSystem::PublishInstall()
        {
            NSString* nsAppID = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"FacebookAppID"];
            
            if(nsAppID)
            {
                [FBSettings setDefaultAppID:nsAppID];
                [FBAppEvents activateApp];
            }
            else
            {
                CS_LOG_ERROR("No app ID specified in plist (FacebookAppID)");
            }
        }
        
        FacebookAuthenticationSystem::~FacebookAuthenticationSystem()
        {
            [FBSession.activeSession close];
        }
	}
}

