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
#include <CSBackend/Rendering/OpenGL/Texture/GLCubemap.h>

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
            /// @param textureData
            ///     Texture data for each cubemap face
            /// @param dataSize
            ///     Size of the data for any face (as face textures are all the same dimensions and format)
            /// @param renderTexture
            ///     Settings for all cubemap textures
            ///
            /// @return Handle to the cubemap
            ///
            GLuint BuildCubemap(const std::array<std::unique_ptr<const u8[]>, 6>& textureData, u32 dataSize, const ChilliSource::RenderTexture* renderTexture) noexcept
            {
                GLuint handle;
                glGenTextures(1, &handle);
                
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
                
                const auto& dimensions = renderTexture->GetDimensions();
                
                u32 faceIndex = 0;
                for(const auto& data : textureData)
                {
                    GLenum faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex;
                    
                    switch(renderTexture->GetImageCompression())
                    {
                        case ChilliSource::ImageCompression::k_none:
                            GLTextureUtils::UploadImageDataNoCompression(faceTarget, renderTexture->GetImageFormat(), dimensions, data.get());
                            break;
                        case ChilliSource::ImageCompression::k_ETC1:
                            GLTextureUtils::UploadImageDataETC1(faceTarget, renderTexture->GetImageFormat(), dimensions, data.get(), dataSize);
                            break;
                        case ChilliSource::ImageCompression::k_PVR2Bpp:
                            GLTextureUtils::UploadImageDataPVR2(faceTarget, renderTexture->GetImageFormat(), dimensions, data.get(), dataSize);
                            break;
                        case ChilliSource::ImageCompression::k_PVR4Bpp:
                            GLTextureUtils::UploadImageDataPVR4(faceTarget, renderTexture->GetImageFormat(), dimensions, data.get(), dataSize);
                            break;
                    };
                    
                    ++faceIndex;
                }
                
                if(renderTexture->IsMipmapped())
                {
                    CS_ASSERT(ChilliSource::MathUtils::IsPowerOfTwo(dimensions.x) && ChilliSource::MathUtils::IsPowerOfTwo(dimensions.y), "Mipmapped images must be a power of two.");
                    
                    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
                }
                
                GLTextureUtils::ApplyFilterMode(GL_TEXTURE_CUBE_MAP, renderTexture->GetFilterMode(), renderTexture->IsMipmapped());
                GLTextureUtils::ApplyWrapMode(GL_TEXTURE_CUBE_MAP, renderTexture->GetWrapModeS(), renderTexture->GetWrapModeT());
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while building cubemap.");
                
                return handle;
            }
        }
        
        //------------------------------------------------------------------------------
        GLCubemap::GLCubemap(const std::array<std::unique_ptr<const u8[]>, 6>& textureData, u32 dataSize, const ChilliSource::RenderTexture* renderTexture) noexcept
        : m_imageDataSize(dataSize), m_renderTexture(renderTexture)
        {
#ifdef CS_ENABLE_DEBUG
            auto renderCapabilities = ChilliSource::Application::Get()->GetSystem<ChilliSource::RenderCapabilities>();
            auto dimensions = m_renderTexture->GetDimensions();
            CS_ASSERT(u32(dimensions.x) <= renderCapabilities->GetMaxTextureSize() && u32(dimensions.y) <= renderCapabilities->GetMaxTextureSize(), "OpenGL does not support textures of this size on this device (" + ChilliSource::ToString(dimensions.x) + ", " + ChilliSource::ToString(dimensions.y) + ")");
#endif
            m_handle = BuildCubemap(textureData, dataSize, m_renderTexture);
            
            if(k_shouldBackupMeshDataFromMemory && renderTexture->ShouldBackupData())
            {
                for(std::size_t i=0; i<textureData.size(); ++i)
                {
                    u8* imageDataCopy = new u8[dataSize];
                    memcpy(imageDataCopy, textureData[i].get(), dataSize);
                    m_imageDataBackup[i] = std::unique_ptr<const u8[]>(imageDataCopy);
                }
            }
        }
        
        //------------------------------------------------------------------------------
        void GLCubemap::Restore() noexcept
        {
            if(m_invalidData)
            {
                m_handle = BuildCubemap(m_imageDataBackup, m_imageDataSize, m_renderTexture);

                m_invalidData = false;
            }
        }
        
        //------------------------------------------------------------------------------
        GLCubemap::~GLCubemap() noexcept
        {
            if(!m_invalidData)
            {
                glDeleteTextures(1, &m_handle);
            }
        }
    }
}
