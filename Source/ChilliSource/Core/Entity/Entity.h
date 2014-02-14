/*
 *  Entity.h
 *  moFlo
 *
 *  Created by Scott Downie on 21/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_ENTITY_H_
#define _MO_FLO_CORE_ENTITY_H_

#include <ChilliSource/BuildProperties.h>

#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Entity/Transform.h>
#include <ChilliSource/Core/JSON/json.h>

#include <list>

namespace moFlo
{
	namespace Core 
	{
		class CEntity;
		typedef SHARED_PTR<CEntity> EntityPtr;
        typedef WEAK_PTR<CEntity> EntityWeakPtr;
		typedef DYNAMIC_ARRAY<ComponentPtr> ComponentList;
		
#ifdef ENTITY_LINKED_LIST
		typedef std::list<EntityPtr> SharedEntityList;
		typedef std::list<CEntity*> EntityList;
#else		
		typedef DYNAMIC_ARRAY<EntityPtr> SharedEntityList;
		typedef DYNAMIC_ARRAY<CEntity*> EntityList;
#endif
		
		//--------------------------------------------------------------------------------------------------
		/// Description:
		///
		/// Entity type. This represents any scene object as a series
		/// of aggregated components. These components could be
		/// materials or physics etc.
		//--------------------------------------------------------------------------------------------------
		class CEntity : public ENABLE_SHARED_FROM_THIS<CEntity>
		{	
		public:
			CEntity();
			virtual ~CEntity();
            
            //----------------------------------------------------------------
            /// Transform
            ///
            /// @return Transform object to allow for spatial manipulation
            //----------------------------------------------------------------
            CTransform& Transform();
            //----------------------------------------------------------------
            /// Transform
            ///
            /// @return Transform object to allow for spatial manipulation
            //----------------------------------------------------------------
            const CTransform& Transform() const;
            //-------------------------------------------------------------
            /// Set Visible
            ///
            /// @param Whether any renderable components should be shown
            //-------------------------------------------------------------
            void SetVisible(bool inbVisible);
            //-------------------------------------------------------------
            /// Is Visible
            ///
            /// @return Whether any renderable components should be shown
            //-------------------------------------------------------------
            bool IsVisible() const;
			//-------------------------------------------------------------
			/// Set Owning Scene
			///
			/// @param Scene owner
			//-------------------------------------------------------------
			void SetOwningScene(CScene* inpScene);
			//-------------------------------------------------------------
			/// Get Owning Scene
			///
			/// @return Scene owner
			//-------------------------------------------------------------
			CScene* GetOwningScene();
            //-------------------------------------------------------------
            /// Has A
            ///
            /// @return Whether the entity has a component of type T
            //-------------------------------------------------------------
            template <typename T> bool HasA() const
            {
                return GetComponent(T::InterfaceID) != NULL;
            }
			//-------------------------------------------------------------
			/// Get Component  (Managed)
			///
            /// @param Component ID
            /// @param Query mask used to filter the results
			/// @return the first component of the given type
			//-------------------------------------------------------------
			ComponentPtr GetManagedComponent(InterfaceIDType inInterfaceID, u32 inudwQueryMask = 0) const;
			//-------------------------------------------------------------
			/// Get Component  (Managed)
			///
            /// @param Query mask used to filter the results
			/// @return the first component of the given type
			//-------------------------------------------------------------
			template <typename T> SHARED_PTR<T> GetManagedComponent(u32 inudwQueryMask = 0)
			{
				return SHARED_PTR_CAST<T>(GetManagedComponent(T::InterfaceID, inudwQueryMask));
			}
            //-------------------------------------------------------------
			/// Get Component  (Managed)
			///
            /// @param Query mask used to filter the results
			/// @return the first component of the given type
			//-------------------------------------------------------------
			template <typename T> SHARED_PTR<const T> GetManagedComponent(u32 inudwQueryMask = 0) const
			{
				return SHARED_PTR_CAST<T>(GetManagedComponent(T::InterfaceID, inudwQueryMask));
			}
			//-------------------------------------------------------------
			/// Get Components  (Managed)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type
			///
			/// @param Interface ID
			/// @param Out - Vector to populate with components
            /// @param Query mask used to filter the results
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			void GetManagedComponents(InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<ComponentPtr> & outComponentsWithInterface, u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true) const;
			//-------------------------------------------------------------
			/// Get Components  (Managed)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type
			///
			/// @param Out - Vector to populate with components
            /// @param Query mask used to filter the results
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			template <typename T> void GetManagedComponents(DYNAMIC_ARRAY<SHARED_PTR<T> > & outComponents, u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true)
			{
				if(inbClearVectorBeforeUsing) 
				{
					outComponents.clear();
				}
                
                for (ComponentList::const_iterator pItr = mComponents.begin(); pItr != mComponents.end(); ++pItr)
                {
                    if(inudwQueryMask != 0)
                    {
                        u32 udwMaskComponent = (*pItr)->GetQueryMask();
                        
                        if((inudwQueryMask & udwMaskComponent) == 0)
                        {
                            continue;
                        }
                    }
                    
                    if( (*pItr)->IsA(T::InterfaceID))
                    {
                        outComponents.push_back(SHARED_PTR_CAST<T>(*pItr));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Get Components  (Managed)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type
			///
			/// @param Out - Vector to populate with components T1
            /// @param Out - Vector to populate with components T2
            /// @param Query mask used to filter the results
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			template <typename T1, typename T2> void GetManagedComponents(DYNAMIC_ARRAY<SHARED_PTR<T1> > & outComponents1, DYNAMIC_ARRAY<SHARED_PTR<T2> > & outComponents2,
                                                                          u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true)
			{
				if(inbClearVectorBeforeUsing)
				{
                    outComponents1.clear();
                    outComponents2.clear();
				}
                
                for (ComponentList::const_iterator pItr = mComponents.begin(); pItr != mComponents.end(); ++pItr)
                {
                    if(inudwQueryMask != 0)
                    {
                        u32 udwMaskComponent = (*pItr)->GetQueryMask();
                        
                        if((inudwQueryMask & udwMaskComponent) == 0)
                        {
                            continue;
                        }
                    }
                    
                    if((*pItr)->IsA(T1::InterfaceID))
                    {
                        outComponents1.push_back(SHARED_PTR_CAST<T1>(*pItr));
                    }
                    if((*pItr)->IsA(T2::InterfaceID))
                    {
                        outComponents2.push_back(SHARED_PTR_CAST<T2>(*pItr));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Get Components  (Managed)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type
			///
			/// @param Out - Vector to populate with components T1
            /// @param Out - Vector to populate with components T2
            /// @param Out - Vector to populate with components T3
            /// @param Query mask used to filter the results
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			template <typename T1, typename T2, typename T3> void GetManagedComponents(DYNAMIC_ARRAY<SHARED_PTR<T1> > & outComponents1, DYNAMIC_ARRAY<SHARED_PTR<T2> > & outComponents2, DYNAMIC_ARRAY<SHARED_PTR<T3> > & outComponents3,
                                                                                       u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true)
			{
				if(inbClearVectorBeforeUsing)
				{
                    outComponents1.clear();
                    outComponents2.clear();
                    outComponents3.clear();
				}
                
                for (ComponentList::const_iterator pItr = mComponents.begin(); pItr != mComponents.end(); ++pItr)
                {
                    if(inudwQueryMask != 0)
                    {
                        u32 udwMaskComponent = (*pItr)->GetQueryMask();
                        
                        if((inudwQueryMask & udwMaskComponent) == 0)
                        {
                            continue;
                        }
                    }
                    
                    if((*pItr)->IsA(T1::InterfaceID))
                    {
                        outComponents1.push_back(SHARED_PTR_CAST<T1>(*pItr));
                    }
                    if((*pItr)->IsA(T2::InterfaceID))
                    {
                        outComponents2.push_back(SHARED_PTR_CAST<T2>(*pItr));
                    }
                    if((*pItr)->IsA(T3::InterfaceID))
                    {
                        outComponents3.push_back(SHARED_PTR_CAST<T3>(*pItr));
                    }
                }
			}
			//-------------------------------------------------------------
			/// Get Component  (Non managed)
			///
            /// @param Component type to find
            /// @param Query mask used to filter the results
			/// @return the first component of the given type
			//-------------------------------------------------------------
			IComponent* GetComponent(InterfaceIDType inInterfaceID, u32 inudwQueryMask = 0) const;
			//-------------------------------------------------------------
			/// Get Component  (Non managed)
			///
            /// @param Query mask used to filter the results
			/// @return the first component of the given type
			//-------------------------------------------------------------
			template <typename T> T* GetComponent(u32 inudwQueryMask = 0)
			{
				return static_cast<T*>(GetComponent(T::InterfaceID, inudwQueryMask));
			}
            //-------------------------------------------------------------
			/// Get Component  (Non managed)
			///
            /// @param Query mask used to filter the results
			/// @return the first component of the given type
			//-------------------------------------------------------------
			template <typename T> const T* GetComponent(u32 inudwQueryMask = 0) const
			{
				return static_cast<T*>(GetComponent(T::InterfaceID, inudwQueryMask));
			}
			//-------------------------------------------------------------
			/// Get Components  (Non managed)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type
			///
			/// @param Interface ID
			/// @param Out - Vector to populate with components
            /// @param Query mask used to filter the results
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			void GetComponents(InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<IComponent*> & outComponentsWithInterface, u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true) const;
			//-------------------------------------------------------------
			/// Get Components  (Non managed)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type
			///
			/// @param Out - Vector to populate with components
            /// @param Query mask used to filter the results
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			template <typename T> void GetComponents(DYNAMIC_ARRAY<T*> & outComponents, u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true)
			{
				if(inbClearVectorBeforeUsing) 
				{
					outComponents.clear();
				}
                
                for (ComponentList::const_iterator pItr = mComponents.begin(); pItr != mComponents.end(); ++pItr)
                {
                    if(inudwQueryMask != 0)
                    {
                        u32 udwMaskComponent = (*pItr)->GetQueryMask();
                        
                        if((inudwQueryMask & udwMaskComponent) == 0)
                        {
                            continue;
                        }
                    }
                    
                    if( (*pItr)->IsA(T::InterfaceID))
                    {
                        outComponents.push_back(static_cast<T*>(pItr->get()));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Get Components  (Non managed)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type
			///
			/// @param Out - Vector to populate with components T1
            /// @param Out - Vector to populate with components T2
            /// @param Query mask used to filter the results
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			template <typename T1, typename T2> void GetComponents(DYNAMIC_ARRAY<T1*> & outComponents1, DYNAMIC_ARRAY<T2*> & outComponents2,
                                                                   u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true)
			{
				if(inbClearVectorBeforeUsing)
				{
					outComponents1.clear();
                    outComponents2.clear();
				}

                for (ComponentList::const_iterator pItr = mComponents.begin(); pItr != mComponents.end(); ++pItr)
                {
                    if(inudwQueryMask != 0)
                    {
                        u32 udwMaskComponent = (*pItr)->GetQueryMask();
                    
                        if((inudwQueryMask & udwMaskComponent) == 0)
                        {
                            continue;
                        }
                    }
                    
                    if((*pItr)->IsA(T1::InterfaceID))
                    {
                        outComponents1.push_back(static_cast<T1*>(pItr->get()));
                    }
                    if((*pItr)->IsA(T2::InterfaceID))
                    {
                        outComponents2.push_back(static_cast<T2*>(pItr->get()));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Get Components  (Non managed)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type
			///
			/// @param Out - Vector to populate with components T1
            /// @param Out - Vector to populate with components T2
            /// @param Out - Vector to populate with components T3
            /// @param Query mask used to filter the results
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			template <typename T1, typename T2, typename T3> void GetComponents(DYNAMIC_ARRAY<T1*> & outComponents1, DYNAMIC_ARRAY<T2*> & outComponents2, DYNAMIC_ARRAY<T3*> & outComponents3,
                                                                                u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true)
			{
				if(inbClearVectorBeforeUsing)
				{
                    outComponents1.clear();
                    outComponents2.clear();
                    outComponents3.clear();
				}
                
                for (ComponentList::const_iterator pItr = mComponents.begin(); pItr != mComponents.end(); ++pItr)
                {
                    if(inudwQueryMask != 0)
                    {
                        u32 udwMaskComponent = (*pItr)->GetQueryMask();
                        
                        if((inudwQueryMask & udwMaskComponent) == 0)
                        {
                            continue;
                        }
                    }
                    
                    if((*pItr)->IsA(T1::InterfaceID))
                    {
                        outComponents1.push_back(static_cast<T1*>(pItr->get()));
                    }
                    if((*pItr)->IsA(T2::InterfaceID))
                    {
                        outComponents2.push_back(static_cast<T2*>(pItr->get()));
                    }
                    if((*pItr)->IsA(T3::InterfaceID))
                    {
                        outComponents3.push_back(static_cast<T3*>(pItr->get()));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Get Component  (Managed Recursive)
			///
			/// @return the first component of the given type within
            /// entity or children
			//-------------------------------------------------------------
			ComponentPtr GetManagedComponentRecursive(InterfaceIDType inInterfaceID, u32 inudwQueryMask = 0) const;
			//-------------------------------------------------------------
			/// Get Component  (Managed Recursive)
			///
			/// @return the first component of the given type within
            /// entity or children
			//-------------------------------------------------------------
			template <typename T> SHARED_PTR<T> GetManagedComponentRecursive(u32 inudwQueryMask = 0)
			{
				return SHARED_PTR_CAST<T>(GetManagedComponentRecursive(T::InterfaceID, inudwQueryMask));
			}
            //-------------------------------------------------------------
			/// Get Component  (Managed Recursive)
			///
			/// @return the first component of the given type within
            /// entity or children
			//-------------------------------------------------------------
			template <typename T> SHARED_PTR<const T> GetManagedComponentRecursive(u32 inudwQueryMask = 0) const
			{
				return SHARED_PTR_CAST<T>(GetManagedComponentRecursive(T::InterfaceID, inudwQueryMask));
			}
			//-------------------------------------------------------------
			/// Get Components  (Managed Recursive)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type. This will recurse
            /// into child entities
			///
			/// @param Interface ID
			/// @param Out - Vector to populate with components
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			void GetManagedComponentsRecursive(InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<ComponentPtr> & outComponentsWithInterface, u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true) const;
			//-------------------------------------------------------------
			/// Get Components  (Managed Recursive)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type. This will recurse
            /// into child entities
			///
			/// @param Interface ID
			/// @param Out - Vector to populate with components
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			template <typename T> void GetManagedComponentsRecursive(DYNAMIC_ARRAY<SHARED_PTR<T> > & outComponentsWithInterface, u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true)
			{
				if(inbClearVectorBeforeUsing) 
				{
					outComponentsWithInterface.clear();
				}
				for (ComponentList::const_iterator pItr = mComponents.begin(); pItr != mComponents.end(); ++pItr)
				{
					u32 udwMaskComponent = (*pItr)->GetQueryMask();
					if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
					{
						if((*pItr)->IsA(T::InterfaceID)) 
						{
							outComponentsWithInterface.push_back(SHARED_PTR_CAST<T>(*pItr));
						}
					}
				}
                
                for(SharedEntityList::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
                {
                    (*it)->GetManagedComponentsRecursive(outComponentsWithInterface, inudwQueryMask, false);
                }
			}
			//-------------------------------------------------------------
			/// Get Component  (Non managed Recursive)
			///
			/// @return the first component of the given type. This will recurse
            /// into child entities
			//-------------------------------------------------------------
			IComponent* GetComponentRecursive(InterfaceIDType inInterfaceID, u32 inudwQueryMask = 0) const;
			//-------------------------------------------------------------
			/// Get Component  (Non managed Recursive)
			///
			/// @return the first component of the given type. This will recurse
            /// into child entities
			//-------------------------------------------------------------
			template <typename T> T* GetComponentRecursive(u32 inudwQueryMask = 0)
			{
				return static_cast<T*>(GetComponentRecursive(T::InterfaceID, inudwQueryMask));
			}
            //-------------------------------------------------------------
			/// Get Component  (Non managed Recursive)
			///
			/// @return the first component of the given type. This will recurse
            /// into child entities
			//-------------------------------------------------------------
			template <typename T> const T* GetComponentRecursive(u32 inudwQueryMask = 0) const
			{
				return static_cast<T*>(GetComponentRecursive(T::InterfaceID, inudwQueryMask));
			}
			//-------------------------------------------------------------
			/// Get Components  (Non managed Recursive)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type. This will recurse
            /// into child entities
			///
			/// @param Interface ID
			/// @param Out - Vector to populate with components
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			void GetComponentsRecursive(InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<IComponent*> & outComponentsWithInterface, u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true) const;
			//-------------------------------------------------------------
			/// Get Components  (Non managed Recursive)
			///
			/// Search the list of components and add ones to the list
			/// that are of given interface type. This will recurse
            /// into child entities
			///
			/// @param Interface ID
			/// @param Out - Vector to populate with components
			/// @param Whether to clear the vector before populating
			//-------------------------------------------------------------
			template <typename T> void GetComponentsRecursive(DYNAMIC_ARRAY<T*> & outComponentsWithInterface, u32 inudwQueryMask = 0, bool inbClearVectorBeforeUsing = true)
			{
				if(inbClearVectorBeforeUsing) 
				{
					outComponentsWithInterface.clear();
				}
				for (ComponentList::const_iterator pItr = mComponents.begin(); pItr != mComponents.end(); ++pItr)
				{
					u32 udwMaskComponent = (*pItr)->GetQueryMask();
					if(inudwQueryMask == 0 || inudwQueryMask & udwMaskComponent)
					{
						if((*pItr)->IsA(T::InterfaceID)) 
						{
							outComponentsWithInterface.push_back(static_cast<T*>(pItr->get()));
						}
					}
				}
                
                for(SharedEntityList::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
                {
                    (*it)->GetComponentsRecursive<T>(outComponentsWithInterface, inudwQueryMask, false);
                }
			}
			//-------------------------------------------------------------
			/// Attach
			///
			/// @param Component to attach to the entity
			//-------------------------------------------------------------
			void Attach(const ComponentPtr & inpComponent);
			//-------------------------------------------------------------
			/// Detach
			///
			/// @param Component to remove from the entity
			//-------------------------------------------------------------
			void Detach(const ComponentPtr & inpComponent);
            //-------------------------------------------------------------
            /// Detach All
            ///
            /// Removes all components
            //-------------------------------------------------------------
            void DetachAll();
			//-------------------------------------------------------------
			/// Add Child
			///
			/// @param Entity to derive transform from
			//-------------------------------------------------------------
			void AddChild(const EntityPtr& inpEntity);
			//-------------------------------------------------------------
			/// Remove Child
			///
			/// @param Entity no longer with hierarchical relationship
			//-------------------------------------------------------------
			void RemoveChild(const EntityPtr& inpEntity);
			//-------------------------------------------------------------
			/// Remove From Parent
			///
			/// Detach the entity from its parent. If it has no parent
            /// then remove it from the scene
			//-------------------------------------------------------------
			void RemoveFromParent();
			//-------------------------------------------------------------
			/// Remove All Children
			///
			//-------------------------------------------------------------
			void RemoveAllChildren();
			//-------------------------------------------------------------
			/// Has Name
			///
			/// @return Whether this entity is named
			//-------------------------------------------------------------
			bool HasName();
			//-------------------------------------------------------------
			/// Set Name
			///
			/// @param Give this entity a name
			//-------------------------------------------------------------
			void SetName(const std::string & incName);
			//-------------------------------------------------------------
			/// Get Name
			///
			/// @return Entity name
			//-------------------------------------------------------------
			const std::string & GetName() const;
			//-------------------------------------------------------------
			/// Remove From Scene
			///
			/// Will remove all children from the scene
			///
			/// @param Scene
			//-------------------------------------------------------------
			void RemoveFromScene();			
			//------------------------------------------------------------------
			/// GetParent
			/// 
			/// Return the parent entity of this entity. NULL is returned naturally if
			/// there is no parent.
			///
			/// @return Pointer to the parent entity of this
			//------------------------------------------------------------------
			CEntity* GetParent();
			
			//------------------------------------------------------------------
			/// FindChildEntityWithName
			/// 
			/// Recursively search the entity hierarchy in a depth-first manner
			/// and return a pointer to the first entity it finds with the name matching
			/// parameter instrName, or NULL if no match found.
			///
			/// @return A child entity with the specified name
			//------------------------------------------------------------------
			CEntity* FindChildEntityWithName(const std::string& instrName);
            
            //------------------------------------------------------------------
			/// GetNumberOfChildEntities
			/// 
			/// Return the number of child entities
			///
			/// @return the number of child entities
			//------------------------------------------------------------------
            u32 GetNumberOfChildEntities() const;
            //------------------------------------------------------------------
			/// GetImmediateChildWithName
			/// 
			/// Search only immediate children to an entity and return a pointer
			/// to the first entity it finds with the name matching parameter 
			/// instrName, or NULL if no match found.
			///
			/// @return A child entity with the specified name
			//------------------------------------------------------------------
            EntityPtr GetImmediateChildWithName(const std::string& instrName);
			//------------------------------------------------------------------
			/// Get Entity AnimationC ontroller Ptr
			/// 
			/// Return the entity animation controller pointer for this entity
			///
			/// @return Anim controller
			//------------------------------------------------------------------
			EntityAnimationControllerPtr GetEntityAnimationControllerPtr();
            //------------------------------------------------------------------
			/// Get Entity Animation Controller
			/// 
			/// Return the entity animation controller pointer for this entity
			///
			/// @return Anim controller
			//------------------------------------------------------------------
			EntityAnimationControllerPtr& GetEntityAnimationController();
            //------------------------------------------------------------------
			/// Get Child Entites
			/// 
            /// @return List of child entities
			//------------------------------------------------------------------
            SharedEntityList& GetChildEntities();
            //------------------------------------------------------------------
            /// To JSON
            /// 
            /// @return A JSON object of the hierarchy of entities and components
            //------------------------------------------------------------------
            Json::Value ToJSON() const;
            
            //------------------------------------------------------------------
            /// Reset
            ///
            /// Utility function to reset an entity to blank slate
            /// useful for pooling
            ///
            //------------------------------------------------------------------
            void Reset();
            //------------------------------------------------------------------
            /// Get All Managed Components
            ///
            /// @return Array of components
            //------------------------------------------------------------------
            const ComponentList& GetAllManagedComponents() const;
            
		private:
            
            // Direct copying of Entity is undefined and forbidden
            CEntity(const CEntity& inCopy) { *this= inCopy; }
			CEntity& operator= (const CEntity& inRHS) { return *this; }
            
			//-------------------------------------------------------------
			/// Add To Scene
			///
			/// @param Scene
			//-------------------------------------------------------------
			void AddToScene(CScene* inpScene);
			//-------------------------------------------------------------
			/// Remove From Scene
			///
			/// @param Scene
			//-------------------------------------------------------------
			void RemoveFromScene(CScene* inpScene);
			
		protected:
			
			ComponentList mComponents;
			SharedEntityList mEntities;	
            
            CTransform mLocalTransform;
			
			std::string mstrName;
			
			CEntity* mpParent;
			CScene* mpOwningScene;
            
            bool mbVisible;
			EntityAnimationControllerPtr mpEntityAnimationController;
			
		};
		
		//------------------------------------------------------------------
		/// Create Entity
		/// 
		/// Convienience method to create an empty entity
		///
		/// @return Entity
		//------------------------------------------------------------------
		EntityPtr CreateEntity();
		//------------------------------------------------------------------
		/// Create Entity
		/// 
		/// Convienience method to create an entity and attach component
		///
		/// @return Entity
		//------------------------------------------------------------------
		EntityPtr CreateEntity(const ComponentPtr & inpComponent);
		
		void operator +=(const EntityPtr& pEnt, const ComponentPtr& pComp);
		void operator -=(const EntityPtr& pEnt, const ComponentPtr& pComp);
		void operator +=(const EntityPtr& pEnt, const EntityPtr& pChild);
		void operator -=(const EntityPtr& pEnt, const EntityPtr& pChild);
	}
}

#endif
