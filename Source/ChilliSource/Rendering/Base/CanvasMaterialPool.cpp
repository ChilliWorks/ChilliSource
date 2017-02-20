//
//  CanvasMaterialPool.cpp
//  ChilliSource
//  Created by Ian Copland on 30/07/2015
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <ChilliSource/Rendering/Base/CanvasMaterialPool.h>

#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    CanvasMaterialPool::CanvasMaterialPool(MaterialFactory* materialFactory, const std::string& materialNamePrefix, const MaterialSetupDelegate& materialSetupDelegate)
        : m_materialFactory(materialFactory), m_materialNamePrefix(materialNamePrefix), m_materialSetupDelegate(materialSetupDelegate)
    {
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    MaterialCSPtr CanvasMaterialPool::GetMaterial(const TextureCSPtr& in_texture, s32 stencilRef)
    {
        auto it = m_associations.find(std::make_tuple(in_texture.get(), stencilRef));
        if(it != m_associations.end())
        {
            return it->second;
        }
        
        if (m_nextMaterial >= m_materials.size())
        {
            auto material = m_materialFactory->CreateCustom(m_materialNamePrefix + ToString(m_materials.size()));
            m_materialSetupDelegate(material.get());
            m_materials.push_back(material);
            CS_ASSERT(m_nextMaterial < m_materials.size(), "We've added a new material yet we still don't have enough - something has gone wrong.");
        }
        
        auto material = m_materials[m_nextMaterial++];
        material->AddTexture(in_texture);
        material->SetStencilTestFunc(material->GetStencilTestFunc(), stencilRef, material->GetStencilTestFuncMask());
        material->SetLoadState(Resource::LoadState::k_loaded);
        
        m_associations.emplace(std::make_tuple(in_texture.get(), stencilRef), material);
        return material;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void CanvasMaterialPool::Clear()
    {
        m_nextMaterial = 0;
        m_associations.clear();
        
        for (auto& material : m_materials)
        {
            material->SetLoadState(Resource::LoadState::k_loading);
            material->RemoveAllTextures();
        }
    }
}
