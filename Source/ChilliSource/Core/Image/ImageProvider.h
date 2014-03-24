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
#include <ChilliSource/Core/Image/Image.h>

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
            //-------------------------------------------------------
            /// Creates a new image from file and converts to the
            /// given image format.
            ///
            /// @author S Downie
            ///
            /// @param The storage location to load from.
            /// @param File path to resource.
            /// @param The format to convert to.
            /// @param [Out] The output resource.
            ///
            /// @return Whether or not the image was successfully
            /// creates.
            //-------------------------------------------------------
            virtual bool CreateImageFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filePath, Core::Image::Format in_format, Core::ResourceSPtr& out_resource) = 0;
            //--------------------------------------------------
            /// @author S Downie
            /// @param Image format that will be used in lieu
            /// of specific
            //--------------------------------------------------
            static void SetDefaultFormat(Core::Image::Format in_format);
            
        protected:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            ImageProvider(){}
            
        protected:
            static Core::Image::Format s_defaultFormat;
        };
    }
}

#endif