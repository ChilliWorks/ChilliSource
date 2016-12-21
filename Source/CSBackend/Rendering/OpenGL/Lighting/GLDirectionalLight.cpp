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

#include <CSBackend/Rendering/OpenGL/Lighting/GLDirectionalLight.h>

#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>
#include <CSBackend/Rendering/OpenGL/Texture/GLTextureUnitManager.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            const std::string k_uniformLightCol = "u_lightCol";
            const std::string k_uniformLightDir = "u_lightDir";
            const std::string k_uniformShadowMap = "u_shadowMap";
            const std::string k_uniformShadowTolerance = "u_shadowTolerance";
            const std::string k_uniformLightMat = "u_lightMat";
        }
        
        //------------------------------------------------------------------------------
        GLDirectionalLight::GLDirectionalLight(const ChilliSource::Colour& colour, const ChilliSource::Vector3& direction, const ChilliSource::Matrix4& lightViewProjection, f32 shadowTolerance,
                                               const ChilliSource::RenderTexture* shadowMapRenderTexture) noexcept
            : m_colour(colour), m_direction(direction), m_lightViewProjection(lightViewProjection), m_shadowTolerance(shadowTolerance), m_shadowMapRenderTexture(shadowMapRenderTexture)
        {
        }
            
        //------------------------------------------------------------------------------
        void GLDirectionalLight::Apply(GLShader* glShader, GLTextureUnitManager* glTextureUnitManager) const noexcept
        {
            glShader->SetUniform(k_uniformLightCol, m_colour, GLShader::FailurePolicy::k_silent);
            glShader->SetUniform(k_uniformLightDir, m_direction, GLShader::FailurePolicy::k_silent);
            
            if (m_shadowMapRenderTexture)
            {
                auto texUnit = glTextureUnitManager->BindAdditional(GL_TEXTURE_2D, m_shadowMapRenderTexture);
                
                glShader->SetUniform(k_uniformShadowMap, s32(texUnit), GLShader::FailurePolicy::k_silent);
                glShader->SetUniform(k_uniformShadowTolerance, m_shadowTolerance, GLShader::FailurePolicy::k_silent);
                glShader->SetUniform(k_uniformLightMat, m_lightViewProjection, GLShader::FailurePolicy::k_silent);
            }
        }
    }
}
