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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_LOADMESHRENDERCOMMAND_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_LOADMESHRENDERCOMMAND_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommand.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>

namespace ChilliSource
{
    /// A render command for loading the mesh data pertaining to a single render mesh into
    /// render memory.
    ///
    /// This must be instantiated via a RenderCommandList.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class LoadMeshRenderCommand final : public RenderCommand
    {
    public:
        /// @return The render mesh that should be loaded.
        ///
        RenderMesh* GetRenderMesh() const noexcept { return m_renderMesh; }
        
        /// @return The vertex data buffer.
        ///
        const u8* GetVertexData() const noexcept { return m_vertexData.get(); }
        
        /// Moves the Vertex data out of this class. Use with caution as this command
        /// will be in a broken state after this is used.
        ///
        /// @return The vertex data buffer.
        ///
        std::unique_ptr<const u8[]> ClaimVertexData() noexcept;
        
        /// @return The size of the vertex data buffer.
        ///
        u32 GetVertexDataSize() const noexcept { return m_vertexDataSize; }
        
        /// @return The index data buffer.
        ///
        const u8* GetIndexData() const noexcept { return m_indexData.get(); }
        
        /// Moves the Index data out of this class. Use with caution as this command
        /// will be in a broken state after this is used.
        ///
        /// @return The index data buffer.
        ///
        std::unique_ptr<const u8[]> ClaimIndexData() noexcept;
        
        /// @return The size of the index data buffer.
        ///
        u32 GetIndexDataSize() const noexcept { return m_indexDataSize; }
        
    private:
        friend class RenderCommandList;
        
        /// Constructs a new instance with the given render mesh and mesh data.
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
        LoadMeshRenderCommand(RenderMesh* renderMesh, std::unique_ptr<const u8[]> vertexData, u32 vertexDataSize, std::unique_ptr<const u8[]> indexData, u32 indexDataSize) noexcept;
        
        RenderMesh* m_renderMesh;
        std::unique_ptr<const u8[]> m_vertexData;
        u32 m_vertexDataSize;
        std::unique_ptr<const u8[]> m_indexData;
        u32 m_indexDataSize;
        bool m_shouldBackupData = false;
    };
}

#endif
