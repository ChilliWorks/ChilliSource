/*
 *  FacebookPostSystem.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 06/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Social/Facebook/FacebookPostSystem.h>

#ifdef TARGET_OS_IPHONE
#include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookPostSystem.h>
#elif TARGET_ANDROID
#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookPostSystem.h>
#endif

/*
 Facebook documentation claims that the actions property should be a JSON array of JSON objects contain names and links
 but actually it only accepts one object containing a name and link. Damn you Zuckerberg (personally)
 */
#define FACEBOOK_API_NOW_ACTUALLY_DOES_WHAT_IT_SAYS_WITH_ACTIONLINKS 0

namespace ChilliSource
{
	namespace Social
	{
		DEFINE_NAMED_INTERFACE(IFacebookPostSystem);
        
        IFacebookPostSystem* IFacebookPostSystem::CreatePostSystem(ChilliSource::Social::IFacebookAuthenticationSystem* inpAuthSystem)
        {
#ifdef TARGET_OS_IPHONE
            return new ChilliSource::iOS::CFacebookPostSystem(inpAuthSystem);
#elif TARGET_ANDROID
            return new ChilliSource::Android::CFacebookPostSystem(inpAuthSystem);
#endif
			return nullptr;
        }
        
		void FacebookPostDesc::ToJSON(Json::Value & outsRoot) const
		{
			using namespace Json;
			
			if (!strTo.empty())
			{
				outsRoot["to"] = strTo;
			}
			
			outsRoot["name"] = Value(strName);
			outsRoot["caption"] = Value(strCaption);
			outsRoot["description"] = Value(strDescription);
			outsRoot["link"] = Value(strURL);
			
			if (!strPictureURL.empty())
			{
				outsRoot["picture"] = Value(strPictureURL);
			}
		}
	};
};