//
//  IAPSystem.cpp
//  MoFlow
//
//  Created by Scott Downie on 10/06/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Networking/IAP/IAPSystem.h>

#ifdef TARGET_OS_IPHONE
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
        DEFINE_NAMED_INTERFACE(IIAPSystem);
        //---------------------------------------------------------------
        /// Create
        //---------------------------------------------------------------
        IIAPSystem* IIAPSystem::Create(const Core::ParamDictionary& inParams)
        {
#ifdef TARGET_OS_IPHONE
        	return new iOSPlatform::CIAPSystem();
#elif defined(TARGET_ANDROID)
#	ifdef MOFLOWSKU_IAPMETHOD_GOOGLEPLAY
        	return new AndroidPlatform::CGooglePlayIAPSystem(inParams);
#	elif defined(MOFLOWSKU_IAPMETHOD_AMAZON)
        	return new AndroidPlatform::CAmazonIAPSystem(inParams);
#	endif
#endif
        	return NULL;
        }
        //---------------------------------------------------------------
        /// Is A
        //---------------------------------------------------------------
        bool IIAPSystem::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == IIAPSystem::InterfaceID;
        }
    }
}

