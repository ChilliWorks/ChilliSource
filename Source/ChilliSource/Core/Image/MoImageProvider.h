//
//  MoImageProvider.h
//  MoFlow
//
//  Created by Scott Downie on 10/08/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_CORE_MOIMAGEPROVIDER_H_
#define _MOFLOW_CORE_MOIMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Image/ImageResourceProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        class MoImageProvider : public ImageResourceProvider
        {
        public:
            //----------------------------------------------------------------
            /// Is A
            ///
            /// Is the object of the given interface type
            /// @param Interface type to query
            /// @return Whether the object is of given type
            //----------------------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //----------------------------------------------------------------
            /// Can Create Resource Of Kind
            ///
            /// @param Resource to compare against
            /// @return Whether the object can load a resource of given type
            //----------------------------------------------------------------
            bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const override;
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
            bool CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource) override;
            //----------------------------------------------------------------
            /// Create Image From File
            ///
            /// @param The storage location to load from
            /// @param File path to resource
            /// @param Image format
            /// @param Out: Resource
            /// @return Whether the resource loaded
            //----------------------------------------------------------------
            bool CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::ResourceSPtr& outpResource);
        private:
            struct ImageHeaderVersion2
            {
                u32 udwWidth;
                u32 udwHeight;
                u32 udwImageFormat;
                u32 udwCompression;
                u32 udwDataSize;
            };
            struct ImageHeaderVersion3
            {
                u32 udwWidth;
                u32 udwHeight;
                u32 udwImageFormat;
                u32 udwCompression;
                u64 uddwChecksum;   // CRC32
                u32 udwOriginalDataSize;
                u32 udwCompressedDataSize;
            };
            //----------------------------------------------------------------
            /// Reads a version 2 formatted .moimage file
            ///
            /// @param Pointer to image data file
            /// @param Pointer to resource destination
            //----------------------------------------------------------------
            void ReadFileVersion2(Core::FileStreamSPtr inpImageFile, Core::ResourceSPtr& outpResource);
            //----------------------------------------------------------------
            /// Reads a version 3 formatted .moimage file
            ///
            /// @param Pointer to image data file
            /// @param Pointer to resource destination
            //----------------------------------------------------------------
            void ReadFileVersion3(Core::FileStreamSPtr inpImageFile, Core::ResourceSPtr& outpResource);
            //----------------------------------------------------------------
            /// Get Format Info
            ///
            /// @param The integer image format
            /// @param The image width
            /// @param The image height
            /// @param Out: The image format
            /// @param Out: The image size
            /// @return Whether the format was found
            //----------------------------------------------------------------
            bool GetFormatInfo(const u32 inudwFormat, const u32 inudwWidth, const u32 inudwHeight,
                               Core::Image::Format& outFormat, u32& outudwImageSize);
        };
    }
}

#endif

