/*
 *  FacebookPostSystem.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 06/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Social/Facebook/FacebookPostSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Social/Facebook/FacebookPostSystem.h>
#elif CS_TARGETPLATFORM_ANDROID
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
		CS_DEFINE_NAMEDTYPE(FacebookPostSystem);
        //---------------------------------------------------
        //---------------------------------------------------
        FacebookPostSystemUPtr FacebookPostSystem::Create(FacebookAuthenticationSystem* inpAuthSystem)
        {
#ifdef CS_TARGETPLATFORM_IOS
            return FacebookPostSystemUPtr(new iOS::FacebookPostSystem(inpAuthSystem));
#elif CS_TARGETPLATFORM_ANDROID
            return FacebookPostSystemUPtr(new Android::FacebookPostSystem(inpAuthSystem));
#else
			return FacebookPostSystemUPtr();
#endif
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