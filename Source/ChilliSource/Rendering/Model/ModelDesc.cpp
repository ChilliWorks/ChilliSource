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

#include <ChilliSource/Rendering/Model/ModelDesc.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    ModelDesc::ModelDesc(std::vector<MeshDesc> meshDescs, const AABB& aabb, const Sphere& boundingSphere, bool shouldBackupData) noexcept
        : m_meshDescs(std::move(meshDescs)), m_aabb(aabb), m_boundingSphere(boundingSphere), m_shouldBackupData(shouldBackupData)
    {
    }
    
    //------------------------------------------------------------------------------
    ModelDesc::ModelDesc(std::vector<MeshDesc> meshDescs, const AABB& aabb, const Sphere& boundingSphere, const SkeletonDesc& skeletonDesc, bool shouldBackupData) noexcept
    : m_meshDescs(std::move(meshDescs)), m_aabb(aabb), m_boundingSphere(boundingSphere), m_skeletonDesc(skeletonDesc), m_shouldBackupData(shouldBackupData)
    {
    }
    
    //------------------------------------------------------------------------------
    MeshDesc& ModelDesc::GetMeshDesc(u32 index) noexcept
    {
        CS_ASSERT(index < GetNumMeshDescs(), "Index out of bounds.");
        
        return m_meshDescs[index];
    }
    
    //------------------------------------------------------------------------------
    const MeshDesc& ModelDesc::GetMeshDesc(u32 index) const noexcept
    {
        CS_ASSERT(index < GetNumMeshDescs(), "Index out of bounds.");
        
        return m_meshDescs[index];
    }
}
