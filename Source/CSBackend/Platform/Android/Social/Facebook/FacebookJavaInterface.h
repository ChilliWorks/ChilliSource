/**
 * CFacebookJavaInterface.cpp
 * moFlow
 *
 * Created by Robert Henning on 01/05/2012
 * Copyright �2012 Tag Games Limited - All rights reserved
 */

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _MOFLO_PLATFORM_ANDROID_FACEBOOK_JAVA_INTERFACE_
#define _MOFLO_PLATFORM_ANDROID_FACEBOOK_JAVA_INTERFACE_

#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		class FacebookAuthenticationSystem;
		class FacebookPostSystem;

		class FacebookJavaInterface : public IJavaInterface
		{
		public:

			FacebookJavaInterface();

			CS_DECLARE_NAMEDTYPE(FacebookJavaInterface);
			bool IsA(Core::InterfaceIDType inInterfaceID) const;

			//--------------------------------------------------------------------------------------
			/// Set Authentication System
			///
			/// Used to callback into the auth system
			///
			/// @param Auth system
			//--------------------------------------------------------------------------------------
			void SetAuthenticationSystem(FacebookAuthenticationSystem* inpAuthSystem);
			//--------------------------------------------------------------------------------------
			/// Set Post System
			///
			/// Used to callback into the post system
			///
			/// @param Post system
			//--------------------------------------------------------------------------------------
			void SetPostSystem(FacebookPostSystem* inpPostSystem);
			//--------------------------------------------------------------------------------------
			/// Authenticate
			///
			/// Attempt to open an existing session or prompt the user to login if no
			/// session exists
			//--------------------------------------------------------------------------------------
			void Authenticate(const std::vector<std::string>& inaReadPerms);
			//--------------------------------------------------------------------------------------
			/// Is Signed In
			///
			/// @return Whether a session exists and is open
			//--------------------------------------------------------------------------------------
			bool IsSignedIn();
			//--------------------------------------------------------------------------------------
			/// Get Active Token
			///
			/// @return The access token of the active session
			//--------------------------------------------------------------------------------------
			std::string GetActiveToken();
			//--------------------------------------------------------------------------------------
			/// Has Permission
			///
			/// @param Permission ID
		    /// @return Whether the current session user has given the permission
			//--------------------------------------------------------------------------------------
		    bool HasPermission(const std::string& instrPermission);
			//--------------------------------------------------------------------------------------
			/// Authorise Read Permissions
			///
		    /// Prompt Facebook to ask the users to grant the given permissions.
		    /// Calls into native with the result
		    ///
			/// @param List of read permissions IDs
			//--------------------------------------------------------------------------------------
		    void AuthoriseReadPermissions(const std::vector<std::string>& inaReadPerms);
			//--------------------------------------------------------------------------------------
			/// Authorise Write Permissions
			///
		    /// Prompt Facebook to ask the users to grant the given permissions.
		    /// Calls into native with the result
		    ///
			/// @param List of write permissions IDs
			//--------------------------------------------------------------------------------------
		    void AuthoriseWritePermissions(const std::vector<std::string>& inaWritePerms);
			//--------------------------------------------------------------------------------------
			/// Sign Out
			///
		    /// If a session exists log the user out and destroy any cached tokens
			//--------------------------------------------------------------------------------------
		    void SignOut();
			//--------------------------------------------------------------------------------------
			/// Try Post To Feed
			///
		    /// Attempt to make a Graph API request to post to feed with the following
		    /// path and key value array
			//--------------------------------------------------------------------------------------
		    void TryPostToFeed(const std::string& instrGraphPath, const std::vector<std::string>& inaKeyValues);
			//--------------------------------------------------------------------------------------
			/// Try Post To Feed
			///
		    /// Attempt to make a Graph API request to post to feed with the following
		    /// path and key value array
			//--------------------------------------------------------------------------------------
		    void TryPostRequest(const std::vector<std::string>& inaKeyValues);
		};
	}
}

#endif

#endif