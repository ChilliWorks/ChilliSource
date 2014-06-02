//
//  FacebookAuthenticationSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 01/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_SOCIAL_FACEBOOKAUTHENTICATIONSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_SOCIAL_FACEBOOKAUTHENTICATIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Social/Facebook/FacebookAuthenticationSystem.h>

#include <FacebookSDK/FacebookSDK.h>

namespace ChilliSource
{
	namespace iOS
	{
        //------------------------------------------------
        /// Class for logging in to Facebook using
        /// the iOS Facebook framework. Also handles
        /// the granting of read and write permissions
        ///
        /// @author S Downie
        //------------------------------------------------
		class FacebookAuthenticationSystem final : public Social::FacebookAuthenticationSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(FacebookAuthenticationSystem);
        
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID to compare
            ///
            /// @return Whether the interface ID matches this object
            //----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------
            /// Log the user into Facebook with the given
            /// read permissions. If no permissions are specified
            /// then only basic read permissions will be granted
            /// on authentication success
            ///
            /// Only one auth request can be active at any time
            /// otherwise an assert will be triggered
            ///
            /// @author S Downie
            ///
            /// @param Read permissions (http://developers.facebook.com/docs/authentication/permissions/)
            /// @param Result delegate
            //------------------------------------------------
            void Authenticate(const std::vector<std::string>& in_readPermissions, const AuthenticationCompleteDelegate& in_delegate) override;
			//------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether there is an active session
            //------------------------------------------------
            bool IsSignedIn() const override;
            //------------------------------------------------
            /// Request that the user grant additional read
            /// permissions (http://developers.facebook.com/docs/authentication/permissions/)
            ///
            /// Only one auth request can be active at any time
            /// otherwise an assert will be triggered
            ///
            /// @author S Downie
            ///
            /// @param Read permissions
            /// @param Result delegate
            //------------------------------------------------
            void AuthoriseReadPermissions(const std::vector<std::string>& in_readPermissions, const AuthenticationCompleteDelegate& in_delegate) override;
            //------------------------------------------------
            /// Request that the user grant write
            /// permissions (http://developers.facebook.com/docs/authentication/permissions/)
            ///
            /// Only one auth request can be active at any time
            /// otherwise an assert will be triggered
            ///
            /// @author S Downie
            ///
            /// @param Write permissions
            /// @param Result delegate
            //------------------------------------------------
            void AuthoriseWritePermissions(const std::vector<std::string>& in_writePermissions, const AuthenticationCompleteDelegate& in_delegate) override;
            //------------------------------------------------
            /// @author S Downie
            ///
            /// @param Permission to check
            ///
            /// @return Whether the given permission has been
            /// granted by the user
            //------------------------------------------------
            bool HasPermission(const std::string& in_permission) const override;
            //------------------------------------------------
            /// Log the current user out of Facebook and
            /// close the active session
            ///
            /// @author S Downie
            //------------------------------------------------
            void SignOut() override;
            //----------------------------------------------------
            /// Called when the system is destroyed. This
            /// will close any open sessions
            ///
            /// @author S Downie
            //----------------------------------------------------
            void OnDestroy() override;
			
		private:
            
            friend Social::FacebookAuthenticationSystemUPtr Social::FacebookAuthenticationSystem::Create();
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //----------------------------------------------------
            FacebookAuthenticationSystem() = default;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Active session token
            //----------------------------------------------------
            std::string GetActiveToken() const;
            //----------------------------------------------------
            /// Create a new Facebook session with the given
            /// read permissions. This should only be called
            /// when there is no active Facebook session. This
            /// will cause the Facebook login dialog to appear
            ///
            /// @author S Downie
            ///
            /// @param Read permissions
            //----------------------------------------------------
            void CreateNewSession(const std::vector<std::string>& in_readPermissions);
            //----------------------------------------------------
            /// Attempt to resume a cached Facebook session with
            /// the given read permissions
            ///
            /// @author S Downie
            ///
            /// @param Read permissions
            ///
            /// @return Whether a session now exists or a new one
            /// should be opened
            //----------------------------------------------------
            bool TryResumeExisitingSession(const std::vector<std::string>& in_readPermissions);
            //----------------------------------------------------
            /// Open session can be used to attempt to resume
            /// a previous session or to create a new one. The
            /// function will always attempt to open an existing session
            /// the "in_shouldPresentLogin" option determines whether
            /// it presents the Facebook login dialogue. You don't want
            /// the dialogue to appear if we have an active session so this
            /// function may need to be called twice with false then true.
            ///
            /// @author S Downie
            ///
            /// @param Read permissions
            /// @param Whether the login dialogue is presented
            ///
            /// @return Whether an existing session was opened
            //----------------------------------------------------
            bool OpenSession(const std::vector<std::string>& in_readPermissions, bool in_shouldPresentLogin);
            //----------------------------------------------------
            /// Triggered when the state of the active session
            /// changes i.e. open, closed, etc.
            ///
            /// @param Session
            /// @param Current state
            //----------------------------------------------------
            void OnSessionStateChanged(FBSession* in_session, FBSessionState in_state);
            
        private:
            
            AuthenticationCompleteDelegate m_authenticateDelegate;
            AuthenticationCompleteDelegate m_authoriseReadDelegate;
            AuthenticationCompleteDelegate m_authoriseWriteDelegate;
            
            std::vector<std::string> m_currentRequestedWritePermissions;
		};
	}
}

#endif

#endif