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
        DestroyRenderMeshes();
        
        m_aabb = modelDesc.GetAABB();
        m_boundingSphere = modelDesc.GetBoundingSphere();
        m_skeleton = Skeleton(modelDesc.GetSkeletonDesc());
        
        auto renderMeshManager = Application::Get()->GetSystem<RenderMeshManager>();
        for (u32 meshIndex = 0; meshIndex < modelDesc.GetNumMeshDescs(); ++meshIndex)
        {
            auto& meshDesc = modelDesc.GetMeshDesc(meshIndex);
            
            m_meshNames.push_back(meshDesc.GetName());
            
            auto poylgonType = meshDesc.GetPolygonType();
            auto vertexFormat = meshDesc.GetVertexFormat();
            auto indexFormat = meshDesc.GetIndexFormat();
            auto numVertices = meshDesc.GetNumVertices();
            auto numIndices = meshDesc.GetNumIndices();
            auto boundingSphere = meshDesc.GetBoundingSphere();
            auto vertexData = meshDesc.ClaimVertexData();
            auto indexData = meshDesc.ClaimIndexData();
            auto vertexDataSize = meshDesc.GetNumVertices() * meshDesc.GetVertexFormat().GetSize();
            auto indexDataSize = meshDesc.GetNumIndices() * GetIndexSize(meshDesc.GetIndexFormat());
            auto inverseBindPoseMatrices = meshDesc.ClaimInverseBindPoseMatrices();
            
            auto renderMesh = renderMeshManager->CreateRenderMesh(poylgonType, vertexFormat, indexFormat, numVertices, numIndices, boundingSphere, std::move(vertexData), vertexDataSize, std::move(indexData), indexDataSize,
                                                                  modelDesc.ShouldBackupData(), std::move(inverseBindPoseMatrices));
            m_renderMeshes.push_back(std::move(renderMesh));
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
    const Sphere& Model::GetBoundingSphere() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access a model before it is loaded.");
        CS_ASSERT(m_renderMeshes.size() > 0, "Cannot access a model which has not been built.");
        
        return m_boundingSphere;
    }
    
    //------------------------------------------------------------------------------
    const Skeleton& Model::GetSkeleton() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access a model before it is loaded.");
        CS_ASSERT(m_renderMeshes.size() > 0, "Cannot access a model which has not been built.");
        
        return m_skeleton;
    }
    
    //------------------------------------------------------------------------------
    u32 Model::GetNumMeshes() const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access a model before it is loaded.");
        CS_ASSERT(m_renderMeshes.size() > 0, "Cannot access a model which has not been built.");
        
        return u32(m_renderMeshes.size());
    }
    
    //------------------------------------------------------------------------------
    const std::string& Model::GetMeshName(u32 index) const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access a model before it is loaded.");
        CS_ASSERT(m_renderMeshes.size() > 0, "Cannot access a model which has not been built.");
        CS_ASSERT(index < m_meshNames.size(), "Index is out of bounds.");
        
        return m_meshNames[index];
    }
    
    //------------------------------------------------------------------------------
    const u32 Model::GetMeshIndex(const std::string& name) const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access a model before it is loaded.");
        CS_ASSERT(m_renderMeshes.size() > 0, "Cannot access a model which has not been built.");
        
        for (u32 index = 0; index < m_meshNames.size(); ++index)
        {
            if (m_meshNames[index] == name)
            {
                return index;
            }
        }
        
        CS_LOG_FATAL("Could not find mesh name with name '" + name + "' in model.");
        return 0;
    }
    
    //------------------------------------------------------------------------------
    const RenderMesh* Model::GetRenderMesh(u32 index) const noexcept
    {
        CS_ASSERT(GetLoadState() == LoadState::k_loaded, "Cannot access a model before it is loaded.");
        CS_ASSERT(m_renderMeshes.size() > 0, "Cannot access a model which has not been built.");
        CS_ASSERT(index < m_meshNames.size(), "Index is out of bounds.");
        
        return m_renderMeshes[index].get();
    }
    
    //------------------------------------------------------------------------------
    void Model::DestroyRenderMeshes() noexcept
    {
        auto renderMeshManager = Application::Get()->GetSystem<RenderMeshManager>();
        for (auto& renderMesh : m_renderMeshes)
        {
            renderMeshManager->DestroyRenderMesh(std::move(renderMesh));
        }
        m_renderMeshes.clear();
        m_meshNames.clear();
    }
    
    //------------------------------------------------------------------------------
    Model::~Model() noexcept
    {
        DestroyRenderMeshes();
    }
}
