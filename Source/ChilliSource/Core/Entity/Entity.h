//
//  Entity.h
//  ChilliSource
//
//  Created by Scott Downie on 21/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_ENTITY_ENTITY_H_
#define _CHILLISOURCE_CORE_ENTITY_ENTITY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Entity/Transform.h>
#include <ChilliSource/Core/JSON/json.h>

#ifdef CS_ENABLE_ENTITYLINKEDLIST
#include <list>
#else
#include <vector>
#endif

namespace ChilliSource
{
	namespace Core 
	{
		typedef std::vector<ComponentSPtr> ComponentList;
		
#ifdef CS_ENABLE_ENTITYLINKEDLIST
		typedef std::list<EntitySPtr> SharedEntityList;
		typedef std::list<Entity*> EntityList;
#else		
		typedef std::vector<EntitySPtr> SharedEntityList;
		typedef std::vector<Entity*> EntityList;
#endif
		
		//--------------------------------------------------------------------------------------------------
		/// A game object that's behaviour is defined by its components
		//--------------------------------------------------------------------------------------------------
		class Entity final : public std::enable_shared_from_this<Entity>
		{	
		public:
            
            //----------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------
			Entity();
            //----------------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------
            ~Entity();
            //----------------------------------------------------------------
            /// Direct copying of Entity is undefined and forbidden
            ///
            /// @author A Glass
            //----------------------------------------------------------------
            Entity(const Entity&) = delete;
            //----------------------------------------------------------------
            /// Direct copying of Entity is undefined and forbidden
            ///
            /// @author A Glass
            //----------------------------------------------------------------
			Entity& operator= (const Entity&) = delete;
        
