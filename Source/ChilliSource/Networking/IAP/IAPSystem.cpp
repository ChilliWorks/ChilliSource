//
//  IAPSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 10/06/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Networking/IAP/IAPSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#	include <ChilliSource/Backend/Platform/iOS/Networking/IAP/IAPSystem.h>
#elif defined(CS_TARGETPLATFORM_ANDROID)
#	ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY
#		include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayIAPSystem.h>
#	elif defined(CS_ANDROIDEXTENSION_AMAZON)
#		include <ChilliSource/Backend/Platform/Android/Extensions/Amazon/AmazonIAPSystem.h>
#	endif
#endif

namespace ChilliSource
{
	namespace Networking
    {
        CS_DEFINE_NAMEDTYPE(IAPSystem);
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        IAPSystemUPtr IAPSystem::Create(const Core::ParamDictionary& inParams)
        {
#ifdef CS_TARGETPLATFORM_IOS
        	return IAPSystemUPtr(new iOS::IAPSystem());
#elif defined(CS_TARGETPLATFORM_ANDROID)
#	ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY
        	return IAPSystemUPtr(new Android::GooglePlayIAPSystem(inParams));
#	elif defined(CS_ANDROIDEXTENSION_AMAZON)
        	return IAPSystemUPtr(new Android::AmazonIAPSystem(inParams));
#	endif
#endif
        	return nullptr;
        }
    }
}

