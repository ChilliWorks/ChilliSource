//
//  ImageProvider.cpp
//  Chilli Source
//  Created by S McGaw on 01/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Backend/Platform/iOS/Core/Image/ImageProvider.h>

#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#import <UIKit/UIKit.h>

namespace ChilliSource
{
	namespace iOS
	{
        namespace
        {
            //TODO: Move all this stuff into a PVR provider
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
            
            const std::string k_pngExtension("png");
            const std::string k_jpgExtension("jpg");
            const std::string k_jpegExtension("jpeg");
            const std::string k_pvrExtension("pvr");
            
            //-----------------------------------------------------------
            /// Create a PNG image using the Apple API
            ///
            /// @author S McGaw
            ///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param Image format
			/// @param [Out] Image resource
            //-----------------------------------------------------------
            void CreatePNGImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image* out_image)
            {
                CFDataRef pData = CFDataCreateWithBytesNoCopy(nullptr, (u8*)in_data, in_dataSize, kCFAllocatorNull);
                CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData(pData);
                CGImageRef cgImage = CGImageCreateWithPNGDataProvider(imgDataProvider, nullptr, true, kCGRenderingIntentDefault);
                
                // Get the width and height of the image
                u32 udwWidth = CGImageGetWidth(cgImage);
                u32 udwHeight = CGImageGetHeight(cgImage);
                u32 udwBitsPerComponent = CGImageGetBitsPerComponent(cgImage);
                u32 udwBytesPerPixel = (udwBitsPerComponent * 4)/8;
                u32 udwArea = udwWidth * udwHeight;
                
                //Check if the image has alpha
                CGImageAlphaInfo AlphaInfo = CGImageGetAlphaInfo(cgImage);
                
                bool bHasAlpha = ((AlphaInfo == kCGImageAlphaPremultipliedLast) || (AlphaInfo == kCGImageAlphaPremultipliedFirst) || (AlphaInfo == kCGImageAlphaLast) || (AlphaInfo == kCGImageAlphaFirst));
                
                AlphaInfo = (bHasAlpha ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNoneSkipLast);
                
                // Grab image's color format
                CGColorSpaceRef ColorSpaceInfo = CGImageGetColorSpace(cgImage);
                
                // Retain here as we may need to create one and release later
                CGColorSpaceRetain(ColorSpaceInfo);
                
                // Special case for GreyScale images
                Core::ImageFormat format = Core::ImageFormat::k_RGBA8888;
                if(CGColorSpaceGetModel(ColorSpaceInfo) == kCGColorSpaceModelMonochrome)
                {
                    if(bHasAlpha)
                    {
                        format = Core::ImageFormat::k_LumA88;
                        ColorSpaceInfo = CGColorSpaceCreateDeviceRGB();
                    }
                    else
                    {
                        format = Core::ImageFormat::k_Lum8;
                        ColorSpaceInfo = CGColorSpaceCreateDeviceRGB();
                    }
                }
                
                // Allocated memory needed for the bitmap context
                u32 dataSize = udwArea * udwBytesPerPixel;
                u8* pubyBitmapData8888 = new u8[dataSize];

                // Uses the bitmatp creation function provided by the Core Graphics framework.
                CGContextRef bitmapContext = CGBitmapContextCreate(pubyBitmapData8888, udwWidth, udwHeight, udwBitsPerComponent, udwWidth * udwBytesPerPixel, ColorSpaceInfo, AlphaInfo);
                
                // After you create the context, you can draw the sprite image to the context.
                CGContextDrawImage(bitmapContext, CGRectMake(0.0f, 0.0f, (CGFloat)udwWidth, (CGFloat)udwHeight), cgImage);
                CGImageRelease(cgImage);
                
                // You don't need the context at this point, so you need to release it to avoid memory leaks.
                CGContextRelease(bitmapContext);
                
                CGColorSpaceRelease(ColorSpaceInfo);
                CGDataProviderRelease(imgDataProvider);
                CFRelease(pData);

                Core::Image::Descriptor desc;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_format = format;
                desc.m_width = udwWidth;
                desc.m_height = udwHeight;
                desc.m_dataSize = dataSize;
                out_image->Build(desc, Core::Image::ImageDataUPtr(pubyBitmapData8888));
            }
            //-----------------------------------------------------------
            /// Create a JPG image using the Apple API
            ///
            /// @author S McGaw
            ///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param Image format
			/// @param [Out] Image resource
            //-----------------------------------------------------------
            void CreateJPGImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image* out_image)
            {
                CFDataRef pData = CFDataCreateWithBytesNoCopy(nullptr, (u8*)in_data, in_dataSize, kCFAllocatorNull);
                CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData(pData);
                CGImageRef cgImage = CGImageCreateWithJPEGDataProvider(imgDataProvider, nullptr, true, kCGRenderingIntentDefault);
                
                // Get the width and height of the image
                u32 udwWidth = CGImageGetWidth(cgImage);
                u32 udwHeight = CGImageGetHeight(cgImage);
                u32 udwBitsPerComponent = CGImageGetBitsPerComponent(cgImage);
                u32 udwBytesPerPixel = (udwBitsPerComponent * 4)/8;
                
                //Check if the image has alpha
                CGImageAlphaInfo AlphaInfo = CGImageGetAlphaInfo(cgImage);
                
                bool bHasAlpha = ((AlphaInfo == kCGImageAlphaPremultipliedLast) || (AlphaInfo == kCGImageAlphaPremultipliedFirst) || (AlphaInfo == kCGImageAlphaLast) || (AlphaInfo == kCGImageAlphaFirst));
                
                AlphaInfo = (bHasAlpha ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNoneSkipLast);
                
                // Allocated memory needed for the bitmap context
                u32 dataSize = udwWidth * udwBytesPerPixel;
                u8* pubyBitmapData8888 = new u8[dataSize];
                // Uses the bitmatp creation function provided by the Core Graphics framework.
                CGContextRef bitmapContext = CGBitmapContextCreate(pubyBitmapData8888, udwWidth, udwHeight, udwBitsPerComponent, dataSize, CGImageGetColorSpace(cgImage), AlphaInfo);
                // After you create the context, you can draw the sprite image to the context.
                CGContextDrawImage(bitmapContext, CGRectMake(0.0f, 0.0f, (CGFloat)udwWidth, (CGFloat)udwHeight), cgImage);
                
                CGImageRelease(cgImage);
                // You don't need the context at this point, so you need to release it to avoid memory leaks.
                CGContextRelease(bitmapContext);
                
                CFRelease(pData);
                CGDataProviderRelease(imgDataProvider);
                
                Core::Image::Descriptor desc;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_format = Core::ImageFormat::k_RGBA8888;
                desc.m_width = udwWidth;
                desc.m_height = udwHeight;
                desc.m_dataSize = dataSize;
                out_image->Build(desc, Core::Image::ImageDataUPtr(pubyBitmapData8888));
            }
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
                bool bSupported = false;
                
