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
#elif defined(TARGET_ANDROID)
#	ifdef MOFLOWSKU_IAPMETHOD_GOOGLEPLAY
#		include <ChilliSource/Backend/Platform/Android/GooglePlay/GooglePlayIAPSystem.h>
#	elif defined(MOFLOWSKU_IAPMETHOD_AMAZON)
#		include <ChilliSource/Backend/Platform/Android/Amazon/AmazonIAPSystem.h>
#	endif
#endif

namespace ChilliSource
{
	namespace Networking
    {
        DEFINE_NAMED_INTERFACE(IAPSystem);
        //---------------------------------------------------------------
        /// Create
        //---------------------------------------------------------------
        IAPSystem* IAPSystem::Create(const Core::ParamDictionary& inParams)
        {
#ifdef CS_TARGETPLATFORM_IOS
        	return new iOS::CIAPSystem();
#elif defined(TARGET_ANDROID)
#	ifdef MOFLOWSKU_IAPMETHOD_GOOGLEPLAY
        	return new Android::CGooglePlayIAPSystem(inParams);
#	elif defined(MOFLOWSKU_IAPMETHOD_AMAZON)
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

