//
//  ETC1ImageProvider.cpp
//  moFlow
//
//  Created by Ian Copland on 09/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Image/ETC1ImageProvider.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>

const std::string ETC1Extension("pkm");

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------
        /// Is A
        //----------------------------------------------------------------
        bool ETC1ImageProvider::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == ChilliSource::Core::ResourceProvider::InterfaceID;
        }
        //----------------------------------------------------------------
        /// Can Create Resource Of Kind
        //----------------------------------------------------------------
        bool ETC1ImageProvider::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == Core::Image::InterfaceID;
        }
        //----------------------------------------------------------------
        /// Can Create Resource From File With Extension
        //----------------------------------------------------------------
        bool ETC1ImageProvider::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
        {
            return (inExtension == ETC1Extension);
        }
        //----------------------------------------------------------------
        /// Create Resource From File
        //----------------------------------------------------------------
        bool ETC1ImageProvider::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)
        {
            return CreateImageFromFile(ineStorageLocation, inFilePath, Core::Image::Format::k_default, outpResource);
        }
        //----------------------------------------------------------------
        /// Create Image From File
        //----------------------------------------------------------------
        bool ETC1ImageProvider::CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::ResourceSPtr& outpResource)
        {
            //ensure the extension is correct.
            if (ChilliSource::Core::CStringUtils::EndsWith(inFilePath, ETC1Extension, true) == false)
                return false;

            Core::FileStreamSPtr pImageFile = Core::Application::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);

            if (pImageFile != nullptr && pImageFile->IsOpen() == true && pImageFile->IsBad() == false)
            {
                //ETC1 Format is in big endian format. As all the platforms we support are little endian we will have to convert the data to little endian.
                //read the header.
                ETC1Header sHeader;
                pImageFile->Read((s8*)sHeader.abyPKMTag, sizeof(u8) * 6);

                pImageFile->Read((s8*)&sHeader.wNumberOfMipmaps, sizeof(u16));
                sHeader.wNumberOfMipmaps = ChilliSource::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wNumberOfMipmaps));

                pImageFile->Read((s8*)&sHeader.wTextureWidth, sizeof(u16));
                sHeader.wTextureWidth = ChilliSource::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wTextureWidth));

                pImageFile->Read((s8*)&sHeader.wTextureHeight, sizeof(u16));
                sHeader.wTextureHeight = ChilliSource::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wTextureHeight));

                pImageFile->Read((s8*)&sHeader.wOriginalWidth, sizeof(u16));
                sHeader.wOriginalWidth = ChilliSource::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wOriginalWidth));

                pImageFile->Read((s8*)&sHeader.wOriginalHeight, sizeof(u16));
                sHeader.wOriginalHeight = ChilliSource::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wOriginalHeight));

                //get the size of the rest of the data
                const u32 kstrHeaderSize = 16;
                pImageFile->SeekG(0, Core::SeekDir::k_end);
                u32 dwDataSize = pImageFile->TellG() - kstrHeaderSize;
                pImageFile->SeekG(kstrHeaderSize, Core::SeekDir::k_beginning);

                //read the rest of the data
                u8* pData = new u8[dwDataSize];
                pImageFile->Read((s8*)pData, dwDataSize);
                pImageFile->Close();

                //setup the output image
                Core::Image* outpImage = (Core::Image*)outpResource.get();
                outpImage->SetFormat(Core::Image::Format::k_RGBA8888);
                outpImage->SetData(pData);
                outpImage->SetDataLength(dwDataSize);
                outpImage->SetWidth(sHeader.wTextureWidth);
                outpImage->SetHeight(sHeader.wTextureHeight);
                outpImage->SetCompression(Core::ImageCompression::k_ETC1);

                return true;
            }

            return false;
        }
    }
}
