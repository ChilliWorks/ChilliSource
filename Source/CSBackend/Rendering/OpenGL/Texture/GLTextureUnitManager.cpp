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

#include <CSBackend/Rendering/OpenGL/Texture/GLTextureUnitManager.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Texture/GLCubemap.h>
#include <CSBackend/Rendering/OpenGL/Texture/GLTexture.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>

namespace CSBackend
{
    namespace OpenGL
    {
        //------------------------------------------------------------------------------
        GLTextureUnitManager::GLTextureUnitManager() noexcept
        {
            u32 numTextureUnits = CS::Application::Get()->GetSystem<CS::RenderCapabilities>()->GetNumTextureUnits();
 
            m_boundTextures.reserve(numTextureUnits);
            for (u32 i = 0; i < numTextureUnits; ++i)
            {
                m_boundTextures.push_back(nullptr);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting up the texture unit manager.");
        }
        
        //------------------------------------------------------------------------------
        void GLTextureUnitManager::Bind(GLenum target, const std::vector<const ChilliSource::RenderTexture*>& textures, u32 startingBindIdx) noexcept
        {
            for(std::size_t i=0; i<textures.size(); ++i)
            {
                auto textureUnitIndex = startingBindIdx + i;
                
                if (m_boundTextures[textureUnitIndex] != textures[i])
                {
                    m_boundTextures[textureUnitIndex] = textures[i];
                    
                    glActiveTexture((GLenum)(GL_TEXTURE0 + textureUnitIndex));
                    
                    switch(target)
                    {
                        case GL_TEXTURE_2D:
                        {
                            auto glTexture = static_cast<GLTexture*>(m_boundTextures[textureUnitIndex]->GetExtraData());
                            
                            CS_ASSERT(glTexture, "Cannot bind a texture which hasn't been loaded.");
                            CS_ASSERT(!glTexture->IsDataInvalid(), "GLTextureUnitManager::Bind(): Failed to bind texture, its context is invalid!");
                            glBindTexture(target, glTexture->GetHandle());
                            break;
                        }
                        case GL_TEXTURE_CUBE_MAP:
                        {
                            auto glCubemap = static_cast<GLCubemap*>(m_boundTextures[textureUnitIndex]->GetExtraData());
                            
                            CS_ASSERT(glCubemap, "Cannot bind a cubemap which hasn't been loaded.");
                            CS_ASSERT(!glCubemap->IsDataInvalid(), "GLTextureUnitManager::Bind(): Failed to bind cubemap, its context is invalid!");
                            glBindTexture(target, glCubemap->GetHandle());
                            break;
                        }
                    }
                }
            }
            
            glActiveTexture(GL_TEXTURE0);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding textures.");
        }
        
        //------------------------------------------------------------------------------
        GLuint GLTextureUnitManager::BindAdditional(GLenum target, const ChilliSource::RenderTexture* texture) noexcept
        {
            GLuint textureIndex = GetBoundOrAvailableUnit(texture);
            
            if (m_boundTextures[textureIndex] != texture)
            {
                m_boundTextures[textureIndex] = texture;
                
                glActiveTexture((GLenum)GL_TEXTURE0 + textureIndex);
                
                switch(target)
                {
                    case GL_TEXTURE_2D:
                    {
                        auto glTexture = static_cast<GLTexture*>(texture->GetExtraData());
                        CS_ASSERT(glTexture, "Cannot bind a texture which hasn't been loaded.");
                        glBindTexture(target, glTexture->GetHandle());
                        break;
                    }
                    case GL_TEXTURE_CUBE_MAP:
                    {
                        auto glCubemap = static_cast<GLCubemap*>(texture->GetExtraData());
                        CS_ASSERT(glCubemap, "Cannot bind a cubemap which hasn't been loaded.");
                        glBindTexture(target, glCubemap->GetHandle());
                        break;
                    }
                }
            }
        
            glActiveTexture(GL_TEXTURE0);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding an additional texture.");
            
            return textureIndex;
        }
        
        //------------------------------------------------------------------------------
        void GLTextureUnitManager::Reset() noexcept
        {
            for (auto& texture : m_boundTextures)
            {
                texture = nullptr;
            }
        }
        
        //------------------------------------------------------------------------------
        GLint GLTextureUnitManager::GetBoundOrAvailableUnit(const ChilliSource::RenderTexture* texture) const noexcept
        {
            GLint freeIndex = -1;
            
            for(u32 i=0; i<m_boundTextures.size(); ++i)
            {
                if(m_boundTextures[i] == texture)
                {
                    return (GLint)i;
                }
                
                if(m_boundTextures[i] == nullptr && freeIndex == -1)
                {
                    freeIndex = (GLint)i;
                }
            }
            
            CS_ASSERT(freeIndex >= 0, "No free GL texture units");
            return freeIndex;
        }
    }
}
