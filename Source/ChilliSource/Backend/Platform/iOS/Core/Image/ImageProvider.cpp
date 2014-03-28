//
//  ImageProvider.cpp
//  Chilli Source
//
//  Created by S McGaw on 01/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/Image/ImageProvider.h>

#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringUtils.h>

#import <UIKit/UIKit.h>

namespace ChilliSource
{
	namespace iOS
	{
        namespace
        {
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
                Core::Image::Format format = Core::Image::Format::k_RGBA8888;
                if(CGColorSpaceGetModel(ColorSpaceInfo) == kCGColorSpaceModelMonochrome)
                {
                    if(bHasAlpha)
                    {
                        format = Core::Image::Format::k_LumA88;
                        ColorSpaceInfo = CGColorSpaceCreateDeviceRGB();
                    }
                    else
                    {
                        format = Core::Image::Format::k_Lum8;
                        ColorSpaceInfo = CGColorSpaceCreateDeviceRGB();
                    }
                }
                
                // Allocated memory needed for the bitmap context
                u8* pubyBitmapData8888 = (u8*) calloc(udwArea, udwBytesPerPixel);
                
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

                out_image->SetData(pubyBitmapData8888);
                out_image->SetWidth(udwWidth);
                out_image->SetHeight(udwHeight);
                out_image->SetFormat(format);
                out_image->SetLoaded(true);
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
                u32 udwArea = udwWidth * udwHeight;
                
                //Check if the image has alpha
                CGImageAlphaInfo AlphaInfo = CGImageGetAlphaInfo(cgImage);
                
                bool bHasAlpha = ((AlphaInfo == kCGImageAlphaPremultipliedLast) || (AlphaInfo == kCGImageAlphaPremultipliedFirst) || (AlphaInfo == kCGImageAlphaLast) || (AlphaInfo == kCGImageAlphaFirst));
                
                AlphaInfo = (bHasAlpha ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNoneSkipLast);
                
                // Allocated memory needed for the bitmap context
                u8* pubyBitmapData8888 = (u8*) calloc(udwArea, udwBytesPerPixel);
                // Uses the bitmatp creation function provided by the Core Graphics framework.
                CGContextRef bitmapContext = CGBitmapContextCreate(pubyBitmapData8888, udwWidth, udwHeight, udwBitsPerComponent, udwWidth * udwBytesPerPixel, CGImageGetColorSpace(cgImage), AlphaInfo);
                // After you create the context, you can draw the sprite image to the context.
                CGContextDrawImage(bitmapContext, CGRectMake(0.0f, 0.0f, (CGFloat)udwWidth, (CGFloat)udwHeight), cgImage);
                
                CGImageRelease(cgImage);
                // You don't need the context at this point, so you need to release it to avoid memory leaks.
                CGContextRelease(bitmapContext);
                
                CFRelease(pData);
                CGDataProviderRelease(imgDataProvider);
                
                
                out_image->SetData(pubyBitmapData8888);
                out_image->SetWidth(udwWidth);
                out_image->SetHeight(udwHeight);
                out_image->SetFormat(Core::Image::Format::k_RGBA8888);
                out_image->SetLoaded(true);
            }
			//-----------------------------------------------------------
			/// Create an image in raw byte format
            ///
            /// @author S McGaw
			///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param [Out] Image resource
			//------------------------------------------------------------
			void CreatePVRImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image* out_image)
            {
                s8* pData = (s8*)malloc(in_dataSize);
                memcpy(pData, in_data, sizeof(s8) * in_dataSize);
                
                out_image->SetData((u8*)pData);
                out_image->UnpackPVRData();
                out_image->SetFormat(Core::Image::Format::k_RGBA8888);
                out_image->SetLoaded(true);
            }
        }
        CS_DEFINE_NAMEDTYPE(ImageProvider);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::IsA(Core::InterfaceIDType in_interaceId) const
		{
			return (in_interaceId == Core::ResourceProvider::InterfaceID || in_interaceId == Core::ImageProvider::InterfaceID || in_interaceId == ImageProvider::InterfaceID);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CanCreateResourceOfKind(Core::InterfaceIDType in_interaceId) const
		{
			return in_interaceId == Core::Image::InterfaceID;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CanCreateResourceFromFileWithExtension(const std::string & in_extension) const
		{
			return (in_extension == k_pngExtension || in_extension == k_jpgExtension || in_extension == k_jpegExtension || in_extension == k_pvrExtension);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filepath, Core::ResourceSPtr& out_resource)
		{
			Core::FileStreamSPtr pImageFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filepath, Core::FileMode::k_readBinary);
            
            if(pImageFile && !pImageFile->IsBad())
			{
                std::string strName;
                std::string strExtension;
                
                //Get the name of the file and append the high res identifier to it
                Core::StringUtils::SplitBaseFilename(in_filepath, strName, strExtension);
                
                //Based on the extension decide how to load the file
                std::string abyData;
                pImageFile->GetAll(abyData);
                
                if(strExtension == k_pngExtension)
                {
                    CreatePNGImageFromFile(abyData.data(), abyData.size(), (Core::Image*)out_resource.get());
					return true;
                }
                else if(strExtension == k_pvrExtension)
                {
                    CreatePVRImageFromFile(abyData.data(), abyData.size(), (Core::Image*)out_resource.get());
					return true;
                }
                else if(strExtension == k_jpgExtension || strExtension == k_jpegExtension)
                {
                    CS_LOG_WARNING("JPG image loading is not cross-platform and will only work on iOS. Do not use JPEGs in a cross-platform project.");
                    CreateJPGImageFromFile(abyData.data(), abyData.size(), (Core::Image*)out_resource.get());
					return true;
                }
            }
            
            return false;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        bool ImageProvider::AsyncCreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filePath, Core::ResourceSPtr& out_resource)
        {
            CS_LOG_WARNING("Async loading is not implemented by the Image Provider.");
            
            return false;
        }
	}
}