                //Calculate the data size for each texture level and respect the minimum number of blocks
                u32 udwBpp = 4;
                
                //Where the most significant 4 bytes have been set to ‘0’ the least significant 4 bytes will contain a 32bit unsigned integer value identifying the pixel format.
                if(udwHigh32Bits == 0)
                {
                    if(udwLow32Bits == (u32)PixelFormat::k_2BppRGB || udwLow32Bits == (u32)PixelFormat::k_2BppRGBA)
                    {
                        compression = Core::ImageCompression::k_PVR2Bpp;
                        bSupported = true;
                        
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
                        bSupported = true;
                        
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
                    
                    //TODO:: Should never reach here through pvr format
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
        }
        CS_DEFINE_NAMEDTYPE(ImageProvider);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::IsA(Core::InterfaceIDType in_interaceId) const
		{
			return (in_interaceId == Core::ResourceProvider::InterfaceID || in_interaceId == Core::ImageProvider::InterfaceID || in_interaceId == ImageProvider::InterfaceID);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        Core::InterfaceIDType ImageProvider::GetResourceType() const
        {
            return Core::Image::InterfaceID;
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return (in_extension == k_pngExtension || in_extension == k_jpgExtension || in_extension == k_jpegExtension || in_extension == k_pvrExtension);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ImageProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::ResourceSPtr& out_resource)
		{
            LoadImage(in_storageLocation, in_filePath, nullptr, out_resource);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void ImageProvider::CreateResourceFromFileAsync(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
            auto task = std::bind(&ImageProvider::LoadImage, this, in_storageLocation, in_filePath, in_delegate, out_resource);
            
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void ImageProvider::LoadImage(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
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
            
            std::string strName;
            std::string strExtension;
            
            //Get the name of the file and append the high res identifier to it
            Core::StringUtils::SplitBaseFilename(in_filePath, strName, strExtension);
            
            //Based on the extension decide how to load the file
            std::string abyData;
            pImageFile->GetAll(abyData);
            
            if(strExtension == k_pngExtension)
            {
                CreatePNGImageFromFile(abyData.data(), abyData.size(), (Core::Image*)out_resource.get());
            }
            else if(strExtension == k_pvrExtension)
            {
                CreatePVRImageFromFile(abyData.data(), abyData.size(), (Core::Image*)out_resource.get());
            }
            else if(strExtension == k_jpgExtension || strExtension == k_jpegExtension)
            {
                CS_LOG_WARNING("JPG image loading is not cross-platform and will only work on iOS. Do not use JPEGs in a cross-platform project.");
                CreateJPGImageFromFile(abyData.data(), abyData.size(), (Core::Image*)out_resource.get());
            }
            else
            {
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
                    Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
            out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
            if(in_delegate != nullptr)
            {
                Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
            }
        }
	}
}
