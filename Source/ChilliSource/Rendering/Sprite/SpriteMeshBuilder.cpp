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

#include <ChilliSource/Rendering/Sprite/SpriteMeshBuilder.h>

#include <ChilliSource/Core/Base/ByteBuffer.h>
#include <ChilliSource/Core/Base/ByteColour.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Base/ColourUtils.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Model/IndexFormat.h>
#include <ChilliSource/Rendering/Model/PolygonType.h>
#include <ChilliSource/Rendering/Model/RenderDynamicMesh.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

#include <cstring>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_numVertices = 4;
        constexpr u32 k_numIndices = 6;
        
        const u16 k_indices[k_numIndices] { 0, 1, 2, 1, 3, 2 };
        
        /// A description of a sprite vertex.
        ///
        struct SpriteVertex final
        {
            Vector4 m_position;
            Vector2 m_uv;
            ByteColour m_colour;
        };
        
        /// An enum describing each of the vertices in a sprite.
        ///
        enum class SpriteVertices
        {
            k_topLeft,
            k_bottomLeft,
            k_topRight,
            k_bottomRight
        };
        
        constexpr u32 k_vertexDataSize = sizeof(SpriteVertex) * k_numVertices;
        constexpr u32 k_indexDataSize = sizeof(u16) * k_numIndices;
        
        /// Apples the positional data derrived from the given description to the vertices.
        ///
        /// @param vertices
        ///     The vertices to apply the positional data to.
        /// @param localPosition
        ///     The local position of the sprite. This is often used to offset the sprite as required
        ///     by the cropping performed by texture packing.
        /// @param localSize
        ///     The local size of the sprite.
        /// @param alignmentAnchor
        ///     The alignmentAnchor of the sprite.
        ///
        void ApplyVertexPosition(SpriteVertex* vertices, const Vector3& localPosition, const Vector2& localSize, AlignmentAnchor alignmentAnchor) noexcept
        {
            Vector2 halfSize = 0.5f * localSize;
            Vector2 anchor = GetAnchorPoint(alignmentAnchor, halfSize);
            Vector4 centre = Vector4(localPosition, 1.0f) + Vector4(-anchor.x, -anchor.y, 0.0f, 0.0f);
            
            vertices[static_cast<u32>(SpriteVertices::k_topLeft)].m_position = centre + Vector4(-halfSize.x, halfSize.y, 0.0f, 0.0f);
            vertices[static_cast<u32>(SpriteVertices::k_bottomLeft)].m_position = centre + Vector4(-halfSize.x, -halfSize.y, 0.0f, 0.0f);
            vertices[static_cast<u32>(SpriteVertices::k_topRight)].m_position = centre + Vector4(halfSize.x, halfSize.y, 0.0f, 0.0f);
            vertices[static_cast<u32>(SpriteVertices::k_bottomRight)].m_position = centre + Vector4(halfSize.x, -halfSize.y, 0.0f, 0.0f);
        }
        
        /// Apples the given UVs to the vertices that make up a sprite.
        ///
        /// @param vertices
        ///     The vertices to apply the colour to.
        /// @param uvs
        ///     The UVs which should be applied.
        ///
        void ApplyVertexUvs(SpriteVertex* vertices, const UVs& uvs) noexcept
        {
            vertices[static_cast<u32>(SpriteVertices::k_topLeft)].m_uv.x = uvs.m_u;
            vertices[static_cast<u32>(SpriteVertices::k_topLeft)].m_uv.y = uvs.m_v;
            vertices[static_cast<u32>(SpriteVertices::k_bottomLeft)].m_uv.x = uvs.m_u;
            vertices[static_cast<u32>(SpriteVertices::k_bottomLeft)].m_uv.y = uvs.m_v + uvs.m_t;
            vertices[static_cast<u32>(SpriteVertices::k_topRight)].m_uv.x = uvs.m_u + uvs.m_s;
            vertices[static_cast<u32>(SpriteVertices::k_topRight)].m_uv.y = uvs.m_v;
            vertices[static_cast<u32>(SpriteVertices::k_bottomRight)].m_uv.x = uvs.m_u + uvs.m_s;
            vertices[static_cast<u32>(SpriteVertices::k_bottomRight)].m_uv.y = uvs.m_v + uvs.m_t;
        }
        
        /// Applies the given colour to all of the vertices in the sprite.
        ///
        /// @param vertices
        ///     The vertices to apply the colour to.
        /// @param colour
        ///     The colour to apply.
        ///
        void ApplyVertexColour(SpriteVertex* vertices, const Colour& colour) noexcept
        {
            ByteColour byteColour = ColourUtils::ColourToByteColour(colour);
            
            vertices[static_cast<u32>(SpriteVertices::k_topLeft)].m_colour = byteColour;
            vertices[static_cast<u32>(SpriteVertices::k_bottomLeft)].m_colour = byteColour;
            vertices[static_cast<u32>(SpriteVertices::k_topRight)].m_colour = byteColour;
            vertices[static_cast<u32>(SpriteVertices::k_bottomRight)].m_colour = byteColour;
        }
        
        /// Creates the sprite vertex data from the given description. The data will be allocated from
        /// the given allocator.
        ///
        /// @param allocator
        ///     The allocator which should be used to create the buffer.
        /// @param localPosition
        ///     The local position of the sprite. This is often used to offset the sprite as required
        ///     by the cropping performed by texture packing.
        /// @param localSize
        ///     The local size of the sprite.
        /// @param uvs
        ///     The UVs which should be applied.
        /// @param colour
        ///     The colour to apply.
        /// @param alignmentAnchor
        ///     The alignmentAnchor of the sprite.
        ///
        UniquePtr<u8[]> BuildVertexData(IAllocator* allocator, const Vector3& localPosition, const Vector2& localSize, const UVs& uvs, const Colour& colour, AlignmentAnchor alignmentAnchor) noexcept
        {
            auto vertexData = MakeUniqueArray<u8>(*allocator, k_vertexDataSize);
            
            ApplyVertexPosition(reinterpret_cast<SpriteVertex*>(vertexData.get()), localPosition, localSize, alignmentAnchor);
            ApplyVertexUvs(reinterpret_cast<SpriteVertex*>(vertexData.get()), uvs);
            ApplyVertexColour(reinterpret_cast<SpriteVertex*>(vertexData.get()), colour);
            
            return vertexData;
        }
        
        /// Creates the index data. This data will be allocated from the given allocator.
        ///
        /// @return A new instance of the index data. This will always be the same 6 indices.
        ///
        UniquePtr<u8[]> BuildIndexData(IAllocator* allocator) noexcept
        {
            auto indexData = MakeUniqueArray<u8>(*allocator, k_indexDataSize);
            
            memcpy(indexData.get(), k_indices, k_indexDataSize);
            
            return indexData;
        }
        
        /// Calculates the bounding sphere required for sprite with the given positional data.
        ///
        /// @param localPosition
        ///     The local position of the sprite.
        /// @param localSize
        ///     The local size of the sprite.
        /// @param alignmentAnchor
        ///     The alignment anchor of the sprite.
        ///
        Sphere CalcBoundingSphere(const Vector3& localPosition, const Vector2& localSize, AlignmentAnchor alignmentAnchor) noexcept
        {
            Vector2 halfSize = 0.5f * localSize;
            Vector2 anchor = GetAnchorPoint(alignmentAnchor, halfSize);
            
            Vector3 centre = localPosition + Vector3(-anchor.x, -anchor.y, 0.0f);
            f32 radius = halfSize.Length();
            
            return Sphere(centre, radius);
        }
    }
    
    //------------------------------------------------------------------------------
    RenderDynamicMeshAUPtr SpriteMeshBuilder::Build(IAllocator* allocator, const Vector3& localPosition, const Vector2& localSize, const UVs& uvs, const Colour& colour, AlignmentAnchor alignmentAnchor) noexcept
    {
        auto polygonType = PolygonType::k_triangle;
        auto vertexData = BuildVertexData(allocator, localPosition, localSize, uvs, colour, alignmentAnchor);
        auto indexData = BuildIndexData(allocator);
        Sphere boundingSphere = CalcBoundingSphere(localPosition, localSize, alignmentAnchor);
        
        return MakeUnique<RenderDynamicMesh>(*allocator, polygonType, VertexFormat::k_sprite, IndexFormat::k_short, k_numVertices, k_numIndices, boundingSphere, std::move(vertexData), k_vertexDataSize, std::move(indexData), k_indexDataSize);
    }
}
