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
        
        /// @return The shader applied by this material.
        ///
        const RenderShader* GetRenderShader() const noexcept { return m_renderShader; }
        
        /// @return The list of textures applied by this material.
        ///
        const std::vector<const RenderTexture*>& GetRenderTextures() const noexcept { return m_renderTextures; }
        
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
        
        /// @return The source blend mode. This only applies if transparency is enabled.
        ///
        BlendMode GetSourceBlendMode() const noexcept { return m_sourceBlendMode; }
        
        /// @return The destination blend mode. This only applies if transparency is enabled.
        ///
        BlendMode GetDestinationBlendMode() const noexcept { return m_destinationBlendMode; }
        
        /// @return The face which should be called. This only applies if face culling is enabled.
        ///
        CullFace GetCullFace() const noexcept { return m_cullFace; }
        
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
        friend class ForwardRenderMaterialGroupManager;
        
        /// Creates a new instance with the given material data.
        ///
        /// @param renderShader
        ///     The shader applied by this material.
        /// @param renderTextures
        ///     The list of textures applied by this material.
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
        /// @param sourceBlendMode
        ///     The source blend mode. This only applies if transparency is enabled.
        /// @param destinationBlendMode
        ///     The destination blend mode. This only applies if transparency is enabled.
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
        ///
        RenderMaterial(const RenderShader* renderShader, const std::vector<const RenderTexture*>& renderTextures, bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled,
                       bool isDepthTestEnabled, bool isFaceCullingEnabled, BlendMode sourceBlendMode, BlendMode destinationBlendMode, CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour,
                       const Colour& diffuseColour, const Colour& specularColour) noexcept;
        
        const RenderShader* m_renderShader;
        std::vector<const RenderTexture*> m_renderTextures;
        bool m_isTransparencyEnabled;
        bool m_isColourWriteEnabled;
        bool m_isDepthWriteEnabled;
        bool m_isDepthTestEnabled;
        bool m_isFaceCullingEnabled;
        BlendMode m_sourceBlendMode;
        BlendMode m_destinationBlendMode;
        CullFace m_cullFace;
        Colour m_emissiveColour;
        Colour m_ambientColour;
        Colour m_diffuseColour;
        Colour m_specularColour;
        void* m_extraData = nullptr;
    };
}

#endif
