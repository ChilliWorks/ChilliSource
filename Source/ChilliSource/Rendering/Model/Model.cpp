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

#include <ChilliSource/Rendering/Model/Model.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/ModelDesc.h>
#include <ChilliSource/Rendering/Model/RenderMeshManager.h>

#include <algorithm>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Model);
    
    //------------------------------------------------------------------------------
    ModelUPtr Model::Create() noexcept
    {
        return ModelUPtr(new Model());
    }
    
    //------------------------------------------------------------------------------
    bool Model::IsA(InterfaceIDType in_interfaceId) const noexcept
    {
        return in_interfaceId == Model::InterfaceID;
    }
    
    //------------------------------------------------------------------------------
    void Model::Build(ModelDesc modelDesc) noexcept
    {
        //TODO: correctly handle bounding sphere.
        
        m_aabb = modelDesc.GetAABB();
        m_skeleton = Skeleton(modelDesc.GetSkeletonDesc());
        
        auto renderMeshManager = Application::Get()->GetSystem<RenderMeshManager>();
        for (u32 meshIndex = 0; meshIndex < modelDesc.GetNumMeshDescs(); ++meshIndex)
        {
            auto& meshDesc = modelDesc.GetMeshDesc(meshIndex);
            
            auto poylgonType = meshDesc.GetPolygonType();
            auto vertexFormat = meshDesc.GetVertexFormat();
            auto indexFormat = meshDesc.GetIndexFormat();
            auto numVertices = meshDesc.GetNumVertices();
            auto numIndices = meshDesc.GetNumIndices();
            Sphere boundingSphere(Vector3::k_zero, 1.0f);
            auto vertexData = meshDesc.ClaimVertexData();
            auto indexData = meshDesc.ClaimIndexData();
            u32 vertexDataSize = meshDesc.GetNumVertices() * meshDesc.GetVertexFormat().GetSize();
            u32 indexDataSize = meshDesc.GetNumIndices() * GetIndexSize(meshDesc.GetIndexFormat());
            
            auto renderMesh = renderMeshManager->CreateRenderMesh(poylgonType, vertexFormat, indexFormat, numVertices, numIndices, boundingSphere, std::move(vertexData), vertexDataSize, std::move(indexData), indexDataSize);
            m_renderMeshes.push_back(renderMesh);
        }
    }
    
    //------------------------------------------------------------------------------
    const AABB& Model::GetAABB() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access a model before it is loaded.");
        CS_ASSERT(m_renderMeshes.size() > 0, "Cannot access a model which has not been built.");
        
        return m_aabb;
    }
    
    //------------------------------------------------------------------------------
    const Skeleton& Model::GetSkeleton() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access a model before it is loaded.");
        CS_ASSERT(m_renderMeshes.size() > 0, "Cannot access a model which has not been built.");
        
        return m_skeleton;
    }
}
