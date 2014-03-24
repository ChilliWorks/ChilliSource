/*
 *  ImageProvider.cpp
 *  moFlow
 *
 *  Created by Scott Downie on 18/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Image/ImageProvider.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Core/Image/ImageProvider.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Core/Image/ImageProvider.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <ChilliSource/Backend/Platform/Windows/Core/Image/ImageProvider.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------
        //-------------------------------------------------------
        ImageProviderUPtr ImageProvider::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return ImageProviderUPtr(new iOS::ImageProvider());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return ImageProviderUPtr(new Android::ImageProvider());
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
            return ImageProviderUPtr(new Windows::ImageProvider());
#endif
        }
    }
}
