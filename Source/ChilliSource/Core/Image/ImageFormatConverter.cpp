//
//  ImageFormatConverter.cpp
//  moFlow
//
//  Created by Ian Copland on 10/04/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Image/ImageFormatConverter.h>
#include <ChilliSource/Core/Image/Image.h>

namespace moFlo
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
        void RGBA8888ToRGB888(Core::CImage* inpImage)
        {
            MOFLOW_ASSERT(inpImage->GetFormat() == Core::CImage::Format::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
            
            u8* pOriginalImageData = (u8*)inpImage->GetData();
            u32 udwBytesPerPixel = 3;
            u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
			u8* pubyBitmapData888 = (u8*)malloc(udwArea * udwBytesPerPixel);
			PixelRGBA8888* pPixel32 = (PixelRGBA8888*)pOriginalImageData;
			PixelRGB888* pPixel24 = (PixelRGB888*)pubyBitmapData888;
			
			for(u32 i=0; i<udwArea; ++i, ++pPixel32)
			{
				pPixel24->mbyR = pPixel32->mbyR;
                pPixel24->mbyG = pPixel32->mbyG;
                pPixel24->mbyB = pPixel32->mbyB;
			}
            
            inpImage->SetData(pubyBitmapData888);
            inpImage->SetFormat(Core::CImage::Format::k_RGB888);
            free(pOriginalImageData);
        }
        //---------------------------------------------------
        /// RGBA8888 To RGBA4444
        //---------------------------------------------------
        void RGBA8888ToRGBA4444(Core::CImage* inpImage)
        {
            MOFLOW_ASSERT(inpImage->GetFormat() == Core::CImage::Format::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
            
            u8* pOriginalImageData = (u8*)inpImage->GetData();
            u32 udwBytesPerPixel = 2;
            u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
			u8* pubyBitmapData4444 = (u8*)malloc(udwArea * udwBytesPerPixel);
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
            
            inpImage->SetData(pubyBitmapData4444);
            inpImage->SetFormat(Core::CImage::Format::k_RGBA4444);
            free(pOriginalImageData);
        }
        //---------------------------------------------------
        /// RGBA8888 To RGB565
        //---------------------------------------------------
        void RGBA8888ToRGB565(Core::CImage* inpImage)
        {
            MOFLOW_ASSERT(inpImage->GetFormat() == Core::CImage::Format::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
            
            u8* pOriginalImageData = (u8*)inpImage->GetData();
            u32 udwBytesPerPixel = 2;
			u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
			u8* pubyBitmapData565 = (u8*)malloc(udwArea * udwBytesPerPixel);
			u32* pPixel32 = (u32*)pOriginalImageData;
			u16* pPixel16 = (u16*)pubyBitmapData565;
			
			for(u32 i=0; i<udwArea; ++i, ++pPixel32)
			{
				*pPixel16++ = ((((*pPixel32 >> 0) & 0xFF) >> 3) << 11) |
				((((*pPixel32 >> 8) & 0xFF) >> 2) << 5) |
				((((*pPixel32 >> 16) & 0xFF) >> 3) << 0);
			}
            
            inpImage->SetData(pubyBitmapData565);
            inpImage->SetFormat(Core::CImage::Format::k_RGB565);
            free(pOriginalImageData);
        }
        //---------------------------------------------------
        /// RGBA8888 To LUMA88
        //---------------------------------------------------
        void RGBA8888ToLUMA88(Core::CImage* inpImage)
        {
            MOFLOW_ASSERT(inpImage->GetFormat() == Core::CImage::Format::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
            
            u8* pOriginalImageData = (u8*)inpImage->GetData();
			u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
			u32 udwBytesPerPixel = 2;
			u8* pubyBitmapData88 = (u8*)malloc(udwArea * udwBytesPerPixel);
			u32* pPixel32 = (u32*)pOriginalImageData;
			u16* pPixel16 = (u16*)pubyBitmapData88;
			
			for(u32 i=0; i<udwArea; ++i, ++pPixel32)
			{
				*pPixel16++ =
				((*pPixel32)& 0xFF) | // L
				((*pPixel32 >> 16) & 0xFF00); // A
			}
            
            inpImage->SetData(pubyBitmapData88);
            inpImage->SetFormat(Core::CImage::Format::k_LumA88);
            free(pOriginalImageData);
        }
        //---------------------------------------------------
        /// RGBA8888 To LUM8
        //---------------------------------------------------
        void RGBA8888ToLUM8(Core::CImage* inpImage)
        {
            MOFLOW_ASSERT(inpImage->GetFormat() == Core::CImage::Format::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
            
            u8* pOriginalImageData = (u8*)inpImage->GetData();
			u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
			u32 udwBytesPerPixel = 1;
            u8* pubyBitmapData8 = (u8*)malloc(udwArea * udwBytesPerPixel);
            u32* pPixel32 = (u32*)pOriginalImageData;
            u8* pPixel8 = (u8*)pubyBitmapData8;
            
            //We only take over the lumanance, discard the alpha (if any)
			for(u32 i=0; i<udwArea; ++i, ++pPixel32)
			{
				*pPixel8++ =
                ((*pPixel32)& 0xFF);
			}
            
            inpImage->SetData(pubyBitmapData8);
            inpImage->SetFormat(Core::CImage::Format::k_Lum8);
            free(pOriginalImageData);
        }
        //---------------------------------------------------
        /// RGBA8888 To Depth 32
        //---------------------------------------------------
        void RGBA8888ToDepth32(Core::CImage* inpImage)
        {
            MOFLOW_ASSERT(inpImage->GetFormat() == Core::CImage::Format::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
            
            inpImage->SetFormat(Core::CImage::Format::k_Depth32);
        }
        //---------------------------------------------------
        /// RGBA8888 To Depth 16
        //---------------------------------------------------
        void RGBA8888ToDepth16(Core::CImage* inpImage)
        {
            MOFLOW_ASSERT(inpImage->GetFormat() == Core::CImage::Format::k_RGBA8888, "Cannot convert an image that is not in RGBA8888 format.");
            
            u8* pOriginalImageData = (u8*)inpImage->GetData();
            u32 udwBytesPerPixel = 2;
            u32 udwArea = inpImage->GetWidth() * inpImage->GetHeight();
			u8* pubyBitmapData16 = (u8*)malloc(udwArea * udwBytesPerPixel);
			u32* pPixel32 = (u32*)pOriginalImageData;
			u16* pPixel16 = (u16*)pubyBitmapData16;
			
			for(u32 i=0; i<udwArea; ++i, ++pPixel32)
			{
				*pPixel16++ =
				((((*pPixel32 >> 0) & 0xFF) >> 4) << 12) |
				((((*pPixel32 >> 8) & 0xFF) >> 4) << 8) |
				((((*pPixel32 >> 16) & 0xFF) >> 4) << 4) |
				((((*pPixel32 >> 24) & 0xFF) >> 4) << 0);
			}
            
            inpImage->SetData(pubyBitmapData16);
            inpImage->SetFormat(Core::CImage::Format::k_Depth16);
            free(pOriginalImageData);
        }
    }
}