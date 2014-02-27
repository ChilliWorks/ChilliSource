//
//  ETC1ImageProvider.h
//  moFlow
//
//  Created by Ian Copland on 09/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_CORE_ETC1IMAGEPROVIDER_H_
#define _MOFLOW_CORE_ETC1IMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Image/ImageResourceProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        //====================================================================
        /// ETC1 Image Provider
        ///
        /// An image provider for the ETC1 format. The ETC1 format is not
        /// available on all platforms and you should be sure it is supported
        /// if using it.
        //====================================================================
        class ETC1ImageProvider : public Core::ImageResourceProvider
        {
        public:
            //----------------------------------------------------------------
            /// Is A
            ///
            /// Is the object of the given interface type
            /// @param Interface type to query
            /// @return Whether the object is of given type
            //----------------------------------------------------------------
            bool IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const override;
            //----------------------------------------------------------------
            /// Can Create Resource Of Kind
            ///
            /// @param Resource to compare against
            /// @return Whether the object can load a resource of given type
            //----------------------------------------------------------------
            bool CanCreateResourceOfKind(ChilliSource::Core::InterfaceIDType inInterfaceID) const override;
            //----------------------------------------------------------------
            /// Can Create Resource From File With Extension
            ///
            /// @param Extension to compare against
            /// @return Whether the object can load a resource with that extension
            //----------------------------------------------------------------
            bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const override;
            //----------------------------------------------------------------
            /// Create Resource From File
            ///
            /// @param The storage location to load from
            /// @param File path to resource
            /// @param Out: Resource
            /// @return Whether the resource loaded
            //----------------------------------------------------------------
            bool CreateResourceFromFile(ChilliSource::Core::StorageLocation ineStorageLocation, const std::string & inFilePath, ChilliSource::Core::ResourceSPtr& outpResource) override;
            //----------------------------------------------------------------
            /// Create Image From File
            ///
            /// @param The storage location to load from
            /// @param File path to resource
            /// @param Image format
            /// @param Out: Resource
            /// @return Whether the resource loaded
            //----------------------------------------------------------------
            bool CreateImageFromFile(ChilliSource::Core::StorageLocation ineStorageLocation, const std::string & inFilePath, ChilliSource::Core::Image::Format ineFormat, ChilliSource::Core::ResourceSPtr& outpResource);
        private:
            struct ETC1Header
            {
                u8 abyPKMTag[6];
                u16 wNumberOfMipmaps;
                u16 wTextureWidth;
                u16 wTextureHeight;
                u16 wOriginalWidth;
                u16 wOriginalHeight;
            };
        };
    }
}

#endif
