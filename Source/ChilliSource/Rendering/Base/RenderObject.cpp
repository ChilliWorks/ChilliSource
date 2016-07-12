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

#include <ChilliSource/Rendering/Base/RenderObject.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderObject::RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderMesh* renderMesh, const Matrix4& worldMatrix, const Sphere& boundingSphere, bool shouldCastShadows,
                               RenderLayer renderLayer, u32 priority) noexcept
        : m_type(Type::k_static), m_renderMaterialGroup(renderMaterialGroup), m_renderMesh(renderMesh), m_worldMatrix(worldMatrix), m_boundingSphere(boundingSphere), m_shouldCastShadows(shouldCastShadows),
          m_renderLayer(renderLayer), m_priority(priority)
    {
        CS_ASSERT(m_renderMaterialGroup, "Cannot supply a null render material group.");
        CS_ASSERT(m_renderMesh, "Cannot supply a null render mesh.");
    }
    
    //------------------------------------------------------------------------------
    RenderObject::RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderMesh* renderMesh, const RenderSkinnedAnimation* renderSkinnedAnimation, const Matrix4& worldMatrix,
                               const Sphere& boundingSphere, bool shouldCastShadows, RenderLayer renderLayer, u32 priority) noexcept
        : m_type(Type::k_staticAnimated), m_renderMaterialGroup(renderMaterialGroup), m_renderMesh(renderMesh), m_renderSkinnedAnimation(renderSkinnedAnimation), m_worldMatrix(worldMatrix),
          m_boundingSphere(boundingSphere), m_shouldCastShadows(shouldCastShadows), m_renderLayer(renderLayer), m_priority(priority)
    {
        CS_ASSERT(m_renderMaterialGroup, "Cannot supply a null render material group.");
        CS_ASSERT(m_renderMesh, "Cannot supply a null render mesh.");
        CS_ASSERT(m_renderSkinnedAnimation, "Cannot supply a null render skinned animation.");
    }
    
    //------------------------------------------------------------------------------
    RenderObject::RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderDynamicMesh* renderDynamicMesh, const Matrix4& worldMatrix, const Sphere& boundingSphere, bool shouldCastShadows,
                               RenderLayer renderLayer, u32 priority) noexcept
        : m_type(Type::k_dynamic), m_renderMaterialGroup(renderMaterialGroup), m_renderDynamicMesh(renderDynamicMesh), m_worldMatrix(worldMatrix), m_boundingSphere(boundingSphere),
          m_shouldCastShadows(shouldCastShadows), m_renderLayer(renderLayer), m_priority(priority)
    {
        CS_ASSERT(m_renderMaterialGroup, "Cannot supply a null render material group.");
        CS_ASSERT(m_renderDynamicMesh, "Cannot supply a null render dynamic mesh.");
    }
    
    //------------------------------------------------------------------------------
    RenderObject::RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderDynamicMesh* renderDynamicMesh, const RenderSkinnedAnimation* renderSkinnedAnimation,
                               const Matrix4& worldMatrix, const Sphere& boundingSphere, bool shouldCastShadows, RenderLayer renderLayer, u32 priority) noexcept
        : m_type(Type::k_dynamicAnimated), m_renderMaterialGroup(renderMaterialGroup), m_renderDynamicMesh(renderDynamicMesh), m_renderSkinnedAnimation(renderSkinnedAnimation), m_worldMatrix(worldMatrix),
          m_boundingSphere(boundingSphere), m_shouldCastShadows(shouldCastShadows), m_renderLayer(renderLayer), m_priority(priority)
    {
        CS_ASSERT(m_renderMaterialGroup, "Cannot supply a null render material group.");
        CS_ASSERT(m_renderDynamicMesh, "Cannot supply a null render dynamic mesh.");
        CS_ASSERT(m_renderSkinnedAnimation, "Cannot supply a null render skinned animation.");
    }
}
