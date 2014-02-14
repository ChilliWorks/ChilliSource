/*
 *  Entity.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 21/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Scene/Scene.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <algorithm>

namespace moFlo
{
	namespace Core
	{
		//-------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------------
		CEntity::CEntity() : mbVisible(true), mpParent(NULL), mpOwningScene(NULL), mpEntityAnimationController(EntityAnimationControllerPtr())
		{
		}
		//-------------------------------------------------------------
		/// Add Child
		///
		/// @param Entity to derive transform from
		//-------------------------------------------------------------
		void CEntity::AddChild(const EntityPtr& inpEntity)
		{
            if(inpEntity->mpParent == this)
                return;
            
            inpEntity->RemoveFromParent();
                
			mEntities.push_back(inpEntity);
			mLocalTransform.AddChildTransform(&inpEntity->Transform());
			
			//Set the parent and add it to the scene
			inpEntity->mpParent = this;
			
			if(mpOwningScene && inpEntity->GetOwningScene() != mpOwningScene)
			{
				//This will add the entity and any children it may have
				inpEntity->AddToScene(mpOwningScene);
			}
		}
		//-------------------------------------------------------------
		/// Remove Child
		///
		/// @param Entity no longer with hierarchical relationship
		//-------------------------------------------------------------
		void CEntity::RemoveChild(const EntityPtr& inpEntity)
		{
			SharedEntityList::iterator it = std::find(mEntities.begin(), mEntities.end(), inpEntity);
			
			if(it != mEntities.end())
			{
				mLocalTransform.RemoveChildTransform(&inpEntity->Transform());
				
				inpEntity->mpParent = NULL;
				
				if(mpOwningScene)
				{
					//Remove the child from the scene
					inpEntity->RemoveFromScene(mpOwningScene);
				}
                std::swap(mEntities.back(), *it);
				mEntities.pop_back();
			}
		}
		//-------------------------------------------------------------
		/// Remove All Children
		///
		//-------------------------------------------------------------
		void CEntity::RemoveAllChildren()
		{
			mLocalTransform.RemoveAllChildTransforms();
			
			for(SharedEntityList::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
			{
				(*it)->mpParent = NULL;
				
				if(mpOwningScene)
				{
					//Remove the child from the scene
					(*it)->RemoveFromScene(mpOwningScene);
				}
			}
			
			mEntities.clear();
		}
		//-------------------------------------------------------------
		/// Remove From Parent
		///
		/// Detahc the entity from its parent. If it has no parent
        /// then remove it from the scene
		//-------------------------------------------------------------
		void CEntity::RemoveFromParent()
		{
			if (!GetParent())
            {
                RemoveFromScene();
				return;
            }
			
			SharedEntityList apListChildren = GetParent()->GetChildEntities();
			
			for (SharedEntityList::iterator it = apListChildren.begin(); it != apListChildren.end(); ++it)
			{
				if ((*it).get() == this)
				{
					GetParent()->RemoveChild(*it);
					break;
				}
			}
		}
		//-------------------------------------------------------------
		/// Add To Scene
		///
		/// @param Scene
		//-------------------------------------------------------------
		void CEntity::AddToScene(CScene* inpScene)
		{
			inpScene->AddEntity(shared_from_this());
			
			for(SharedEntityList::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
			{
				(*it)->AddToScene(inpScene);
			}
		}
		//-------------------------------------------------------------
		/// Remove From Scene
		///
		/// @param Scene
		//-------------------------------------------------------------
		void CEntity::RemoveFromScene(CScene* inpScene)
		{
            for(SharedEntityList::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
			{
				(*it)->RemoveFromScene(inpScene);
			}
            
			inpScene->RemoveEntity(shared_from_this());
		}
		//-------------------------------------------------------------
		/// Remove From Scene
		///
		/// Will remove all children from the scene
		///
		/// @param Scene
		//-------------------------------------------------------------
		void CEntity::RemoveFromScene()
		{
			if(mpOwningScene)
			{
				RemoveFromScene(mpOwningScene);
			}
		}
		//------------------------------------------------------------------
		/// GetParent
		/// 
		/// Return the parent entity of this entity. NULL is returned naturally if
		/// there is no parent.
		///
		/// @return Pointer to the parent entity of this
		//------------------------------------------------------------------
		CEntity* CEntity::GetParent() 
		{ 
			return mpParent; 
		}
		//------------------------------------------------------------------
		/// FindChildEntityWithName
		/// 
		/// Recursively search the entity hierarchy in a depth-first manner
		/// and return a pointer to the first entity it finds with the name matching
		/// parameter instrName, or NULL if no match found.
		///
		/// @return A child entity with the specified name
		//------------------------------------------------------------------
		CEntity* CEntity::FindChildEntityWithName(const std::string& instrName)
		{
			std::string strName = mstrName;
			
			if (!instrName.compare(mstrName))
			{
				return this;
			}
			else 			
            {
				for (SharedEntityList::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
				{
					CEntity* pcEntity = (*it)->FindChildEntityWithName(instrName);
					if (pcEntity)
						return pcEntity;
				}
			}
			
			return NULL;
		}
        //------------------------------------------------------------------
        /// To JSON
        /// 
        /// @return A JSON object of the hierarchy of entities and components
        //------------------------------------------------------------------
        Json::Value CEntity::ToJSON() const
        {
            Json::Value jResult;
            Json::Value jComponents(Json::arrayValue), jEntities(Json::arrayValue);
            
            for(ComponentList::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it)
            {
                jComponents.append((*it)->GetInterfaceTypeName());
            }
            
            for(SharedEntityList::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
            {
                Json::Value jEntity = (*it)->ToJSON();
                jEntities.append(jEntity);
            }
            
            jResult[mstrName]["Components"] = jComponents;
            jResult[mstrName]["Entities"] = jEntities;
            
            return jResult;
        }
        //------------------------------------------------------------------
        /// Get Number Of Child Entities
        /// 
        /// Return the number of child entities
        ///
        /// @return the number of child entities
        //------------------------------------------------------------------
        u32 CEntity::GetNumberOfChildEntities() const
        {
            return mEntities.size();
        }
		
		//------------------------------------------------------------------
        /// Get Immediate Child With Name
        /// 
        /// Search only immediate children to an entity and return a pointer
        /// to the first entity it finds with the name matching parameter 
        /// instrName, or NULL if no match found.
        ///
        /// @return A child entity with the specified name
        //------------------------------------------------------------------
        EntityPtr CEntity::GetImmediateChildWithName(const std::string &instrName)
        {
            for(SharedEntityList::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
                if((*it)->GetName() == instrName)
                    return (*it);
            
            return EntityPtr();
        }
        //------------------------------------------------------------------
        /// Get Entity AnimationC ontroller Ptr
        /// 
        /// Return the entity animation controller pointer for this entity
        ///
        /// @return Anim controller
        //------------------------------------------------------------------
		EntityAnimationControllerPtr CEntity::GetEntityAnimationControllerPtr()
		{
			return mpEntityAnimationController;
		}
        //------------------------------------------------------------------
        /// Get Entity Animation Controller
        /// 
        /// Return the entity animation controller pointer for this entity
        ///
        /// @return Anim controller
        //------------------------------------------------------------------
		EntityAnimationControllerPtr& CEntity::GetEntityAnimationController()
		{
			return mpEntityAnimationController;
		}
        //------------------------------------------------------------------
        /// Get Child Entites
        /// 
        /// @return List of child entities
        //------------------------------------------------------------------
        SharedEntityList& CEntity::GetChildEntities()
        {
            return mEntities;
        }
		//-------------------------------------------------------------
		/// Set Owning Scene
		///
		/// @param Scene owner
		//-------------------------------------------------------------
		void CEntity::SetOwningScene(CScene* inpScene)
		{
			mpOwningScene = inpScene;
			
			//If we are the root entity we must now add all children
			if(!mpParent && mpOwningScene)
			{
				for(SharedEntityList::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
				{
					(*it)->AddToScene(mpOwningScene);
				}
			}
		}
		//-------------------------------------------------------------
		/// Get Owning Scene
		///
		/// @return Scene owner
		//-------------------------------------------------------------
		CScene* CEntity::GetOwningScene()
		{
			return mpOwningScene;
		}
        //----------------------------------------------------------------
        /// Transform
        ///
        /// @return Transform object to allow for spatial manipulation
        //----------------------------------------------------------------
        CTransform& CEntity::Transform()
        {
            return mLocalTransform;
        }
        //----------------------------------------------------------------
        /// Transform
        ///
        /// @return Transform object to allow for spatial manipulation
        //----------------------------------------------------------------
        const CTransform& CEntity::Transform() const
        {
            return mLocalTransform;
        }
        //-------------------------------------------------------------
        /// Set Visible
        ///
        /// @param Whether any renderable components should be shown
        //-------------------------------------------------------------
        void CEntity::SetVisible(bool inbVisible)
        {
            mbVisible = inbVisible;
        }
        //-------------------------------------------------------------
        /// Is Visible
        ///
        /// @return Whether any renderable components should be shown
        //-------------------------------------------------------------
        bool CEntity::IsVisible() const
        {
            return mpParent ? mbVisible && mpParent->IsVisible() : mbVisible;
        }
		//-------------------------------------------------------------
		/// Get Component  (Managed)
		///
		/// @return the first component of the given type
		//-------------------------------------------------------------
		ComponentPtr CEntity::GetManagedComponent(InterfaceIDType inInterfaceID, u32 inudwQueryMask) const
		{
			for(ComponentList::const_iterator itr = mComponents.begin(); itr != mComponents.end(); ++itr) 
			{
				u32 udwMaskComponent = (*itr)->GetQueryMask();
				if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
				{
					if((*itr)->IsA(inInterfaceID)) 
					{
						return *itr;
					}
				}
			}
			
			return ComponentPtr();
		}
		//-------------------------------------------------------------
		/// Get Components  (Managed)
		///
		/// Search the list of components and add ones to the list
		/// that are of given interface type
		///
		/// @param Interface ID
		/// @param Out - Vector to populate with components
		/// @param Whether to clear the vector before populating
		//-------------------------------------------------------------
		void CEntity::GetManagedComponents(InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<ComponentPtr> & outComponentsWithInterface, u32 inudwQueryMask, bool inbClearVectorBeforeUsing) const
		{
			if (inbClearVectorBeforeUsing) 
			{
				outComponentsWithInterface.clear();
			}
			
			for (ComponentList::const_iterator itr = mComponents.begin(); itr != mComponents.end(); ++itr) 
			{
				u32 udwMaskComponent = (*itr)->GetQueryMask();
				if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
				{
					if ((*itr)->IsA(inInterfaceID)) 
					{
						outComponentsWithInterface.push_back(*itr);
					}
				}
			}
		}
		//-------------------------------------------------------------
		/// Get Component  (Non managed)
		///
		/// @return the first component of the given type
		//-------------------------------------------------------------
		IComponent* CEntity::GetComponent(InterfaceIDType inInterfaceID, u32 inudwQueryMask) const
		{
			for(ComponentList::const_iterator itr = mComponents.begin(); itr != mComponents.end(); ++itr) 
			{
				u32 udwMaskComponent = (*itr)->GetQueryMask();
				if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
				{
					if((*itr)->IsA(inInterfaceID)) 
					{
						return itr->get();
					}
				}
			}
			
			return NULL;
		}
		//-------------------------------------------------------------
		/// Get Components  (Non managed)
		///
		/// Search the list of components and add ones to the list
		/// that are of given interface type
		///
		/// @param Interface ID
		/// @param Out - Vector to populate with components
		/// @param Whether to clear the vector before populating
		//-------------------------------------------------------------
		void CEntity::GetComponents(InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<IComponent*> & outComponentsWithInterface, u32 inudwQueryMask, bool inbClearVectorBeforeUsing) const
		{
			if (inbClearVectorBeforeUsing) 
			{
				outComponentsWithInterface.clear();
			}
			
			for (ComponentList::const_iterator itr = mComponents.begin(); itr != mComponents.end(); ++itr) 
			{
				u32 udwMaskComponent = (*itr)->GetQueryMask();
				if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
				{
					if ((*itr)->IsA(inInterfaceID)) 
					{
						outComponentsWithInterface.push_back(itr->get());
					}
				}
			}
		}
        //-------------------------------------------------------------
		/// Get Component  (Managed Recursive)
		///
		/// @return the first component of the given type. This will
        /// recurse into chile entities
		//-------------------------------------------------------------
		ComponentPtr CEntity::GetManagedComponentRecursive(InterfaceIDType inInterfaceID, u32 inudwQueryMask) const
		{
			for(ComponentList::const_iterator itr = mComponents.begin(); itr != mComponents.end(); ++itr) 
			{
				u32 udwMaskComponent = (*itr)->GetQueryMask();
				if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
				{
					if((*itr)->IsA(inInterfaceID)) 
					{
						return *itr;
					}
				}
			}
            
            for(SharedEntityList::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
            {
                ComponentPtr pComp = (*it)->GetManagedComponentRecursive(inInterfaceID, inudwQueryMask);
                
                if(pComp)
                {
                    return pComp;
                }
            }
			
			return ComponentPtr();
		}
		//-------------------------------------------------------------
		/// Get Components  (Managed Recursive)
		///
		/// Search the list of components and add ones to the list
		/// that are of given interface type. This will
        /// recurse into chile entities
		///
		/// @param Interface ID
		/// @param Out - Vector to populate with components
		/// @param Whether to clear the vector before populating
		//-------------------------------------------------------------
		void CEntity::GetManagedComponentsRecursive(InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<ComponentPtr> & outComponentsWithInterface, u32 inudwQueryMask, bool inbClearVectorBeforeUsing) const
		{
			if (inbClearVectorBeforeUsing) 
			{
				outComponentsWithInterface.clear();
			}
			
			for (ComponentList::const_iterator itr = mComponents.begin(); itr != mComponents.end(); ++itr) 
			{
				u32 udwMaskComponent = (*itr)->GetQueryMask();
				if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
				{
					if ((*itr)->IsA(inInterfaceID)) 
					{
						outComponentsWithInterface.push_back(*itr);
					}
				}
			}
            
            for(SharedEntityList::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
            {
                (*it)->GetManagedComponentsRecursive(inInterfaceID, outComponentsWithInterface, inudwQueryMask, false);
            }
		}
		//-------------------------------------------------------------
		/// Get Component  (Non managed Recursive)
		///
		/// @return the first component of the given type. This will
        /// recurse into chile entities
		//-------------------------------------------------------------
		IComponent* CEntity::GetComponentRecursive(InterfaceIDType inInterfaceID, u32 inudwQueryMask) const
		{
			for(ComponentList::const_iterator itr = mComponents.begin(); itr != mComponents.end(); ++itr) 
			{
				u32 udwMaskComponent = (*itr)->GetQueryMask();
				if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
				{
					if((*itr)->IsA(inInterfaceID)) 
					{
						return itr->get();
					}
				}
			}
            
            for(SharedEntityList::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
            {
                IComponent* pComp = (*it)->GetComponentRecursive(inInterfaceID, inudwQueryMask);
                
                if(pComp)
                {
                    return pComp;
                }
            }
			
			return NULL;
		}
		//-------------------------------------------------------------
		/// Get Components  (Non managed Recursive)
		///
		/// Search the list of components and add ones to the list
		/// that are of given interface type. This will
        /// recurse into chile entities
		///
		/// @param Interface ID
		/// @param Out - Vector to populate with components
		/// @param Whether to clear the vector before populating
		//-------------------------------------------------------------
		void CEntity::GetComponentsRecursive(InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<IComponent*> & outComponentsWithInterface, u32 inudwQueryMask, bool inbClearVectorBeforeUsing) const
		{
			if (inbClearVectorBeforeUsing) 
			{
				outComponentsWithInterface.clear();
			}
			
			for (ComponentList::const_iterator itr = mComponents.begin(); itr != mComponents.end(); ++itr) 
			{
				u32 udwMaskComponent = (*itr)->GetQueryMask();
				if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
				{
					if ((*itr)->IsA(inInterfaceID)) 
					{
						outComponentsWithInterface.push_back(itr->get());
					}
				}
			}
            
            for(SharedEntityList::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
            {
                (*it)->GetComponentsRecursive(inInterfaceID, outComponentsWithInterface, inudwQueryMask, false);
            }
		}
		//-------------------------------------------------------------
		/// Attach
		///
		/// @param Component to attach to the entity
		//-------------------------------------------------------------
		void CEntity::Attach(const ComponentPtr & inpComponent)
		{
			mComponents.push_back(inpComponent);
			inpComponent->SetEntityOwner(this);
		}
		//-------------------------------------------------------------
		/// Detach
		///
		/// @param Component to remove from the entity
		//-------------------------------------------------------------
		void CEntity::Detach(const ComponentPtr & inpComponent)
		{
			if(inpComponent)
			{
				for(ComponentList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
				{
					if(inpComponent.get() == (*it).get())
					{
                        inpComponent->SetEntityOwner(NULL);
                        std::swap(mComponents.back(), *it);
                        mComponents.pop_back();
                        
						return;
					}
				}
			}
		}
        //-------------------------------------------------------------
		/// Detach All
		///
		/// Removes all components
		//-------------------------------------------------------------
		void CEntity::DetachAll()
		{
            while(mComponents.size() > 0)
            {
                Detach(mComponents.front());
            }
		}
		//------------------------------------------------------------------
		/// Create Entity
		/// 
		/// Convienience method to create an empty entity
		///
		/// @return Entity
		//------------------------------------------------------------------
		EntityPtr CreateEntity()
		{
			return EntityPtr(new CEntity());
		}
		//------------------------------------------------------------------
		/// Create Entity
		/// 
		/// Convienience method to create an entity and attach component
		///
		/// @return Entity
		//------------------------------------------------------------------
		EntityPtr CreateEntity(const ComponentPtr & inpComponent)
		{
			EntityPtr pEntity(new CEntity());
			pEntity->Attach(inpComponent);
			return pEntity;
		}
		//-------------------------------------------------------------
		/// Attach
		///
		/// @param Component to attach to the entity
		//-------------------------------------------------------------
		void operator +=(const EntityPtr& pEnt, const ComponentPtr& pComp)
		{
			pEnt->Attach(pComp);
		}
		//-------------------------------------------------------------
		/// Detach
		///
		/// @param Component to remove from the entity
		//-------------------------------------------------------------
		void operator -=(const EntityPtr& pEnt, const ComponentPtr& pComp)
		{
			pEnt->Detach(pComp);
		}
		//-------------------------------------------------------------
		/// Add Child
		///
		/// @param Entity to derive transform from
		//-------------------------------------------------------------
		void operator +=(const EntityPtr& pEnt, const EntityPtr& pChild)
		{
			pEnt->AddChild(pChild);
		}
		//-------------------------------------------------------------
		/// Remove Child
		///
		/// @param Entity to remove
		//-------------------------------------------------------------
		void operator -=(const EntityPtr& pEnt, const EntityPtr& pChild)
		{
			pEnt->RemoveChild(pChild);
		}
		//-------------------------------------------------------------
		/// Has Name
		///
		/// @return Whether this entity is named
		//-------------------------------------------------------------
		bool CEntity::HasName()
		{
			return mstrName != CStringUtils::BLANK;
		}
		//-------------------------------------------------------------
		/// Set Name
		///
		/// @param Give this entity a name
		//-------------------------------------------------------------
		void CEntity::SetName(const std::string & incName)
		{
			mstrName = incName;
		}
		//-------------------------------------------------------------
		/// Get Name
		///
		/// @return Entity name
		//-------------------------------------------------------------
		const std::string & CEntity::GetName() const
		{
			return mstrName;
		}
        //------------------------------------------------------------------
        /// Reset
        ///
        /// Utility function to reset an entity to blank slate
        /// useful for pooling
        ///
        //------------------------------------------------------------------
        void CEntity::Reset()
        {
            ComponentList ComponentsCopy = mComponents;
            for (ComponentList::const_iterator itr = ComponentsCopy.begin(); itr != ComponentsCopy.end(); ++itr)
			{
				(*itr)->SetEntityOwner(NULL);
			}
            mComponents.clear();
            
            RemoveAllChildren();
            RemoveFromParent();
            RemoveFromScene();
            
            mstrName = std::string();
            mLocalTransform = CTransform();
            
            mbVisible = true;
			mpEntityAnimationController = EntityAnimationControllerPtr();
        }
        //------------------------------------------------------------------
        /// Get All Managed Components
        ///
        /// @return Array of components
        //------------------------------------------------------------------
        const ComponentList& CEntity::GetAllManagedComponents() const
        {
            return mComponents;
        }
		//-------------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------------
		CEntity::~CEntity()
		{
			ComponentList ComponentsCopy = mComponents;
            for (ComponentList::const_iterator itr = ComponentsCopy.begin(); itr != ComponentsCopy.end(); ++itr) 
			{
				(*itr)->SetEntityOwner(NULL);
			}
            
            mpOwningScene = NULL;
			
			RemoveAllChildren();
		}
	}
}