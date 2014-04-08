/*
 *  Skeleton.h
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 13/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_SKELETON_H_
#define _MOFLO_RENDERING_SKELETON_H_

#include <ChilliSource/ChilliSource.h>

#include <vector>

namespace ChilliSource
{
	namespace Rendering
	{
        //=========================================================================
		/// Skeleton Node
		///
		/// a struct describing a single skeleton node.
		//=========================================================================
		struct SkeletonNode
		{
			std::string mstrName;
			s32 mdwParentIndex;
		};
		//=========================================================================
		/// Skeleton
		///
		/// a simple class that describes a skeleton for skeletal animation.
		//=========================================================================
		class Skeleton
		{
		public:
            //-------------------------------------------------------------------------
            /// Construct the skeleton from the given descriptor.
            ///
            /// @author S Downie
            ///
            /// @param Skeleton descriptor
            //-------------------------------------------------------------------------
            void Build(const SkeletonDescriptor& in_desc);
			//-------------------------------------------------------------------------
			/// Get Node By Name
			///
			/// returns the node with the given name.
			///
			/// @param the nodes name
			/// @return a smart pointer to the node
			//-------------------------------------------------------------------------
			const SkeletonNode* GetNodeByName(const std::string& instrName) const;
            //-------------------------------------------------------------------------
			/// Get Node Index By Name
			///
			/// returns the index of the node with the given name. This will return
            /// -1 if no node with the given name could be found.
			///
			/// @param the nodes name
			/// @return The nodes index
			//-------------------------------------------------------------------------
			s32 GetNodeIndexByName(const std::string& instrName) const;
			//-------------------------------------------------------------------------
			/// Get Node By Index
			///
			/// returns the node at the given index.
			///
			/// @param the index.
			/// @param a smart pointer to the node.
			//-------------------------------------------------------------------------
			const SkeletonNode* GetNodeByIndex(u32 indwIndex) const;
			//-------------------------------------------------------------------------
			/// Get Num Nodes
			///
			/// returns the number of nodes in the skeleton
			///
			/// @return the number of nodes
			//-------------------------------------------------------------------------
			s32 GetNumNodes() const;
            //-------------------------------------------------------------------------
			/// Get Num Joints
			///
			/// returns the number of joints in the skeleton
			///
			/// @return the number of joints.
			//-------------------------------------------------------------------------
			u32 GetNumJoints() const;
			//-------------------------------------------------------------------------
			/// Get Nodes
			///
			/// returns the array of nodes
			///
			/// @return the dynamic array of nodes
			//-------------------------------------------------------------------------
			const std::vector<SkeletonNodeCUPtr>& GetNodes() const;
            //-------------------------------------------------------------------------
			/// Get Joint Indices
			///
			/// @return the array of joint indices
			//-------------------------------------------------------------------------
			const std::vector<s32>& GetJointIndices() const;
			
		private:
			
			std::vector<SkeletonNodeCUPtr> mapNodes;
            std::vector<s32> madwJoints;
		};
	}
}
#endif