//
//  CSImageProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 10/08/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Core/Image/CSImageProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <minizip/unzip.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            const std::string k_csImageExtension("csimage");
            
            //------------------------------------------------------
            /// A container for the imformation provided in the
            /// csimage header version 2.
            ///
            /// @author S Downie
            //------------------------------------------------------
            struct ImageHeaderVersion2
            {
                u32 m_width;
                u32 m_height;
                u32 m_imageFormat;
                u32 m_compression;
                u32 m_dataSize;
            };
            //------------------------------------------------------
            /// A container for the imformation provided in the
            /// csimage header version 3.
            ///
            /// @author S Downie
            //------------------------------------------------------
            struct ImageHeaderVersion3
            {
                u32 m_width;
                u32 m_height;
                u32 m_imageFormat;
                u32 m_compression;
                u64 m_checksum;
                u32 m_originalDataSize;
                u32 m_compressedDataSize;
            };
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The integer image format
            /// @param The image width
            /// @param The image height
            /// @param [Out] The image format
            /// @param [Out] The image size
            ///
            /// @return Whether the format was found
            //-------------------------------------------------------
            bool GetFormatInfo(const u32 in_format, const u32 in_width, const u32 in_height, ImageFormat& out_format, u32& out_imageSize)
            {
                switch(in_format)
                {
                    case 1:
                        out_format = ImageFormat::k_Lum8;
                        out_imageSize = in_width * in_height * 1;
                        return true;
                    case 2:
                        out_format = ImageFormat::k_LumA88;
                        out_imageSize = in_width * in_height * 2;
                        return true;
                    case 3:
                        out_format = ImageFormat::k_RGB565;
                        out_imageSize = in_width * in_height * 2;
                        return true;
                    case 4:
                        out_format = ImageFormat::k_RGBA4444;
                        out_imageSize = in_width * in_height * 2;
                        return true;
                    case 5:
                        out_format = ImageFormat::k_RGB888;
                        out_imageSize = in_width * in_height * 3;
                        return true;
                    case 6:
                        out_format = ImageFormat::k_RGBA8888;
                        out_imageSize = in_width * in_height * 4;
                        return true;
                    default:
                    case 0:
                        break;
                }
                return false;
            }
            //-------------------------------------------------------
            /// Reads a version 3 formatted .csimage file
            ///
            /// @author S Downie
            ///
            /// @param Pointer to image data file
            /// @param Pointer to resource destination
            //-------------------------------------------------------
			void ReadFileVersion3(const FileStreamSPtr& in_stream, const ResourceSPtr& out_resource)
            {
                //Read the header
                ImageHeaderVersion3 sHeader;
                in_stream->Read((s8*)&sHeader.m_width, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_height, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_imageFormat, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_compression, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_checksum, sizeof(u64));
                in_stream->Read((s8*)&sHeader.m_originalDataSize, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_compressedDataSize, sizeof(u32));
                
                u32 udwSize = 0;
                ImageFormat eFormat = ImageFormat::k_RGBA8888;
                bool bFoundFormat = GetFormatInfo(sHeader.m_imageFormat, sHeader.m_width, sHeader.m_height, eFormat, udwSize);
                CS_ASSERT(bFoundFormat, "Invalid CSImage Format.");
                
                u8* pubyBitmapData = nullptr;
                if(sHeader.m_compression != 0)
                {
                    // Allocated memory needed for the compressed image data
                    u8* pubyCompressedData = (u8*)malloc(sHeader.m_compressedDataSize);
                    in_stream->Read((s8*)pubyCompressedData, sHeader.m_compressedDataSize);
                    in_stream->Close();
                    
                    // Allocated memory need for for the bitmap context
                    pubyBitmapData = new u8[sHeader.m_originalDataSize];
                    
                    // Inflate data (I like to think this is the machine equvilent to eating lots of pizza!)
                    z_stream infstream;
                    infstream.zalloc = Z_NULL;
                    infstream.zfree = Z_NULL;
                    infstream.opaque = Z_NULL;
                    infstream.avail_in = sHeader.m_compressedDataSize;		// size of input
                    infstream.next_in = (Bytef*)pubyCompressedData;			// input data
                    infstream.avail_out = sHeader.m_originalDataSize;		// size of output
                    infstream.next_out = (Bytef*)pubyBitmapData;			// output char array
                    
                    inflateInit(&infstream);
                    inflate(&infstream, Z_FINISH);
                    inflateEnd(&infstream);
                    
                    // Checksum test
                    u32 udwInflatedChecksum = HashCRC32::GenerateHashCode((const s8*)pubyBitmapData, sHeader.m_originalDataSize);
                    if(sHeader.m_checksum != (u64)udwInflatedChecksum)
                    {
                        CS_LOG_ERROR("CSImage checksum of "+ToString(udwInflatedChecksum)+" does not match expected checksum "+ToString(sHeader.m_checksum));
                    }
                    
                    free(pubyCompressedData);
                }
                else
                {
                    // Allocated memory needed for the bitmap context
                    pubyBitmapData = new u8[sHeader.m_originalDataSize];;
                    in_stream->Read((s8*)pubyBitmapData, udwSize);
                    in_stream->Close();
                }
                
                Image::ImageDataUPtr imageData(pubyBitmapData);
                
                Image::Descriptor desc;
                desc.m_format = eFormat;
                desc.m_compression = ImageCompression::k_none;
                desc.m_width = sHeader.m_width;
                desc.m_height = sHeader.m_height;
                desc.m_dataSize = udwSize;
                
                Image* outpImage = (Image*)out_resource.get();
                outpImage->Build(desc, std::move(imageData));
            }
            //----------------------------------------------------
            /// Performs the heavy lifting for the 2 create methods
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param Completion delegate
            /// @param [Out] The output resource.
            //----------------------------------------------------
			void LoadImage(StorageLocation in_storageLocation, const std::string& in_filepath, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
            {
                FileStreamSPtr pImageFile = Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filepath, FileMode::k_readBinary);
                
                if(pImageFile == nullptr || pImageFile->IsBad() == true)
                {
                    out_resource->SetLoadState(Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
						Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                    }
                    return;
                }
                
                //Read the byte order mark and ensure it is 123456
                u32 udwByteOrder = 0;
                pImageFile->Read((s8*)&udwByteOrder, sizeof(u32));
                CS_ASSERT(udwByteOrder == 123456, "Endianess not supported");
                
                //Read the version
                u32 udwVersion = 0;
                pImageFile->Read((s8*)&udwVersion, sizeof(u32));
                CS_ASSERT(udwVersion >= 3, "Only version 3 and above supported");
   
                ReadFileVersion3(pImageFile, out_resource);
                
                out_resource->SetLoadState(Resource::LoadState::k_loaded);
                if(in_delegate != nullptr)
                {
					auto task = std::bind(in_delegate, out_resource);
					Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
                }
            }
        }
        
        CS_DEFINE_NAMEDTYPE(CSImageProvider);
        //-------------------------------------------------------
        //-------------------------------------------------------
        CSImageProviderUPtr CSImageProvider::Create()
        {
            return CSImageProviderUPtr(new CSImageProvider());
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool CSImageProvider::IsA(InterfaceIDType in_interfaceId) const
        {
            return (in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CSImageProvider::InterfaceID);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        InterfaceIDType CSImageProvider::GetResourceType() const
        {
            return Image::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool CSImageProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
        {
            return (in_extension == k_csImageExtension);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
		void CSImageProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filepath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
        {
            LoadImage(in_storageLocation, in_filepath, nullptr, out_resource);
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void CSImageProvider::CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filepath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
        {
			auto task = std::bind(LoadImage, in_storageLocation, in_filepath, in_delegate, out_resource);
            Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
    }
}
