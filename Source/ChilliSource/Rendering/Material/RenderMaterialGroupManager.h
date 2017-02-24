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

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_RENDERMATERIALGROUPMANAGER_H_
#define _CHILLISOURCE_RENDERING_MATERIAL_RENDERMATERIALGROUPMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Material/RenderMaterialGroup.h>

#include <mutex>

namespace ChilliSource
{
    /// Manages the lifecycle of all RenderMaterialGroup instances.
    ///
    /// On creation of a RenderMaterialGroup, a LoadMaterialRenderCommand is queued for each
    /// of the contained materials in the next render snapshot phase, ensuring that related
    /// material data is processed before the RenderMaterial is used.
    ///
    /// On deletion an UnloadMaterialRenderCommand is queued for each RenderMaterialand given ownership of the
    /// RenderMesh. The RenderMesh is then deleted once the command has been processed.
    ///
    /// This is thread-safe and can be called from any thread. If it is called on a background
    /// thread, care needs to be taken to ensure any created RenderMeshes are not used prior
    /// to being loaded.
    ///
    class RenderMaterialGroupManager : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(RenderMaterialGroupManager);
        
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
        virtual UniquePtr<RenderMaterialGroup> CreateUnlitRenderMaterialGroup(const RenderTexture* renderTexture,
                                                                  bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled, bool isFaceCullingEnabled, bool isStencilTestEnabled,
                                                                  TestFunc depthTestFunc,
                                                                  BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                                                                  StencilOp stencilFailOp, StencilOp stencilDepthFailOp, StencilOp stencilPassOp, TestFunc stencilTestFunc, s32 stencilRef, u32 stencilMask,
                                                                  CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour) noexcept = 0;
        
        /// Creates a new skybox RenderMaterialGroup and queues a LoadMaterialGroupRenderCommand for the next
        /// Render Snapshot stage in the render pipeline.
        ///
        /// @param renderCubmap
        ///     The render cubemap.
        ///
        /// @return The new material group.
        ///
        virtual UniquePtr<RenderMaterialGroup> CreateSkyboxRenderMaterialGroup(const RenderTexture* renderCubmap) noexcept = 0;
        
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
        virtual UniquePtr<RenderMaterialGroup> CreateBlinnRenderMaterialGroup(const RenderTexture* renderTexture, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour,
                                                                          const Colour& specularColour) noexcept = 0;
        
        
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
        virtual UniquePtr<RenderMaterialGroup> CreateCustomRenderMaterialGroup(MaterialShadingType fallbackType, const VertexFormat& vertexFormat, const std::vector<std::pair<const RenderShader*, RenderPasses>>& renderShaders, const std::vector<const RenderTexture*>& renderTextures2D, const std::vector<const RenderTexture*>& renderTexturesCubemap,
                                                                   bool isTransparencyEnabled, bool isColourWriteEnabled, bool isDepthWriteEnabled, bool isDepthTestEnabled, bool isFaceCullingEnabled, bool isStencilTestEnabled,
                                                                   TestFunc depthTestFunc,
                                                                   BlendMode sourceBlendMode, BlendMode destinationBlendMode,
                                                                   StencilOp stencilFailOp, StencilOp stencilDepthFailOp, StencilOp stencilPassOp, TestFunc stencilTestFunc, s32 stencilRef, u32 stencilMask,
                                                                   CullFace cullFace, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour, const Colour& specularColour,
                                                                   RenderShaderVariablesUPtr renderShaderVariables) noexcept = 0;
        
        /// Removes the RenderMaterialGroup from the manager and queues an UnloadMaterialGroupRenderCommand
        /// for the next Render Snapshot stage in the render pipeline. The render command is given ownership
        /// of the RenderMaterialGroup, ensuring it won't be destroyed until it is no longer used.
        ///
        /// @param renderMaterialGroup
        ///     The RenderMaterialGroup which should be destroyed.
        ///
        void DestroyRenderMaterialGroup(UniquePtr<RenderMaterialGroup> renderMaterialGroup) noexcept;
        
        virtual ~RenderMaterialGroupManager() noexcept {};
        
    protected:
        RenderMaterialGroupManager() = default;
        
        /// Adds a new RenderMaterialGroup to the manager and schedules the load material commands.
        ///
        /// @param renderMaterialGroup
        ///     The new render material group to add.
        ///
        void AddRenderMaterialGroup(RenderMaterialGroup* renderMaterialGroup) noexcept;
        
    private:
        friend class Application;
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RenderMaterialGroupManagerUPtr Create() noexcept;
        
        /// Called during the Render Snapshot stage of the render pipeline. All pending load and
        /// unload commands are added to the render snapshot.
        ///
        /// @param targetType
        ///     Whether the snapshot is for the main screen or an offscreen render target
        /// @param renderSnapshot
        ///     The render shapshot for storing snapshotted data.
        /// @param frameAllocator
        ///     Allocate memory for this render frame from here
        ///
        void OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        /// Clears up any pending commands
        ///
        void Destroy();
        
        std::mutex m_mutex;
        std::vector<RenderMaterialGroup*> m_pendingLoadCommands;
        std::vector<UniquePtr<RenderMaterialGroup>> m_pendingUnloadCommands;
    };
}

#endif
