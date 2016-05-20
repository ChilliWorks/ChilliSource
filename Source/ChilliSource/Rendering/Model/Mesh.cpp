//
//  Mesh.cpp
//  Chilli Source
//  Created by Scott Downie on 06/10/2010.
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

#include <ChilliSource/Rendering/Model/Mesh.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/MeshDescriptor.h>

#include <algorithm>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Mesh);
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    MeshUPtr Mesh::Create()
    {
        return MeshUPtr(new Mesh());
    }
    //--------------------------------------------------------------------
    //--------------------------------------------------------------------
    Mesh::Mesh()
    : m_totalNumVerts(0), m_totalNumIndices(0), m_skeleton(new Skeleton())
    {
    }
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    bool Mesh::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == Mesh::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    bool Mesh::Build(const MeshDescriptor& in_meshDesc)
    {
        //TODO: Re-implement in new system
        return false;
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    const AABB& Mesh::GetAABB() const
    {
        return m_aabb;
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    u32 Mesh::GetNumVerts() const
    {
        return m_totalNumVerts;
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    u32 Mesh::GetNumIndices() const
    {
        return m_totalNumIndices;
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    const Skeleton* Mesh::GetSkeleton() const
    {
        return m_skeleton.get();
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    void Mesh::SetBounds(const Vector3& in_minBounds, const Vector3& in_maxBounds)
    {
        //Calculate the size of this meshes bounding box
        Vector3 vSize = in_maxBounds - in_minBounds;
        
        //Build our bounding box based on the size of all our sub-meshes
        m_aabb = AABB((in_maxBounds + in_minBounds) * 0.5f, vSize);
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    void Mesh::CalcVertexAndIndexCounts()
    {
        //TODO: Re-implement in new system
    }
}
