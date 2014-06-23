//
//  ETC1ImageProvider.cpp
//  Chilli Source
//  Created by Ian Copland on 09/11/2012.
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

#include <ChilliSource/Core/Image/ETC1ImageProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

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
            /// @author Ian Copland
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
            //----------------------------------------------------
            /// Actually performs the heavy lifting for the 2
            /// create methods
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
                
                //ETC1 Format is in big endian format. As all the platforms we support are little endian we will have to convert the data to little endian.
                //read the header.
                ETC1Header sHeader;
                pImageFile->Read((s8*)sHeader.m_pkmTag, sizeof(u8) * 6);
                
                pImageFile->Read((s8*)&sHeader.m_numberOfMipmaps, sizeof(u16));
                sHeader.m_numberOfMipmaps = Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_numberOfMipmaps));
                
                pImageFile->Read((s8*)&sHeader.m_textureWidth, sizeof(u16));
                sHeader.m_textureWidth = Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_textureWidth));
                
                pImageFile->Read((s8*)&sHeader.m_textureHeight, sizeof(u16));
                sHeader.m_textureHeight = Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_textureHeight));
                
                pImageFile->Read((s8*)&sHeader.m_originalWidth, sizeof(u16));
                sHeader.m_originalWidth = Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_originalWidth));
                
                pImageFile->Read((s8*)&sHeader.m_originalHeight, sizeof(u16));
                sHeader.m_originalHeight = Utils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.m_originalHeight));
                
                //get the size of the rest of the data
                const u32 kstrHeaderSize = 16;
                pImageFile->SeekG(0, SeekDir::k_end);
                u32 dwDataSize = pImageFile->TellG() - kstrHeaderSize;
                pImageFile->SeekG(kstrHeaderSize, SeekDir::k_beginning);
                
                //read the rest of the data
                u8* pData = new u8[dwDataSize];
                pImageFile->Read((s8*)pData, dwDataSize);
                pImageFile->Close();
                Image::ImageDataUPtr imageData(pData);
                
                //setup the output image
                Image::Descriptor desc;
                desc.m_format = ImageFormat::k_RGB888;
                desc.m_compression = ImageCompression::k_ETC1;
                desc.m_width = sHeader.m_textureWidth;
                desc.m_height = sHeader.m_textureHeight;
                desc.m_dataSize = dwDataSize;
                
                Image* outpImage = (Image*)out_resource.get();
                outpImage->Build(desc, std::move(imageData));
                outpImage->SetLoadState(Resource::LoadState::k_loaded);
                
                if(in_delegate != nullptr)
                {
					Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
            }
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
        InterfaceIDType ETC1ImageProvider::GetResourceType() const
        {
            return Image::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool ETC1ImageProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
        {
            return in_extension == k_etc1Extension;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
		void ETC1ImageProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filepath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
        {
            LoadImage(in_storageLocation, in_filepath, nullptr, out_resource);
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void ETC1ImageProvider::CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filepath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
        {
			auto task = std::bind(LoadImage, in_storageLocation, in_filepath, in_delegate, out_resource);
			Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
    }
}
