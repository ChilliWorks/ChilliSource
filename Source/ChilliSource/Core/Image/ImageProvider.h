//
//  ImageProvider.h
//  Chilli Source
//
//  Created by Scott Downie on 01/08/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_IMAGE_IMAGEPROVIDER_H_
#define _CHILLISOURCE_CORE_IMAGE_IMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------------------------
        /// An image loader that providers PNG resources. On iOS it also
        /// provides JPEG and PVR images.
        ///
        /// @author S Downie
        //---------------------------------------------------------------
        class ImageProvider : public Core::ResourceProvider
        {
        public:
            //-------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //-------------------------------------------------------
            static ImageProviderUPtr Create();
        protected:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            ImageProvider(){}
        };
    }
}

#endif