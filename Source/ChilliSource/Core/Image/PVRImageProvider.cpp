//
//  PVRImageProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 08/05/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Core/Image/PVRImageProvider.h>

#include <CSBackend/Platform/iOS/Core/File/FileSystem.h>
#include <ChilliSource/Core/Base/Application.h>
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
            const u32 k_pvrVersionMismatch = 0x50565203;
            
            struct PVRTCTexHeader
            {
                u32 udwVersion;
                u32 udwFlags;
                u64 udwPixelFormat;
                u32 udwColourSpace;
                u32 udwChannelType;
                u32 udwHeight;
                u32 udwWidth;
                u32 udwDepth;
                u32 udwNumSurfaces;
                u32 udwNumFaces;
                u32 udwMipMapCount;
                u32 udwMetaDataSize;
            };
            
            //Anything outside these is not supported on iDevices
            enum class PixelFormat
            {
                k_2BppRGB,
                k_2BppRGBA,
                k_4BppRGB,
                k_4BppRGBA
            };
            
            enum class ColourSpace
            {
                k_linearRGB,
                k_SRGB
            };
            
            enum class ChannelType
            {
                k_unsignedByteNormalised,
                k_signedByteNormalised,
                k_unsignedByte,
                k_signedByte,
                k_unsignedShortNormalised,
                k_signedShortNormalised,
                k_unsignedShort,
                k_signedShort,
                k_unsignedIntNormalised,
                k_signedIntNormalised,
                k_unsignedInt,
                k_signedInt,
                k_float
            };
            
            const std::string k_pvrExtension("pvr");
            
			//-----------------------------------------------------------
			/// Create an image in raw byte format
            ///
            /// @author S McGaw
			///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param [Out] Image resource
			//------------------------------------------------------------
			void CreatePVRImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image* out_image)
            {
                //Get the header data from the image file
                const PVRTCTexHeader* header = reinterpret_cast<const PVRTCTexHeader*>(in_data);
                
                //Check the version so determine endianess correctness
                if(header->udwVersion == k_pvrVersionMismatch)
                {
                    //TODO:: Endianess is not correct, need to flip bits in the header data, possibly image data?
                    CS_LOG_FATAL("Image::UnpackPVRTCData >> Endianess Check failed for creating PVR");
                }
                
                u64 udwPFormat = header->udwPixelFormat;
                
                u32 udwLow32Bits  = udwPFormat & 0x00000000ffffffff;
                u32 udwHigh32Bits = udwPFormat >> 32; //Shift to right 32bits
                Core::ImageCompression compression = Core::ImageCompression::k_none;
                Core::ImageFormat format = Core::ImageFormat::k_RGB888;
                
                //Calculate the data size for each texture level and respect the minimum number of blocks
                u32 udwBpp = 4;
                
                //Where the most significant 4 bytes have been set to ‘0’ the least significant 4 bytes will contain a 32bit unsigned integer value identifying the pixel format.
                if(udwHigh32Bits == 0)
                {
                    if(udwLow32Bits == (u32)PixelFormat::k_2BppRGB || udwLow32Bits == (u32)PixelFormat::k_2BppRGBA)
                    {
                        compression = Core::ImageCompression::k_PVR2Bpp;
                        
                        //Pixel by pixel block size for 2bpp
                        udwBpp = 2;
                        
                        //Set if Alpha in image
                        if(udwLow32Bits == (u32)PixelFormat::k_2BppRGBA)
                        {
                            format = Core::ImageFormat::k_RGBA8888;
                        }
                    }
                    else if(udwLow32Bits == (u32)PixelFormat::k_4BppRGB || udwLow32Bits == (u32)PixelFormat::k_4BppRGBA)
                    {
                        compression = Core::ImageCompression::k_PVR4Bpp;
                        
                        //Pixel by pixel block size for 4bpp
                        udwBpp = 4;
                        
                        //Set if Alpha in image
                        if(udwLow32Bits == (u32)PixelFormat::k_4BppRGBA)
                        {
                            format = Core::ImageFormat::k_RGBA8888;
                        }
                    }
                    else
                    {
                        CS_LOG_FATAL("Unrecognised PixelFormat for image");
                    }
                }
                else
                {
                    //If the most significant 4 bytes contain a value, the full 8 bytes are used to determine the pixel format. The least significant 4 bytes contain the channel order,
                    //each byte containing a single character, or a null character if there are fewer than four channels; for example, {‘r’,‘g’,‘b’,‘a’} or {‘r’,‘g’,‘b’,‘\0’}.
                    //The most significant 4 bytes state the bit rate for each channel in the same order, each byte containing a single 8bit unsigned integer value,
                    //or zero if there are fewer than four channels; for example, {8,8,8,8} or {5,6,5,0}.
                    CS_LOG_FATAL("Unimplemented PixelFormat for image");
                }
                
                Core::Image::Descriptor desc;
                desc.m_width = header->udwWidth;
                desc.m_height = header->udwHeight;
                desc.m_compression = compression;
                desc.m_format = format;
                desc.m_dataSize = (header->udwWidth * header->udwHeight * udwBpp) >> 3;
                
                u8* pData = new u8[desc.m_dataSize];
                memcpy(pData, in_data + sizeof(PVRTCTexHeader), sizeof(u8) * desc.m_dataSize);
                out_image->Build(desc, Core::Image::ImageDataUPtr(pData));
            }
            //-----------------------------------------------------------
            /// Performs the heavy lifting for the 2 create methods
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param Completion delegate
            /// @param [Out] The output resource
            //-----------------------------------------------------------
            void LoadImage(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
            {
                Core::FileStreamSPtr pImageFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filePath, Core::FileMode::k_readBinary);
                
                if(pImageFile == nullptr || pImageFile->IsBad() == true)
                {
                    out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
                        Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                    }
                    return;
                }
                
                std::string abyData;
                pImageFile->GetAll(abyData);
                
                CreatePVRImageFromFile(abyData.data(), abyData.size(), (Core::Image*)out_resource.get());
                
                out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
                if(in_delegate != nullptr)
                {
                    Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
            }
        }
        CS_DEFINE_NAMEDTYPE(PVRImageProvider);
        //-------------------------------------------------------
        //-------------------------------------------------------
        PVRImageProviderUPtr PVRImageProvider::Create()
        {
            return PVRImageProviderUPtr(new PVRImageProvider());
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool PVRImageProvider::IsA(Core::InterfaceIDType in_interaceId) const
		{
			return (in_interaceId == ResourceProvider::InterfaceID || in_interaceId == PVRImageProvider::InterfaceID);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        Core::InterfaceIDType PVRImageProvider::GetResourceType() const
        {
            return Core::Image::InterfaceID;
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool PVRImageProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_pvrExtension;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void PVRImageProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
		{
            LoadImage(in_storageLocation, in_filePath, nullptr, out_resource);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void PVRImageProvider::CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
        {
            auto task = std::bind(LoadImage, in_storageLocation, in_filePath, in_delegate, out_resource);
            
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
	}
}
