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

#ifndef _CHILLISOURCE_RENDERING_BASE_VERTEXFORMAT_H_
#define _CHILLISOURCE_RENDERING_BASE_VERTEXFORMAT_H_

#include <ChilliSource/ChilliSource.h>

#include <array>
#include <vector>

namespace ChilliSource
{
    /// Describes the format and layout of the vertices used in a single mesh. This
    /// consists of a series of data types and semantics.
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
        
        /// An enum describing the various possible data types in a vertex format.
        ///
        enum class DataType
        {
            k_float2,
            k_float3,
            k_float4,
            k_byte4
        };
        
        /// An enum describing the various possible semantics in a vertex format.
        ///
        enum class Semantic
        {
            k_position,
            k_normal,
            k_uv,
            k_colour,
            k_weight,
            k_jointIndex
        };
        
        /// A container for information pertaining to a single vertex element within a
        /// vertex format.
        ///
        /// This is immutable, and therefore thread-safe.
        ///
        class Element
        {
        public:
            Element() = default;
            
            /// Constructs a new instance with the given data type and semantic.
            ///
            /// @param dataType
            ///     The data type of the element
            /// @param semantic
            ///     The semantic of the element
            ///
            Element(DataType dataType, Semantic semantic) noexcept;
            
            /// @return The data type of the element
            ///
            DataType GetDataType() const noexcept { return m_dataType; }
            
            /// @return The semantic of the element
            ///
            Semantic GetSemantic() const noexcept { return m_semantic; }
            
            /// @return The total size of the element in bytes.
            ///
            u32 GetSize() const noexcept;
            
            /// @return The number of values that make up the element, in other words the dimensions
            ///     of the vector described.
            u32 GetNumValues() const noexcept;
            
            /// @param other
            ///     The other element to compare.
            ///
            /// @return Whether or not the two are equal in value.
            ///
            bool operator==(const VertexFormat::Element& other) const noexcept;
            
            /// @param other
            ///     The other element to compare.
            ///
            /// @return Whether or not the two are inequal in value.
            ///
            bool operator!=(const VertexFormat::Element& other) const noexcept;
            
        private:
            DataType m_dataType = DataType::k_float4;
            Semantic m_semantic = Semantic::k_position;
        };
        
        VertexFormat() = default;
        
        /// Creates a new vertex format with the given vector of elements.
        ///
        /// @param elements
        ///     The elements that make up the format.
        ///
        VertexFormat(const std::vector<Element>& elements) noexcept;
        
        /// @return The number of elements in the vertex format.
        ///
        u32 GetNumElements() const noexcept { return m_numElements; }
        
        /// Gets the element at the requested index.
        ///
        /// @return index
        ///     The index of the element
        ///
        /// @return The element at the requested index.
        ///
        const Element& GetElement(u32 index) const noexcept;
        
        /// @return The total size of the vertex this this describes in bytes.
        ///
        u32 GetSize() const noexcept { return m_size; }
        
        /// @return The total size of the vertex this this describes in bytes.
        ///
        u32 GetNumValues() const noexcept { return m_numValues; }
        
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
        u32 m_numValues = 0;
        std::array<Element, k_maxElements> m_elements;
    };
}

#endif