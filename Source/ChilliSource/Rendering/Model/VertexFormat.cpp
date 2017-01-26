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

#include <ChilliSource/Rendering/Model/VertexFormat.h>

namespace ChilliSource
{
    namespace
    {
        const std::vector<VertexFormat::ElementType> k_staticMeshElements =
        {
            VertexFormat::ElementType::k_position4,
            VertexFormat::ElementType::k_normal3,
            VertexFormat::ElementType::k_tangent3,
            VertexFormat::ElementType::k_bitangent3,
            VertexFormat::ElementType::k_uv2
        };
        
        const std::vector<VertexFormat::ElementType> k_animatedMeshElements =
        {
            VertexFormat::ElementType::k_position4,
            VertexFormat::ElementType::k_normal3,
            VertexFormat::ElementType::k_tangent3,
            VertexFormat::ElementType::k_bitangent3,
            VertexFormat::ElementType::k_uv2,
            VertexFormat::ElementType::k_weight4,
            VertexFormat::ElementType::k_jointIndex4
        };
        
        const std::vector<VertexFormat::ElementType> k_spriteElements =
        {
            VertexFormat::ElementType::k_position4,
            VertexFormat::ElementType::k_uv2,
            VertexFormat::ElementType::k_colour4
        };
    }
    
    const VertexFormat VertexFormat::k_staticMesh(k_staticMeshElements);
    const VertexFormat VertexFormat::k_animatedMesh(k_animatedMeshElements);
    const VertexFormat VertexFormat::k_sprite(k_spriteElements);
    
    //------------------------------------------------------------------------------
    VertexFormat::DataType VertexFormat::GetDataType(ElementType elementType) noexcept
    {
        switch (elementType)
        {
            case ElementType::k_position4:
                return DataType::k_float;
            case ElementType::k_normal3:
                return DataType::k_float;
            case ElementType::k_tangent3:
                return DataType::k_float;
            case ElementType::k_bitangent3:
                return DataType::k_float;
            case ElementType::k_uv2:
                return DataType::k_float;
            case ElementType::k_colour4:
                return DataType::k_byte;
            case ElementType::k_weight4:
                return DataType::k_float;
            case ElementType::k_jointIndex4:
                return DataType::k_byte;
            default:
                CS_LOG_FATAL("Invalid vertex element type.");
                return DataType::k_byte;
        }
    }
    
    //------------------------------------------------------------------------------
    u32 VertexFormat::GetNumComponents(ElementType elementType) noexcept
    {
        switch (elementType)
        {
            case ElementType::k_position4:
                return 4;
            case ElementType::k_normal3:
                return 3;
            case ElementType::k_tangent3:
                return 3;
            case ElementType::k_bitangent3:
                return 3;
            case ElementType::k_uv2:
                return 2;
            case ElementType::k_colour4:
                return 4;
            case ElementType::k_weight4:
                return 4;
            case ElementType::k_jointIndex4:
                return 4;
            default:
                CS_LOG_FATAL("Invalid vertex element type.");
                return 0;
        }
    }
    
    //------------------------------------------------------------------------------
    u32 VertexFormat::GetSize(ElementType elementType) noexcept
    {
        return GetSize(GetDataType(elementType)) * GetNumComponents(elementType);
    }
    
    //------------------------------------------------------------------------------
    u32 VertexFormat::GetSize(DataType dataType) noexcept
    {
        switch (dataType)
        {
            case DataType::k_byte:
                return sizeof(u8);
            case DataType::k_float:
                return sizeof(f32);
            default:
                CS_LOG_FATAL("Invalid vertex data type.");
                return 0;
        }
    }
    
    //------------------------------------------------------------------------------
    VertexFormat::VertexFormat(const std::vector<ElementType>& elements) noexcept
        : m_numElements(u32(elements.size()))
    {
        for (u32 index = 0; index < m_numElements; ++index)
        {
            m_elements[index] = elements[index];
            
            m_size += GetSize(m_elements[index]);
        }
    }

    //------------------------------------------------------------------------------
    VertexFormat::ElementType VertexFormat::GetElement(u32 index) const noexcept
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
            offset += GetSize(m_elements[i]);
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
        
        if (m_numElements != other.m_numElements || m_size != other.m_size)
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
}
