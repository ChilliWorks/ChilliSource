//
//  FacebookPostSystem.h
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

#ifndef _CSBACKEND_PLATFORM_IOS_SOCIAL_FACEBOOK_FACEBOOKPOSTSYSTEM_H_
#define _CSBACKEND_PLATFORM_IOS_SOCIAL_FACEBOOK_FACEBOOKPOSTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Social/Facebook/FacebookPostSystem.h>

namespace CSBackend
{
	namespace iOS
	{
        //----------------------------------------------------
        /// Class for posting messages and request
        /// to the Facebook wall via the iOS Facebook framework.
        ///
        /// @author S McGaw
        //----------------------------------------------------
		class FacebookPostSystem final : public CSSocial::FacebookPostSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(FacebookPostSystem);
			//----------------------------------------------------
            /// @author S McGaw
            ///
            /// @param Interface ID to compare
            ///
            /// @return Whether the object has the same interface ID
            //----------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------
            /// Share from the app to appear in the user's timeline and in their friends' News Feeds.
            ///
            /// Note: For the time being only one post can
            /// happen at a time
            ///
            /// @author T Kane
            ///
            /// @param Post description
            /// @param Connection to result delegate
            //----------------------------------------------------
            void Post(const PostDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection) override;
            //---------------------------------------------------
            /// Send a request to a group of friends using the
            /// iOS Facebook framework
            ///
            /// User must be authenticated
            ///
            /// Note: For the time being only one request can
            /// happen at a time
            ///
            /// @author A Mackie
            ///
            /// @param Request description
            /// @param Connection to result delegate
            //---------------------------------------------------
            void SendRequest(const RequestDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection) override;

        private:
            friend CSSocial::FacebookPostSystemUPtr CSSocial::FacebookPostSystem::Create();
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            FacebookPostSystem() = default;
            //----------------------------------------------------
            /// Initialisation method called at a time when
            /// all App Systems have been created. System
            /// initialisation occurs in the order they were
            /// created.
            ///
            /// @author T Kane
            //----------------------------------------------------
            void OnInit() override;
            //----------------------------------------------------
            /// Share via the web-based Feed Dialog. Note that this
            /// does not support publishing Open Graph stories.
            ///
            /// @author T Kane
            ///
            /// @param Post description
            //----------------------------------------------------
            void PostWeb(const PostDesc& in_desc);
            //----------------------------------------------------
            /// Attempt to share story via the Facebook app Share Dialog
            ///
            /// @author T Kane
            ///
            /// @param Post description
            /// @return True if Facebook app is installed and sharing
            ///         can be done via this method. False if not.
            //----------------------------------------------------
            bool PostViaShareDialog(const PostDesc& in_desc);
            //----------------------------------------------------
            /// Determines if we are signed in to Facebook or not.
            ///
            /// @author T Kane
            ///
            /// @return True if signed in. False otherwise.
            //----------------------------------------------------
            bool IsSignedIn() const;
            
        private:
            CSSocial::FacebookAuthenticationSystem* m_authSystem = nullptr;
            PostResultDelegate::Connection m_postCompleteDelegateConnection;
            PostResultDelegate::Connection m_requestCompleteDelegateConnection;
		};
	}
}

#endif

#endif