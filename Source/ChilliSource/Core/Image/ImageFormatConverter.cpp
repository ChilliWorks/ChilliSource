//
//  ImageFormatConverter.cpp
//  Chilli Source
//  Created by Ian Copland on 10/04/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#include <ChilliSource/Core/Image/ImageFormatConverter.h>

#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageFormat.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace ImageFormatConverter
        {
            //=======================================
            /// PixelRGBA8888
            ///
            /// Container for a single RGBA8888 pixel
            //=======================================
            struct PixelRGBA8888
            {
                u8 mbyR;
                u8 mbyG;
                u8 mbyB;
                u8 mbyA;
            };
            //=======================================
            /// PixelRGB888
            ///
            /// Container for a single RGB888 pixel
            //=======================================
            struct PixelRGB888
            {
                u8 mbyR;
                u8 mbyG;
                u8 mbyB;
            };
            //---------------------------------------------------
            /// RGBA8888 To RGB888
            //---------------------------------------------------
            void RGBA8888ToRGB888(Image* inpImage)
            {
                CS_ASSERT(inpImage->GetFormat() == ImageFormat::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
                
                u8* pOriginalImageData = (u8*)inpImage->GetData();
                u32 udwBytesPerPixel = 3;
                u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
                u8* pubyBitmapData888 = new u8[udwArea * udwBytesPerPixel];
                PixelRGBA8888* pPixel32 = (PixelRGBA8888*)pOriginalImageData;
                PixelRGB888* pPixel24 = (PixelRGB888*)pubyBitmapData888;
                
                for(u32 i=0; i<udwArea; ++i, ++pPixel32)
                {
                    pPixel24->mbyR = pPixel32->mbyR;
                    pPixel24->mbyG = pPixel32->mbyG;
                    pPixel24->mbyB = pPixel32->mbyB;
                }
            
                Image::Descriptor desc;
                desc.m_width = inpImage->GetWidth();
                desc.m_height = inpImage->GetHeight();
                desc.m_dataSize = inpImage->GetDataSize();
                desc.m_compression = inpImage->GetCompression();
                desc.m_format = ImageFormat::k_RGB888;
                inpImage->Build(desc, Image::ImageDataUPtr(pubyBitmapData888));
            }
            //---------------------------------------------------
            /// RGBA8888 To RGBA4444
            //---------------------------------------------------
            void RGBA8888ToRGBA4444(Image* inpImage)
            {
                CS_ASSERT(inpImage->GetFormat() == ImageFormat::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
                
                u8* pOriginalImageData = (u8*)inpImage->GetData();
                u32 udwBytesPerPixel = 2;
                u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
                u8* pubyBitmapData4444 = new u8[udwArea * udwBytesPerPixel];
                u32* pPixel32 = (u32*)pOriginalImageData;
                u16* pPixel16 = (u16*)pubyBitmapData4444;
                
                for(u32 i=0; i<udwArea; ++i, ++pPixel32)
                {
                    *pPixel16++ =
                    ((((*pPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
                    ((((*pPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
                    ((((*pPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
                    ((((*pPixel32 >> 24) & 0xFF) >> 4) << 0); // A
                }
                
                Image::Descriptor desc;
                desc.m_width = inpImage->GetWidth();
                desc.m_height = inpImage->GetHeight();
                desc.m_dataSize = inpImage->GetDataSize();
                desc.m_compression = inpImage->GetCompression();
                desc.m_format = ImageFormat::k_RGBA4444;
                inpImage->Build(desc, Image::ImageDataUPtr(pubyBitmapData4444));
            }
            //---------------------------------------------------
            /// RGBA8888 To RGB565
            //---------------------------------------------------
            void RGBA8888ToRGB565(Image* inpImage)
            {
                CS_ASSERT(inpImage->GetFormat() == ImageFormat::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
                
                u8* pOriginalImageData = (u8*)inpImage->GetData();
                u32 udwBytesPerPixel = 2;
                u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
                u8* pubyBitmapData565 = new u8[udwArea * udwBytesPerPixel];
                u32* pPixel32 = (u32*)pOriginalImageData;
                u16* pPixel16 = (u16*)pubyBitmapData565;
                
                for(u32 i=0; i<udwArea; ++i, ++pPixel32)
                {
                    *pPixel16++ = ((((*pPixel32 >> 0) & 0xFF) >> 3) << 11) |
                    ((((*pPixel32 >> 8) & 0xFF) >> 2) << 5) |
                    ((((*pPixel32 >> 16) & 0xFF) >> 3) << 0);
                }
            
                Image::Descriptor desc;
                desc.m_width = inpImage->GetWidth();
                desc.m_height = inpImage->GetHeight();
                desc.m_dataSize = inpImage->GetDataSize();
                desc.m_compression = inpImage->GetCompression();
                desc.m_format = ImageFormat::k_RGB565;
                inpImage->Build(desc, Image::ImageDataUPtr(pubyBitmapData565));
            }
            //---------------------------------------------------
            /// RGBA8888 To LUMA88
            //---------------------------------------------------
            void RGBA8888ToLUMA88(Image* inpImage)
            {
                CS_ASSERT(inpImage->GetFormat() == ImageFormat::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
                
                u8* pOriginalImageData = (u8*)inpImage->GetData();
                u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
                u32 udwBytesPerPixel = 2;
                u8* pubyBitmapData88 = new u8[udwArea * udwBytesPerPixel];
                u32* pPixel32 = (u32*)pOriginalImageData;
                u16* pPixel16 = (u16*)pubyBitmapData88;
                
                for(u32 i=0; i<udwArea; ++i, ++pPixel32)
                {
                    *pPixel16++ =
                    ((*pPixel32)& 0xFF) | // L
                    ((*pPixel32 >> 16) & 0xFF00); // A
                }
                
                Image::Descriptor desc;
                desc.m_width = inpImage->GetWidth();
                desc.m_height = inpImage->GetHeight();
                desc.m_dataSize = inpImage->GetDataSize();
                desc.m_compression = inpImage->GetCompression();
                desc.m_format = ImageFormat::k_LumA88;
                inpImage->Build(desc, Image::ImageDataUPtr(pubyBitmapData88));
            }
            //---------------------------------------------------
            /// RGBA8888 To LUM8
            //---------------------------------------------------
            void RGBA8888ToLUM8(Image* inpImage)
            {
                CS_ASSERT(inpImage->GetFormat() == ImageFormat::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
                
                u8* pOriginalImageData = (u8*)inpImage->GetData();
                u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
                u32 udwBytesPerPixel = 1;
                u8* pubyBitmapData8 = new u8[udwArea * udwBytesPerPixel];
                u32* pPixel32 = (u32*)pOriginalImageData;
                u8* pPixel8 = (u8*)pubyBitmapData8;
                
                //We only take over the lumanance, discard the alpha (if any)
                for(u32 i=0; i<udwArea; ++i, ++pPixel32)
                {
                    *pPixel8++ =
                    ((*pPixel32)& 0xFF);
                }
                
                Image::Descriptor desc;
                desc.m_width = inpImage->GetWidth();
                desc.m_height = inpImage->GetHeight();
                desc.m_dataSize = inpImage->GetDataSize();
                desc.m_compression = inpImage->GetCompression();
                desc.m_format = ImageFormat::k_Lum8;
                inpImage->Build(desc, Image::ImageDataUPtr(pubyBitmapData8));
            }
        }
    }
}