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
#include <CSBackend/Rendering/OpenGL/Texture/GLTextureUtils.h>

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
        namespace
        {
#ifdef CS_TARGETPLATFORM_ANDROID
            //Should maintain memory backups on android to restore data when the context
            //is lost when dealing with meshes that are not loaded from file.
            const bool k_shouldBackupMeshDataFromMemory = true;
#else
            const bool k_shouldBackupMeshDataFromMemory = false;
#endif
    
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
                        GLTextureUtils::UploadImageDataNoCompression(GL_TEXTURE_2D, renderTexture->GetImageFormat(), dimensions, data);
                        break;
                    case ChilliSource::ImageCompression::k_ETC1:
                        GLTextureUtils::UploadImageDataETC1(GL_TEXTURE_2D, renderTexture->GetImageFormat(), dimensions, data, dataSize);
                        break;
                    case ChilliSource::ImageCompression::k_PVR2Bpp:
                        GLTextureUtils::UploadImageDataPVR2(GL_TEXTURE_2D, renderTexture->GetImageFormat(), dimensions, data, dataSize);
                        break;
                    case ChilliSource::ImageCompression::k_PVR4Bpp:
                        GLTextureUtils::UploadImageDataPVR4(GL_TEXTURE_2D, renderTexture->GetImageFormat(), dimensions, data, dataSize);
                        break;
                };
                
                if(renderTexture->IsMipmapped())
                {
                    CS_ASSERT(ChilliSource::MathUtils::IsPowerOfTwo(dimensions.x) && ChilliSource::MathUtils::IsPowerOfTwo(dimensions.y), "Mipmapped images must be a power of two.");
                    
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                
                GLTextureUtils::ApplyFilterMode(GL_TEXTURE_2D, renderTexture->GetFilterMode(), renderTexture->IsMipmapped());
                GLTextureUtils::ApplyWrapMode(GL_TEXTURE_2D, renderTexture->GetWrapModeS(), renderTexture->GetWrapModeT());
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while building texture.");
                
                return handle;
            }
        }
        
        //------------------------------------------------------------------------------
        GLTexture::GLTexture(const u8* data, u32 dataSize, const ChilliSource::RenderTexture* renderTexture) noexcept
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
        void GLTexture::UpdateRestorableBackup() noexcept
        {
            if(k_shouldBackupMeshDataFromMemory && m_renderTexture->ShouldBackupData())
            {
                //create an bind a new frame buffer.
                GLuint frameBufferHandle = 0;
                glGenFramebuffers(1, &frameBufferHandle);
                glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);
                
                //attach the texture
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_handle, 0);
                GLuint glCheck = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if(glCheck != GL_FRAMEBUFFER_COMPLETE)
                {
                    CS_LOG_FATAL("Framebuffer incomplete while updating texture restoration data.");
                }
                
                //read the data from the texture. This can only be read back in RGBA8888 format so it will need
                //to be converted back to the format of the texture.
                u32 unconvertedDataSize = m_renderTexture->GetDimensions().x * m_renderTexture->GetDimensions().y * 4;
                std::unique_ptr<u8[]> unconvertedData(new u8[unconvertedDataSize]);
                glReadPixels(0, 0, m_renderTexture->GetDimensions().x, m_renderTexture->GetDimensions().y, GL_RGBA, GL_UNSIGNED_BYTE, unconvertedData.get());
                
                //Convert to the format of this texture
                ChilliSource::ImageFormatConverter::ImageBuffer convertedData;
                switch (m_renderTexture->GetImageFormat())
                {
                    case ChilliSource::ImageFormat::k_RGBA8888:
                        convertedData.m_size = unconvertedDataSize;
                        convertedData.m_data = std::move(unconvertedData);
                        break;
                    case ChilliSource::ImageFormat::k_RGB888:
                        convertedData = CS::ImageFormatConverter::RGBA8888ToRGB888(unconvertedData.get(), unconvertedDataSize);
                        unconvertedData.reset();
                        break;
                    case ChilliSource::ImageFormat::k_RGBA4444:
                        convertedData = CS::ImageFormatConverter::RGBA8888ToRGBA4444(unconvertedData.get(), unconvertedDataSize);
                        unconvertedData.reset();
                        break;
                    case ChilliSource::ImageFormat::k_RGB565:
                        convertedData = CS::ImageFormatConverter::RGBA8888ToRGB565(unconvertedData.get(), unconvertedDataSize);
                        unconvertedData.reset();
                        break;
                    default:
                        CS_LOG_FATAL("Texture is not in a restorable format. The restorable texture data option must be disabled for this texture.");
                        break;
                }
                
                m_imageDataSize = convertedData.m_size;
                m_imageDataBackup = std::move(convertedData.m_data);
                
                //clean up the frame buffer.
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glDeleteFramebuffers(1, &frameBufferHandle);
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while updating texture restoration data.");
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
