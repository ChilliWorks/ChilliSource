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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMeshRenderCommand.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    LoadMeshRenderCommand::LoadMeshRenderCommand(RenderMesh* renderMesh, std::unique_ptr<const u8[]> vertexData, u32 vertexDataSize, std::unique_ptr<const u8[]> indexData, u32 indexDataSize) noexcept
        : RenderCommand(Type::k_loadMesh), m_renderMesh(renderMesh), m_vertexData(std::move(vertexData)), m_vertexDataSize(vertexDataSize), m_indexData(std::move(indexData)), m_indexDataSize(indexDataSize)
    {
    }
    //------------------------------------------------------------------------------
    std::unique_ptr<const u8[]> LoadMeshRenderCommand::ClaimVertexData() noexcept
    {
        CS_ASSERT(m_vertexData, "Cannot claim nullptr data! Data may have already been claimed.");
        return std::move(m_vertexData);
    }
    //------------------------------------------------------------------------------
    std::unique_ptr<const u8[]> LoadMeshRenderCommand::ClaimIndexData() noexcept
    {
        CS_ASSERT(m_indexData, "Cannot claim nullptr data! Data may have already been claimed.");
        return std::move(m_indexData);
    }
}
