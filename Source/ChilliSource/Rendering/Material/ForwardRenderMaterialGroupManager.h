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

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_FORWARDRENDERMATERIALGROUPMANAGER_H_
#define _CHILLISOURCE_RENDERING_MATERIAL_FORWARDRENDERMATERIALGROUPMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/ObjectPoolAllocator.h>
#include <ChilliSource/Rendering/Material/RenderMaterialGroupManager.h>

namespace ChilliSource
{
    /// The forward rendered version of the RenderMaterialGroupManager. This creates the
    /// necessary materials to perform multi-pass forward rendering for each of the different
    /// material types. The various passes are described in ForwardRenderPasses.h
    ///
    /// This is thread safe.
    ///
    class ForwardRenderMaterialGroupManager final : public RenderMaterialGroupManager
    {
    public:
        CS_DECLARE_NAMEDTYPE(ForwardRenderMaterialGroupManager);
        
        /// Allows querying of whether or not this system implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// Creates a new unlit RenderMaterialGroup and queues a LoadMaterialGroupRenderCommand for the next
        /// Render Snapshot stage in the render pipeline.
        ///
        /// @param renderTexture
        ///     The render texture.
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
        /// @param ambientColour
        ///     The ambient colour.
        /// @param emissiveColour
        ///     The ambient colour.
        ///
        /// @return The new material group.
        ///
        UniquePtr<RenderMaterialGroup> CreateUnlitRenderMaterialGroup(const RenderTexture* renderTexture,
                                                                  bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled, bool isFaceCullingEnabled, bool isStencilTestEnabled,
                                                                  TestFunc depthTestFunc,
                                                                  BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                                                                  StencilOp stencilFailOp, StencilOp stencilDepthFailOp, StencilOp stencilPassOp, TestFunc stencilTestFunc, s32 stencilRef, u32 stencilMask,
                                                                  CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour) noexcept override;
        
        /// Creates a new skybox RenderMaterialGroup and queues a LoadMaterialGroupRenderCommand for the next
        /// Render Snapshot stage in the render pipeline.
        ///
        /// @param renderCubmap
        ///     The render cubemap.
        ///
        /// @return The new material group.
        ///
        UniquePtr<RenderMaterialGroup> CreateSkyboxRenderMaterialGroup(const RenderTexture* renderCubmap) noexcept override;
        
        /// Creates a new blinn RenderMaterialGroup and queues a LoadMaterialGroupRenderCommand for the next
        /// Render Snapshot stage in the render pipeline.
        ///
        /// @param renderTexture
        ///     The render texture.
        /// @param ambientColour
        ///     The ambient colour.
        /// @param emissiveColour
        ///     The ambient colour.
        /// @param diffuseColour
        ///     The diffuse colour.
        /// @param specularColour
        ///     The specular colour.
        ///
        /// @return The new material group.
        ///
        UniquePtr<RenderMaterialGroup> CreateBlinnRenderMaterialGroup(const RenderTexture* renderTexture, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour,
                                                                  const Colour& specularColour) noexcept override;
        
        /// Creates a new custom RenderMaterialGroup and queues a LoadMaterialGroupRenderCommand for the next
        /// Render Snapshot stage in the render pipeline.
        ///
        /// @param fallbackType
        ///     Any unspecified render pass shaders us the equivalent shader from the fallback type
        /// @param vertexFormat
        ///     The vertex format this material is for.
        /// @param renderShaders
        ///     The render shaders for each pass.
        /// @param renderTextures2D
        ///     The list of render textures.
        /// @param renderTexturesCubemap
        ///     The list of render cubemaps.
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
        ///     The ambient colour.
        /// @param ambientColour
        ///     The ambient colour.
        /// @param diffuseColour
        ///     The diffuse colour.
        /// @param specularColour
        ///     The specular colour.
        /// @param renderShaderVariables
        ///     The container for all render shader variables.
        ///
        /// @return The new material group.
        ///
        UniquePtr<RenderMaterialGroup> CreateCustomRenderMaterialGroup(MaterialShadingType fallbackType, const VertexFormat& vertexFormat, const std::vector<std::pair<const RenderShader*, RenderPasses>>& renderShaders, const std::vector<const RenderTexture*>& renderTextures2D, const std::vector<const RenderTexture*>& renderTexturesCubemap,
                                                                   bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled, bool isFaceCullingEnabled, bool isStencilTestEnabled,
                                                                   TestFunc depthTestFunc,
                                                                   BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                                                                   StencilOp stencilFailOp, StencilOp stencilDepthFailOp, StencilOp stencilPassOp, TestFunc stencilTestFunc, s32 stencilRef, u32 stencilMask,
                                                                   CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour, const Colour& specularColour,
                                                                   RenderShaderVariablesUPtr renderShaderVariables) noexcept override;
        
