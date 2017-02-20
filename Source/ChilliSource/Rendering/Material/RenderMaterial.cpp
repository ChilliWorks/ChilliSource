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

#include <ChilliSource/Rendering/Material/RenderMaterial.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderMaterial::RenderMaterial(const RenderShader* renderShader, std::vector<const RenderTexture*> renderTextures2D, std::vector<const RenderTexture*> renderTexturesCubemap,
                                   bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled, bool isFaceCullingEnabled, bool isStencilTestEnabled,
                                   TestFunc depthTestFunc,
                                   BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                                   StencilOp stencilFailOp, StencilOp stencilDepthFailOp, StencilOp stencilPassOp, TestFunc stencilTestFunc, s32 stencilRef, u32 stencilMask,
                                   CullFace cullFace,
                                   const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour, const Colour& specularColour,
                                   RenderShaderVariablesUPtr renderShaderVariables) noexcept
    :   m_renderShader(renderShader), m_renderTextures2D(std::move(renderTextures2D)), m_renderTexturesCubemap(std::move(renderTexturesCubemap)),
            m_isTransparencyEnabled(isTransparencyEnabled), m_isColourWriteEnabled(isColourWriteEnabled), m_isDepthWriteEnabled(isDepthWriteEnabled), m_isDepthTestEnabled(isDepthTestEnabled), m_isFaceCullingEnabled(isFaceCullingEnabled), m_isStencilTestEnabled(isStencilTestEnabled),
            m_depthTestFunc(depthTestFunc),
            m_sourceBlendMode(sourceBlendMode), m_destinationBlendMode(destinationBlendMode),
            m_stencilFailOp(stencilFailOp), m_stencilDepthFailOp(stencilDepthFailOp), m_stencilPassOp(stencilPassOp), m_stencilTestFunc(stencilTestFunc), m_stencilTestFuncRef(stencilRef), m_stencilTestFuncMask(stencilMask),
            m_cullFace(cullFace), m_emissiveColour(emissiveColour), m_ambientColour(ambientColour), m_diffuseColour(diffuseColour), m_specularColour(specularColour), m_renderShaderVariables(std::move(renderShaderVariables))
    {
    }
}
