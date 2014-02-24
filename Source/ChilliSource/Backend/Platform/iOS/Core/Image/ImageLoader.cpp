/*
 *  ImageLoader.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */



#include <ChilliSource/Backend/Platform/iOS/Core/Image/ImageLoader.h>

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Base/Screen.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>

#import <UIKit/UIKit.h>

namespace ChilliSource
{
	namespace iOS
	{
		const std::string PNGExtension("png");
		const std::string JPGExtension("jpg");
		const std::string JPEGExtension("jpeg");
		const std::string PVRExtension("pvr");
	
		//----------------------------------------------------------------
		/// Is A
		///
		/// Is the object of the given interface type
		/// @param Interface type to query
		/// @return Whether the object is of given type
		//----------------------------------------------------------------
		bool ImageLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Can Create Resource Of Kind
		///
		/// @param Resource to compare against
		/// @return Whether the object can load a resource of given type
		//----------------------------------------------------------------
		bool ImageLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Core::CImage::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Can Create Resource From File With Extension
		///
		/// @param Extension to compare against
		/// @return Whether the object can load a resource with that extension
		//----------------------------------------------------------------
		bool ImageLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == PNGExtension || inExtension == JPGExtension || inExtension == JPEGExtension || inExtension == PVRExtension);
		}
		//----------------------------------------------------------------
		/// Create Resource From File
		///
        /// @param The storage location to load from
		/// @param File path to resource
		/// @param Out: Resource
		/// @return Whether the resource loaded 
		//----------------------------------------------------------------
		bool ImageLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			return CreateImageFromFile(ineStorageLocation, inFilePath, Core::CImage::Format::k_default, outpResource);
		}
		//----------------------------------------------------------------
		/// Create Image From File
		///
        /// @param The storage location to load from
		/// @param File path to resource
		/// @param Image format
		/// @param Out: Resource
		/// @return Whether the resource loaded 
		//----------------------------------------------------------------
		bool ImageLoader::CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, Core::ResourcePtr& outpResource)
		{
			Core::FileStreamPtr pImageFile = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);
   
            if(pImageFile && !pImageFile->IsBad())
			{
                std::string strName;
                std::string strExtension;
                
                //Get the name of the file and append the high res identifier to it
                Core::CStringUtils::SplitBaseFilename(inFilePath, strName, strExtension);
                
                //Based on the extension decide how to load the file
                std::string abyData;
                pImageFile->GetAll(abyData);
                
                if(strExtension == PNGExtension)
                {
                    CreatePNGImageFromFile(abyData.data(), abyData.size(), ineFormat, (Core::CImage*)outpResource.get());
					return true;
                }
                else if(strExtension == PVRExtension)
                {
                    CreatePVRImageFromFile(abyData.data(), abyData.size(), (Core::CImage*)outpResource.get());
					return true;
                }
                else if(strExtension == JPGExtension || strExtension == JPEGExtension)	
                {
                    WARNING_LOG("JPG image loading is not cross-platform and will only work on iOS. Do not use PNGs in a cross-platform project.");
                    CreateJPGImageFromFile(abyData.data(), abyData.size(), ineFormat, (Core::CImage*)outpResource.get());
					return true;
                }
            }
            
            return false;
		}
		//----------------------------------------------------------------
		/// Create PNG Image From File
		///
		/// Create a PNG image using the Apple API
		///
		/// @param Image data in bytes
		/// @param Size of data in bytes
		/// @param Whether the asset is high res
		/// @param Image format
		/// @param Out: Image resource
		//----------------------------------------------------------------
		void ImageLoader::CreatePNGImageFromFile(const s8* inpbyData, u32 inudwDataSize, Core::CImage::Format ineFormat, Core::CImage* outpImage)
		{
            CFDataRef pData = CFDataCreateWithBytesNoCopy(NULL, (u8*)inpbyData, inudwDataSize, kCFAllocatorNull);
            CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData(pData);
            CGImageRef cgImage = CGImageCreateWithPNGDataProvider(imgDataProvider, NULL, true, kCGRenderingIntentDefault);
            
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
				if(bHasAlpha && ineFormat != Core::CImage::Format::k_Lum8)
				{
					ineFormat = Core::CImage::Format::k_LumA88;
					ColorSpaceInfo = CGColorSpaceCreateDeviceRGB();
				}
				else 
				{
					ineFormat = Core::CImage::Format::k_Lum8;
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

	
			if(ineFormat == Core::CImage::Format::k_default)
			{
				ineFormat = meDefaultFormat;
			}
			
			//We always load the image as RGBA_8888 but we convert on the fly to the correct format
			switch(ineFormat)
			{
				case Core::CImage::Format::k_RGBA4444:
				{
					DEBUG_LOG("Converting to RGBA_4444");
				
					u8* pubyBitmapData4444 = RGBA8888ToRGBA4444(pubyBitmapData8888, udwArea);
			
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData4444;
					break;
				}
				case Core::CImage::Format::k_RGB565:
				{
					DEBUG_LOG("Converting to RGBA_565");
					
					u8* pubyBitmapData565 = RGBA8888ToRGB565(pubyBitmapData8888, udwArea);
					
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData565;
					break;
				}
				case Core::CImage::Format::k_LumA88:
				{
					DEBUG_LOG("Converting to LUMA_88");
					
					u8* pubyBitmapData88 = RGBA8888ToLUM88(pubyBitmapData8888, udwArea);
					
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData88;
					break;
				}
                case Core::CImage::Format::k_Lum8:
                {
                    DEBUG_LOG("Converting to LUM_8");
                    
                    u8* pubyBitmapData8 = RGBA8888ToLUM8(pubyBitmapData8888, udwArea);
                    
                    free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData8;
                    break;
                }
                    
                default:
                    break;
			};
			
			outpImage->SetData(pubyBitmapData8888);
			outpImage->SetWidth(udwWidth);
			outpImage->SetHeight(udwHeight);
			outpImage->SetFormat(ineFormat);
		}
        //----------------------------------------------------------------
        /// Create JPG Image From File
        ///
        /// Create a JPG image using the Apple API
        ///
		/// @param Image data in bytes
		/// @param Size of data in bytes
		/// @param Whether the asset is high res
		/// @param Image format
		/// @param Out: Image resource
        //----------------------------------------------------------------
        void ImageLoader::CreateJPGImageFromFile(const s8* inpbyData, u32 inudwDataSize, Core::CImage::Format ineFormat, Core::CImage* outpImage)
        {
            CFDataRef pData = CFDataCreateWithBytesNoCopy(NULL, (u8*)inpbyData, inudwDataSize, kCFAllocatorNull);
            CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData(pData);
            CGImageRef cgImage = CGImageCreateWithJPEGDataProvider(imgDataProvider, NULL, true, kCGRenderingIntentDefault);
            
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
			switch(ineFormat)
			{
				case Core::CImage::Format::k_RGBA4444:
				{
					DEBUG_LOG("Converting to RGBA_4444");
					
					u8* pubyBitmapData4444 = RGBA8888ToRGBA4444(pubyBitmapData8888, udwArea);
					
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData4444;
					break;
				}
				case Core::CImage::Format::k_RGB565:
				{
					DEBUG_LOG("Converting to RGB_565");
					
					u8* pubyBitmapData565 = RGBA8888ToRGB565(pubyBitmapData8888, udwArea);
					
					free(pubyBitmapData8888);
					pubyBitmapData8888 = pubyBitmapData565;
					break;
				}
                    
                default:
                    break;
			};
			
			outpImage->SetData(pubyBitmapData8888);
			outpImage->SetWidth(udwWidth);
			outpImage->SetHeight(udwHeight);
			outpImage->SetFormat(ineFormat);
        }
		//----------------------------------------------------------------
		/// Create PVR Image From File
		///
		/// Create an image in raw byte format
		///
		/// @param Image data in bytes
		/// @param Size of data in bytes
		/// @param Whether the asset is high res
		/// @param Out: Image resource
		//----------------------------------------------------------------
		void ImageLoader::CreatePVRImageFromFile(const s8* inpbyData, u32 inudwDataSize, Core::CImage* outpImage)
		{
            s8* pData = (s8*)malloc(inudwDataSize);
            memcpy(pData, inpbyData, sizeof(s8) * inudwDataSize);
          
			outpImage->SetData((u8*)pData);
            outpImage->UnpackPVRData();
			outpImage->SetFormat(Core::CImage::Format::k_RGBA8888);
		}
		//----------------------------------------------------------------
		/// RGBA8888 To RGB565
		///
		/// Downsize the image from 32 bits to 16 bits no alpha
		///
		/// @param RGBA8888 data
		/// @param Width x Height
		/// @return RGB565 data
		//-----------------------------------------------------------------
		u8* ImageLoader::RGBA8888ToRGB565(u8* inpData, u32 inudwArea)
		{
			u32 udwBytesPerPixel = 2;
			
			u8* pubyBitmapData565 = (u8*)calloc(inudwArea, udwBytesPerPixel);
			u32* pPixel32 = (u32*)inpData;
			u16* pPixel16 = (u16*)pubyBitmapData565;
			
			for(u32 i=0; i<inudwArea; ++i, ++pPixel32)
			{
				*pPixel16++ = ((((*pPixel32 >> 0) & 0xFF) >> 3) << 11) | 
				((((*pPixel32 >> 8) & 0xFF) >> 2) << 5) |
				((((*pPixel32 >> 16) & 0xFF) >> 3) << 0);
			}
			
			return pubyBitmapData565;
		}
		//----------------------------------------------------------------
		/// RGBA8888 To RGBA4444
		///
		/// Downsize the image from 32 bits to 16 bits
		///
		/// @param RGBA8888 data
		/// @param Width x Height
		/// @return RGBA4444 data
		//-----------------------------------------------------------------
		u8* ImageLoader::RGBA8888ToRGBA4444(u8* inpData, u32 inudwArea)
		{
			u32 udwBytesPerPixel = 2;
			
			u8* pubyBitmapData4444 = (u8*)calloc(inudwArea, udwBytesPerPixel);
			u32* pPixel32 = (u32*)inpData;
			u16* pPixel16 = (u16*)pubyBitmapData4444;
			
			for(u32 i=0; i<inudwArea; ++i, ++pPixel32)
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
		/// RGBA8888 To LUM88
		///
		/// Downsize the image from 32 bits to 16 bits
		///
		/// @param RGBA8888 data
		/// @param Width x Height
		/// @return LUM88 data
		//-----------------------------------------------------------------
		u8* ImageLoader::RGBA8888ToLUM88(u8* inpData, u32 inudwArea)
		{
			u32 udwBytesPerPixel = 2;
			
			u8* pubyBitmapData88 = (u8*)calloc(inudwArea, udwBytesPerPixel);
			u32* pPixel32 = (u32*)inpData;
			u16* pPixel16 = (u16*)pubyBitmapData88;
			
			for(u32 i=0; i<inudwArea; ++i, ++pPixel32)
			{
				*pPixel16++ = 
				((*pPixel32)& 0xFF) | // L
				((*pPixel32 >> 16) & 0xFF00); // A
			}
			
			return pubyBitmapData88;
		}
   
        //----------------------------------------------------------------
		/// RGBA8888 To LUM8
		///
		/// Downsize the image from 32 bits to 8 bits
		///
		/// @param RGBA8888 data
		/// @param Width x Height
		/// @return LUM8 data
		//-----------------------------------------------------------------
        u8* ImageLoader::RGBA8888ToLUM8(u8* inpData, u32 inudwArea)
		{
			u32 udwBytesPerPixel = 1;
			
            u8* pubyBitmapData8 = (u8*)calloc(inudwArea, udwBytesPerPixel);
            u32* pPixel32 = (u32*)inpData;
            u8* pPixel8 = (u8*)pubyBitmapData8;
            
            //We only take over the lumanance, discard the alpha (if any)
			for(u32 i=0; i<inudwArea; ++i, ++pPixel32)
			{
				*pPixel8++ = 
                ((*pPixel32)& 0xFF);//| // L
				//((*pPixel32 >> 8) & 0xFF00); // A
			}
            
			return pubyBitmapData8;
		}
	}
}
