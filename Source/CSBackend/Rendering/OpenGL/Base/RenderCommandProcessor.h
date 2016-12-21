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

#ifndef _CSBACKEND_RENDERING_OPENGL_BASE_RENDERCOMMANDPROCESSOR_H_
#define _CSBACKEND_RENDERING_OPENGL_BASE_RENDERCOMMANDPROCESSOR_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>

#include <CSBackend/Rendering/OpenGL/Camera/GLCamera.h>
#include <CSBackend/Rendering/OpenGL/Lighting/GLLight.h>
#include <CSBackend/Rendering/OpenGL/Model/GLDynamicMesh.h>
#include <CSBackend/Rendering/OpenGL/Texture/GLTextureUnitManager.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Rendering/Base/IRenderCommandProcessor.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// An OpenGL implementation of the IRenderCommandProcessor interface. This processes
        /// buffers of render commands using the OpenGL render API.
        ///
        /// This is not thread-safe and must be called on the render thread.
        ///
        class RenderCommandProcessor final : public ChilliSource::IRenderCommandProcessor
        {
        public:
            /// Processes the given render command buffer, performing the required actions for
            /// the command using OpenGL.
            ///
            /// @param renderCommandBuffer
            ///     The buffer of render commands that should be processed.
            ///
            void Process(const ChilliSource::RenderCommandBuffer* renderCommandBuffer) noexcept override;
            
            /// Called when the GL context is lost, iterate any GL resources and place
            /// them in an invalid state
            ///
            void Invalidate() noexcept override;
            
            /// Called when the GL context is restored, need to rebuild any GL resources
            ///
            void Restore() noexcept override;

            ~RenderCommandProcessor() noexcept;
            
        private:
            /// Initialises the render command processor. This is deferred until Process is first called
            /// to ensure it is run on the render thread.
            ///
            void Init() noexcept;
            
            /// Loads the shader described by the given load command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void LoadShader(const ChilliSource::LoadShaderRenderCommand* renderCommand) noexcept;
            
            /// Loads the texture described by the given load command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void LoadTexture(const ChilliSource::LoadTextureRenderCommand* renderCommand) noexcept;
            
            /// Loads the cubemap described by the given load command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void LoadCubemap(const ChilliSource::LoadCubemapRenderCommand* renderCommand) noexcept;
            
            /// Loads the mesh described by the given load command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void LoadMesh(const ChilliSource::LoadMeshRenderCommand* renderCommand) noexcept;
            
            /// Restores the texture given by the command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void RestoreTexture(const ChilliSource::RestoreTextureRenderCommand* renderCommand) noexcept;
            
            /// Restores the cubemap given by the command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void RestoreCubemap(const ChilliSource::RestoreCubemapRenderCommand* renderCommand) noexcept;
            
            /// Restores the mesh given by the command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void RestoreMesh(const ChilliSource::RestoreMeshRenderCommand* renderCommand) noexcept;

            /// Restores the render target group given by the command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void RestoreRenderTargetGroup(const ChilliSource::RestoreRenderTargetGroupCommand* renderCommand) noexcept;
            
            /// Loads the target group described by the given load command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void LoadTargetGroup(const ChilliSource::LoadTargetGroupRenderCommand* renderCommand) noexcept;

            /// Begins rendering to the default render target.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void Begin(const ChilliSource::BeginRenderCommand* renderCommand) noexcept;
            
            /// Begins rendering to the the described render target group.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void BeginWithTargetGroup(const ChilliSource::BeginWithTargetGroupRenderCommand* renderCommand) noexcept;
            
            /// Caches the given camera data. All applied materials after this will use this camera data.
            /// The currently applied material will be invalidated and needs to be re-applied.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplyCamera(const ChilliSource::ApplyCameraRenderCommand* renderCommand) noexcept;
            
            /// Caches the given ambient light data and invalidates all other light data. All applied materials
            /// after this will use this data.
            ///
            /// The currently applied material will be invalidated and needs to be re-applied.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplyAmbientLight(const ChilliSource::ApplyAmbientLightRenderCommand* renderCommand) noexcept;
            
            /// Caches the given directional light data and invalidates all other light data. All applied materials
            /// after this will use this data.
            ///
            /// The currently applied material will be invalidated and needs to be re-applied.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplyDirectionalLight(const ChilliSource::ApplyDirectionalLightRenderCommand* renderCommand) noexcept;
            
            /// Caches the given point light data and invalidates all other light data. All applied materials
            /// after this will use this data.
            ///
            /// The currently applied material will be invalidated and needs to be re-applied.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplyPointLight(const ChilliSource::ApplyPointLightRenderCommand* renderCommand) noexcept;
            
            /// Applies the given material to the OpenGL Context. The cached camera data will be used.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplyMaterial(const ChilliSource::ApplyMaterialRenderCommand* renderCommand) noexcept;
            
            /// Applies the given mesh to the OpenGL Context.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplyMesh(const ChilliSource::ApplyMeshRenderCommand* renderCommand) noexcept;
            
            /// Applies the given dynamic mesh to the OpenGL Context.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplyDynamicMesh(const ChilliSource::ApplyDynamicMeshRenderCommand* renderCommand) noexcept;
            
            /// Applies the given mesh batch to the OpenGL Context.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplyMeshBatch(const ChilliSource::ApplyMeshBatchRenderCommand* renderCommand) noexcept;
            
            /// Applies the given skinned animation to the OpenGL Context. If the skinned animation is null
            /// then skinned animation will be disabled.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void ApplySkinnedAnimation(const ChilliSource::ApplySkinnedAnimationRenderCommand* renderCommand) noexcept;
            
            /// Renders an instance of the mesh described by the current OpenGL context state. A camera,
            /// material and mesh must all currently be appled to the context.
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void RenderInstance(const ChilliSource::RenderInstanceRenderCommand* renderCommand) noexcept;
            
            /// Ends rendering to the current render target.
            ///
            void End() noexcept;
            
            /// Unloads the shader described by the given unload command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void UnloadShader(const ChilliSource::UnloadShaderRenderCommand* renderCommand) noexcept;
            
            /// Unloads the texture described by the given unload command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void UnloadTexture(const ChilliSource::UnloadTextureRenderCommand* renderCommand) noexcept;
            
            /// Unloads the cubemap described by the given unload command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void UnloadCubemap(const ChilliSource::UnloadCubemapRenderCommand* renderCommand) noexcept;
            
            /// Unloads the mesh described by the given unload command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void UnloadMesh(const ChilliSource::UnloadMeshRenderCommand* renderCommand) noexcept;
            
            /// Unloads the target group described by the given unload command
            ///
            /// @param renderCommand
            ///     The render command
            ///
            void UnloadTargetGroup(const ChilliSource::UnloadTargetGroupRenderCommand* renderCommand) noexcept;
            
            /// Resets the cached values back to thier original state.
            ///
            void ResetCache() noexcept;
            
            bool m_initRequired = true;
            
            GLTextureUnitManagerUPtr m_textureUnitManager;
            GLDynamicMeshUPtr m_glDynamicMesh;
            
            GLCamera m_currentCamera;
            GLLightUPtr m_currentLight;
            const ChilliSource::RenderTargetGroup* m_currentRenderTargetGroup = nullptr;
            const ChilliSource::RenderShader* m_currentShader = nullptr;
            const ChilliSource::RenderMaterial* m_currentMaterial = nullptr;
            const ChilliSource::RenderMesh* m_currentMesh = nullptr;
            const ChilliSource::RenderDynamicMesh* m_currentDynamicMesh = nullptr;
            const ChilliSource::RenderSkinnedAnimation* m_currentSkinnedAnimation = nullptr;
        };
    }
}

#endif
