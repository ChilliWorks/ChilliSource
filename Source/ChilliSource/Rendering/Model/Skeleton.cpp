//
//  Skeleton.cpp
//  ChilliSource
//  Created by Ian Copland on 13/10/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Rendering/Model/Skeleton.h>

#include <ChilliSource/Rendering/Model/SkeletonDesc.h>

#include <vector>

namespace ChilliSource
{
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    Skeleton::Skeleton(const SkeletonDesc& in_desc) noexcept
    {
        mapNodes.reserve(in_desc.GetNodeNames().size());
        
        for(u32 i=0; i<in_desc.GetNodeNames().size(); ++i)
        {
            SkeletonNode* node(new SkeletonNode());
            node->mstrName = in_desc.GetNodeNames()[i];
            node->mdwParentIndex = in_desc.GetParentNodeIndices()[i];
            mapNodes.push_back(SkeletonNodeCUPtr(node));
        }
        
        madwJoints = in_desc.GetJointIndices();
    }
    //-------------------------------------------------------------------------
    /// Get Node By Name
    //-------------------------------------------------------------------------
    const SkeletonNode* Skeleton::GetNodeByName(const std::string& instrName) const
    {
        for (auto it = mapNodes.begin(); it != mapNodes.end(); ++it)
        {
            if (instrName == (*it)->mstrName)
                return it->get();
        }
        
        return nullptr;
    }
    //-------------------------------------------------------------------------
    /// Get Node Index By Name
    //-------------------------------------------------------------------------
    s32 Skeleton::GetNodeIndexByName(const std::string& instrName) const
    {
        s32 dwIndex = 0;
        for (auto it = mapNodes.begin(); it != mapNodes.end(); ++it)
        {
            if (instrName == (*it)->mstrName)
                return dwIndex;
            dwIndex++;
        }
        return -1;
    }
    //-------------------------------------------------------------------------
    /// Get Node By Index
    //-------------------------------------------------------------------------
    const SkeletonNode* Skeleton::GetNodeByIndex(u32 indwIndex) const
    {
        if (indwIndex < mapNodes.size())
        {
            return mapNodes[indwIndex].get();
        }
        
        return nullptr;
    }
    //-------------------------------------------------------------------------
    /// Get Num Nodes
    //-------------------------------------------------------------------------
    s32 Skeleton::GetNumNodes() const
    {
        return (s32)mapNodes.size();
    }
    //-------------------------------------------------------------------------
    /// Get Num Joints
    ///
    /// returns the number of joints in the skeleton
    ///
    /// @return the number of joints.
    //-------------------------------------------------------------------------
    u32 Skeleton::GetNumJoints() const
    {
        return static_cast<u32>(madwJoints.size());
    }
    //-------------------------------------------------------------------------
    /// Get Nodes
    //-------------------------------------------------------------------------
    const std::vector<SkeletonNodeCUPtr>& Skeleton::GetNodes() const
    {
        return mapNodes;
    }
    //-------------------------------------------------------------------------
    /// Get Joint Indices
    //-------------------------------------------------------------------------
    const std::vector<s32>& Skeleton::GetJointIndices() const
    {
        return madwJoints;
    }
}
