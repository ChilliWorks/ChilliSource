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

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------------
		Entity::Entity() : mbVisible(true), mpParent(nullptr), mpOwningScene(nullptr), mpEntityAnimationController(EntityAnimationControllerSPtr())
		{
		}
		//-------------------------------------------------------------
		/// Add Child
		///
		/// @param Entity to derive transform from
		//-------------------------------------------------------------
		void Entity::AddChild(const EntitySPtr& inpEntity)
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
		void Entity::RemoveChild(const EntitySPtr& inpEntity)
		{
			SharedEntityList::iterator it = std::find(mEntities.begin(), mEntities.end(), inpEntity);
			
			if(it != mEntities.end())
			{
				mLocalTransform.RemoveChildTransform(&inpEntity->Transform());
				
				inpEntity->mpParent = nullptr;
				
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
		void Entity::RemoveAllChildren()
		{
			mLocalTransform.RemoveAllChildTransforms();
			
			for(SharedEntityList::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
			{
				(*it)->mpParent = nullptr;
				
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
		void Entity::RemoveFromParent()
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
		void Entity::AddToScene(Scene* inpScene)
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
		void Entity::RemoveFromScene(Scene* inpScene)
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
		void Entity::RemoveFromScene()
		{
			if(mpOwningScene)
			{
				RemoveFromScene(mpOwningScene);
			}
		}
		//------------------------------------------------------------------
		/// GetParent
		/// 
		/// Return the parent entity of this entity. nullptr is returned naturally if
		/// there is no parent.
		///
		/// @return Pointer to the parent entity of this
		//------------------------------------------------------------------
		Entity* Entity::GetParent() 
		{ 
			return mpParent; 
		}
		//------------------------------------------------------------------
		/// FindChildEntityWithName
		/// 
		/// Recursively search the entity hierarchy in a depth-first manner
		/// and return a pointer to the first entity it finds with the name matching
		/// parameter instrName, or nullptr if no match found.
		///
		/// @return A child entity with the specified name
		//------------------------------------------------------------------
		Entity* Entity::FindChildEntityWithName(const std::string& instrName)
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
					Entity* pcEntity = (*it)->FindChildEntityWithName(instrName);
					if (pcEntity)
						return pcEntity;
				}
			}
			
			return nullptr;
		}
        //------------------------------------------------------------------
        /// To JSON
        /// 
        /// @return A JSON object of the hierarchy of entities and components
        //------------------------------------------------------------------
        Json::Value Entity::ToJSON() const
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
        u32 Entity::GetNumberOfChildEntities() const
        {
            return mEntities.size();
        }
		
		//------------------------------------------------------------------
        /// Get Immediate Child With Name
        /// 
        /// Search only immediate children to an entity and return a pointer
        /// to the first entity it finds with the name matching parameter 
        /// instrName, or nullptr if no match found.
        ///
        /// @return A child entity with the specified name
        //------------------------------------------------------------------
        EntitySPtr Entity::GetImmediateChildWithName(const std::string &instrName)
        {
            for(SharedEntityList::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
                if((*it)->GetName() == instrName)
                    return (*it);
            
            return EntitySPtr();
        }
        //------------------------------------------------------------------
        /// Get Entity AnimationC ontroller Ptr
        /// 
        /// Return the entity animation controller pointer for this entity
        ///
        /// @return Anim controller
        //------------------------------------------------------------------
		EntityAnimationControllerSPtr Entity::GetEntityAnimationControllerPtr()
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
		EntityAnimationControllerSPtr& Entity::GetEntityAnimationController()
		{
			return mpEntityAnimationController;
		}
        //------------------------------------------------------------------
        /// Get Child Entites
        /// 
        /// @return List of child entities
        //------------------------------------------------------------------
        SharedEntityList& Entity::GetChildEntities()
        {
            return mEntities;
        }
		//-------------------------------------------------------------
		/// Set Owning Scene
		///
		/// @param Scene owner
		//-------------------------------------------------------------
		void Entity::SetOwningScene(Scene* inpScene)
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
		Scene* Entity::GetOwningScene()
		{
			return mpOwningScene;
		}
        //----------------------------------------------------------------
        /// GEt Transform
        ///
        /// @return Transform object to allow for spatial manipulation
        //----------------------------------------------------------------
        Transform& Entity::GetTransform()
        {
            return mLocalTransform;
        }
        //----------------------------------------------------------------
        /// Get Transform
        ///
        /// @return Transform object to allow for spatial manipulation
        //----------------------------------------------------------------
        const Transform& Entity::GetTransform() const
        {
            return mLocalTransform;
        }
        //-------------------------------------------------------------
        /// Set Visible
        ///
        /// @param Whether any renderable components should be shown
        //-------------------------------------------------------------
        void Entity::SetVisible(bool inbVisible)
        {
            mbVisible = inbVisible;
        }
        //-------------------------------------------------------------
        /// Is Visible
        ///
        /// @return Whether any renderable components should be shown
        //-------------------------------------------------------------
        bool Entity::IsVisible() const
        {
            return mpParent ? mbVisible && mpParent->IsVisible() : mbVisible;
        }
		//-------------------------------------------------------------
		/// Get Component  (Managed)
		///
		/// @return the first component of the given type
		//-------------------------------------------------------------
		ComponentSPtr Entity::GetManagedComponent(InterfaceIDType inInterfaceID, u32 inudwQueryMask) const
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
			
			return ComponentSPtr();
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
		void Entity::GetManagedComponents(InterfaceIDType inInterfaceID, std::vector<ComponentSPtr> & outComponentsWithInterface, u32 inudwQueryMask, bool inbClearVectorBeforeUsing) const
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
		Component* Entity::GetComponent(InterfaceIDType inInterfaceID, u32 inudwQueryMask) const
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
			
			return nullptr;
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
		void Entity::GetComponents(InterfaceIDType inInterfaceID, std::vector<Component*> & outComponentsWithInterface, u32 inudwQueryMask, bool inbClearVectorBeforeUsing) const
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
		ComponentSPtr Entity::GetManagedComponentRecursive(InterfaceIDType inInterfaceID, u32 inudwQueryMask) const
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
                ComponentSPtr pComp = (*it)->GetManagedComponentRecursive(inInterfaceID, inudwQueryMask);
                
                if(pComp)
                {
                    return pComp;
                }
            }
			
			return ComponentSPtr();
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
		void Entity::GetManagedComponentsRecursive(InterfaceIDType inInterfaceID, std::vector<ComponentSPtr> & outComponentsWithInterface, u32 inudwQueryMask, bool inbClearVectorBeforeUsing) const
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
		Component* Entity::GetComponentRecursive(InterfaceIDType inInterfaceID, u32 inudwQueryMask) const
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
                Component* pComp = (*it)->GetComponentRecursive(inInterfaceID, inudwQueryMask);
                
                if(pComp)
                {
                    return pComp;
                }
            }
			
			return nullptr;
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
		void Entity::GetComponentsRecursive(InterfaceIDType inInterfaceID, std::vector<Component*> & outComponentsWithInterface, u32 inudwQueryMask, bool inbClearVectorBeforeUsing) const
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
		void Entity::Attach(const ComponentSPtr & inpComponent)
		{
			mComponents.push_back(inpComponent);
			inpComponent->SetEntityOwner(this);
		}
		//-------------------------------------------------------------
		/// Detach
		///
		/// @param Component to remove from the entity
		//-------------------------------------------------------------
		void Entity::Detach(const ComponentSPtr & inpComponent)
		{
			if(inpComponent)
			{
				for(ComponentList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
				{
					if(inpComponent.get() == (*it).get())
					{
                        inpComponent->SetEntityOwner(nullptr);
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
		void Entity::DetachAll()
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
		EntitySPtr CreateEntity()
		{
			return EntitySPtr(new Entity());
		}
		//------------------------------------------------------------------
		/// Create Entity
		/// 
		/// Convienience method to create an entity and attach component
		///
		/// @return Entity
		//------------------------------------------------------------------
		EntitySPtr CreateEntity(const ComponentSPtr & inpComponent)
		{
			EntitySPtr pEntity(new Entity());
			pEntity->Attach(inpComponent);
			return pEntity;
		}
		//-------------------------------------------------------------
		/// Has Name
		///
		/// @return Whether this entity is named
		//-------------------------------------------------------------
		bool Entity::HasName()
		{
			return mstrName != CStringUtils::BLANK;
		}
		//-------------------------------------------------------------
		/// Set Name
		///
		/// @param Give this entity a name
		//-------------------------------------------------------------
		void Entity::SetName(const std::string & incName)
		{
			mstrName = incName;
		}
		//-------------------------------------------------------------
		/// Get Name
		///
		/// @return Entity name
		//-------------------------------------------------------------
		const std::string & Entity::GetName() const
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
        void Entity::Reset()
        {
            ComponentList ComponentsCopy = mComponents;
            for (ComponentList::const_iterator itr = ComponentsCopy.begin(); itr != ComponentsCopy.end(); ++itr)
			{
				(*itr)->SetEntityOwner(nullptr);
			}
            mComponents.clear();
            
            RemoveAllChildren();
            RemoveFromParent();
            RemoveFromScene();
            
            mstrName = std::string();
            mLocalTransform = Transform();
            
            mbVisible = true;
			mpEntityAnimationController = EntityAnimationControllerSPtr();
        }
        //------------------------------------------------------------------
        /// Get All Managed Components
        ///
        /// @return Array of components
        //------------------------------------------------------------------
        const ComponentList& Entity::GetAllManagedComponents() const
        {
            return mComponents;
        }
		//-------------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------------
		Entity::~Entity()
		{
			ComponentList ComponentsCopy = mComponents;
            for (ComponentList::const_iterator itr = ComponentsCopy.begin(); itr != ComponentsCopy.end(); ++itr) 
			{
				(*itr)->SetEntityOwner(nullptr);
			}
            
            mpOwningScene = nullptr;
			
			RemoveAllChildren();
		}
	}
}