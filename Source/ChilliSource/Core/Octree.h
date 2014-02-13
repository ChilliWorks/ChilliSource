/*
 *  Octree.h
 *  moFloTest
 *
 *  Created by Scott Downie on 20/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

//==========================================================================
/// Description:
///
/// Octree
///
/// A loose octree implementation. This is a rougher version of the 3D
/// spatial subdivision tree, that is more suited to dynamic objects.
/// Objects are place into octants based on their radius and their radius
/// is assumed to be twice the size. An object will be placed in the tree
/// at the point where it's radius overlaps the octant.
/// This method means that we know how deep an object will be inserted
/// based on it's radius.
///
/// Unlike standard Octrees the loose implementation will place an object
/// into an the deepest octant that contains its centre within the inner 
/// boundary while the object fits entirely into the node’s outer boundary.
///
/// The greater the outer boundary the higher chance of hotspots (i.e overlaps
/// that cause the object to be tested in two nodes).
///
/// Due to performance issues we have implemented a linear octree. This
/// uses an array over a traditional tree structure to store the nodes.
///
/// This means the size of the tree is static.
//==========================================================================

#ifndef _MO_FLO_CORE_OCTREE_H_
#define _MO_FLO_CORE_OCTREE_H_

#include <moFlo/Core/ForwardDeclarations.h>
#include <moFlo/Core/Math/Vector3.h>
#include <moFlo/Core/Math/Shapes.h>
#include <moFlo/Core/Component.h>

#include <moFlo/Rendering/ForwardDeclarations.h>

const u32 kudwMaximumTreeDepth  = 5;
const u32 kudwNumChildNodes     = 8;
const u32 kudwTotalNumNodes     = 4681; //NOTE: If the depth is changed then this must be changed

const f32 kfLooseScale          = 2;    //NOTE: DO NO CHANGE THIS!

namespace moFlo
{
	namespace Core
	{
		class COctree
		{
            typedef DYNAMIC_ARRAY<VolumeComponentPtr> ObjectList;
            
            struct Node
			{
                ObjectList Components;	//List of components that reside in this node.
                
                Sphere BoundingSphere;  //Intersection volume
                
                u32 udwTreeOffset;      //Index of the node in the tree
				u32 udwDepth;			//The depth of the node within the tree
			};
			
		public:
			COctree(const Core::CVector3& invWorldCentre, f32 infHalfSize);
			~COctree();
            //--------------------------------------------------------------------------------------------------
            /// Query Scene For Visible Components
            ///
            /// Traverse the octree for render components within the frustum and fill the list with 
            /// those components
            ///
            /// @param Frustum
            /// @param Empty container to be filled
            /// @param Query mask
            //--------------------------------------------------------------------------------------------------
            void QuerySceneForVisibleComponents(const Core::CFrustum* inpFrustum, DYNAMIC_ARRAY<ComponentPtr> &outComponents, u32 inudwQueryMask);
			//---------------------------------------------------------------
			/// Add 
			///
			/// Add an object to the tree by testing it's bounding volume 
			/// to identify which node/octant it resides in
			///
			/// @param Node to add 
			//---------------------------------------------------------------
			void Add(const VolumeComponentPtr& inpComponent);
			//---------------------------------------------------------------
			/// Remove 
			///
			/// Find the given object in the tree and remove it
			///
			/// @param Node to remove
			//---------------------------------------------------------------
			void Remove(const VolumeComponentPtr& inpComponent);
			//---------------------------------------------------------------
			/// Destroy
			///
			/// Release all the memory held by the octree
			//---------------------------------------------------------------
			void Destroy();
			
		private:
			//---------------------------------------------------------------
			/// Create Octant
			///
			/// Create a new spatial cube. This function is recursive
			///
			/// @param Parent spatial partition centre
			/// @param Parent cube half width and height
			/// @param Current tree depth
			//---------------------------------------------------------------
			COctree::Node* CreateOctant(const Core::CVector3& invWorldCentre, f32 infHalfSize, s32 inuwDepth, u32 inudwOffset);
            //----------------------------------------------------------
			/// Get Child Node Attached To Node
			///
			/// Recurse through all the child nodes and get the 
			/// entities from them
			//----------------------------------------------------------
			void GetChildNodeAttachedToNode(COctree::Node* pNode, const Core::CFrustum* inpFrustum, DYNAMIC_ARRAY<ComponentPtr> &outComponents, u32 inudwQueryMask);
			//----------------------------------------------------------
			/// Get Components Attached To Node
			///
			/// Recurse through all the components and do the cull
            /// test
			//----------------------------------------------------------
			void GetComponentsAttachedToNode(COctree::Node* pNode, DYNAMIC_ARRAY<ComponentPtr> &outComponents, u32 inudwQueryMask);
			
		private:
		
			//Root node 
            Node* mpRoot;
            //Linear octree data structure
            Node* mTree[kudwTotalNumNodes];
			
			f32 mfWorldWidth;
			
			u32 mudwCurrentNumNodes;
		};
	}
}

#endif
