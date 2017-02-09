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

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_RENDERMATERIAL_H_
#define _CHILLISOURCE_RENDERING_MATERIAL_RENDERMATERIAL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/Shader/RenderShaderVariables.h>

#include <vector>

namespace ChilliSource
{
    /// A container for information which the renderer needs pertaining to a single material.
    ///
    /// This is immutable and therefore thread-safe, aside from the extra data pointer
    /// which should only be accessed on the render thread.
    ///
    class RenderMaterial final
    {
    public:
        CS_DECLARE_NOCOPY(RenderMaterial);
        
        /// Creates a new instance with the given material data.
        ///
        /// @param renderShader
        ///     The shader applied by this material.
        /// @param renderTextures2D
        ///     The list of textures applied by this material. Should be moved in
        /// @param renderTexturesCubemap
        ///     The list of cubemaps applied by this material. Should be moved in
        /// @param isTransparencyEnabled
        ///     Whether or not transparency is enabled.
        /// @param isColourWriteEnabled
        ///     Whether or not colour write is enabled.
        /// @param isDepthWriteEnabled
        ///     Whether or not depth write is enabled.
        /// @param isDepthTestEnabled
        ///     Whether or not the depth test will be performed.
        /// @param isFaceCullingEnabled
        ///     Whether or not face culling will be performed.
        /// @param isStencilTestEnabled
        ///     Whether or not to perform stencil testing
        /// @param depthTestFunc
        ///     Function that determines whether a depth test comparison should pass or fail
        /// @param sourceBlendMode
        ///     The source blend mode. This only applies if transparency is enabled.
        /// @param destinationBlendMode
        ///     The destination blend mode. This only applies if transparency is enabled.
        /// @param stencilFailOp
        ///     Op applied if stencil test fails
        /// @param stencilDepthFailOp
        ///     Op applied if stencil depth test fails
        /// @param stencilPassOp
        ///     Op applied if stencil and depth tests pass
        /// @param stencilTestFunc
        ///     Function that determines whether a stencil test comparison should pass or fail
        /// @param stencilRef
        ///     Value used as comparison for stencil tests
        /// @param stencilMask
        ///     Value ANDed to with the comparison and stencil value
        /// @param cullFace
        ///     The face which should be called. This only applies if face culling is enabled.
        /// @param emissiveColour
        ///     The emissive colour.
        /// @param ambientColour
        ///     The ambient colour.
        /// @param diffuseColour
        ///     The diffuseColour.
        /// @param specularColour
        ///     The specularColour.
        /// @param renderShaderVariables
        ///     The container for all render shader variables. May be null if there are no shader variables.
        ///
        RenderMaterial(const RenderShader* renderShader, std::vector<const RenderTexture*> renderTextures2D, std::vector<const RenderTexture*> renderTexturesCubemap,
                       bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled, bool isFaceCullingEnabled, bool isStencilTestEnabled,
                       TestFunc depthTestFunc,
                       BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                       StencilOp stencilFailOp, StencilOp stencilDepthFailOp, StencilOp stencilPassOp, TestFunc stencilTestFunc, s32 stencilRef, u32 stencilMask,
                       CullFace cullFace,
                       const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour, const Colour& specularColour,
                       RenderShaderVariablesUPtr renderShaderVariables) noexcept;
        
        /// @return The shader applied by this material.
        ///
        const RenderShader* GetRenderShader() const noexcept { return m_renderShader; }
        
        /// @return The list of textures applied by this material.
        ///
        const std::vector<const RenderTexture*>& GetRenderTextures2D() const noexcept { return m_renderTextures2D; }
        
        /// @return The list of cubemaps applied by this material.
        ///
        const std::vector<const RenderTexture*>& GetRenderTexturesCubemap() const noexcept { return m_renderTexturesCubemap; }
        
        /// @return Whether or not transparency is enabled.
        ///
        bool IsTransparencyEnabled() const noexcept { return m_isTransparencyEnabled; }
        
        /// @return Whether or not colour write is enabled.
        ///
        bool IsColourWriteEnabled() const noexcept { return m_isColourWriteEnabled; }
        
        /// @return Whether or not depth write is enabled.
        ///
        bool IsDepthWriteEnabled() const noexcept { return m_isDepthWriteEnabled; }
        
