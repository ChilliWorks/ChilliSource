//
// FacebookAuthenticationSystem.h
// Chilli Source
//
// Created by Robert Henning on 03/05/2012
// Copyright 2012 Tag Games Limited - All rights reserved
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_SOCIAL_FACEBOOKAUTHENTICATIONSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_SOCIAL_FACEBOOKAUTHENTICATIONSYSTEM_H_

#include <CSBackend/Platform/Android/Social/Facebook/FacebookJavaInterface.h>
#include <ChilliSource/Core/Delegate/ConnectableDelegate.h>
#include <ChilliSource/Social/Facebook/FacebookAuthenticationSystem.h>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------
		/// Class for logging in to Facebook using
		/// the Android Facebook SDK. Also handles
		/// the granting of read and write permissions
		///
		/// @author R Henning
		//------------------------------------------------
		class FacebookAuthenticationSystem final : public CSSocial::FacebookAuthenticationSystem
		{
		public:

			CS_DECLARE_NAMEDTYPE(FacebookAuthenticationSystem);

            //----------------------------------------------------
            /// @author R Henning
            ///
            /// @param Interface ID to compare
            ///
            /// @return Whether the interface ID matches this object
            //----------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------
            /// Called when the system is created allocating any
			/// resources
            ///
            /// @author S Downie
            //----------------------------------------------------
            void OnInit() override;
            //------------------------------------------------
            /// Log the user into Facebook with the given
            /// read permissions. If no permissions are specified
            /// then only basic read permissions will be granted
            /// on authentication success
            ///
            /// Only one auth request can be active at any time
            /// otherwise an assert will be triggered
            ///
            /// @author R Henning
            ///
            /// @param Read permissions (http://developers.facebook.com/docs/authentication/permissions/)
            /// @param Connection to result delegate
            //------------------------------------------------
            void Authenticate(const std::vector<std::string>& in_readPermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection) override;
			//------------------------------------------------
            /// @author R Henning
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
            /// @author R Henning
            ///
            /// @param Read permissions
            /// @param Connection to result delegate
            //------------------------------------------------
            void AuthoriseReadPermissions(const std::vector<std::string>& in_readPermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection) override;
            //------------------------------------------------
            /// Request that the user grant write
            /// permissions (http://developers.facebook.com/docs/authentication/permissions/)
            ///
            /// Only one auth request can be active at any time
            /// otherwise an assert will be triggered
            ///
            /// @author R Henning
            ///
            /// @param Write permissions
            /// @param Connection to result delegate
            //------------------------------------------------
            void AuthoriseWritePermissions(const std::vector<std::string>& in_writePermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection) override;
            //------------------------------------------------
            /// @author R Henning
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
            /// @author R Henning
            //------------------------------------------------
            void SignOut() override;
            //----------------------------------------------------
            /// Called when the system is destroyed. This
            /// will close any open sessions
            ///
            /// @author S Downie
            //----------------------------------------------------
            void OnDestroy() override;

            //---Internal functions called by JNI
            //----------------------------------------------------
            /// Called by JNI when authentication completes
            /// either with or without success
            ///
            /// @author R Henning
            ///
            /// @param Success
            //----------------------------------------------------
			void OnAuthenticationComplete(bool in_success);
            //----------------------------------------------------
            /// Called by JNI when read permission authoring completes
            /// either with or without success
            ///
            /// @author R Henning
            ///
            /// @param Success
            //----------------------------------------------------
			void OnAuthoriseReadPermissionsComplete(bool in_success);
            //----------------------------------------------------
            /// Called by JNI when write permission authoring completes
            /// either with or without success
            ///
            /// @author R Henning
            ///
            /// @param Success
            //----------------------------------------------------
			void OnAuthoriseWritePermissionsComplete(bool in_success);

		private:
			friend CSSocial::FacebookAuthenticationSystemUPtr CSSocial::FacebookAuthenticationSystem::Create();
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //----------------------------------------------------
            FacebookAuthenticationSystem() = default;
            //----------------------------------------------------
            /// @author R Henning
            ///
            /// @return Token from active session
            //----------------------------------------------------
            std::string GetActiveToken() const;

		private:

			FacebookJavaInterfaceSPtr m_javaInterface;

			AuthenticationCompleteDelegate::Connection m_authDelegateConnection;
			AuthenticationCompleteDelegate::Connection m_authReadDelegateConnection;
			AuthenticationCompleteDelegate::Connection m_authWriteDelegateConnection;
		};
	}
}

#endif

#endif
