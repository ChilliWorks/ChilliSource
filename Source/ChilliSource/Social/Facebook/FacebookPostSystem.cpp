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
#include <ChilliSource/Platform/iOS/Social/Facebook/FacebookPostSystem.h>
#elif TARGET_ANDROID
#include <ChilliSource/Platform/Android/Social/Facebook/FacebookPostSystem.h>
#endif

/*
 Facebook documentation claims that the actions property should be a JSON array of JSON objects contain names and links
 but actually it only accepts one object containing a name and link. Damn you Zuckerberg (personally)
 */
#define FACEBOOK_API_NOW_ACTUALLY_DOES_WHAT_IT_SAYS_WITH_ACTIONLINKS 0

namespace moFlo
{
	namespace Social
	{
		DEFINE_NAMED_INTERFACE(IFacebookPostSystem);
        
        IFacebookPostSystem* IFacebookPostSystem::CreatePostSystem(moFlo::Social::IFacebookAuthenticationSystem* inpAuthSystem)
        {
#ifdef TARGET_OS_IPHONE
            return new moFlo::iOSPlatform::CFacebookPostSystem(inpAuthSystem);
#elif TARGET_ANDROID
            return new moFlo::AndroidPlatform::CFacebookPostSystem(inpAuthSystem);
#endif
			return NULL;
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