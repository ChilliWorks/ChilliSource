//
//  PNGImageProvider.cpp
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

#ifdef CS_TARGETPLATFORM_IOS

#include <CSBackend/Platform/iOS/Core/Image/PNGImageProvider.h>

#include <CSBackend/Platform/iOS/Core/File/FileSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#import <UIKit/UIKit.h>

namespace CSBackend
{
	namespace iOS
	{
        namespace
        {
            const std::string k_pngExtension("png");
            
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
            void CreatePNGImageFromFile(const s8* in_data, u32 in_dataSize, CSCore::Image* out_image)
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
                CSCore::ImageFormat format = CSCore::ImageFormat::k_RGBA8888;
                if(CGColorSpaceGetModel(ColorSpaceInfo) == kCGColorSpaceModelMonochrome)
                {
                    if(bHasAlpha)
                    {
                        format = CSCore::ImageFormat::k_LumA88;
                        ColorSpaceInfo = CGColorSpaceCreateDeviceRGB();
                    }
                    else
                    {
                        format = CSCore::ImageFormat::k_Lum8;
                        ColorSpaceInfo = CGColorSpaceCreateDeviceRGB();
                    }
                }
                
                // Allocated memory needed for the bitmap context
                u32 dataSize = udwArea * udwBytesPerPixel;
                u8* pubyBitmapData8888 = new u8[dataSize];
                memset(pubyBitmapData8888, 0, dataSize);

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

                CSCore::Image::Descriptor desc;
                desc.m_compression = CSCore::ImageCompression::k_none;
                desc.m_format = format;
                desc.m_width = udwWidth;
                desc.m_height = udwHeight;
                desc.m_dataSize = dataSize;
                out_image->Build(desc, CSCore::Image::ImageDataUPtr(pubyBitmapData8888));
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
            void LoadImage(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, const CSCore::ResourceProvider::AsyncLoadDelegate& in_delegate, const CSCore::ResourceSPtr& out_resource)
            {
                CSCore::FileStreamSPtr pImageFile = CSCore::Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filePath, CSCore::FileMode::k_readBinary);
                
                if(pImageFile == nullptr || pImageFile->IsBad() == true)
                {
                    out_resource->SetLoadState(CSCore::Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
                        CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                    }
                    return;
                }
                
                std::string abyData;
                pImageFile->GetAll(abyData);
                
                CreatePNGImageFromFile(abyData.data(), abyData.size(), (CSCore::Image*)out_resource.get());
                
                out_resource->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                if(in_delegate != nullptr)
                {
                    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
            }
        }
        CS_DEFINE_NAMEDTYPE(PNGImageProvider);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool PNGImageProvider::IsA(CSCore::InterfaceIDType in_interaceId) const
		{
			return (in_interaceId == CSCore::ResourceProvider::InterfaceID || in_interaceId == CSCore::PNGImageProvider::InterfaceID || in_interaceId == PNGImageProvider::InterfaceID);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        CSCore::InterfaceIDType PNGImageProvider::GetResourceType() const
        {
            return CSCore::Image::InterfaceID;
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool PNGImageProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_pngExtension;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void PNGImageProvider::CreateResourceFromFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, const CSCore::IResourceOptionsBaseCSPtr& in_options, const CSCore::ResourceSPtr& out_resource)
		{
            LoadImage(in_storageLocation, in_filePath, nullptr, out_resource);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void PNGImageProvider::CreateResourceFromFileAsync(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, const CSCore::IResourceOptionsBaseCSPtr& in_options, const CSCore::ResourceProvider::AsyncLoadDelegate& in_delegate, const CSCore::ResourceSPtr& out_resource)
        {
            auto task = std::bind(LoadImage, in_storageLocation, in_filePath, in_delegate, out_resource);
            
            CSCore::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
	}
}

#endif