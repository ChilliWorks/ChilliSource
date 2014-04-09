/*
 *  Skeleton.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 13/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/Skeleton.h>

#include <ChilliSource/Rendering/Model/MeshDescriptor.h>

#include <vector>

namespace ChilliSource
{
	namespace Rendering
	{
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        void Skeleton::Build(const SkeletonDescriptor& in_desc)
        {
            mapNodes.reserve(in_desc.m_nodeNames.size());
            
            for(u32 i=0; i<in_desc.m_nodeNames.size(); ++i)
            {
                SkeletonNode* node(new SkeletonNode());
                node->mstrName = in_desc.m_nodeNames[i];
                node->mdwParentIndex = in_desc.m_parentNodeIndices[i];
                mapNodes.push_back(SkeletonNodeCUPtr(node));
            }
            
            madwJoints = in_desc.m_jointIndices;
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
            return madwJoints.size();
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
}