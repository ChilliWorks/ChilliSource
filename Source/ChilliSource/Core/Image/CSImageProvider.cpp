//
//  CSImageProvider.cpp
//  Chilli Source
//
//  Created by Scott Downie on 10/08/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/CSImageProvider.h>
#include <ChilliSource/Core/Minizip/unzip.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            const std::string k_csImageExtension("moimage");
            
            //------------------------------------------------------
            /// A container for the imformation provided in the
            /// moimage header version 2.
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
            /// moimage header version 3.
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
            bool GetFormatInfo(const u32 in_format, const u32 in_width, const u32 in_height, Image::Format& out_format, u32& out_imageSize)
            {
                switch(in_format)
                {
                    case 1:
                        out_format = Image::Format::k_Lum8;
                        out_imageSize = in_width * in_height * 1;
                        return true;
                    case 2:
                        out_format = Image::Format::k_LumA88;
                        out_imageSize = in_width * in_height * 2;
                        return true;
                    case 3:
                        out_format = Image::Format::k_RGB565;
                        out_imageSize = in_width * in_height * 2;
                        return true;
                    case 4:
                        out_format = Image::Format::k_RGBA4444;
                        out_imageSize = in_width * in_height * 2;
                        return true;
                    case 5:
                        out_format = Image::Format::k_RGB888;
                        out_imageSize = in_width * in_height * 3;
                        return true;
                    case 6:
                        out_format = Image::Format::k_RGBA8888;
                        out_imageSize = in_width * in_height * 4;
                        return true;
                    default:
                    case 0:
                        break;
                }
                return false;
            }
            //-------------------------------------------------------
            /// Reads a version 2 formatted .moimage file
            ///
            /// @author S Downie
            ///
            /// @param Pointer to image data file
            /// @param Pointer to resource destination
            //-------------------------------------------------------
            void ReadFileVersion2(const Core::FileStreamSPtr& in_stream, Core::ResourceSPtr& out_resource)
            {
                //Read the header
                ImageHeaderVersion2 sHeader;
                in_stream->Read((s8*)&sHeader.m_width, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_height, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_imageFormat, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_compression, sizeof(u32));
                in_stream->Read((s8*)&sHeader.m_dataSize, sizeof(u32));
                
                u32 udwSize = 0;
                Core::Image::Format eFormat;
                bool bFoundFormat = GetFormatInfo(sHeader.m_imageFormat, sHeader.m_width, sHeader.m_height, eFormat, udwSize);
                CS_ASSERT(bFoundFormat, "Invalid CSImage Format.");
                
                // Allocated memory needed for the bitmap context
                u8* pubyBitmapData = (u8*) malloc(udwSize);
                in_stream->Read((s8*)pubyBitmapData, udwSize);
                in_stream->Close();
                
                Core::Image* outpImage = (Core::Image*)out_resource.get();
                outpImage->SetFormat(eFormat);
                outpImage->SetData(pubyBitmapData);
                outpImage->SetWidth(sHeader.m_width);
                outpImage->SetHeight(sHeader.m_height);
                outpImage->SetLoaded(true);
            }
            //-------------------------------------------------------
            /// Reads a version 3 formatted .moimage file
            ///
            /// @author S Downie
            ///
            /// @param Pointer to image data file
            /// @param Pointer to resource destination
            //-------------------------------------------------------
            void ReadFileVersion3(const Core::FileStreamSPtr& in_stream, Core::ResourceSPtr& out_resource)
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
                Core::Image::Format eFormat;
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
                    pubyBitmapData = (u8*)malloc(sHeader.m_originalDataSize);
                    
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
                    pubyBitmapData = (u8*) malloc(udwSize);
                    in_stream->Read((s8*)pubyBitmapData, udwSize);
                    in_stream->Close();
                }
                
                Core::Image* outpImage = (Core::Image*)out_resource.get();
                outpImage->SetFormat(eFormat);
                outpImage->SetData(pubyBitmapData);
                outpImage->SetWidth(sHeader.m_width);
                outpImage->SetHeight(sHeader.m_height);
                outpImage->SetLoaded(true);
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
        bool CSImageProvider::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CSImageProvider::InterfaceID);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool CSImageProvider::CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == Image::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool CSImageProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
        {
            return (in_extension == k_csImageExtension);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool CSImageProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filepath, Core::ResourceSPtr& out_resource)
        {
            //ensure the extension is correct.
            if (ChilliSource::Core::StringUtils::EndsWith(in_filepath, k_csImageExtension, true) == false)
                return false;
            
            Core::FileStreamSPtr pImageFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filepath, Core::FileMode::k_readBinary);
            
            if(pImageFile && !pImageFile->IsBad())
            {
                //Read the byte order mark and ensure it is 123456
                u32 udwByteOrder = 0;
                pImageFile->Read((s8*)&udwByteOrder, sizeof(u32));
                CS_ASSERT(udwByteOrder == 123456, "Endianess not supported");
                
                //Read the version
                u32 udwVersion = 0;
                pImageFile->Read((s8*)&udwVersion, sizeof(u32));
                CS_ASSERT(udwVersion >= 2 && udwVersion <= 3, "Only versions 2 and 3 supported");
                
                if(3 == udwVersion)
                {
                    ReadFileVersion3(pImageFile, out_resource);
                }
                else if (2 == udwVersion)
                {
                    CS_LOG_WARNING("File \"" + in_filepath + "\" moimage version 2 is deprecated. Please use version 3 which is supported from revision 86 in the tool repository.");
                    ReadFileVersion2(pImageFile, out_resource);
                }
                
                return true;
            }
            
            return false;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool CSImageProvider::AsyncCreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceSPtr& out_resource)
        {
            CS_LOG_WARNING("Async load is not implemented in CSImage Provider.");
            return false;
        }
    }
}
