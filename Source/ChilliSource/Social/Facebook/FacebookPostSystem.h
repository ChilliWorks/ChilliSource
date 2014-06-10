//
//  FacebookPostSystem.h
//  Chilli Source
//
//  Created by Stuart McGaw on 06/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_SOCIAL_FACEBOOK_FACEBOOKPOSTSYSTEM_H_
#define _CHILLISOURCE_SOCIAL_FACEBOOK_FACEBOOKPOSTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Delegate/ConnectableDelegate.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Social/ForwardDeclarations.h>

#include <json/json.h>

#include <functional>

namespace ChilliSource
{
	namespace Social
	{
        //----------------------------------------------------
        /// Base class for posting messages and request
        /// to the Facebook wall. The implementation is handled
        /// by the platform Facebook SDK.
        ///
        /// User must be authenticated prior to posting
        ///
        /// @author S McGaw
        //----------------------------------------------------
		class FacebookPostSystem : public Core::AppSystem
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(FacebookPostSystem);
            
            //-------------------------------------------------
            /// @author S McGaw
            ///
            /// Hold the description of a post to the Facebook
            /// wall feed. Including the recipient and the text
            //-------------------------------------------------
            struct PostDesc
            {
                std::string m_to; //ID of the page this post is for (Friend ID perhaps), if blank will default to the user's page
                
                std::string m_name;
                std::string m_url;
                
                std::string m_caption;
                std::string m_description;
                
                std::string m_picUrl; //URL of an image to accompany this. If this isn't set Facebook will scrape the URL page to find a picture.
            };
            
            enum class RequestRecipientMode
            {
                k_optional,
                k_fixed
            };
            //-------------------------------------------------
            /// @author S McGaw
            ///
            /// Hold the description of a request to Facebook
            /// users. Including the recipients and the text
            //-------------------------------------------------
            struct RequestDesc
            {
                std::vector<std::string> m_recipients;
                std::string m_caption;
                std::string m_description;
                RequestRecipientMode m_type;
            };
			
			enum class PostResult
			{
                k_success,
                k_cancelled,
                k_failed
			};
            //-------------------------------------------------
            /// Delegate that is fired for any facebook post
            /// request
            ///
            /// @author S McGaw
            ///
            /// @param Result - Success, failure, etc.
            //-------------------------------------------------
            using PostResultDelegate = Core::ConnectableDelegate<void(const PostResult&)>;
            
            //---------------------------------------------------
            /// Creates a new instance of the facebook post system.
            ///
            /// @author I Copland
            ///
            /// @param The facebook authentication system.
            ///
            /// @return The new instance of the system.
            //---------------------------------------------------
            static FacebookPostSystemUPtr Create(FacebookAuthenticationSystem* inpAuthSystem);
            //---------------------------------------------------
            /// Post to the users wall using the backend Facebook
            /// SDK. If the post description has no recipient
            /// then it posts to the authenticated users wall.
            ///
            /// User must be authenticated before posting and
            /// must have granted publish permissions "publish_actions"
            /// and "publish_stream"
            ///
            /// @author S McGaw
            ///
            /// @param Post description containing recipient
            /// and post text.
            /// @param Connection to result delegate
            //---------------------------------------------------
			virtual void Post(const PostDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection) = 0;
            //---------------------------------------------------
            /// Send a request to a group of friends using the
            /// backend Facebook SDK.
            ///
            /// User must be authenticated and must have granted
            /// publish permissions
            ///
            /// @author A Mackie
            ///
            /// @param Request description
            /// @param Connection to result delegate
            //---------------------------------------------------
            virtual void SendRequest(const RequestDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection) = 0;
            //---------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //---------------------------------------------------
            virtual ~FacebookPostSystem(){};
		};
	}
}

#endif
