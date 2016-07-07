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
#include <CSBackend/Rendering/OpenGL/Texture/GLTexture.h>

#include <ChilliSource/Rendering/Texture/RenderTexture.h>

namespace CSBackend
{
    namespace OpenGL
    {
        //------------------------------------------------------------------------------
        GLTextureUnitManager::GLTextureUnitManager() noexcept
        {
            s32 numTextureUnits = 0;
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numTextureUnits);
            
            m_boundTextures.reserve(numTextureUnits);
            for (s32 i = 0; i < numTextureUnits; ++i)
            {
                m_boundTextures.push_back(nullptr);
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting up the texture unit manager.");
        }
        
        //------------------------------------------------------------------------------
        void GLTextureUnitManager::Bind(const std::vector<const ChilliSource::RenderTexture*>& textures) noexcept
        {
            for (u32 textureUnitIndex = 0; textureUnitIndex < u32(textures.size()); ++textureUnitIndex)
            {
                if (m_boundTextures[textureUnitIndex] != textures[textureUnitIndex])
                {
                    m_boundTextures[textureUnitIndex] = textures[textureUnitIndex];
                    
                    auto glTexture = reinterpret_cast<GLTexture*>(m_boundTextures[textureUnitIndex]->GetExtraData());
                    
                    glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
                    glBindTexture(GL_TEXTURE_2D, glTexture->GetHandle());
                }
            }
            
            glActiveTexture(GL_TEXTURE0);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding textures.");
        }
        
        //------------------------------------------------------------------------------
        void GLTextureUnitManager::Reset() noexcept
        {
            for (auto& texture : m_boundTextures)
            {
                texture = nullptr;
            }
        }
    }
}
