//
// Scene.cpp
// ChilliSource
//
// Created by Scott Downie on 21/09/2010.
// Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Scene/Scene.h>

#include <ChilliSource/GUI/Base/Window.h>

#include <algorithm>

namespace ChilliSource
{
	namespace Core
	{
        CS_DEFINE_NAMEDTYPE(Scene);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        SceneUPtr Scene::Create()
        {
            return SceneUPtr(new Scene());
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		Scene::Scene()
        : m_rootWindow(new GUI::Window())
		{
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Scene::IsA(InterfaceIDType in_interfaceID) const
        {
            return Scene::InterfaceID == in_interfaceID;
        }
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Scene::OnForeground()
		{
            m_rootWindow->ListenForTouches();
		}
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
		void Scene::OnUpdate(f32 in_timeSinceLastUpdate)
		{
			m_rootWindow->Update(in_timeSinceLastUpdate);
            
            for(u32 i=0; i<m_entities.size(); ++i)
			{
                m_entities[i]->OnUpdate(in_timeSinceLastUpdate);
            }
		}
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        void Scene::OnFixedUpdate(f32 in_fixedTimeSinceLastUpdate)
        {
            for(u32 i=0; i<m_entities.size(); ++i)
			{
                m_entities[i]->OnFixedUpdate(in_fixedTimeSinceLastUpdate);
            }
        }
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Scene::OnBackground()
		{
			Input::PointerSystem::Pointer pointer;
			m_rootWindow->_OnPointerUp(pointer, 0.0f, Input::PointerSystem::GetDefaultPressType());
            m_rootWindow->UnlistenFromTouches();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Scene::Add(const EntitySPtr& in_entity)
		{
            CS_ASSERT(in_entity != nullptr, "Cannot add a null entity");
            CS_ASSERT(in_entity->GetScene() == nullptr, "Cannot add an entity with pre-exisitng scene");
            
			m_entities.push_back(in_entity);

			in_entity->SetScene(this);
            in_entity->OnAddedToScene();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Scene::Remove(Entity* in_entity)
		{
            CS_ASSERT(in_entity != nullptr, "Cannot remove a null entity");
            CS_ASSERT(in_entity->GetScene() == this, "Cannot add an entity without a pre-exisitng scene");

            SharedEntityList::iterator it = std::find_if(m_entities.begin(), m_entities.end(), [in_entity](const EntitySPtr& in_entityInList)
            {
                return in_entityInList.get() == in_entity;
            });
            
			if(it != m_entities.end())
			{
                in_entity->OnRemovedFromScene();
                in_entity->SetScene(nullptr);
                
                it->swap(m_entities.back());
                m_entities.pop_back();
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Scene::RemoveAllEntities()
		{
			for(u32 i=0; i<m_entities.size(); ++i)
			{
                m_entities[i]->OnRemovedFromScene();
                m_entities[i]->SetScene(nullptr);
			}
            
            m_entities.clear();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const SharedEntityList& Scene::GetEntities() const
		{
			return m_entities;
		}
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		void Scene::QuerySceneForIntersection(const Ray &in_ray, std::vector<VolumeComponent*>& out_volumeComponents)
		{
			std::vector<VolumeComponent*> intersectableComponents;
			QuerySceneForComponents(intersectableComponents);
			
			//Loop through the render components and check for intersection
			//If any intersect then add them to the intersect list
			for(std::vector<VolumeComponent*>::iterator it = intersectableComponents.begin(); it != intersectableComponents.end(); ++it)
			{
				VolumeComponent* component = (*it);
				
				f32 nearIntersection, farIntersection = 0.0f;
				
                if(component->GetOOBB().Contains(in_ray, nearIntersection, farIntersection))
                {
                    //We use this to sort by
                    component->mfQueryIntersectionValue = nearIntersection;
                    out_volumeComponents.push_back(component);
                }
			}
		}
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        GUI::Window* Scene::GetWindow()
        {
            return m_rootWindow.get();
        }
		//--------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------
		Scene::~Scene()
		{
			RemoveAllEntities();
		}
	}
}