        /// @return Whether or not depth write is enabled.
        ///
        bool IsDepthTestEnabled() const noexcept { return m_isDepthTestEnabled; }
        
        /// @return Whether or not face culling is enabled.
        ///
        bool IsFaceCullingEnabled() const noexcept { return m_isFaceCullingEnabled; }
        
        /// @return Whether or not to perform stencil testing
        ///
        bool IsStencilTestEnabled() const noexcept { return m_isStencilTestEnabled; }
        
        /// @return Function that handles comparing depth values for pass/fail testing
        ///
        TestFunc GetDepthTestFunc() const noexcept { return m_depthTestFunc; }
        
        /// @return The source blend mode. This only applies if transparency is enabled.
        ///
        BlendMode GetSourceBlendMode() const noexcept { return m_sourceBlendMode; }
        
        /// @return The destination blend mode. This only applies if transparency is enabled.
        ///
        BlendMode GetDestinationBlendMode() const noexcept { return m_destinationBlendMode; }
        
        /// @return The face which should be called. This only applies if face culling is enabled.
        ///
        CullFace GetCullFace() const noexcept { return m_cullFace; }
        
        /// @return Op to use if stencil test fails
        ///
        StencilOp GetStencilFailOp() const noexcept { return m_stencilFailOp; }
        
        /// @return Op to use if stencil test passes but depth test fails
        ///
        StencilOp GetStencilDepthFailOp() const noexcept { return m_stencilDepthFailOp; }
        
        /// @return Op to use if stencil and depth tests pass
        ///
        StencilOp GetStencilPassOp() const noexcept { return m_stencilPassOp; }
        
        /// @return Function that handles comparing stencil values for pass/fail testing
        ///
        TestFunc GetStencilTestFunc() const noexcept { return m_stencilTestFunc; }
        
        /// @return The comparison value for the stencil test function
        ///
        s32 GetStencilTestFuncRef() const noexcept { return m_stencilTestFuncRef; }
        
        /// @return The mask that is ANDed with the ref and stored value
        ///
        u32 GetStencilTestFuncMask() const noexcept { return m_stencilTestFuncMask; }
        
        /// @return The emissive colour
        ///
        const Colour& GetEmissiveColour() const noexcept { return m_emissiveColour; }
        
        /// @return The ambient colour
        ///
        const Colour& GetAmbientColour() const noexcept { return m_ambientColour; }
        
        /// @return The diffuse colour
        ///
        const Colour& GetDiffuseColour() const noexcept { return m_diffuseColour; }
        
        /// @return The specular colour
        ///
        const Colour& GetSpecularColour() const noexcept { return m_specularColour; }
        
        /// @return The render shader variables. May be null if there are no shader variables.
        ///
        const RenderShaderVariables* GetRenderShaderVariables() const noexcept { return m_renderShaderVariables.get(); }
        
        /// This is not thread safe and should only be called from the render thread.
        ///
        /// @return A pointer to render system specific additional information.
        ///
        void* GetExtraData() const noexcept { return m_extraData; }
        
        /// This is not thread safe and should only be called from the render thread.
        ///
        /// @param extraData
        ///     A pointer to render system specific additional information.
        ///
        void SetExtraData(void* extraData) noexcept { m_extraData = extraData; }
        
    private:
        const RenderShader* m_renderShader;
        std::vector<const RenderTexture*> m_renderTextures2D;
        std::vector<const RenderTexture*> m_renderTexturesCubemap;
        bool m_isTransparencyEnabled;
        bool m_isColourWriteEnabled;
        bool m_isDepthWriteEnabled;
        bool m_isDepthTestEnabled;
        bool m_isFaceCullingEnabled;
        bool m_isStencilTestEnabled;
        TestFunc m_depthTestFunc;
        BlendMode m_sourceBlendMode;
        BlendMode m_destinationBlendMode;
        StencilOp m_stencilFailOp;
        StencilOp m_stencilDepthFailOp;
        StencilOp m_stencilPassOp;
        s32 m_stencilTestFuncRef;
        u32 m_stencilTestFuncMask;
        TestFunc m_stencilTestFunc;
        CullFace m_cullFace;
        Colour m_emissiveColour;
        Colour m_ambientColour;
        Colour m_diffuseColour;
        Colour m_specularColour;
        RenderShaderVariablesUPtr m_renderShaderVariables;
        void* m_extraData = nullptr;
    };
}

#endif
