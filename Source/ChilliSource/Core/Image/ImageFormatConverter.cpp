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

#include <ChilliSource/Core/Base/ByteColour.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace ImageFormatConverter
        {
#ifdef CS_TARGETPLATFORM_WINDOWS
			//------------------------------------------------
			//------------------------------------------------
			ImageBuffer::ImageBuffer(ImageBuffer&& in_other)
			{
				m_data = std::move(in_other.m_data);
				m_size = in_other.m_size;
			}
#endif
            //---------------------------------------------------
            //---------------------------------------------------
            void RGBA8888ToRGB888(Image* in_image)
            {
                CS_ASSERT(in_image->GetFormat() == ImageFormat::k_RGBA8888 && in_image->GetCompression() == ImageCompression::k_none, "Cannot convert an image that is not in uncompressed RGBA8888 format.");
                
                ImageBuffer rawBuffer = RGBA8888ToRGB888(in_image->GetData(), in_image->GetDataSize());
            
                Image::Descriptor desc;
                desc.m_width = in_image->GetWidth();
                desc.m_height = in_image->GetHeight();
                desc.m_dataSize = rawBuffer.m_size;
                desc.m_compression = in_image->GetCompression();
                desc.m_format = ImageFormat::k_RGB888;
                in_image->Build(desc, std::move(rawBuffer.m_data));
            }
            //---------------------------------------------------
            //---------------------------------------------------
            void RGBA8888ToRGBA4444(Image* in_image)
            {
            	CS_ASSERT(in_image->GetFormat() == ImageFormat::k_RGBA8888 && in_image->GetCompression() == ImageCompression::k_none, "Cannot convert an image that is not in uncompressed RGBA8888 format.");
                
            	ImageBuffer rawBuffer = RGBA8888ToRGBA4444(in_image->GetData(), in_image->GetDataSize());

				Image::Descriptor desc;
				desc.m_width = in_image->GetWidth();
				desc.m_height = in_image->GetHeight();
				desc.m_dataSize = rawBuffer.m_size;
				desc.m_compression = in_image->GetCompression();
				desc.m_format = ImageFormat::k_RGBA4444;
				in_image->Build(desc, std::move(rawBuffer.m_data));
            }
            //---------------------------------------------------
            //---------------------------------------------------
            void RGBA8888ToRGB565(Image* in_image)
            {
            	CS_ASSERT(in_image->GetFormat() == ImageFormat::k_RGBA8888 && in_image->GetCompression() == ImageCompression::k_none, "Cannot convert an image that is not in uncompressed RGBA8888 format.");
                
            	ImageBuffer rawBuffer = RGBA8888ToRGB565(in_image->GetData(), in_image->GetDataSize());

				Image::Descriptor desc;
				desc.m_width = in_image->GetWidth();
				desc.m_height = in_image->GetHeight();
				desc.m_dataSize = rawBuffer.m_size;
				desc.m_compression = in_image->GetCompression();
				desc.m_format = ImageFormat::k_RGB565;
				in_image->Build(desc, std::move(rawBuffer.m_data));
            }
            //---------------------------------------------------
            //---------------------------------------------------
            void RGBA8888ToLumA88(Image* in_image)
            {
            	CS_ASSERT(in_image->GetFormat() == ImageFormat::k_RGBA8888 && in_image->GetCompression() == ImageCompression::k_none, "Cannot convert an image that is not in uncompressed RGBA8888 format.");
                
            	ImageBuffer rawBuffer = RGBA8888ToLumA88(in_image->GetData(), in_image->GetDataSize());

				Image::Descriptor desc;
				desc.m_width = in_image->GetWidth();
				desc.m_height = in_image->GetHeight();
				desc.m_dataSize = rawBuffer.m_size;
				desc.m_compression = in_image->GetCompression();
				desc.m_format = ImageFormat::k_LumA88;
				in_image->Build(desc, std::move(rawBuffer.m_data));
            }
            //---------------------------------------------------
            //---------------------------------------------------
            void RGBA8888ToLum8(Image* in_image)
            {
            	CS_ASSERT(in_image->GetFormat() == ImageFormat::k_RGBA8888 && in_image->GetCompression() == ImageCompression::k_none, "Cannot convert an image that is not in uncompressed RGBA8888 format.");
                
            	ImageBuffer rawBuffer = RGBA8888ToLum8(in_image->GetData(), in_image->GetDataSize());

				Image::Descriptor desc;
				desc.m_width = in_image->GetWidth();
				desc.m_height = in_image->GetHeight();
				desc.m_dataSize = rawBuffer.m_size;
				desc.m_compression = in_image->GetCompression();
				desc.m_format = ImageFormat::k_Lum8;
				in_image->Build(desc, std::move(rawBuffer.m_data));
            }
            //---------------------------------------------------
			//---------------------------------------------------
            ImageBuffer RGBA8888ToRGB888(const u8* in_imageData, u32 in_imageDataSize)
			{
            	CS_ASSERT(in_imageDataSize > 0 && in_imageDataSize % 4 == 0, "Invalid input image data size.");

				struct PixelRGB888
				{
					u8 m_r;
					u8 m_g;
					u8 m_b;
				};

                const u32 k_outputBytesPerPixel = 3;
                const u32 area = in_imageDataSize / 4;

                ImageBuffer outputBuffer;
                outputBuffer.m_size = area * k_outputBytesPerPixel;
                outputBuffer.m_data = std::unique_ptr<u8[]>(new u8[outputBuffer.m_size]);

                const ByteColour* pixel32 = reinterpret_cast<const ByteColour*>(in_imageData);
                PixelRGB888* pixel24 = reinterpret_cast<PixelRGB888*>(outputBuffer.m_data.get());

                for(u32 i = 0; i < area; ++i, ++pixel32, ++pixel24)
                {
                	pixel24->m_r = pixel32->r;
                	pixel24->m_g = pixel32->g;
                	pixel24->m_b = pixel32->b;
                }

                return std::move(outputBuffer);
			}
			//---------------------------------------------------
			//---------------------------------------------------
            ImageBuffer RGBA8888ToRGBA4444(const u8* in_imageData, u32 in_imageDataSize)
			{
            	CS_ASSERT(in_imageDataSize > 0 && in_imageDataSize % 4 == 0, "Invalid input image data size.");

				const u32 k_outputBytesPerPixel = 2;
				const u32 area = in_imageDataSize / 4;

                ImageBuffer outputBuffer;
                outputBuffer.m_size = area * k_outputBytesPerPixel;
                outputBuffer.m_data = std::unique_ptr<u8[]>(new u8[outputBuffer.m_size]);

                const u32* pixel32 = reinterpret_cast<const u32*>(in_imageData);
                u16* pixel16 = reinterpret_cast<u16*>(outputBuffer.m_data.get());

				for(u32 i = 0; i < area; ++i, ++pixel32, ++pixel16)
				{
					*pixel16 = ((((*pixel32 >> 0) & 0xFF) >> 4) << 12) | // R
						((((*pixel32 >> 8) & 0xFF) >> 4) << 8) | // G
						((((*pixel32 >> 16) & 0xFF) >> 4) << 4) | // B
						((((*pixel32 >> 24) & 0xFF) >> 4) << 0); // A
				}

				return std::move(outputBuffer);
			}
			//---------------------------------------------------
			//---------------------------------------------------
            ImageBuffer RGBA8888ToRGB565(const u8* in_imageData, u32 in_imageDataSize)
			{
            	CS_ASSERT(in_imageDataSize > 0 && in_imageDataSize % 4 == 0, "Invalid input image data size.");

            	const u32 k_outputBytesPerPixel = 2;
				const u32 area = in_imageDataSize / 4;

                ImageBuffer outputBuffer;
                outputBuffer.m_size = area * k_outputBytesPerPixel;
                outputBuffer.m_data = std::unique_ptr<u8[]>(new u8[outputBuffer.m_size]);

                const u32* pixel32 = reinterpret_cast<const u32*>(in_imageData);
                u16* pixel16 = reinterpret_cast<u16*>(outputBuffer.m_data.get());

                for(u32 i = 0; i < area; ++i, ++pixel32, ++pixel16)
                {
                    *pixel16 = ((((*pixel32 >> 0) & 0xFF) >> 3) << 11) |
                    	((((*pixel32 >> 8) & 0xFF) >> 2) << 5) |
                    	((((*pixel32 >> 16) & 0xFF) >> 3) << 0);
                }

                return std::move(outputBuffer);
			}
			//---------------------------------------------------
			//---------------------------------------------------
            ImageBuffer RGBA8888ToLumA88(const u8* in_imageData, u32 in_imageDataSize)
			{
            	CS_ASSERT(in_imageDataSize > 0 && in_imageDataSize % 4 == 0, "Invalid input image data size.");

            	const u32 k_outputBytesPerPixel = 2;
				const u32 area = in_imageDataSize / 4;

                ImageBuffer outputBuffer;
                outputBuffer.m_size = area * k_outputBytesPerPixel;
                outputBuffer.m_data = std::unique_ptr<u8[]>(new u8[outputBuffer.m_size]);

                const u32* pixel32 = reinterpret_cast<const u32*>(in_imageData);
                u16* pixel16 = reinterpret_cast<u16*>(outputBuffer.m_data.get());

                for(u32 i = 0; i < area; ++i, ++pixel32, ++pixel16)
                {
                    *pixel16 = ((*pixel32) & 0xFF) | // L
                    	((*pixel32 >> 16) & 0xFF00); // A
                }

                return std::move(outputBuffer);
			}
			//---------------------------------------------------
			//---------------------------------------------------
            ImageBuffer RGBA8888ToLum8(const u8* in_imageData, u32 in_imageDataSize)
			{
            	CS_ASSERT(in_imageDataSize > 0 && in_imageDataSize % 4 == 0, "Invalid input image data size.");

            	const u32 k_outputBytesPerPixel = 1;
				const u32 area = in_imageDataSize / 4;

                ImageBuffer outputBuffer;
                outputBuffer.m_size = area * k_outputBytesPerPixel;
                outputBuffer.m_data = std::unique_ptr<u8[]>(new u8[outputBuffer.m_size]);

                const u32* pixel32 = reinterpret_cast<const u32*>(in_imageData);
                u8* pixel8 = outputBuffer.m_data.get();

				for(u32 i = 0; i < area; ++i, ++pixel32, ++pixel8)
				{
					*pixel8 = ((*pixel32)& 0xFF);
				}

				return std::move(outputBuffer);
			}
        }
    }
}
