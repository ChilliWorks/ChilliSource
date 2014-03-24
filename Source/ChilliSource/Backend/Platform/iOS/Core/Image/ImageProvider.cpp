//
//  ImageProvider.cpp
//  Chilli Source
//
//  Created by S Downie on 01/10/2010.
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
			return CreateImageFromFile(in_storageLocation, in_filepath, Core::Image::Format::k_default, out_resource);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::ResourceSPtr& out_resource)
		{
			Core::FileStreamSPtr pImageFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);
   
            if(pImageFile && !pImageFile->IsBad())
			{
                std::string strName;
                std::string strExtension;
                
                //Get the name of the file and append the high res identifier to it
                Core::StringUtils::SplitBaseFilename(inFilePath, strName, strExtension);
                
                //Based on the extension decide how to load the file
                std::string abyData;
                pImageFile->GetAll(abyData);
                
                if(strExtension == k_pngExtension)
                {
                    CreatePNGImageFromFile(abyData.data(), abyData.size(), ineFormat, (Core::Image*)out_resource.get());
					return true;
                }
                else if(strExtension == k_pvrExtension)
                {
                    CreatePVRImageFromFile(abyData.data(), abyData.size(), (Core::Image*)out_resource.get());
					return true;
                }
                else if(strExtension == k_jpgExtension || strExtension == k_jpegExtension)
                {
                    CS_LOG_WARNING("JPG image loading is not cross-platform and will only work on iOS. Do not use PNGs in a cross-platform project.");
                    CreateJPGImageFromFile(abyData.data(), abyData.size(), ineFormat, (Core::Image*)out_resource.get());
					return true;
                }
            }
            
            return false;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ImageProvider::CreatePNGImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image::Format in_format, Core::Image* out_image)
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
			if(CGColorSpaceGetModel(ColorSpaceInfo) == kCGColorSpaceModelMonochrome)
			{
				//If the inFormat is specified as LUM_8 then we should try to load as such, even if it has alpha
				if(bHasAlpha && in_format != Core::Image::Format::k_Lum8)
				{
					in_format = Core::Image::Format::k_LumA88;
					ColorSpaceInfo = CGColorSpaceCreateDeviceRGB();
				}
				else 
				{
					in_format = Core::Image::Format::k_Lum8;
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

	
			if(in_format == Core::Image::Format::k_default)
			{
				in_format = s_defaultFormat;
			}
			
			//We always load the image as RGBA_8888 but we convert on the fly to the correct format
			switch(in_format)
			{
				case Core::Image::Format::k_RGBA4444:
				{
					CS_LOG_DEBUG("Converting to RGBA_4444");
				
					u8* pubyBitmapData4444 = RGBA8888ToRGBA4444(pubyBitmapData8888, udwArea);
			
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData4444;
					break;
				}
				case Core::Image::Format::k_RGB565:
				{
					CS_LOG_DEBUG("Converting to RGBA_565");
					
					u8* pubyBitmapData565 = RGBA8888ToRGB565(pubyBitmapData8888, udwArea);
					
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData565;
					break;
				}
				case Core::Image::Format::k_LumA88:
				{
					CS_LOG_DEBUG("Converting to LUMA_88");
					
					u8* pubyBitmapData88 = RGBA8888ToLUM88(pubyBitmapData8888, udwArea);
					
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData88;
					break;
				}
                case Core::Image::Format::k_Lum8:
                {
                    CS_LOG_DEBUG("Converting to LUM_8");
                    
                    u8* pubyBitmapData8 = RGBA8888ToLUM8(pubyBitmapData8888, udwArea);
                    
                    free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData8;
                    break;
                }
                    
                default:
                    break;
			};
			
			out_image->SetData(pubyBitmapData8888);
			out_image->SetWidth(udwWidth);
			out_image->SetHeight(udwHeight);
			out_image->SetFormat(in_format);
		}
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void ImageProvider::CreateJPGImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image::Format in_format, Core::Image* out_image)
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
			
			//We always load the image as RGBA_8888 but we convert on the fly to the correct format
			switch(in_format)
			{
				case Core::Image::Format::k_RGBA4444:
				{
					CS_LOG_DEBUG("Converting to RGBA_4444");
					
					u8* pubyBitmapData4444 = RGBA8888ToRGBA4444(pubyBitmapData8888, udwArea);
					
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData4444;
					break;
				}
				case Core::Image::Format::k_RGB565:
				{
					CS_LOG_DEBUG("Converting to RGB_565");
					
					u8* pubyBitmapData565 = RGBA8888ToRGB565(pubyBitmapData8888, udwArea);
					
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData565;
					break;
				}
                    
                default:
                    break;
			};
			
			out_image->SetData(pubyBitmapData8888);
			out_image->SetWidth(udwWidth);
			out_image->SetHeight(udwHeight);
			out_image->SetFormat(in_format);
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ImageProvider::CreatePVRImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image* out_image)
		{
            s8* pData = (s8*)malloc(in_dataSize);
            memcpy(pData, in_data, sizeof(s8) * in_dataSize);
          
			out_image->SetData((u8*)pData);
            out_image->UnpackPVRData();
			out_image->SetFormat(Core::Image::Format::k_RGBA8888);
		}
		//----------------------------------------------------------------
		//-----------------------------------------------------------------
		u8* ImageProvider::RGBA8888ToRGB565(u8* in_data, u32 in_area)
		{
			u32 udwBytesPerPixel = 2;
			
			u8* pubyBitmapData565 = (u8*)calloc(in_area, udwBytesPerPixel);
			u32* pPixel32 = (u32*)in_data;
			u16* pPixel16 = (u16*)pubyBitmapData565;
			
			for(u32 i=0; i<in_area; ++i, ++pPixel32)
			{
				*pPixel16++ = ((((*pPixel32 >> 0) & 0xFF) >> 3) << 11) | 
				((((*pPixel32 >> 8) & 0xFF) >> 2) << 5) |
				((((*pPixel32 >> 16) & 0xFF) >> 3) << 0);
			}
			
			return pubyBitmapData565;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		u8* ImageProvider::RGBA8888ToRGBA4444(u8* in_data, u32 in_area)
		{
			u32 udwBytesPerPixel = 2;
			
			u8* pubyBitmapData4444 = (u8*)calloc(in_area, udwBytesPerPixel);
			u32* pPixel32 = (u32*)in_data;
			u16* pPixel16 = (u16*)pubyBitmapData4444;
			
			for(u32 i=0; i<in_area; ++i, ++pPixel32)
			{
				*pPixel16++ = 
				((((*pPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
				((((*pPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
				((((*pPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
				((((*pPixel32 >> 24) & 0xFF) >> 4) << 0); // A
			}
			
			return pubyBitmapData4444;
		}
		//----------------------------------------------------------------
		//-----------------------------------------------------------------
		u8* ImageProvider::RGBA8888ToLUM88(u8* in_data, u32 in_area)
		{
			u32 udwBytesPerPixel = 2;
			
			u8* pubyBitmapData88 = (u8*)calloc(in_area, udwBytesPerPixel);
			u32* pPixel32 = (u32*)in_data;
			u16* pPixel16 = (u16*)pubyBitmapData88;
			
			for(u32 i=0; i<in_area; ++i, ++pPixel32)
			{
				*pPixel16++ = 
				((*pPixel32)& 0xFF) | // L
				((*pPixel32 >> 16) & 0xFF00); // A
			}
			
			return pubyBitmapData88;
		}
   
        //----------------------------------------------------------------
		//-----------------------------------------------------------------
        u8* ImageProvider::RGBA8888ToLUM8(u8* in_data, u32 in_area)
		{
			u32 udwBytesPerPixel = 1;
			
            u8* pubyBitmapData8 = (u8*)calloc(in_area, udwBytesPerPixel);
            u32* pPixel32 = (u32*)in_data;
            u8* pPixel8 = (u8*)pubyBitmapData8;
            
            //We only take over the lumanance, discard the alpha (if any)
			for(u32 i=0; i<in_area; ++i, ++pPixel32)
			{
				*pPixel8++ = ((*pPixel32)& 0xFF);
			}
            
			return pubyBitmapData8;
		}
	}
}
