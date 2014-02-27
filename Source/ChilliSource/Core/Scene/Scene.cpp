/*
 *  Scene.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 21/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Scene/Scene.h>

#include <ChilliSource/GUI/Base/Window.h>

namespace ChilliSource
{
	namespace Core
	{
        //--------------------------------------------------------------------------------------------------
		/// Query Sort Comparator
		///
		/// Sorts scene intersection tests by depth
		/// @return Whether the lhs is closer to the ray origin than the rhs
		//--------------------------------------------------------------------------------------------------
        struct QuerySortPredicate
        {
            bool operator()(const VolumeComponent* lhs, const VolumeComponent* rhs) const
            {
                return (lhs->mfQueryIntersectionValue < rhs->mfQueryIntersectionValue);
            }
        };
        
		//--------------------------------------------------------------------------------------------------
		/// Constructor
		///
		/// Create the scene graph object 
		//--------------------------------------------------------------------------------------------------
		Scene::Scene(Input::InputSystem* inpInputSystem, f32 infWorldHalfSize) 
        : mpRootWindow(nullptr)
		{
			mpRootWindow = new GUI::Window();
			mpRootWindow->SetInputSystem(inpInputSystem);
		}
		//-------------------------------------------------------
		/// Become Active
		///
		/// Force the scene to become the active one and to
		/// receive user interaction
		//-------------------------------------------------------
		void Scene::BecomeActive()
		{
            mpRootWindow->ListenForTouches();
            
		}
		//-------------------------------------------------------
		/// Become Inactive
		///
		/// Set this scene to be inactive. It will no longer
		/// be interactable for the user
		//-------------------------------------------------------
		void Scene::BecomeInactive()
		{
			Input::TouchInfo sInfo;
			sInfo.eType = Input::TouchInputType::k_ended;
			mpRootWindow->_OnTouchEnded(sInfo);
			//((ISurface*)mpRootWindow)->EnableUserInteraction(false);
            mpRootWindow->UnlistenFromTouches();
		}
		//-------------------------------------------------------
		/// Add Entity
		///
		/// Add the entity to the scene
		///
		/// @param Entity pointer
		//-------------------------------------------------------
		void Scene::AddEntity(const EntitySPtr& inpEntity)
		{
			mEntities.push_back(inpEntity);

			if (inpEntity->GetOwningScene() && inpEntity->GetOwningScene() != this)
				inpEntity->RemoveFromScene();
				
			inpEntity->SetOwningScene(this);
		}
		//-------------------------------------------------------
		/// Remove Entity
		///
		/// Remove the entity from the scene
		///
		/// @param Entity pointer
		//-------------------------------------------------------
		void Scene::RemoveEntity(const EntitySPtr& inpEntity)
		{
			SharedEntityList::iterator it = std::find(mEntities.begin(), mEntities.end(), inpEntity);
			if(it != mEntities.end())
			{
				(*it)->SetOwningScene(nullptr);
                it->swap(mEntities.back());
                mEntities.pop_back();
			}
		}
		//-------------------------------------------------------
		/// Remove All Entities
		///
		/// Remove all the entities from the scene
		//-------------------------------------------------------
		void Scene::RemoveAllEntities()
		{
			while(!mEntities.empty())
			{
				RemoveEntity(mEntities.front());
			}
		}
		//-------------------------------------------------------
		/// Get Entity List
		///
		/// @return the list of all the entities in the scene.
		//-------------------------------------------------------
		const SharedEntityList& Scene::GetEntityList()
		{
			return mEntities;
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Spatial Graph
		///
		/// @return A reference to the octree that manages the spatial relationship of the scene nodes
		//--------------------------------------------------------------------------------------------------
//		const Octree& Scene::GetSpatialGraph() const
//		{
//			return mSpatialGraph;
//		}
		//--------------------------------------------------------------------------------------------------
		/// Query Scene For Intersection
		///
		/// Traverses the contents of the scene and adds any objects that intersect with the ray to the
		/// list.
		/// @param Ray to check intersection
		/// @param A handle to a container to fill with intersecting components (Render components)
		//--------------------------------------------------------------------------------------------------
		void Scene::QuerySceneForIntersection(const Core::Ray &inRay, std::vector<VolumeComponent*> &outIntersectComponents, bool inbIsDepthSorted, u32 inudwQueryMask)
		{
			//Only render components can be checked for ray intersection as they have AABB's
			std::vector<VolumeComponent*> IntersectableComponents;
			QuerySceneForComponents<VolumeComponent>(IntersectableComponents, inudwQueryMask);
			
			//Loop through the render components and check for intersection
			//If any intersect then add them to the intersect list
			for(std::vector<VolumeComponent*>::iterator it = IntersectableComponents.begin(); it != IntersectableComponents.end(); ++it)
			{
				VolumeComponent* pComponent = (VolumeComponent*)(*it);
				
				f32 fNearIntersection, fFarIntersection = 0.0f;
				
				//Don't check for intersection against the UI elements. Make sure we filter out based on the collision mask
				u32 udwMaskComponent = pComponent->GetQueryMask();
				if(inudwQueryMask == 0 || (inudwQueryMask & udwMaskComponent))
				{
					if(pComponent->GetOOBB().Contains(inRay, fNearIntersection, fFarIntersection))
					{
						//We use this to sort by
						pComponent->mfQueryIntersectionValue = fNearIntersection;
						outIntersectComponents.push_back(pComponent);
					}
				}
				
				//Sort this list by intersection depth if necessary
				if(inbIsDepthSorted)
				{
					std::sort(outIntersectComponents.begin(), outIntersectComponents.end(), QuerySortPredicate());
				}
			}
		}
        //--------------------------------------------------------------------------------------------------
        /// Get Window Pointer
        ///
        /// @return The main window that all the scene's UI is attached to.
        //--------------------------------------------------------------------------------------------------
        GUI::Window* Scene::GetWindowPtr()
        {
            return mpRootWindow;
        }
        //--------------------------------------------------------------------------------------------------
        /// Update
        ///
        /// Updates anything in the scene graph which needs to be updated (e.g. NodeAnimations)
        ///
        /// @param delta time in seconds
        //--------------------------------------------------------------------------------------------------		   
		void Scene::Update(f32 infDT)
		{
			mpRootWindow->Update(infDT);
		}
		//--------------------------------------------------------------------------------------------------
		/// Destructor
		///
		/// 
		//--------------------------------------------------------------------------------------------------
		Scene::~Scene()
		{
			RemoveAllEntities();
            delete mpRootWindow;
		}
	}
}
