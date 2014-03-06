/*
 *  ImageResourceProvider.cpp
 *  moFlow
 *
 *  Created by Scott Downie on 18/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Image/ImageResourceProvider.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Core/Image/ImageLoader.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Core/Image/ImageLoader.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <ChilliSource/Backend/Platform/Windows/Core/Image/ImageLoader.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        Core::Image::Format ImageResourceProvider::meDefaultFormat = Core::Image::Format::k_RGBA8888;
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        ImageResourceProviderUPtr ImageResourceProvider::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return ImageResourceProviderUPtr(new iOS::ImageLoader());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return ImageResourceProviderUPtr(new Android::ImageLoader());
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
            return ImageResourceProviderUPtr(new Windows::ImageLoader());
#endif
            return nullptr;
        }
        
        //--------------------------------------------------
        /// Set Default Format
        ///
        /// @param Image format that will be used in lieu
        /// of specific
        //--------------------------------------------------
        void ImageResourceProvider::SetDefaultFormat(Core::Image::Format ineFormat)
        {
            meDefaultFormat = ineFormat;
        }
    }
}
