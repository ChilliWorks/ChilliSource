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

#include <CSBackend/Rendering/OpenGL/Lighting/GLPointLight.h>

#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            const std::string k_uniformLightCol = "u_lightCol";
            const std::string k_uniformLightPos = "u_lightPos";
            const std::string k_uniformAttenuationConstant = "u_attenuationConstant";
            const std::string k_uniformAttenuationLinear = "u_attenuationLinear";
            const std::string k_uniformAttenuationQuadratic = "u_attenuationQuadratic";
        }
        
        //------------------------------------------------------------------------------
        GLPointLight::GLPointLight(const ChilliSource::Colour& colour, const ChilliSource::Vector3& position, const ChilliSource::Vector3& attenuation) noexcept
            : m_colour(colour), m_position(position), m_attenuation(attenuation)
        {
        }
        
        //------------------------------------------------------------------------------
        void GLPointLight::Apply(GLShader* glShader, GLTextureUnitManager* glTextureUnitManager) const noexcept
        {
            glShader->SetUniform(k_uniformLightCol, m_colour, GLShader::FailurePolicy::k_silent);
            glShader->SetUniform(k_uniformLightPos, m_position, GLShader::FailurePolicy::k_silent);
            glShader->SetUniform(k_uniformAttenuationConstant, m_attenuation.x, GLShader::FailurePolicy::k_silent);
            glShader->SetUniform(k_uniformAttenuationLinear, m_attenuation.y, GLShader::FailurePolicy::k_silent);
            glShader->SetUniform(k_uniformAttenuationQuadratic, m_attenuation.z, GLShader::FailurePolicy::k_silent);
        }
    }
}
