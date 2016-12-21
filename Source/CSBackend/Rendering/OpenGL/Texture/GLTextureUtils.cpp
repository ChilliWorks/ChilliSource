//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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
#include <CSBackend/Rendering/OpenGL/Texture/GLTextureUtils.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageFormatConverter.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>
#include <ChilliSource/Rendering/Texture/TextureFilterMode.h>
#include <ChilliSource/Rendering/Texture/TextureWrapMode.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace GLTextureUtils
        {
            //------------------------------------------------------------------------------
            void UploadImageDataNoCompression(GLenum target, ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData)
            {
                switch(format)
                {
                    default:
                    case ChilliSource::ImageFormat::k_RGBA8888:
                        glTexImage2D(target, 0, GL_RGBA, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB888:
                        glTexImage2D(target, 0, GL_RGB, dimensions.x, dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGBA4444:
                        glTexImage2D(target, 0, GL_RGBA, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB565:
                        glTexImage2D(target, 0, GL_RGB, dimensions.x, dimensions.y, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_LumA88:
                        glTexImage2D(target, 0, GL_LUMINANCE_ALPHA, dimensions.x, dimensions.y, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_Lum8:
                        glTexImage2D(target, 0, GL_LUMINANCE, dimensions.x, dimensions.y, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_Depth16:
                        glTexImage2D(target, 0, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_Depth32:
                        glTexImage2D(target, 0, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, imageData);
                        break;
                };
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading uncompressed texture data.");
            }
            
            //------------------------------------------------------------------------------
            void UploadImageDataETC1(GLenum target, ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize)
            {
#ifndef CS_TARGETPLATFORM_ANDROID
                CS_LOG_FATAL("ETC1 compression is only supported on Android");
#endif
                CS_ASSERT(format == ChilliSource::ImageFormat::k_RGB888, "ETC1 only supports RGB image format");
                
#ifdef CS_TARGETPLATFORM_ANDROID
                glCompressedTexImage2D(target, 0, GL_ETC1_RGB8_OES, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading ETC1 texture data.");
            }
            
            //------------------------------------------------------------------------------
            void UploadImageDataPVR2(GLenum target, ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize)
            {
#ifndef CS_TARGETPLATFORM_IOS
                CS_LOG_FATAL("PVR compression is only supported on iOS");
#endif
                
#ifdef CS_TARGETPLATFORM_IOS
                switch(format)
                {
                    default:
                        CS_LOG_FATAL("PVR compression only supports RGB and RGBA image formats");
                        break;
                    case ChilliSource::ImageFormat::k_RGBA8888:
                        glCompressedTexImage2D(target, 0, GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB888:
                        glCompressedTexImage2D(target, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
                        break;
                };
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading PVR2 texture data.");
            }
            
            //------------------------------------------------------------------------------
            void UploadImageDataPVR4(GLenum target, ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize)
            {
#ifndef CS_TARGETPLATFORM_IOS
                CS_LOG_FATAL("PVR compression is only supported on iOS");
#endif
                
#ifdef CS_TARGETPLATFORM_IOS
                switch(format)
                {
                    default:
                        CS_LOG_FATAL("PVR compression only supports RGB and RGBA image formats");
                        break;
                    case ChilliSource::ImageFormat::k_RGBA8888:
                        glCompressedTexImage2D(target, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB888:
                        glCompressedTexImage2D(target, 0, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
                        break;
                };
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading PVR4 texture data.");
            }
            
            //------------------------------------------------------------------------------
            void ApplyWrapMode(GLenum target, ChilliSource::TextureWrapMode wrapModeS, ChilliSource::TextureWrapMode wrapModeT)
            {
                switch(wrapModeS)
                {
                    case ChilliSource::TextureWrapMode::k_clamp:
                        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        break;
                    case ChilliSource::TextureWrapMode::k_repeat:
                        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        break;
                };
                
                switch(wrapModeT)
                {
                    case ChilliSource::TextureWrapMode::k_clamp:
                        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        break;
                    case ChilliSource::TextureWrapMode::k_repeat:
                        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        break;
                };
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while applying texture wrap mode.");
            }
            
            //------------------------------------------------------------------------------
            void ApplyFilterMode(GLenum target, ChilliSource::TextureFilterMode filterMode, bool mipmappingEnabled)
            {
                if(mipmappingEnabled == false)
                {
                    switch(filterMode)
                    {
                        case ChilliSource::TextureFilterMode::k_nearest:
                            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            break;
                        case ChilliSource::TextureFilterMode::k_bilinear:
                            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            break;
                    }
                }
                else
                {
                    switch(filterMode)
                    {
                        case ChilliSource::TextureFilterMode::k_nearest:
                            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            break;
                        case ChilliSource::TextureFilterMode::k_bilinear:
                            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            break;
                    }
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while applying texture filter mode.");
            }
        }
    }
}
