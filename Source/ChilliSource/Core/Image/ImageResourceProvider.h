/*
 *  ImageResourceProvider.h
 *  iOSTemplate
 *
 *  Created by Scott Downie on 01/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_IMAGE_RESOURCE_PROVIDER_H_
#define _MO_FLO_CORE_IMAGE_RESOURCE_PROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
    namespace Core
    {
        class ImageResourceProvider : public Core::ResourceProvider
        {
        public:

            //----------------------------------------------------------------
            /// Create Image From File
            ///
            /// @param The storage location to load from
            /// @param File path to resource
            /// @return Instantiated resource
            //----------------------------------------------------------------
            virtual bool CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::ResourceSPtr& outpResource) = 0;
            //--------------------------------------------------
            /// Set Default Format
            ///
            /// @param Image format that will be used in lieu
            /// of specific
            //--------------------------------------------------
            static void SetDefaultFormat(Core::Image::Format ineFormat);
            
        protected:
            
            static Core::Image::Format meDefaultFormat;
        };
    }
}

#endif