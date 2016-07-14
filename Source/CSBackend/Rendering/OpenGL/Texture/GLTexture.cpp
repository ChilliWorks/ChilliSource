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
#include <CSBackend/Rendering/OpenGL/Texture/GLTexture.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>
#include <ChilliSource/Rendering/Texture/TextureFilterMode.h>
#include <ChilliSource/Rendering/Texture/TextureWrapMode.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            //Should maintain memory backups on android to restore data when the context
            //is lost when dealing with meshes that are not loaded from file.
            const bool k_shouldBackupMeshDataFromMemory = true;
#else
            const bool k_shouldBackupMeshDataFromMemory = false;
#endif
    
            /// Uploads the given uncompressed image data to texture memory.
            ///
            /// @param format
            ///     The format of the image data.
            /// @param dimensions
            ///     The image dimensions.
            /// @param imageData
            ///     The image data.
            ///
            void UploadImageDataNoCompression(ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData)
            {
                switch(format)
                {
                    default:
                    case ChilliSource::ImageFormat::k_RGBA8888:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB888:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dimensions.x, dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGBA4444:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions.x, dimensions.y, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB565:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dimensions.x, dimensions.y, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_LumA88:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, dimensions.x, dimensions.y, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_Lum8:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, dimensions.x, dimensions.y, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_Depth16:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_Depth32:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, imageData);
                        break;
                };
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading uncompressed texture data.");
            }
            
            /// Uploads the given ETC1 image data to texture memory.
            ///
            /// @param format
            ///     The format of the image data.
            /// @param dimensions
            ///     The image dimensions.
            /// @param imageData
            ///     The image data.
            /// @param imageDataSize
            ///     The size, in bytes, of the image data.
            ///
            void UploadImageDataETC1(ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize)
            {
#ifndef CS_TARGETPLATFORM_ANDROID
                CS_LOG_FATAL("ETC1 compression is only supported on Android");
#endif
                CS_ASSERT(format == ChilliSource::ImageFormat::k_RGB888, "ETC1 only supports RGB image format");
                
#ifdef CS_TARGETPLATFORM_ANDROID
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading ETC1 texture data.");
            }
            
            /// Uploads the given PVR2 image data to texture memory.
            ///
            /// @param format
            ///     The format of the image data.
            /// @param dimensions
            ///     The image dimensions.
            /// @param imageData
            ///     The image data.
            /// @param imageDataSize
            ///     The size, in bytes, of the image data.
            ///
            void UploadImageDataPVR2(ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize)
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
                        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB888:
                        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
                        break;
                };
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading PVR2 texture data.");
            }
            
            /// Uploads the given PVR4 image data to texture memory.
            ///
            /// @param format
            ///     The format of the image data.
            /// @param dimensions
            ///     The image dimensions.
            /// @param imageData
            ///     The image data.
            /// @param imageDataSize
            ///     The size, in bytes, of the image data.
            ///
            void UploadImageDataPVR4(ChilliSource::ImageFormat format, const ChilliSource::Integer2& dimensions, const u8* imageData, u32 imageDataSize)
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
                        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB888:
                        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, dimensions.x, dimensions.y, 0, imageDataSize, imageData);
                        break;
                };
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while uploading PVR4 texture data.");
            }
            
            /// Applies the given wrap mode to the currently bound texture.
            ///
            /// @param wrapModeS
            ///     The s-coordinate wrap mode of the texture.
            /// @param wrapModeT
            ///     The t-coordinate wrap mode of the texture.
            ///
            void ApplyWrapMode(ChilliSource::TextureWrapMode wrapModeS, ChilliSource::TextureWrapMode wrapModeT)
            {
                switch(wrapModeS)
                {
                    case ChilliSource::TextureWrapMode::k_clamp:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        break;
                    case ChilliSource::TextureWrapMode::k_repeat:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        break;
                };
                
                switch(wrapModeT)
                {
                    case ChilliSource::TextureWrapMode::k_clamp:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        break;
                    case ChilliSource::TextureWrapMode::k_repeat:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        break;
                };
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while applying texture wrap mode.");
            }
            
            /// Applies the given texture filter mode to the currently bound texture.
            ///
            /// @param filterMode
            ///     The filter mode of the texutre
            /// @param mipmappingEnabled
            ///     Whether or not mipmapping is enabled for the texture.
            ///
            void ApplyFilterMode(ChilliSource::TextureFilterMode filterMode, bool mipmappingEnabled)
            {
                if(mipmappingEnabled == false)
                {
                    switch(filterMode)
                    {
                        case ChilliSource::TextureFilterMode::k_nearest:
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            break;
                        case ChilliSource::TextureFilterMode::k_bilinear:
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            break;
                    }
                }
                else
                {
                    switch(filterMode)
                    {
                        case ChilliSource::TextureFilterMode::k_nearest:
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            break;
                        case ChilliSource::TextureFilterMode::k_bilinear:
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            break;
                    }
                }
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while applying texture filter mode.");
            }
            
            /// Creates a new OpenGL texture with the given texture data.
            ///
            /// @param data
            ///     The texture data.
            /// @param dataSize
            ///     The size of the texture data.
            /// @param renderTexture
            ///     The RenderTexture containing image format data.
            ///
            /// @return Handle to the texture
            ///
            GLuint BuildTexture(const u8* data, u32 dataSize, const ChilliSource::RenderTexture* renderTexture) noexcept
            {
                GLuint handle;
                glGenTextures(1, &handle);
                
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, handle);
                
                const auto& dimensions = renderTexture->GetDimensions();
                
                switch(renderTexture->GetImageCompression())
                {
                    case ChilliSource::ImageCompression::k_none:
                        UploadImageDataNoCompression(renderTexture->GetImageFormat(), dimensions, data);
                        break;
                    case ChilliSource::ImageCompression::k_ETC1:
                        UploadImageDataETC1(renderTexture->GetImageFormat(), dimensions, data, dataSize);
                        break;
                    case ChilliSource::ImageCompression::k_PVR2Bpp:
                        UploadImageDataPVR2(renderTexture->GetImageFormat(), dimensions, data, dataSize);
                        break;
                    case ChilliSource::ImageCompression::k_PVR4Bpp:
                        UploadImageDataPVR4(renderTexture->GetImageFormat(), dimensions, data, dataSize);
                        break;
                };
                
                if(renderTexture->IsMipmapped())
                {
                    CS_ASSERT(ChilliSource::MathUtils::IsPowerOfTwo(dimensions.x) && ChilliSource::MathUtils::IsPowerOfTwo(dimensions.y), "Mipmapped images must be a power of two.");
                    
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                
                ApplyFilterMode(renderTexture->GetFilterMode(), renderTexture->IsMipmapped());
                ApplyWrapMode(renderTexture->GetWrapModeS(), renderTexture->GetWrapModeT());
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while building texture.");
                
                return handle;
            }
        }
        
        //------------------------------------------------------------------------------
        GLTexture::GLTexture(const u8* data, u32 dataSize, ChilliSource::RenderTexture* renderTexture) noexcept
            :m_imageDataSize(dataSize), m_renderTexture(renderTexture)
        {
#ifdef CS_ENABLE_DEBUG
            auto renderCapabilities = ChilliSource::Application::Get()->GetSystem<ChilliSource::RenderCapabilities>();
            auto dimensions = renderTexture->GetDimensions();
            CS_ASSERT(u32(dimensions.x) <= renderCapabilities->GetMaxTextureSize() && u32(dimensions.y) <= renderCapabilities->GetMaxTextureSize(),
                      "OpenGL does not support textures of this size on this device (" + ChilliSource::ToString(dimensions.x) + ", " + ChilliSource::ToString(dimensions.y) + ")");
#endif
            m_handle = BuildTexture(data, dataSize, m_renderTexture);
            
            if(k_shouldBackupMeshDataFromMemory && renderTexture->ShouldBackupData() && data)
            {
                u8* imageDataCopy = new u8[dataSize];
                memcpy(imageDataCopy, data, dataSize);
                m_imageDataBackup = std::unique_ptr<const u8[]>(imageDataCopy);
            }
        }
        
        //------------------------------------------------------------------------------
        void GLTexture::Restore() noexcept
        {
            if(m_invalidData)
            {
                if(m_imageDataBackup)
                {
                    m_handle = BuildTexture(m_imageDataBackup.get(), m_imageDataSize, m_renderTexture);
                }
                else
                {
                    m_handle = BuildTexture(nullptr, 0, m_renderTexture);
                }
                m_invalidData = false;
            }
        }
        
        //------------------------------------------------------------------------------
        GLTexture::~GLTexture() noexcept
        {
            if(!m_invalidData)
            {
                glDeleteTextures(1, &m_handle);
            }
        }
    }
}
