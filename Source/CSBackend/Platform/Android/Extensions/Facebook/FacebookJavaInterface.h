//
//  FacebookJavaInterface.h
//  Chilli Source
//  Created by Robert Henning on 01/05/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_FACEBOOK

#ifndef _CHILLISOURCE_PLATFORM_EXTENSIONS_FACEBOOK_JAVA_INTERFACE_
#define _CHILLISOURCE_PLATFORM_EXTENSIONS_FACEBOOK_JAVA_INTERFACE_

#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

namespace CSBackend
{
	namespace Android
	{
		class FacebookAuthenticationSystem;
		class FacebookPostSystem;

		class FacebookJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(FacebookJavaInterface);
			//--------------------------------------------------------------------------------------
			/// Constructor
			///
			/// @author T Kane
			//--------------------------------------------------------------------------------------
			FacebookJavaInterface();
            //--------------------------------------------------------------------------------------
			/// Is A
			///
			/// Used for type-checking.
			///
			/// @param Interface ID to check against
			/// @return true if this class is a class matched by interface ID
			//--------------------------------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType inInterfaceID) const;
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
		    void TryPostRequest(const std::vector<std::string>& inaKeyValues);
            //----------------------------------------------------
            /// Share.
            ///
            /// @author T Kane
            ///
            /// @param Graph path
            /// @param Parameters for post
            //----------------------------------------------------
            void Post(const std::string& in_graphPath, const std::vector<std::string>& in_postParams);
		};
	}
}

#endif

#endif

#endif
