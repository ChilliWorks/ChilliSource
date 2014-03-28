//
//  ETC1ImageProvider.cpp
//  Chilli Source
//
//  Created by I Copland on 09/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Image/ETC1ImageProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            const std::string k_etc1Extension = "pkm";
            
            //-------------------------------------------------------
            /// A container for the ETC1 Header.
            ///
            /// @author I Copland
            //-------------------------------------------------------
            struct ETC1Header
            {
                u8 m_pkmTag[6];
                u16 m_numberOfMipmaps;
                u16 m_textureWidth;
                u16 m_textureHeight;
                u16 m_originalWidth;
                u16 m_originalHeight;
            };
        }
        
        CS_DEFINE_NAMEDTYPE(ETC1ImageProvider);
        //-------------------------------------------------------
        //-------------------------------------------------------
        ETC1ImageProviderUPtr ETC1ImageProvider::Create()
        {
            return ETC1ImageProviderUPtr(new ETC1ImageProvider());
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool ETC1ImageProvider::IsA(InterfaceIDType in_interfaceId) const
        {
            return (in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == ETC1ImageProvider::InterfaceID);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool ETC1ImageProvider::CanCreateResourceOfKind(InterfaceIDType in_interfaceId) const
        {
            return (in_interfaceId == Image::InterfaceID);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool ETC1ImageProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
        {
            return (in_extension == k_etc1Extension);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool ETC1ImageProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filepath, ResourceSPtr& out_resource)
        {
            //ensure the extension is correct.
            if (StringUtils::EndsWith(in_filepath, k_etc1Extension, true) == false)
                return false;
            
            FileStreamSPtr pImageFile = Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filepath, FileMode::k_readBinary);
            
            if (pImageFile != nullptr && pImageFile->IsOpen() == true && pImageFile->IsBad() == false)
            {
                //ETC1 Format is in big endian format. As all the platforms we support are little endian we will have to convert the data to little endian.
                //read the header.
                ETC1Header sHeader;
                pImageFile->Read((s8*)sHeader.m_pkmTag, sizeof(u8) * 6);
                
                pImageFile->Read((s8*)&sHeader.m_numberOfMipmaps, sizeof(u16));
                sHeader.m_numberOfMipmaps = ChilliSource::Core::Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_numberOfMipmaps));
                
                pImageFile->Read((s8*)&sHeader.m_textureWidth, sizeof(u16));
                sHeader.m_textureWidth = ChilliSource::Core::Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_textureWidth));
                
                pImageFile->Read((s8*)&sHeader.m_textureHeight, sizeof(u16));
                sHeader.m_textureHeight = ChilliSource::Core::Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_textureHeight));
                
                pImageFile->Read((s8*)&sHeader.m_originalWidth, sizeof(u16));
                sHeader.m_originalWidth = ChilliSource::Core::Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_originalWidth));
                
                pImageFile->Read((s8*)&sHeader.m_originalHeight, sizeof(u16));
                sHeader.m_originalHeight = ChilliSource::Core::Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_originalHeight));
                
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
                Core::Image* outpImage = (Core::Image*)out_resource.get();
                outpImage->SetFormat(Core::Image::Format::k_RGBA8888);
                outpImage->SetData(pData);
                outpImage->SetDataLength(dwDataSize);
                outpImage->SetWidth(sHeader.m_textureWidth);
                outpImage->SetHeight(sHeader.m_textureHeight);
                outpImage->SetCompression(Core::ImageCompression::k_ETC1);
                outpImage->SetLoaded(true);
                return true;
            }
            
            return false;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool ETC1ImageProvider::AsyncCreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceSPtr& out_resource)
        {
            CS_LOG_WARNING("Async load not implemented for ETC1 Image Provider");
            return false;
        }
    }
}
