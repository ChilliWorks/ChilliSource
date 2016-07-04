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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_RENDERCOMMANDLIST_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_RENDERCOMMANDLIST_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommand.h>

#include <vector>

namespace ChilliSource
{
    /// Provides the ability to create an ordered list of render commands. Commands are
    /// created contiguously in memory to improve cache locality and reduce fragmentation.
    ///
    /// This is not thread-safe and therefore should only be accessed from one thread
    /// at a time.
    ///
    class RenderCommandList final
    {
    public:
        CS_DECLARE_NOCOPY(RenderCommandList);
        
        RenderCommandList() = default;
        RenderCommandList(RenderCommandList&&) = default;
        RenderCommandList& operator=(RenderCommandList&&) = default;
        
        /// Creates and adds a new load shader command to the render command list.
        ///
        /// @param renderShader
        ///     The render shader that should be loaded.
        /// @param vertexShader
        ///     The vertex shader string.
        /// @param fragmentShader
        ///     The fragment shader string
        ///
        void AddLoadShaderCommand(RenderShader* renderShader, const std::string& vertexShader, const std::string& fragmentShader) noexcept;
        
        /// Creates and adds a new load texture command to the render command list.
        ///
        /// @param renderTexture
        ///     The render texture that should be loaded.
        /// @param textureData
        ///     The data describing the texture.
        /// @param textureDataSize
        ///     The size of the texture data in bytes.
        ///
        void AddLoadTextureCommand(RenderTexture* renderTexture, std::unique_ptr<const u8[]> textureData, u32 textureDataSize) noexcept;
        
        /// Creates and adds a new load material group command to the render command list.
        ///
        /// @param renderMaterialGroup
        ///     The RenderMaterialGroup that should be loaded.
        ///
        void AddLoadMaterialGroupCommand(RenderMaterialGroup* renderMaterialGroup) noexcept;
        
        /// Creates and adds a new load mesh command to the render command list.
        ///
        /// @param renderMesh
        ///     The render mesh that should be loaded.
        /// @param vertexData
        ///     The vertex data buffer.
        /// @param vertexDataSize
        ///     The size of the vertex data buffer.
        /// @param indexData
        ///     The index data buffer.
        /// @param indexDataSize
        ///     The size of the index data buffer.
        ///
        void AddLoadMeshCommand(RenderMesh* renderMesh, std::unique_ptr<const u8[]> vertexData, u32 vertexDataSize, std::unique_ptr<const u8[]> indexData, u32 indexDataSize) noexcept;
        
        /// Creates and adds a new begin rendering command to the render command list.
        ///
        /// @param resolution
        ///     The resolution of the viewport.
        /// @param clearColour
        ///     The clear colour.
        ///
        void AddBeginCommand(const Integer2& resolution, const Colour& clearColour) noexcept;
        
        /// Creates and adds a new apply camera command to the render command list.
        ///
        /// @param position
        ///     The world space position of the camera.
        /// @param viewProjectionMatrix
        ///     The view projection matrix of the camera.
        ///
        void AddApplyCameraCommand(const Vector3& position, const Matrix4& viewProjectionMatrix) noexcept;
        
        /// Creates and adds a new apply ambient light command to the render command list.
        ///
        /// @param colour
        ///     The colour of the light.
        ///
        void AddApplyAmbientLightCommand(const Colour& colour) noexcept;
        
        /// Creates and adds a new apply material command to the render command list.
        ///
        /// @param renderMaterial
        ///     The render material to apply.
        ///
        void AddApplyMaterialCommand(const RenderMaterial* renderMaterial) noexcept;
        
        /// Creates and adds a new apply mesh command to the render command list.
        ///
        /// @param renderMesh
        ///     The render mesh to apply.
        ///
        void AddApplyMeshCommand(const RenderMesh* renderMesh) noexcept;
        
        /// Creates and adds a new apply dynamic mesh command to the render command list.
        ///
        /// @param renderDynamicMesh
        ///     The render dynamic mesh to apply.
        ///
        void AddApplyDynamicMeshCommand(const RenderDynamicMesh* renderDynamicMesh) noexcept;
        
        /// Creates and adds a new render instance command to the render command list.
        ///
        /// @param renderMesh
        ///     The world matrix of the instance.
        ///
        void AddRenderInstanceCommand(const Matrix4& worldMatrix) noexcept;
        
        /// Creates and adds a new end command to the render command list.
        ///
        void AddEndCommand() noexcept;
        
        /// Creates and adds a new unload mesh command to the render command list.
        ///
        /// @param renderMesh
        ///     The render mesh that should be unloaded.
        ///
        void AddUnloadMeshCommand(RenderMeshUPtr renderMesh) noexcept;
        
        /// Creates and adds a new unload material group command to the render command list.
        ///
        /// @param renderMaterialGroup
        ///     The render material group that should be unloaded.
        ///
        void AddUnloadMaterialGroupCommand(RenderMaterialGroupUPtr renderMaterialGroup) noexcept;

        /// Creates and adds a new unload texture command to the render command list.
        ///
        /// @param renderTexture
        ///     The render texture that should be unloaded.
        ///
        void AddUnloadTextureCommand(RenderTextureUPtr renderTexture) noexcept;
        
        /// Creates and adds a new unload shader command to the render command list.
        ///
        /// @param renderShader
        ///     The render shader that should be unloaded.
        ///
        void AddUnloadShaderCommand(RenderShaderUPtr renderShader) noexcept;

        /// @return The ordered list of render commands.
        ///
        const std::vector<const RenderCommand*>& GetOrderedList() const noexcept { return m_orderedCommands; };
        
    private:
        std::vector<const RenderCommand*> m_orderedCommands;
        std::vector<RenderCommandUPtr> m_renderCommands; //TODO: This should be changed to a series of pools of individial render command types.
    };
}

#endif
