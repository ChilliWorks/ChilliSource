//
//  Entity.cpp
//  Chilli Source
//  Created by Scott Downie on 21/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
		EntityUPtr Entity::Create()
		{
			return EntityUPtr(new Entity());
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::AddEntity(const EntitySPtr& in_child)
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
		void Entity::RemoveEntity(Entity* in_child)
		{
            CS_ASSERT(in_child != nullptr, "Cannot remove null child");
            CS_ASSERT(in_child->GetParent() == this, "Cannot remove entity that is not a child of this");
            
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
                m_parent->RemoveEntity(this);
            }
            else if(m_scene != nullptr)
            {
                m_scene->Remove(this);
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
        const Entity* Entity::GetParent() const
        {
            return m_parent;
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const EntitySPtr& Entity::GetEntityWithName(const std::string& in_name) const
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
		const EntitySPtr& Entity::GetEntityWithNameRecursive(const std::string& in_name) const
		{
            for (SharedEntityList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
            {
                if((*it)->GetName() == in_name)
                {
                    return (*it);
                }
                
                const EntitySPtr& entity = (*it)->GetEntityWithNameRecursive(in_name);
                if (entity != nullptr)
                {
                    return entity;
                }
            }
			
			return EntityNullPtr;
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
		void Entity::AddComponent(const ComponentSPtr& in_component)
		{
            CS_ASSERT(in_component != nullptr, "Cannot add null component");
            CS_ASSERT(in_component->GetEntity() == nullptr, "Component cannot be attached to more than 1 entity at a time.");
            
            m_components.push_back(in_component);
            
            in_component->SetEntity(this);
            
            in_component->OnAddedToEntity();
            
            if(GetScene() != nullptr)
            {
                in_component->OnAddedToScene();
                if (m_appActive == true)
                {
                    in_component->OnResume();
                    if (m_appForegrounded == true)
                    {
                        in_component->OnForeground();
                    }
                }
            }
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::RemoveComponent(Component* in_component)
		{
            CS_ASSERT(in_component != nullptr, "Cannot remove a null component");
            CS_ASSERT(in_component->GetEntity() == this, "Cannot remove component that is not attached to this.");
            
            for(ComponentList::iterator it = m_components.begin(); it != m_components.end(); ++it)
            {
                if(in_component == it->get())
                {
                    if(GetScene() != nullptr)
                    {
                        if (m_appActive == true)
                        {
                            if (m_appForegrounded == true)
                            {
                                in_component->OnBackground();
                            }
                            in_component->OnSuspend();
                        }
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
                    if (m_appActive == true)
                    {
                        if (m_appForegrounded == true)
                        {
                            component->OnBackground();
                        }
                        component->OnSuspend();
                    }
                    component->OnRemovedFromScene();
                }
                
                component->OnRemovedFromEntity();
                component->SetEntity(nullptr);
            }
            
            m_components.clear();
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		const ComponentSPtr& Entity::GetComponent(InterfaceIDType in_interfaceId) const
		{
			for(ComponentList::const_iterator itr = m_components.begin(); itr != m_components.end(); ++itr)
			{
                if((*itr)->IsA(in_interfaceId))
                {
                    return *itr;
                }
			}
			
			return ComponentNullPtr;
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::GetComponents(InterfaceIDType in_interfaceId, std::vector<ComponentSPtr>& out_components) const
		{
			for (ComponentList::const_iterator itr = m_components.begin(); itr != m_components.end(); ++itr)
			{
                if ((*itr)->IsA(in_interfaceId))
                {
                    out_components.push_back(*itr);
                }
			}
		}
        //-------------------------------------------------------------
		//-------------------------------------------------------------
		const ComponentSPtr& Entity::GetComponentRecursive(InterfaceIDType in_interfaceId) const
		{
			for(ComponentList::const_iterator itr = m_components.begin(); itr != m_components.end(); ++itr)
			{
                if((*itr)->IsA(in_interfaceId))
                {
                    return *itr;
                }
			}
            
            for(SharedEntityList::const_iterator itr = m_children.begin(); itr != m_children.end(); ++itr)
            {
                const ComponentSPtr& component = (*itr)->GetComponentRecursive(in_interfaceId);
                
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
		void Entity::SetName(const std::string& in_name)
		{
            if(in_name == "Comp 1")
            {
                int i = 9;
                ++i;
            }
            
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
        //----------------------------------------------------
        //----------------------------------------------------
        void Entity::OnUpdate(f32 in_timeSinceLastUpdate)
        {
            for(u32 i=0; i<m_components.size(); ++i)
			{
                m_components[i]->OnUpdate(in_timeSinceLastUpdate);
			}
        }
        //-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::OnAddedToScene()
		{
            for (u32 i = 0; i < m_components.size(); ++i)
            {
                m_components[i]->OnAddedToScene();
            }
            
            for (u32 i = 0; i < m_children.size(); ++i)
            {
                m_scene->Add(m_children[i]);
            }
		}
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        void Entity::OnResume()
        {
            CS_ASSERT(m_appActive == false, "Entity: Received resume while already active.");
            
            m_appActive = true;
            for (u32 i = 0; i < m_components.size(); ++i)
            {
                m_components[i]->OnResume();
            }
        }
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        void Entity::OnForeground()
        {
            CS_ASSERT(m_appForegrounded == false, "Entity: Received foreground while already foregrounded.");
            
            m_appForegrounded = true;
            for (u32 i = 0; i < m_components.size(); ++i)
            {
                m_components[i]->OnForeground();
            }
        }
        //-------------------------------------------------------------
		//-------------------------------------------------------------
		void Entity::OnRemovedFromScene()
		{
            for (s32 i = m_children.size() - 1; i >= 0; --i)
            {
                m_scene->Remove(m_children[i].get());
            }
            
            for (s32 i = m_components.size() - 1; i >= 0; --i)
            {
                m_components[i]->OnRemovedFromScene();
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
        }
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        void Entity::OnBackground()
        {
            CS_ASSERT(m_appForegrounded == true, "Entity: Received background while already backgrounded.");
            
            m_appForegrounded = false;
            for (s32 i = m_components.size() - 1; i >= 0; --i)
            {
                m_components[i]->OnBackground();
            }
        }
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        void Entity::OnSuspend()
        {
            CS_ASSERT(m_appActive == true, "Entity: Received suspend while already suspended.");
            
            m_appActive = false;
            for (s32 i = m_components.size() - 1; i >= 0; --i)
            {
                m_components[i]->OnSuspend();
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