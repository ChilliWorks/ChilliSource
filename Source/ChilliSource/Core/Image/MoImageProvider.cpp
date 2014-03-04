//
//  MoImageProvider.cpp
//  MoFlow
//
//  Created by Scott Downie on 10/08/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/Minizip/unzip.h>
#include <ChilliSource/Core/Image/MoImageProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            const std::string MoImageExtension("moimage");
        }
        //----------------------------------------------------------------
        /// Is A
        //----------------------------------------------------------------
        bool MoImageProvider::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == ResourceProvider::InterfaceID;
        }
        //----------------------------------------------------------------
        /// Can Create Resource Of Kind
        //----------------------------------------------------------------
        bool MoImageProvider::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == Core::Image::InterfaceID;
        }
        //----------------------------------------------------------------
        /// Can Create Resource From File With Extension
        //----------------------------------------------------------------
        bool MoImageProvider::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
        {
            return (inExtension == MoImageExtension);
        }
        //----------------------------------------------------------------
        /// Create Resource From File
        //----------------------------------------------------------------
        bool MoImageProvider::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)
        {
            return CreateImageFromFile(ineStorageLocation, inFilePath, Core::Image::Format::k_default, outpResource);
        }
        //----------------------------------------------------------------
        /// Create Image From File
        //----------------------------------------------------------------
        bool MoImageProvider::CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::ResourceSPtr& outpResource)
        {
            //ensure the extension is correct.
            if (ChilliSource::Core::StringUtils::EndsWith(inFilePath, MoImageExtension, true) == false)
                return false;

            Core::FileStreamSPtr pImageFile = Core::Application::Get()->GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);

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
                    ReadFileVersion3(pImageFile, outpResource);
                }
                else
                if(2 == udwVersion)
                {
                    CS_LOG_WARNING("File \""+inFilePath+"\" moimage version 2 is deprecated. Please use version 3 which is supported from revision 86 in the tool repository.");
                    ReadFileVersion2(pImageFile, outpResource);
                }
                
                return true;
            }
            
            return false;
        }
        //----------------------------------------------------------------
        /// Reads a version 2 formatted .moimage file
        //----------------------------------------------------------------
        void MoImageProvider::ReadFileVersion2(Core::FileStreamSPtr inpImageFile, Core::ResourceSPtr& outpResource)
        {
            //Read the header
            ImageHeaderVersion2 sHeader;
            inpImageFile->Read((s8*)&sHeader.udwWidth, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.udwHeight, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.udwImageFormat, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.udwCompression, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.udwDataSize, sizeof(u32));
            
            u32 udwSize = 0;
            Core::Image::Format eFormat;
            bool bFoundFormat = GetFormatInfo(sHeader.udwImageFormat, sHeader.udwWidth, sHeader.udwHeight, eFormat, udwSize);
            CS_ASSERT(bFoundFormat, "Invalid MoImage Format.");
            
            // Allocated memory needed for the bitmap context
            u8* pubyBitmapData = (u8*) malloc(udwSize);
            inpImageFile->Read((s8*)pubyBitmapData, udwSize);
            inpImageFile->Close();
            
            Core::Image* outpImage = (Core::Image*)outpResource.get();
            outpImage->SetFormat(eFormat);
            outpImage->SetData(pubyBitmapData);
            outpImage->SetWidth(sHeader.udwWidth);
            outpImage->SetHeight(sHeader.udwHeight);
        }
        //----------------------------------------------------------------
        /// Reads a version 3 formatted .moimage file
        //----------------------------------------------------------------
        void MoImageProvider::ReadFileVersion3(Core::FileStreamSPtr inpImageFile, Core::ResourceSPtr& outpResource)
        {
            //Read the header
            ImageHeaderVersion3 sHeader;
            inpImageFile->Read((s8*)&sHeader.udwWidth, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.udwHeight, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.udwImageFormat, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.udwCompression, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.uddwChecksum, sizeof(u64));
            inpImageFile->Read((s8*)&sHeader.udwOriginalDataSize, sizeof(u32));
            inpImageFile->Read((s8*)&sHeader.udwCompressedDataSize, sizeof(u32));
            
            u32 udwSize = 0;
            Core::Image::Format eFormat;
            bool bFoundFormat = GetFormatInfo(sHeader.udwImageFormat, sHeader.udwWidth, sHeader.udwHeight, eFormat, udwSize);
            CS_ASSERT(bFoundFormat, "Invalid MoImage Format.");
            
            u8* pubyBitmapData = nullptr;
            if(sHeader.udwCompression != 0)
            {
                // Allocated memory needed for the compressed image data
                u8* pubyCompressedData = (u8*)malloc(sHeader.udwCompressedDataSize);
                inpImageFile->Read((s8*)pubyCompressedData, sHeader.udwCompressedDataSize);
                inpImageFile->Close();
                
                // Allocated memory need for for the bitmap context
                pubyBitmapData = (u8*)malloc(sHeader.udwOriginalDataSize);
                
                // Inflate data (I like to think this is the machine equvilent to eating lots of pizza!)
                z_stream infstream;
                infstream.zalloc = Z_NULL;
                infstream.zfree = Z_NULL;
                infstream.opaque = Z_NULL;
                infstream.avail_in = sHeader.udwCompressedDataSize;		// size of input
                infstream.next_in = (Bytef*)pubyCompressedData;			// input data
                infstream.avail_out = sHeader.udwOriginalDataSize;		// size of output
                infstream.next_out = (Bytef*)pubyBitmapData;			// output char array

                inflateInit(&infstream);
                inflate(&infstream, Z_FINISH);
                inflateEnd(&infstream);
                
                // Checksum test
                u32 udwInflatedChecksum = HashCRC32::GenerateHashCode((const s8*)pubyBitmapData, sHeader.udwOriginalDataSize);
                if(sHeader.uddwChecksum != (u64)udwInflatedChecksum)
                {
                    CS_LOG_ERROR("MoImage checksum of "+ToString(udwInflatedChecksum)+" does not match expected checksum "+ToString(sHeader.uddwChecksum));
                }
                
                free(pubyCompressedData);
            }
            else
            {
                // Allocated memory needed for the bitmap context
                pubyBitmapData = (u8*) malloc(udwSize);
                inpImageFile->Read((s8*)pubyBitmapData, udwSize);
                inpImageFile->Close();
            }
            
            Core::Image* outpImage = (Core::Image*)outpResource.get();
            outpImage->SetFormat(eFormat);
            outpImage->SetData(pubyBitmapData);
            outpImage->SetWidth(sHeader.udwWidth);
            outpImage->SetHeight(sHeader.udwHeight);
        }
        //----------------------------------------------------------------
        /// Get Format Info
        //----------------------------------------------------------------
        bool MoImageProvider::GetFormatInfo(const u32 inudwFormat, const u32 inudwWidth, const u32 inudwHeight,
                                             Core::Image::Format& outFormat, u32& outudwImageSize)
        {
            switch(inudwFormat)
            {
                case 1:
                    outFormat = Core::Image::Format::k_Lum8;
                    outudwImageSize = inudwWidth * inudwHeight * 1;
                    return true;
                case 2:
                    outFormat = Core::Image::Format::k_LumA88;
                    outudwImageSize = inudwWidth * inudwHeight * 2;
                    return true;
                case 3:
                    outFormat = Core::Image::Format::k_RGB565;
                    outudwImageSize = inudwWidth * inudwHeight * 2;
                    return true;
                case 4:
                    outFormat = Core::Image::Format::k_RGBA4444;
                    outudwImageSize = inudwWidth * inudwHeight * 2;
                    return true;
                case 5:
                    outFormat = Core::Image::Format::k_RGB888;
                    outudwImageSize = inudwWidth * inudwHeight * 3;
                    return true;
                case 6:
                    outFormat = Core::Image::Format::k_RGBA8888;
                    outudwImageSize = inudwWidth * inudwHeight * 4;
                    return true;
                default:
                case 0:
                    break;
            }
            return false;
        }
    }
}
