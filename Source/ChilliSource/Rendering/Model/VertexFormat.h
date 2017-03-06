//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_MODEL_VERTEXFORMAT_H_
#define _CHILLISOURCE_RENDERING_MODEL_VERTEXFORMAT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/ByteColour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>

#include <array>
#include <vector>

namespace ChilliSource
{
    /// A container for the data in a single sprite vertex.
    ///
    struct SpriteVertex final
    {
        Vector4 m_position;
        Vector2 m_texCoord;
        ByteColour m_colour;
    };
    
    /// A container for the data in a single static mesh vertex.
    ///
    struct StaticMeshVertex
    {
        Vector4 m_position;
        Vector3 m_normal;
        Vector3 m_tangent;
        Vector3 m_bitangent;
        Vector2 m_texCoord;
    };
    
    /// Describes the format and layout of the vertices used in a single mesh. This
    /// consists of a series of vertex elements.
    ///
    /// Standard format types describe the common formats used in the engine, but
    /// custom types can also be declared.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class VertexFormat
    {
    public:
        static constexpr u32 k_maxElements = 8;
        
        static const VertexFormat k_staticMesh;
        static const VertexFormat k_animatedMesh;
        static const VertexFormat k_sprite;
        
        /// An enum describing the various possible element types in a vertex format.
        /// The number suffix indicates the number of components in the element, i.e
        /// k_position4 represents a position vector with [ X, Y, Z, W ] components.
        ///
		/// The order of this enum is important, as the elements are used as indices when
		/// setting shader attributes. See GLShader.cpp.
        enum class ElementType
        {
            k_position4,
            k_normal3,
			k_tangent3,
			k_bitangent3,
            k_uv2,
            k_colour4,
            k_weight4,
            k_jointIndex4
  
        };
        
        /// An enum describing the possible data types for each component in a vertex
        /// element.
        enum class DataType
        {
            k_byte,
            k_float
        };
        
        /// @param elementType
        ///     The element type.
        ///
        /// @return The data type used for this element.
        ///
        static DataType GetDataType(ElementType elementType) noexcept;
        
        /// @param elementType
        ///     The element type.
        ///
        /// @return The number of components that make up the given element type.
        ///
        static u32 GetNumComponents(ElementType elementType) noexcept;
        
        /// @param elementType
        ///     The element type.
        ///
        /// @return The size of the given element type in bytes.
        ///
        static u32 GetSize(ElementType elementType) noexcept;
        
        /// @param dataType
        ///     The data type.
        ///
        /// @return The size of the given data type in bytes.
        ///
        static u32 GetSize(DataType dataType) noexcept;
        
        VertexFormat() = default;
        
        /// Creates a new vertex format with the given vector of elements.
        ///
        /// @param elements
        ///     The elements that make up the format.
        ///
        VertexFormat(const std::vector<ElementType>& elements) noexcept;
        
        /// @return The number of elements in the vertex format.
        ///
        u32 GetNumElements() const noexcept { return m_numElements; }
        
        /// Gets the element type at the requested index.
        ///
        /// @return index
        ///     The index of the element
        ///
        /// @return The element at the requested index.
        ///
        ElementType GetElement(u32 index) const noexcept;
        
        /// @return The total size of the vertex this this describes in bytes.
        ///
        u32 GetSize() const noexcept { return m_size; }
        
        /// Gets the offset of the element at the given index.
        ///
        /// @param index
        ///     The index of the element.
        ///
        /// @return The offset of the element at the given index.
        ///
        u32 GetElementOffset(u32 index) const noexcept;
        
        /// @param other
        ///     The other vertex format to compare.
        ///
        /// @return Whether or not the two are equal in value.
        ///
        bool operator==(const VertexFormat& other) const noexcept;
        
        /// @param other
        ///     The other vertex format to compare.
        ///
        /// @return Whether or not the two are inequal in value.
        ///
        bool operator!=(const VertexFormat& other) const noexcept;
        
    private:
        u32 m_numElements = 0;
        u32 m_size = 0;
        std::array<ElementType, k_maxElements> m_elements;
    };
}

#endif
