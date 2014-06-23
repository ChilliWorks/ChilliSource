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
        /// User must be authenticated prior to posting
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
            /// Post the wall of the user specified in the post
            /// description. If no user is specified then post
            /// to the wall of the currently authenticated user.
            ///
            /// User must be authenticated
            ///
            /// Note: For the time being only one post can
            /// happen at a time
            ///
            /// @author S McGaw
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
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            ///
            /// @param The facebook authentication system.
            //----------------------------------------------------
            FacebookPostSystem(CSSocial::FacebookAuthenticationSystem* in_authSystem);
            //----------------------------------------------------
            /// Post to wall using the native iOS view
            ///
            /// @param Post description
            //----------------------------------------------------
            void PostNative(const PostDesc& in_desc);
            //----------------------------------------------------
            /// Post to wall using the native web view
            ///
            /// @param Post description
            //----------------------------------------------------
            void PostWeb(const PostDesc& in_desc);
			
            friend CSSocial::FacebookPostSystemUPtr CSSocial::FacebookPostSystem::Create(CSSocial::FacebookAuthenticationSystem* in_authSystem);
            
        private:
            
            CSSocial::FacebookAuthenticationSystem* m_authSystem;
            PostResultDelegate::Connection m_postCompleteDelegateConnection;
            PostResultDelegate::Connection m_requestCompleteDelegateConnection;
		};
	}
}

#endif

#endif