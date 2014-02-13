/*
 *  Skeleton.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 13/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <moFlo/Rendering/Skeleton.h>

namespace moFlo
{
	namespace Rendering
	{
		CSkeleton::CSkeleton()
		{
			
		}
		
		CSkeleton::~CSkeleton()
		{
			
		}
		//-------------------------------------------------------------------------
		/// Get Node By Name
		//-------------------------------------------------------------------------
		SkeletonNodePtr CSkeleton::GetNodeByName(const std::string& instrName) const
		{
			for (DYNAMIC_ARRAY<SkeletonNodePtr>::const_iterator it = mapNodes.begin(); it != mapNodes.end(); ++it)
			{
				if (instrName == (*it)->mstrName)
					return *it;
			}
			
			return SkeletonNodePtr();
		}
        //-------------------------------------------------------------------------
        /// Get Node Index By Name
        //-------------------------------------------------------------------------
        s32 CSkeleton::GetNodeIndexByName(const std::string& instrName) const
        {
            s32 dwIndex = 0;
            for (DYNAMIC_ARRAY<SkeletonNodePtr>::const_iterator it = mapNodes.begin(); it != mapNodes.end(); ++it)
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
		SkeletonNodePtr CSkeleton::GetNodeByIndex(u32 indwIndex) const
		{
			if (indwIndex < mapNodes.size())
			{
				return mapNodes[indwIndex];
			}
			
			return SkeletonNodePtr();
		}
		//-------------------------------------------------------------------------
		/// Get Num Nodes
		//-------------------------------------------------------------------------
		s32 CSkeleton::GetNumNodes() const
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
        u32 CSkeleton::GetNumJoints() const
        {
            return madwJoints.size();
        }
		//-------------------------------------------------------------------------
		/// Get Nodes
		//-------------------------------------------------------------------------
		const DYNAMIC_ARRAY<SkeletonNodePtr>& CSkeleton::GetNodes() const
		{
			return mapNodes;
		}
        //-------------------------------------------------------------------------
        /// Get Joint Indices
        //-------------------------------------------------------------------------
        const DYNAMIC_ARRAY<s32>& CSkeleton::GetJointIndices() const
        {
            return madwJoints;
        }
		//-------------------------------------------------------------------------
		/// Add Node
		//-------------------------------------------------------------------------
		void CSkeleton::AddNode(const std::string& instrName, s32 indwParentIndex)
		{
			SkeletonNodePtr newNode(new SkeletonNode());
			newNode->mstrName = instrName;
			newNode->mdwParentIndex = indwParentIndex;
			mapNodes.push_back(newNode);
		}
        //-------------------------------------------------------------------------
        /// Add Joint Index
        //-------------------------------------------------------------------------
        void CSkeleton::AddJointIndex(s32 indwJointIndex)
        {
            madwJoints.push_back(indwJointIndex);
        }
	}
}