    private:
        friend class RenderMaterialGroupManager;
        
        /// The init lifecycle event. Loads all shaders used by the system.
        ///
        void OnInit() noexcept override;
        
        /// The destroy lifecycle event. Unloads all shaders used by the system.
        ///
        void OnDestroy() noexcept override;
        
        ForwardRenderMaterialGroupManager();
        
        ///
        /// Helper methods that creates the render materials on behalf of the CreateXXXXGroup methods for the given vertex format
        ///
        void CreateUnlitRenderMaterialGroupCollection(const VertexFormat& format, const RenderTexture* renderTexture,
                                                      bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled, bool isFaceCullingEnabled, bool isStencilTestEnabled,
                                                      TestFunc depthTestFunc,
                                                      BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                                                      StencilOp stencilFailOp, StencilOp stencilDepthFailOp, StencilOp stencilPassOp, TestFunc stencilTestFunc, s32 stencilRef, u32 stencilMask,
                                                      CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour,
                                                      std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots>& out_renderMaterialSlots, std::vector<UniquePtr<RenderMaterial>>& out_renderMaterials) noexcept;
        
        ///
        /// Helper methods that creates the render materials on behalf of the CreateXXXXGroup methods for the given vertex format
        ///
        void CreateSkyboxRenderMaterialGroupCollection(const VertexFormat& format, const RenderTexture* renderCubemap,
                                                      std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots>& out_renderMaterialSlots, std::vector<UniquePtr<RenderMaterial>>& out_renderMaterials) noexcept;
        
        ///
        /// Helper methods that creates the render materials on behalf of the CreateXXXXGroup methods for the given vertex format
        ///
        void CreateBlinnRenderMaterialGroupCollection(const VertexFormat& format, const RenderTexture* renderTexture, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour, const Colour& specularColour,
                                                      std::array<const RenderMaterial*, RenderMaterialGroup::k_numMaterialSlots>& out_renderMaterialSlots, std::vector<UniquePtr<RenderMaterial>>& out_renderMaterials) noexcept;
        
        
        ObjectPoolAllocator<RenderMaterial> m_renderMaterialPool;
        ObjectPoolAllocator<RenderMaterialGroup> m_renderMaterialGroupPool;
        
        bool m_shadowsSupported = false;
        
        ShaderCSPtr m_spriteUnlit;
        
        ShaderCSPtr m_skybox;
        
        ShaderCSPtr m_staticShadowMap;
        ShaderCSPtr m_staticUnlit;
        ShaderCSPtr m_staticBlinnBase;
        ShaderCSPtr m_staticBlinnDirectional;
        ShaderCSPtr m_staticBlinnDirectionalShadows;
        ShaderCSPtr m_staticBlinnPoint;
        
        ShaderCSPtr m_animatedShadowMap;
        ShaderCSPtr m_animatedUnlit;
        ShaderCSPtr m_animatedBlinnBase;
        ShaderCSPtr m_animatedBlinnDirectional;
        ShaderCSPtr m_animatedBlinnDirectionalShadows;
        ShaderCSPtr m_animatedBlinnPoint;
    };
}

#endif