            //----------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Transform object to allow for spatial manipulation
            //----------------------------------------------------------------
            Transform& GetTransform();
            //----------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Transform object to allow for spatial manipulation
            //----------------------------------------------------------------
            const Transform& GetTransform() const;
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Whether any renderable components should be shown
            //-------------------------------------------------------------
            void SetVisible(bool in_isVisible);
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether any renderable components should be shown
            //-------------------------------------------------------------
            bool IsVisible() const;
			//-------------------------------------------------------------
			/// Get Scene
			///
			/// @return Scene owner
			//-------------------------------------------------------------
			Scene* GetScene();
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the entity has component of type
            //-------------------------------------------------------------
            template <typename TComponentType>
            bool HasA() const
            {
                return GetComponent<TComponentType>() != nullptr;
            }
			//-------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The first component of type
			//-------------------------------------------------------------
			template <typename TComponentType>
            std::shared_ptr<TComponentType>& GetComponent()
			{
				return std::static_pointer_cast<TComponentType>(GetComponent(TComponentType::InterfaceID));
			}
            //-------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The first component of type
			//-------------------------------------------------------------
			template <typename TComponentType>
            const std::shared_ptr<const TComponentType>& GetComponent() const
			{
				return std::static_pointer_cast<const TComponentType>(GetComponent(TComponentType::InterfaceID));
			}
			//-------------------------------------------------------------
			/// Search the list of components and add ones to the list
			/// that are of given interface type
            ///
            /// @author S Downie
			///
			/// @param [Out] Vector to populate with components
			//-------------------------------------------------------------
			template <typename TComponentType>
            void GetComponents(std::vector<std::shared_ptr<TComponentType> >& out_components) const
			{
                for (ComponentList::const_iterator it = m_components.begin(); it != m_components.end(); ++it)
                {
                    if((*it)->IsA(TComponentType::InterfaceID))
                    {
                        out_components.push_back(std::static_pointer_cast<TComponentType>(*it));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Search the list of components and add ones to the list
			/// that are of given interface type
            ///
            /// @author S Downie
			///
			/// @param [Out] Vector to populate with components
			//-------------------------------------------------------------
			template <typename TComponentType>
            void GetComponents(std::vector<TComponentType*>& out_components) const
			{
                for (ComponentList::const_iterator it = m_components.begin(); it != m_components.end(); ++it)
                {
                    if((*it)->IsA(TComponentType::InterfaceID))
                    {
                        out_components.push_back(static_cast<TComponentType*>(it->get()));
                    }
                }
			}
			//-------------------------------------------------------------
			/// Search the list of components and add ones to the lists
			/// that are of given interface types
            ///
            /// @author S Downie
			///
			/// @param [Out] Vector to populate with components type 1
            /// @param [Out] Vector to populate with components type 2
			//-------------------------------------------------------------
			template <typename TComponentType1, typename TComponentType2>
            void GetComponents(std::vector<std::shared_ptr<TComponentType1> >& out_components1, std::vector<std::shared_ptr<TComponentType2> >& out_components2) const
			{
                for (ComponentList::const_iterator it = m_components.begin(); it != m_components.end(); ++it)
                {
                    if((*it)->IsA(TComponentType1::InterfaceID))
                    {
                        out_components1.push_back(std::static_pointer_cast<TComponentType1>(*it));
                    }
                    if((*it)->IsA(TComponentType2::InterfaceID))
                    {
                        out_components2.push_back(std::static_pointer_cast<TComponentType2>(*it));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Search the list of components and add ones to the lists
			/// that are of given interface types
            ///
            /// @author S Downie
			///
			/// @param [Out] Vector to populate with components type 1
            /// @param [Out] Vector to populate with components type 2
			//-------------------------------------------------------------
			template <typename TComponentType1, typename TComponentType2>
            void GetComponents(std::vector<TComponentType1*>& out_components1, std::vector<TComponentType2*>& out_components2) const
			{
                for (ComponentList::const_iterator it = m_components.begin(); it != m_components.end(); ++it)
                {
                    if((*it)->IsA(TComponentType1::InterfaceID))
                    {
                        out_components1.push_back(static_cast<TComponentType1*>(it->get()));
                    }
                    if((*it)->IsA(TComponentType2::InterfaceID))
                    {
                        out_components2.push_back(static_cast<TComponentType2*>(it->get()));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Search the list of components and add ones to the lists
			/// that are of given interface types
            ///
            /// @author S Downie
			///
			/// @param [Out] Vector to populate with components type 1
            /// @param [Out] Vector to populate with components type 2
            /// @param [Out] Vector to populate with components type 3
			//-------------------------------------------------------------
			template <typename TComponentType1, typename TComponentType2, typename TComponentType3>
            void GetComponents(std::vector<std::shared_ptr<TComponentType1> >& out_components1, std::vector<std::shared_ptr<TComponentType2> >& out_components2, std::vector<std::shared_ptr<TComponentType3> >& out_components3) const
			{
                for (ComponentList::const_iterator it = m_components.begin(); it != m_components.end(); ++it)
                {
                    if((*it)->IsA(TComponentType1::InterfaceID))
                    {
                        out_components1.push_back(std::static_pointer_cast<TComponentType1>(*it));
                    }
                    if((*it)->IsA(TComponentType2::InterfaceID))
                    {
                        out_components2.push_back(std::static_pointer_cast<TComponentType2>(*it));
                    }
                    if((*it)->IsA(TComponentType3::InterfaceID))
                    {
                        out_components3.push_back(std::static_pointer_cast<TComponentType3>(*it));
                    }
                }
			}
            //-------------------------------------------------------------
			/// Search the list of components and add ones to the lists
			/// that are of given interface types
            ///
            /// @author S Downie
			///
			/// @param [Out] Vector to populate with components type 1
            /// @param [Out] Vector to populate with components type 2
            /// @param [Out] Vector to populate with components type 3
			//-------------------------------------------------------------
			template <typename TComponentType1, typename TComponentType2, typename TComponentType3>
            void GetComponents(std::vector<TComponentType1*>& out_components1, std::vector<TComponentType2*>& out_components2, std::vector<TComponentType3*>& out_components3) const
			{
                for (ComponentList::const_iterator it = m_components.begin(); it != m_components.end(); ++it)
                {
                    if((*it)->IsA(TComponentType1::InterfaceID))
                    {
                        out_components1.push_back(static_cast<TComponentType1*>(it->get()));
                    }
                    if((*it)->IsA(TComponentType2::InterfaceID))
                    {
                        out_components2.push_back(static_cast<TComponentType2*>(it->get()));
                    }
                    if((*it)->IsA(TComponentType3::InterfaceID))
                    {
                        out_components3.push_back(static_cast<TComponentType3*>(it->get()));
                    }
                }
			}
			//-------------------------------------------------------------
			/// Recursively descend from the entity through its children
            /// searching for components with given interface ID
            ///
            /// @author A Mackie
			///
			/// @return the first component of the given type. This will recurse
            /// into child entities
			//-------------------------------------------------------------
			template <typename TComponentType>
            std::shared_ptr<TComponentType>& GetComponentRecursive()
			{
				return std::static_pointer_cast<TComponentType>(GetComponentRecursive(TComponentType::InterfaceID));
			}
            //-------------------------------------------------------------
			/// Recursively descend from the entity through its children
            /// searching for components with given interface ID
            ///
            /// @author A Mackie
			///
			/// @return the first component of the given type. This will recurse
            /// into child entities
			//-------------------------------------------------------------
			template <typename TComponentType>
            const std::shared_ptr<const TComponentType>& GetComponentRecursive() const
			{
				return std::static_pointer_cast<const TComponentType>(GetComponentRecursive(TComponentType::InterfaceID));
			}
			//-------------------------------------------------------------
			/// Recursively descend from the entity through its children
            /// searching for components with given interface ID
            ///
            /// @author A Mackie
			///
			/// @return the first component of the given type. This will recurse
            /// into child entities
			//-------------------------------------------------------------
			template <typename TComponentType>
            void GetComponentsRecursive(std::vector<std::shared_ptr<TComponentType> >& out_components) const
			{
                for (ComponentList::const_iterator itr = m_components.begin(); itr != m_components.end(); ++itr)
                {
                    if ((*itr)->IsA(TComponentType::InterfaceID))
                    {
                        out_components.push_back(std::static_pointer_cast<TComponentType>(*itr));
                    }
                }
                
                for(SharedEntityList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
                {
                    (*it)->GetComponentsRecursive<TComponentType>(out_components);
                }
			}
            //-------------------------------------------------------------
			/// Recursively descend from the entity through its children
            /// searching for components with given interface ID
            ///
            /// @author A Mackie
			///
			/// @return the first component of the given type. This will recurse
            /// into child entities
			//-------------------------------------------------------------
			template <typename TComponentType>
            void GetComponentsRecursive(std::vector<TComponentType*>& out_components)
			{
                for (ComponentList::const_iterator itr = m_components.begin(); itr != m_components.end(); ++itr)
                {
                    if ((*itr)->IsA(TComponentType::InterfaceID))
                    {
                        out_components.push_back(static_cast<TComponentType*>(itr->get()));
                    }
                }
                
                for(SharedEntityList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
                {
                    (*it)->GetComponentsRecursive<TComponentType>(out_components);
                }
			}
            //------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Array of components
            //------------------------------------------------------------------
            const ComponentList& GetComponents() const;
			//-------------------------------------------------------------
			/// Add a component to the entity. It will now receieve
            /// entity lifecycle events. The entity will have shared
            /// ownership of the component
            ///
            /// @author S Downie
			///
			/// @param Component to attach to the entity
			//-------------------------------------------------------------
			void Add(const ComponentSPtr& in_component);
			//-------------------------------------------------------------
			/// Remove a component from the entity. The component will no
            /// longer be owned by the entity and will no longer receieve
            /// entity lifecycle events.
            ///
            /// @author S Downie
			///
			/// @param Component to remove from the entity
			//-------------------------------------------------------------
			void Remove(Component* in_component);
            //-------------------------------------------------------------
            /// Removes all components
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            void RemoveAllComponents();
			//-------------------------------------------------------------
			/// Add an entity as a child. This will link the transforms
            /// in a hierarchy and will link lifecycle and scene events
            ///
            /// @author S Downie
			///
			/// @param Entity
			//-------------------------------------------------------------
			void Add(const EntitySPtr& in_child);
			//-------------------------------------------------------------
			/// Remove an entity as a child. This will unlink the transforms
            /// in a hierarchy and will unlink lifecycle and scene events
			///
			/// @param Entity
			//-------------------------------------------------------------
			void Remove(Entity* in_child);
			//-------------------------------------------------------------
			/// Detach the entity from its parent. If it has no parent
            /// then remove it from the scene
            ///
            /// @author S Downie
			//-------------------------------------------------------------
			void RemoveFromParent();
			//-------------------------------------------------------------
			/// Remove All Children
			///
            /// @author S Downie
			//-------------------------------------------------------------
			void RemoveAllChildren();
			//-------------------------------------------------------------
			/// @author I Copland
			///
			/// @return Whether this entity is named
			//-------------------------------------------------------------
            bool HasName() const;
			//-------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Name
			//-------------------------------------------------------------
			void SetName(const std::string& in_name);
			//-------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Entity name
			//-------------------------------------------------------------
			const std::string & GetName() const;
			//------------------------------------------------------------------
			/// Return the parent entity of this entity. nullptr is returned if
			/// there is no parent.
			///
            /// @author T Kane
            ///
			/// @return Pointer to the parent entity or null
			//------------------------------------------------------------------
			Entity* GetParent();
			
            //------------------------------------------------------------------
			/// Search only immediate children to find an entity and return a pointer
			/// to the first entity it finds with the name matching that given,
            /// or nullptr if no match found.
			///
			/// @return An entity with the specified name or null
			//------------------------------------------------------------------
            const EntitySPtr&  GetChildEntityWithName(const std::string& in_name) const;
			//------------------------------------------------------------------
			/// Recursively search the entity hierarchy in a depth-first manner
			/// and return a pointer to the first entity it finds with the name matching
			/// given, or nullptr if no match found.
			///
            /// @author A Mackie
            ///
			/// @return An entity with the specified name or null
			//------------------------------------------------------------------
			const EntitySPtr& GetChildEntityWithNameRecursive(const std::string& in_name) const;
            //------------------------------------------------------------------
            /// @author A Mackie
			///
			/// @return the number of child entities
			//------------------------------------------------------------------
            u32 GetNumEntities() const;
            //------------------------------------------------------------------
            /// @author A Mackie
			///
			/// @return the number of components
			//------------------------------------------------------------------
            u32 GetNumComponents() const;

			//------------------------------------------------------------------
			/// @param A Glass
			///
			/// @return Anim controller
			//------------------------------------------------------------------
			const EntityAnimationControllerSPtr& GetEntityAnimationController() const;
            //------------------------------------------------------------------
            /// @param A Glass
            ///
			/// @return Anim controller
			//------------------------------------------------------------------
			EntityAnimationControllerSPtr& GetEntityAnimationController();
            //------------------------------------------------------------------
			/// @author S Downie
			/// 
            /// @return List of child entities
			//------------------------------------------------------------------
            const SharedEntityList& GetEntities() const;
            //------------------------------------------------------------------
            /// Serialise the entity tree to JSON
            ///
            /// @author S Downie
            /// 
            /// @return A JSON object of the hierarchy of entities and components
            //------------------------------------------------------------------
            Json::Value ToJSON() const;
            //------------------------------------------------------------------
            /// Utility function to reset an entity to blank slate
            /// useful for pooling
            ///
            /// @author A Glass
            //------------------------------------------------------------------
            void Reset();
            
		private:
            
            friend class Scene;
            
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Time since last update in seconds
            //----------------------------------------------------
            void OnUpdate(f32 in_timeSinceLastUpdate);
            //----------------------------------------------------
            /// Fixed update is triggered at fixed time periods
            ///
            /// @author S Downie
            ///
            /// @param Time since last fixed update in seconds
            //----------------------------------------------------
            void OnFixedUpdate(f32 in_fixedTimeSinceLastUpdate);
            //-------------------------------------------------------------
			/// @author S Downie
			///
            /// @param Component type to find
            ///
			/// @return the first component of the given type
			//-------------------------------------------------------------
			const ComponentSPtr& GetComponent(InterfaceIDType in_interfaceID) const;
            //-------------------------------------------------------------
			/// Search the list of components and add ones to the list
			/// that are of given interface type
            ///
            /// @author S Downie
			///
			/// @param Component type to find
			/// @param [Out] Vector to populate with components
			//-------------------------------------------------------------
			void GetComponents(InterfaceIDType in_interfaceID, std::vector<ComponentSPtr>& out_components) const;
            //-------------------------------------------------------------
			/// Recursively descend from the entity through its children
            /// searching for components with given interface ID
            ///
            /// @author A Mackie
			///
			/// @return the first component of the given type. This will recurse
            /// into child entities
			//-------------------------------------------------------------
			const ComponentSPtr& GetComponentRecursive(InterfaceIDType in_interfaceID) const;
            //-------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Scene
			//-------------------------------------------------------------
			void SetScene(Scene* inpScene);
			//-------------------------------------------------------------
			/// Called when the entity is added to the scene
            ///
            /// @author S Downie
			//-------------------------------------------------------------
			void OnAddedToScene();
			//-------------------------------------------------------------
			/// Called when the entity is removed from the scene
            ///
            /// @author S Downie
			//-------------------------------------------------------------
			void OnRemovedFromScene();
			
		private:
			
			ComponentList m_components;
			SharedEntityList m_children;
            
            Transform m_transform;
			
			std::string m_name;
			
			Entity* m_parent;
			Scene* m_scene;
            
            bool m_isVisible;
            
			EntityAnimationControllerSPtr m_entityAnimController;
		};
		
		//------------------------------------------------------------------
		/// Convienience method to create an empty entity
        ///
        /// @author S Downie
		///
		/// @return Entity
		//------------------------------------------------------------------
		EntitySPtr CreateEntity();
	}
}

#endif
