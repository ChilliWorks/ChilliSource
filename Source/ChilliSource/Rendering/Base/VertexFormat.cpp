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

#include <ChilliSource/Rendering/Base/VertexFormat.h>

namespace ChilliSource
{
    namespace
    {
        const std::vector<VertexFormat::Element> k_staticMeshElements =
        {
            VertexFormat::Element(VertexFormat::DataType::k_float4, VertexFormat::Semantic::k_position),
            VertexFormat::Element(VertexFormat::DataType::k_float3, VertexFormat::Semantic::k_normal),
            VertexFormat::Element(VertexFormat::DataType::k_float2, VertexFormat::Semantic::k_uv)
        };
        
        const std::vector<VertexFormat::Element> k_animatedMeshElements =
        {
            VertexFormat::Element(VertexFormat::DataType::k_float4, VertexFormat::Semantic::k_position),
            VertexFormat::Element(VertexFormat::DataType::k_float3, VertexFormat::Semantic::k_normal),
            VertexFormat::Element(VertexFormat::DataType::k_float2, VertexFormat::Semantic::k_uv),
            VertexFormat::Element(VertexFormat::DataType::k_float2, VertexFormat::Semantic::k_weight),
            VertexFormat::Element(VertexFormat::DataType::k_float2, VertexFormat::Semantic::k_jointIndex)
        };
        
        const std::vector<VertexFormat::Element> k_spriteElements =
        {
            VertexFormat::Element(VertexFormat::DataType::k_float4, VertexFormat::Semantic::k_position),
            VertexFormat::Element(VertexFormat::DataType::k_float3, VertexFormat::Semantic::k_normal),
            VertexFormat::Element(VertexFormat::DataType::k_float2, VertexFormat::Semantic::k_uv),
            VertexFormat::Element(VertexFormat::DataType::k_byte4, VertexFormat::Semantic::k_colour)
        };
    }
    
    const VertexFormat VertexFormat::k_staticMesh(k_staticMeshElements);
    const VertexFormat VertexFormat::k_animatedMesh(k_animatedMeshElements);
    const VertexFormat VertexFormat::k_sprite(k_spriteElements);
    
    //------------------------------------------------------------------------------
    VertexFormat::VertexFormat(const std::vector<Element>& elements) noexcept
        : m_numElements(u32(elements.size()))
    {
        for (u32 index = 0; index < m_numElements; ++index)
        {
            m_elements[index] = elements[index];
            
            m_size += m_elements[index].GetSize();
            m_numValues += m_elements[index].GetNumValues();
        }
    }

    //------------------------------------------------------------------------------
    const VertexFormat::Element& VertexFormat::GetElement(u32 index) const noexcept
    {
        CS_ASSERT(index < m_numElements, "Index out of bounds.");
        
        return m_elements[index];
    }

    //------------------------------------------------------------------------------
    u32 VertexFormat::GetElementOffset(u32 index) const noexcept
    {
        CS_ASSERT(index < m_numElements, "Index out of bounds");
        
        u32 offset = 0;
        
        for (u32 i = 0; i < index; ++i)
        {
            offset += m_elements[i].GetSize();
        }
        
        return offset;
    }
    
    //------------------------------------------------------------------------------
    bool VertexFormat::operator==(const VertexFormat& other) const noexcept
    {
        if (this == &other)
        {
            return true;
        }
        
        if (m_numElements != other.m_numElements || m_size != other.m_size || m_numValues != other.m_numValues)
        {
            return false;
        }
        
        for (u32 index = 0; index < m_numElements; ++index)
        {
            if (m_elements[index] != other.m_elements[index])
            {
                return false;
            }
        }
        
        return true;
    }
    
    //------------------------------------------------------------------------------
    bool VertexFormat::operator!=(const VertexFormat& other) const noexcept
    {
        return !(*this == other);
    }
    
    //------------------------------------------------------------------------------
    VertexFormat::Element::Element(DataType dataType, Semantic semantic) noexcept
        : m_dataType(dataType), m_semantic(semantic)
    {
    }
    
    //------------------------------------------------------------------------------
    u32 VertexFormat::Element::GetSize() const noexcept
    {
        switch (m_dataType)
        {
            case VertexFormat::DataType::k_float2:
                return 2;
            case VertexFormat::DataType::k_float3:
                return 3;
            case VertexFormat::DataType::k_float4:
                return 4;
            case VertexFormat::DataType::k_byte4:
                return 4;
            default:
                CS_LOG_FATAL("Invalid vertex element data type.");
                return 0;
        }
    }
    
    //------------------------------------------------------------------------------
    u32 VertexFormat::Element::GetNumValues() const noexcept
    {
        switch (m_dataType)
        {
            case VertexFormat::DataType::k_float2:
                return 2 * sizeof(f32);
            case VertexFormat::DataType::k_float3:
                return 3 * sizeof(f32);
            case VertexFormat::DataType::k_float4:
                return 4 * sizeof(f32);
            case VertexFormat::DataType::k_byte4:
                return 4 * sizeof(u8);
            default:
                CS_LOG_FATAL("Invalid vertex element data type.");
                return 0;
        }
    }
    
    //------------------------------------------------------------------------------
    bool VertexFormat::Element::operator==(const VertexFormat::Element& other) const noexcept
    {
        return m_dataType == other.m_dataType && m_semantic == other.m_semantic;
    }
    
    //------------------------------------------------------------------------------
    bool VertexFormat::Element::operator!=(const VertexFormat::Element& other) const noexcept
    {
        return !(*this == other);
    }
}