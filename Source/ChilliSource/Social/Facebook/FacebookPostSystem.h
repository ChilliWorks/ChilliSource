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
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Social/ForwardDeclarations.h>

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
		class FacebookPostSystem : public ChilliSource::Core::AppSystem
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(FacebookPostSystem);
            
            struct PostDesc
            {
                std::string m_to; //ID of the page this post is for (Friend ID perhaps), if blank will default to the user's page
                
                std::string m_name; //Name in bold of this linky thing
                std::string m_url; //This must be set for your post to show
                
                std::string m_caption; //Subline under name
                std::string m_description; //Body of the post
                
                std::string m_picUrl; //URL of an image to accompany this. If this isn't set Facebook will scrape the URL page to find a picture.
            };
            
            enum class RequestType
            {
                k_suggested, //List of friends in the request are merely suggested
                k_to //List of friends in the request are targetted
            };
            
            struct RequestDesc
            {
                std::string m_caption; //Subline under name
                std::string m_description; //Body of the post
            };
			
			enum class PostResult
			{
                k_success,
                k_cancelled,
                k_failed
			};
			
			typedef std::function<void(const PostResult&)> PostResultDelegate;
            
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
            /// @param Result delegate
            //---------------------------------------------------
			virtual void Post(const PostDesc& in_desc, const PostResultDelegate& in_delegate) = 0;
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
            /// @param List of friends to target
            /// @param Type of request (suggested or targetted)
            /// @param Result delegate
            //---------------------------------------------------
            virtual void SendRequest(const RequestDesc& in_desc, std::vector<std::string>& in_friendIDs, RequestType in_type, const PostResultDelegate& in_delegate) = 0;
            //---------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //---------------------------------------------------
            virtual ~FacebookPostSystem(){};
        protected:
            //---------------------------------------------------
            /// Protected constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //---------------------------------------------------
            FacebookPostSystem(){};
		};
	}
}

#endif
