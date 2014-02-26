/*
 *  Octree.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 20/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Container/Octree.h>

#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Volume/VolumeComponent.h>

#include <limits>

namespace ChilliSource
{
	namespace Core
	{
		//----------------------------------------------------------------
		/// Constructor
		///
		/// @param Centre point of the game world
		/// @param Half width and height of the box enclosing the world
		//----------------------------------------------------------------
		COctree::COctree(const Core::CVector3& invWorldCentre, f32 infHalfSize): mudwCurrentNumNodes(0), mfWorldWidth(infHalfSize * 2.0f * kfLooseScale)
		{
			//Create the root node and allocate the octants upfront. 
			mpRoot = mTree[0] = CreateOctant(invWorldCentre, infHalfSize, 0, 0);
		}
		//---------------------------------------------------------------
		/// Create Octant
		///
		/// Create a new spatial cube. This function is recursive
		///
		/// @param Parent spatial partition centre
		/// @param Parent cube half width and height
		/// @param Current tree depth
		//---------------------------------------------------------------
		COctree::Node* COctree::CreateOctant(const Core::CVector3& invCentre, f32 infHalfSize, s32 inuwDepth, u32 inudwOffset)
		{
            inuwDepth += 1;
            
			//Once we have reached a certain tree depth then the octants have all be created
			if(inuwDepth >= (s32)kudwMaximumTreeDepth)
			{
				return nullptr;
			}
			
			//Create the new child node
			COctree::Node* pNode = new Node();
			pNode->BoundingSphere = Sphere(invCentre, infHalfSize * kfLooseScale);
			pNode->udwDepth = inuwDepth;
            pNode->udwTreeOffset = inudwOffset;

			Core::CVector3 vOffset;
			f32 fStep = infHalfSize * 0.5f;
			
			//Recurively construct the eight grand-children of the node
			for(u32 i=1; i<=kudwNumChildNodes; ++i)
			{
				//Calculate the centre of the child
				vOffset.x = ((i & 1) ? fStep : -fStep);
				vOffset.y = ((i & 2) ? fStep : -fStep);
				vOffset.z = ((i & 4) ? fStep : -fStep);
				
				//Now create our child until the we run out of memory. Only joking we stop at some point
                u32 udwChildOffset = kudwNumChildNodes*inudwOffset + i;
				mTree[udwChildOffset] = CreateOctant(invCentre + vOffset, fStep, inuwDepth, udwChildOffset);
			}
			
			mudwCurrentNumNodes++;
			
			return pNode;
		}
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
        void COctree::QuerySceneForVisibleComponents(const Core::CFrustum* inpFrustum, std::vector<ComponentPtr> &outComponents, u32 inudwQueryMask)
        {
            //Check if the node is inside the frustum
            if(inpFrustum->SphereCullTest(mpRoot->BoundingSphere))
            {
                GetComponentsAttachedToNode(mpRoot, outComponents, inudwQueryMask);
                //Now start recursively getting the children
                GetChildNodeAttachedToNode(mpRoot, inpFrustum, outComponents, inudwQueryMask);
            }
        }
		//---------------------------------------------------------------
		/// Add 
		///
		/// Add an object to the tree by testing it's bounding volume 
		/// to identify which node/octant it resides in
		///
		/// @param Node to add 
		//---------------------------------------------------------------
		void COctree::Add(const VolumeComponentPtr& inpComponent)
		{
			//We can use an equation to work out how deep to place the
			//component and which node to attach it to without recursing the entire tree.
			Sphere BSphere = inpComponent->GetBoundingSphere();
            
            u32 udwDepth = 0;
            
            //If the object is too big for the world just add it to the root node
            if(mpRoot->BoundingSphere.Contains(BSphere))
            {
                udwDepth = std::min((u32)floorf(CMathUtils::Log2f(mfWorldWidth/BSphere.fRadius)), kudwMaximumTreeDepth);
                
                //Insert it into the tree
                mTree[udwDepth]->Components.push_back(inpComponent);
            }
            else
            {
                //Insert it into the tree
                mpRoot->Components.push_back(inpComponent);
            }
            

            
//            CVector3 vNodePos = BSphere.vOrigin/s;
//            int i = (int)pos.x + N / 2;
//            int j = (int)pos.y + N / 2;
//            int k = (int)pos.z + N / 2;
		}
		//---------------------------------------------------------------
		/// Remove 
		///
		/// Find the given object in the tree and remove it
		///
		/// @param Component to remove
		//---------------------------------------------------------------
		void COctree::Remove(const VolumeComponentPtr& inpComponent)
		{
//			//Use the radius of the object to work out what depth it is at
//			Sphere BSphere = inpComponent->GetBoundingSphere();
//			u32 udwDepth = std::min((u32)floorf(log2f(mfWorldWidth/BSphere.fRadius)), kuwMaximumTreeDepth);
//			
//			//Now find it in the child nodes at this depth
//			//Find a node at that depth
//			COctree::Node* pOctant = mpRoot;
//			while(pOctant->uwDepth != (uwDepth - 1))
//			{
//                CVector3 vClosest(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
//                u32 uwNearestIndex = 0;
//                
//                for(u32 i=0; i<kuwNumChildNodes; ++i)
//                {
//                    CVector3 vDistance = pOctant->paChild[i]->vCentre - BSphere.vOrigin;
//                    if(vDistance < vClosest)
//                    {
//                        uwNearestIndex = i;
//                        vClosest = vDistance;
//                    }
//                }
//                
//				pOctant = pOctant->paChild[uwNearestIndex];
//			}
//			
//			//Now we have the depth we can find the component
//			for(u32 i=0; i<kuwNumChildNodes; ++i)
//			{
//				for(ObjectList::iterator it = pOctant->paChild[i]->Components.begin(); it != pOctant->paChild[i]->Components.end(); ++it)
//				{
//					if((*it) == inpComponent)
//					{
//						pOctant->paChild[i]->Components.erase(it);
//						return;
//					}
//				}
//			}
		}
        //----------------------------------------------------------
        /// Get Child Node Attached To Node
        ///
        /// Recurse through all the child nodes and get the 
        /// entities from them
        //----------------------------------------------------------
        void COctree::GetChildNodeAttachedToNode(COctree::Node* pNode, const CFrustum* inpFrustum, std::vector<ComponentPtr> &outComponents, u32 inudwQueryMask)
        {
            if(pNode)
            {
                //Check if the node is inside the frustum
                if(inpFrustum->SphereCullTest(pNode->BoundingSphere))
                {
                    for(u32 i=1; i<=kudwNumChildNodes; ++i)
                    {
                        //Add any renderable objects to the vector
                        u32 udwChildOffset = kudwNumChildNodes*pNode->udwTreeOffset + i;
                        GetComponentsAttachedToNode(mTree[udwChildOffset], outComponents, inudwQueryMask);
                        GetChildNodeAttachedToNode(mTree[udwChildOffset], inpFrustum, outComponents, inudwQueryMask);
                    }
                }
            }
        }
        //----------------------------------------------------------
        /// Get Components Attached To Node
        ///
        /// Recurse through all the components and do the cull
        /// test
        //----------------------------------------------------------
        void COctree::GetComponentsAttachedToNode(COctree::Node* pNode, std::vector<ComponentPtr> &outComponents, u32 inudwQueryMask)
        {
            if(pNode)
            {
                //Check all the components attached to the node are visible
                for(ObjectList::iterator it = pNode->Components.begin(); it != pNode->Components.end(); ++it)
                {
                    if((*it)->IsVisible())
                    {
                        if(inudwQueryMask == 0 || inudwQueryMask & (*it)->GetQueryMask())
                        {
                            outComponents.push_back(*it);
                        }
                    }
                }
            }
        }
		//---------------------------------------------------------------
		/// Destroy
		///
		/// Release all the memory held by the octree
		//---------------------------------------------------------------
		void COctree::Destroy()
		{
			for(u32 i=0; i<kudwTotalNumNodes; ++i)
            {
                CS_SAFE_DELETE(mTree[i]);
            }
		}
		//----------------------------------------------------------------
		/// Destructor
		///
		//----------------------------------------------------------------
		COctree::~COctree()
		{
			Destroy();
		}
	}
}

