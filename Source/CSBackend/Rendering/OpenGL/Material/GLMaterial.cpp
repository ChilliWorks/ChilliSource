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

#include <CSBackend/Rendering/OpenGL/Material/GLMaterial.h>

#include <CSBackend/Rendering/OpenGL/Camera/GLCamera.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLShader.h>

#include <ChilliSource/Rendering/Base/BlendMode.h>
#include <ChilliSource/Rendering/Material/RenderMaterial.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            /// Converts from a ChilliSource blend mode to a OpenGL blend mode.
            ///
            /// @param blendMode
            ///     The ChilliSource blend mode.
            ///
            /// @return The OpenGL blend mode.
            ///
            GLenum ToGLBlendMode(ChilliSource::BlendMode blendMode)
            {
                switch(blendMode)
                {
                    case ChilliSource::BlendMode::k_zero:
                        return GL_ZERO;
                    case ChilliSource::BlendMode::k_one:
                        return GL_ONE;
                    case ChilliSource::BlendMode::k_sourceCol:
                        return GL_SRC_COLOR;
                    case ChilliSource::BlendMode::k_oneMinusSourceCol:
                        return GL_ONE_MINUS_SRC_COLOR;
                    case ChilliSource::BlendMode::k_sourceAlpha:
                        return GL_SRC_ALPHA;
                    case ChilliSource::BlendMode::k_oneMinusSourceAlpha:
                        return GL_ONE_MINUS_SRC_ALPHA;
                    case ChilliSource::BlendMode::k_destAlpha:
                        return GL_DST_ALPHA;
                    case ChilliSource::BlendMode::k_oneMinusDestAlpha:
                        return GL_ONE_MINUS_DST_ALPHA;
                    default:
                        CS_LOG_FATAL("Invalid blend mode.");
                        return GL_ZERO;
                };
            }
        }
        
        //------------------------------------------------------------------------------
        void GLMaterial::Apply(const ChilliSource::RenderMaterial* renderMaterial, const GLCamera& glCamera, GLShader* glShader) noexcept
        {
            if (renderMaterial->IsDepthTestEnabled())
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
            
            if (renderMaterial->IsDepthWriteEnabled())
            {
                glDepthMask(GL_TRUE);
            }
            else
            {
                glDepthMask(GL_FALSE);
            }
            
            if (renderMaterial->IsColourWriteEnabled())
            {
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            }
            else
            {
                glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            }
            
            if (renderMaterial->IsFaceCullingEnabled())
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
            
            if (renderMaterial->IsTransparencyEnabled())
            {
                glEnable(GL_BLEND);
                glBlendFunc(ToGLBlendMode(renderMaterial->GetSourceBlendMode()), ToGLBlendMode(renderMaterial->GetDestinationBlendMode()));
            }
            else
            {
                glDisable(GL_BLEND);
            }
            
            glShader->SetUniform(GLShader::k_defaultUniformEmissive, renderMaterial->GetEmissiveColour(), GLShader::FailurePolicy::k_silent);
            glShader->SetUniform(GLShader::k_defaultUniformAmbient, renderMaterial->GetAmbientColour(), GLShader::FailurePolicy::k_silent);
            glShader->SetUniform(GLShader::k_defaultUniformDiffuse, renderMaterial->GetDiffuseColour(), GLShader::FailurePolicy::k_silent);
            glShader->SetUniform(GLShader::k_defaultUniformSpecular, renderMaterial->GetSpecularColour(), GLShader::FailurePolicy::k_silent);
            
            glShader->SetUniform(GLShader::k_defaultUniformCameraPos, glCamera.GetPosition(), GLShader::FailurePolicy::k_silent);
        }
    }
}
