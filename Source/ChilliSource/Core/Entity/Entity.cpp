//
//  Entity.cpp
//  ChilliSource
//
//  Created by Scott Downie on 21/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Scene/Scene.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <algorithm>

namespace ChilliSource
{
	namespace Core
	{
        namespace
        {
            const EntitySPtr EntityNullPtr;
            const ComponentSPtr ComponentNullPtr;
        }
        
        //------------------------------------------------------------------
		//------------------------------------------------------------------
		EntitySPtr CreateEntity()
		{
			return EntitySPtr(new Entity());
		}
        
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		Entity::Entity()
        : m_isVisible(true)
        , m_parent(nullptr)
        , m_scene(nullptr)
		{
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::Add(const EntitySPtr& in_child)
		{
            CS_ASSERT(in_child != nullptr, "Cannot add null child");
            CS_ASSERT(in_child->GetParent() == nullptr, "Cannot add child with existing parent");
            CS_ASSERT(in_child->GetScene() == nullptr, "Cannot add child with existing scene");
            
            m_children.push_back(in_child);
            m_transform.AddChildTransform(&in_child->GetTransform());
            in_child->m_parent = this;
			
			if(m_scene != nullptr)
			{
				//This will add the entity and any children it may have
				m_scene->Add(in_child);
			}
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::Remove(Entity* in_child)
		{
            CS_ASSERT(in_child != nullptr, "Cannot remove null child");
            CS_ASSERT(in_child->GetParent() == this, "Cannot remove child with existing parent");
            
			SharedEntityList::iterator it = std::find_if(m_children.begin(), m_children.end(), [in_child](const EntitySPtr& in_entity)
            {
                return in_entity.get() == in_child;
            });
			
			if(it != m_children.end())
			{
				m_transform.RemoveChildTransform(&in_child->GetTransform());
				
				if(m_scene != nullptr)
				{
					m_scene->Remove(in_child);
				}
                
                in_child->m_parent = nullptr;
                std::swap(m_children.back(), *it);
				m_children.pop_back();
			}
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::RemoveAllChildren()
		{
			m_transform.RemoveAllChildTransforms();
			
			for(SharedEntityList::iterator it = m_children.begin(); it != m_children.end(); ++it)
			{
                if(m_scene != nullptr)
				{
					m_scene->Remove(it->get());
				}
                
                (*it)->m_parent = nullptr;
			}
			
			m_children.clear();
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::RemoveFromParent()
		{
            CS_ASSERT(m_parent != nullptr || m_scene != nullptr, "Must have a parent to remove from");
            
            if(m_parent != nullptr)
            {
                m_parent->Remove(this);
            }
            else if(m_scene != nullptr)
            {
                m_scene->Remove(this);
            }
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::OnAddedToScene()
		{
            for(ComponentList::iterator it = m_components.begin(); it != m_components.end(); ++it)
            {
                (*it)->OnAddedToScene();
            }
            
			for(SharedEntityList::iterator it = m_children.begin(); it != m_children.end(); ++it)
			{
				(*it)->OnAddedToScene();
			}
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::OnRemovedFromScene()
		{
            for(SharedEntityList::iterator it = m_children.begin(); it != m_children.end(); ++it)
			{
				(*it)->OnRemovedFromScene();
			}
            
            for(ComponentList::iterator it = m_components.begin(); it != m_components.end(); ++it)
            {
                (*it)->OnRemovedFromScene();
            }
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		Entity* Entity::GetParent() 
		{ 
			return m_parent;
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const EntitySPtr& Entity::GetChildEntityWithName(const std::string& in_name) const
        {
            for(SharedEntityList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
            {
                if((*it)->GetName() == in_name)
                {
                    return (*it);
                }
            }
            
            return EntityNullPtr;
        }
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		const EntitySPtr& Entity::GetChildEntityWithNameRecursive(const std::string& in_name) const
		{
            for (SharedEntityList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
            {
                if((*it)->GetName() == in_name)
                {
                    return (*it);
                }
                
                const EntitySPtr& entity = (*it)->GetChildEntityWithNameRecursive(in_name);
                if (entity != nullptr)
                {
                    return entity;
                }
            }
			
			return EntityNullPtr;
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        Json::Value Entity::ToJSON() const
        {
            Json::Value jResult;
            Json::Value jComponents(Json::arrayValue), jEntities(Json::arrayValue);
            
            for(ComponentList::const_iterator it = m_components.begin(); it != m_components.end(); ++it)
            {
                jComponents.append((*it)->GetInterfaceTypeName());
            }
            
            for(SharedEntityList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
            {
                Json::Value jEntity = (*it)->ToJSON();
                jEntities.append(jEntity);
            }
            
            jResult[m_name]["Components"] = jComponents;
            jResult[m_name]["Entities"] = jEntities;
            
            return jResult;
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        u32 Entity::GetNumEntities() const
        {
            return m_children.size();
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        u32 Entity::GetNumComponents() const
        {
            return m_components.size();
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		const EntityAnimationControllerSPtr& Entity::GetEntityAnimationController() const
		{
			return m_entityAnimController;
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
		EntityAnimationControllerSPtr& Entity::GetEntityAnimationController()
		{
			return m_entityAnimController;
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const SharedEntityList& Entity::GetEntities() const
        {
            return m_children;
        }
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::SetScene(Scene* in_scene)
		{
			m_scene = in_scene;
		}
        //-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::Add(const ComponentSPtr& in_component)
		{
            CS_ASSERT(in_component != nullptr, "Cannot add null component");
            CS_ASSERT(in_component->GetEntity() == nullptr, "Component cannot be attached to more than 1 entity at a time.");
            
            m_components.push_back(in_component);
            
            in_component->SetEntity(this);
            
            in_component->OnAddedToEntity();
            
            if(GetScene() != nullptr)
            {
                in_component->OnAddedToScene();
            }
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::Remove(Component* in_component)
		{
            CS_ASSERT(in_component != nullptr, "Cannot remove a null component");
            CS_ASSERT(in_component->GetEntity() == this, "Component must have an owner in order to be removed.");
            
            for(ComponentList::iterator it = m_components.begin(); it != m_components.end(); ++it)
            {
                if(in_component == it->get())
                {
                    if(GetScene() != nullptr)
                    {
                        in_component->OnRemovedFromScene();
                    }
                    
                    in_component->OnRemovedFromEntity();
                    in_component->SetEntity(nullptr);
                    
                    std::swap(m_components.back(), *it);
                    m_components.pop_back();
                    return;
                }
            }
		}
        //-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::RemoveAllComponents()
		{
            for(ComponentList::iterator it = m_components.begin(); it != m_components.end(); ++it)
            {
                Component* component = it->get();
                
                if(GetScene() != nullptr)
                {
                    component->OnRemovedFromScene();
                }
                
                component->OnRemovedFromEntity();
                component->SetEntity(nullptr);
            }
            
            m_components.clear();
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		const ComponentSPtr& Entity::GetComponent(InterfaceIDType in_interfaceID) const
		{
			for(ComponentList::const_iterator itr = m_components.begin(); itr != m_components.end(); ++itr)
			{
                if((*itr)->IsA(in_interfaceID))
                {
                    return *itr;
                }
			}
			
			return ComponentNullPtr;
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::GetComponents(InterfaceIDType in_interfaceID, std::vector<ComponentSPtr>& out_components) const
		{
			for (ComponentList::const_iterator itr = m_components.begin(); itr != m_components.end(); ++itr)
			{
                if ((*itr)->IsA(in_interfaceID))
                {
                    out_components.push_back(*itr);
                }
			}
		}
        //-------------------------------------------------------------
		//-------------------------------------------------------------
		const ComponentSPtr& Entity::GetComponentRecursive(InterfaceIDType in_interfaceID) const
		{
			for(ComponentList::const_iterator itr = m_components.begin(); itr != m_components.end(); ++itr)
			{
                if((*itr)->IsA(in_interfaceID))
                {
                    return *itr;
                }
			}
            
            for(SharedEntityList::const_iterator itr = m_children.begin(); itr != m_children.end(); ++itr)
            {
                const ComponentSPtr& component = (*itr)->GetComponentRecursive(in_interfaceID);
                
                if(component != nullptr)
                {
                    return component;
                }
            }
			
			return ComponentNullPtr;
		}
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const ComponentList& Entity::GetComponents() const
        {
            return m_components;
        }
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		bool Entity::HasName() const
		{
			return m_name.length() > 0;
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::SetName(const std::string & in_name)
		{
			m_name = in_name;
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		const std::string & Entity::GetName() const
		{
			return m_name;
		}
        //-------------------------------------------------------------
		//-------------------------------------------------------------
		Scene* Entity::GetScene()
		{
			return m_scene;
		}
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        Transform& Entity::GetTransform()
        {
            return m_transform;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const Transform& Entity::GetTransform() const
        {
            return m_transform;
        }
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        void Entity::SetVisible(bool in_isVisible)
        {
            m_isVisible = in_isVisible;
        }
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        bool Entity::IsVisible() const
        {
            return m_parent ? m_isVisible && m_parent->IsVisible() : m_isVisible;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Entity::OnUpdate(f32 in_timeSinceLastUpdate)
        {
            for(u32 i=0; i<m_components.size(); ++i)
			{
                m_components[i]->OnUpdate(in_timeSinceLastUpdate);
			}
            
            for(u32 i=0; i<m_children.size(); ++i)
            {
                m_children[i]->OnUpdate(in_timeSinceLastUpdate);
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Entity::OnFixedUpdate(f32 in_fixedTimeSinceLastUpdate)
        {
            for(u32 i=0; i<m_components.size(); ++i)
			{
                m_components[i]->OnFixedUpdate(in_fixedTimeSinceLastUpdate);
			}
            
            for(u32 i=0; i<m_children.size(); ++i)
            {
                m_children[i]->OnFixedUpdate(in_fixedTimeSinceLastUpdate);
            }
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        void Entity::Reset()
        {
            RemoveAllComponents();
            RemoveAllChildren();
            RemoveFromParent();
            
            m_name = std::string();
            m_transform.Reset();
            
            m_isVisible = true;
			m_entityAnimController = EntityAnimationControllerSPtr();
        }
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		Entity::~Entity()
		{
            RemoveAllComponents();
			RemoveAllChildren();
		}
	}
}