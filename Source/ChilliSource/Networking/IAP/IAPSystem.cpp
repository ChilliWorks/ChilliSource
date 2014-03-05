//
//  IAPSystem.cpp
//  MoFlow
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
        /// Create
        //---------------------------------------------------------------
        IAPSystem* IAPSystem::Create(const Core::ParamDictionary& inParams)
        {
#ifdef CS_TARGETPLATFORM_IOS
        	return new iOS::IAPSystem();
#elif defined(CS_TARGETPLATFORM_ANDROID)
#	ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY
        	return new Android::CGooglePlayIAPSystem(inParams);
#	elif defined(CS_ANDROIDEXTENSION_AMAZON)
        	return new Android::CAmazonIAPSystem(inParams);
#	endif
#endif
        	return nullptr;
        }
        //---------------------------------------------------------------
        /// Is A
        //---------------------------------------------------------------
        bool IAPSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == IAPSystem::InterfaceID;
        }
    }
}

