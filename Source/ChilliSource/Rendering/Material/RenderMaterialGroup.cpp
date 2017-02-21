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

#include <ChilliSource/Rendering/Material/RenderMaterialGroup.h>

#include <ChilliSource/Rendering/Model/VertexFormat.h>

#include <vector>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderMaterialGroup::RenderMaterialGroup(std::vector<UniquePtr<RenderMaterial>> renderMaterials, std::vector<Collection> collections) noexcept
        : m_renderMaterials(std::move(renderMaterials)), m_collections(collections)
    {
        m_renderMaterialsRaw.reserve(m_renderMaterials.size());
        for (const auto& renderMaterial : m_renderMaterials)
        {
            m_renderMaterialsRaw.push_back(renderMaterial.get());
        }
    }
    
    //------------------------------------------------------------------------------
    bool RenderMaterialGroup::Contains(const RenderMaterial* renderMaterial) const noexcept
    {
        for (const auto& material : m_renderMaterialsRaw)
        {
            if (material == renderMaterial)
            {
                return true;
            }
        }
        
        return false;
    }

    //------------------------------------------------------------------------------
    const RenderMaterial* RenderMaterialGroup::GetRenderMaterial(const VertexFormat& vertexFormat, u32 passIndex) const noexcept
    {
        CS_ASSERT(passIndex < k_numMaterialSlots, "Pass index is out of bounds.");
        
        for (const auto& collection : m_collections)
        {
            if (collection.GetVertexFormat() == vertexFormat)
            {
                return collection.GetRenderMaterial(passIndex);
            }
        }
        
        CS_LOG_FATAL("Material group does not support the requested vertex format.");
        return nullptr;
    }
    
    //------------------------------------------------------------------------------
    RenderMaterialGroup::Collection::Collection(const VertexFormat& vertexFormat, const std::array<const RenderMaterial*, k_numMaterialSlots>& renderMaterials) noexcept
        : m_vertexFormat(vertexFormat), m_renderMaterials(renderMaterials)
    {
    }
    
    //------------------------------------------------------------------------------
    const RenderMaterial* RenderMaterialGroup::Collection::GetRenderMaterial(u32 passIndex) const noexcept
    {
        CS_ASSERT(passIndex < k_numMaterialSlots, "Pass index is out of bounds.");
        
        return m_renderMaterials[passIndex];
    }
}
