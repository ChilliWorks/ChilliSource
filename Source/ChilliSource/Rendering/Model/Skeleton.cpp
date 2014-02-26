/*
 *  Skeleton.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 13/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/Skeleton.h>

#include <vector>

namespace ChilliSource
{
	namespace Rendering
	{
		Skeleton::Skeleton()
		{
			
		}
		
		Skeleton::~Skeleton()
		{
			
		}
		//-------------------------------------------------------------------------
		/// Get Node By Name
		//-------------------------------------------------------------------------
		SkeletonNodePtr Skeleton::GetNodeByName(const std::string& instrName) const
		{
			for (std::vector<SkeletonNodePtr>::const_iterator it = mapNodes.begin(); it != mapNodes.end(); ++it)
			{
				if (instrName == (*it)->mstrName)
					return *it;
			}
			
			return SkeletonNodePtr();
		}
        //-------------------------------------------------------------------------
        /// Get Node Index By Name
        //-------------------------------------------------------------------------
        s32 Skeleton::GetNodeIndexByName(const std::string& instrName) const
        {
            s32 dwIndex = 0;
            for (std::vector<SkeletonNodePtr>::const_iterator it = mapNodes.begin(); it != mapNodes.end(); ++it)
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
		SkeletonNodePtr Skeleton::GetNodeByIndex(u32 indwIndex) const
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
		const std::vector<SkeletonNodePtr>& Skeleton::GetNodes() const
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
		//-------------------------------------------------------------------------
		/// Add Node
		//-------------------------------------------------------------------------
		void Skeleton::AddNode(const std::string& instrName, s32 indwParentIndex)
		{
			SkeletonNodePtr newNode(new SkeletonNode());
			newNode->mstrName = instrName;
			newNode->mdwParentIndex = indwParentIndex;
			mapNodes.push_back(newNode);
		}
        //-------------------------------------------------------------------------
        /// Add Joint Index
        //-------------------------------------------------------------------------
        void Skeleton::AddJointIndex(s32 indwJointIndex)
        {
            madwJoints.push_back(indwJointIndex);
        }
	}
}