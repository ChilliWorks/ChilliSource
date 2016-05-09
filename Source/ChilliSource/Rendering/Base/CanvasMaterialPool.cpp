//
//  CanvasMaterialPool.cpp
//  Chilli Source
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
    CanvasMaterialPool::CanvasMaterialPool(MaterialFactory* in_materialFactory)
        : m_materialFactory(in_materialFactory)
    {
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    MaterialCSPtr CanvasMaterialPool::GetMaterial(const TextureCSPtr& in_texture)
    {
        auto it = m_associations.find(in_texture.get());
        if(it != m_associations.end())
        {
            return it->second;
        }
        
        if (m_nextMaterial >= m_materials.size())
        {
            m_materials.push_back(m_materialFactory->CreateGUI("_Canvas-" + ToString(m_materials.size())));
            CS_ASSERT(m_nextMaterial < m_materials.size(), "We've added a new material yet we still don't have enough - something has gone wrong.");
        }
        
        auto material = m_materials[m_nextMaterial++];
        material->AddTexture(in_texture);
        m_associations.emplace(in_texture.get(), material);
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
            material->RemoveAllTextures();
        }
    }
}
