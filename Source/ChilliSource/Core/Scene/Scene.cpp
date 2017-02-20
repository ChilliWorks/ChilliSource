//
//  Scene.cpp
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

#include <ChilliSource/Core/Scene/Scene.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Target/TargetGroup.h>

#include <algorithm>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(Scene);
    
    //-------------------------------------------------------
    //-------------------------------------------------------
    SceneUPtr Scene::Create(TargetGroupUPtr renderTarget) noexcept
    {
        return SceneUPtr(new Scene(std::move(renderTarget)));
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    Scene::Scene(TargetGroupUPtr renderTarget) noexcept
    : m_renderTarget(std::move(renderTarget))
    {
        m_clearColour = ChilliSource::Colour::k_black;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    bool Scene::IsA(InterfaceIDType in_interfaceId) const
    {
        return Scene::InterfaceID == in_interfaceId;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::ResumeEntities()
    {
        CS_ASSERT(m_entitiesActive == false, "Received resume entities event while entities are already active.")
        m_entitiesActive = true;
        
        for(u32 i = 0; i < m_entities.size(); ++i)
        {
            m_entities[i]->OnResume();
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::ForegroundEntities()
    {
        CS_ASSERT(m_entitiesForegrounded == false, "Received foreground entities event while entities are already foregrounded.")
        m_entitiesForegrounded = true;
        
        for(u32 i = 0; i<m_entities.size(); ++i)
        {
            m_entities[i]->OnForeground();
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::UpdateEntities(f32 in_timeSinceLastUpdate)
    {
        if(m_enabled)
        {
            for(u32 i = 0; i < m_entities.size(); ++i)
            {
                m_entities[i]->OnUpdate(in_timeSinceLastUpdate);
            }
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::FixedUpdateEntities(f32 in_fixedTimeSinceLastUpdate)
    {
        if(m_enabled)
        {
            for(u32 i=0; i<m_entities.size(); ++i)
            {
                m_entities[i]->OnFixedUpdate(in_fixedTimeSinceLastUpdate);
            }
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::RenderSnapshotEntities(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        for(u32 i=0; i<m_entities.size(); ++i)
        {
            m_entities[i]->OnRenderSnapshot(renderSnapshot, frameAllocator);
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::BackgroundEntities()
    {
        CS_ASSERT(m_entitiesForegrounded == true, "Received background entities event while entities are already backgrounded.")
        m_entitiesForegrounded = false;
        
        for (auto it = m_entities.rbegin(); it != m_entities.rend(); ++it)
        {
            (*it)->OnBackground();
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::SuspendEntities()
    {
        CS_ASSERT(m_entitiesActive == true, "Received suspend entities event while entities are already suspended.")
        m_entitiesActive = false;
        
        for (auto it = m_entities.rbegin(); it != m_entities.rend(); ++it)
        {
            (*it)->OnSuspend();
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::Add(const EntitySPtr& in_entity)
    {
        CS_ASSERT(in_entity != nullptr, "Cannot add a null entity");
        CS_ASSERT(in_entity->GetScene() == nullptr, "Cannot add an entity with pre-exisitng scene");
        CS_ASSERT((in_entity->GetParent() == nullptr || in_entity->GetParent()->GetScene() == this), "Cannot add an entity to a different scene than its parent.");
        CS_ASSERT(m_entities.size() < static_cast<std::vector<EntitySPtr>::size_type>(std::numeric_limits<u32>::max()), "There are too many entities in the scene. It cannot exceed "
                  + ToString(std::numeric_limits<u32>::max()) + ".");
        
        m_entities.push_back(in_entity);

        in_entity->SetScene(this);
        in_entity->OnAddedToScene();
        
        if (m_entitiesActive == true)
        {
            in_entity->OnResume();
            if (m_entitiesForegrounded == true)
            {
                in_entity->OnForeground();
            }
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void Scene::RemoveAllEntities()
    {
        for(u32 i=0; i<m_entities.size(); ++i)
        {
            Entity* ent = m_entities[i].get();
            
            if (ent->GetParent() == nullptr)
            {
                if (m_entitiesActive == true)
                {
                    if (m_entitiesForegrounded == true)
                    {
                        ent->OnBackground();
                    }
                    ent->OnSuspend();
                }
                
                ent->OnRemovedFromScene();
                ent->SetScene(nullptr);
            }
        }
        
        m_entities.clear();
    }
    
    //------------------------------------------------------------------------------
    void Scene::Render(TargetGroup* target) noexcept
    {
        CS_ASSERT(target != nullptr || m_renderTarget != nullptr, "Cannot force render the main scene");
        
        Application::Get()->RenderScene(this, target);
    }
    
    //-------------------------------------------------------
    //-------------------------------------------------------
    const SharedEntityList& Scene::GetEntities() const
    {
        return m_entities;
    }
    //------------------------------------------------------
    //------------------------------------------------------
    void Scene::SetClearColour(const Colour& in_colour)
    {
        m_clearColour = in_colour;
    }
    //------------------------------------------------------
    //------------------------------------------------------
    const Colour& Scene::GetClearColour() const
    {
        return m_clearColour;
    }
    //------------------------------------------------------
    //------------------------------------------------------
    void Scene::SetActiveCamera(CameraComponent* in_cameraComponent) noexcept
    {
        CS_ASSERT(!in_cameraComponent != !m_activeCameraComponent, "A camera component cannot be activated while another is active, nor can it be deactived if there isn't currently one active.");
        
        m_activeCameraComponent = in_cameraComponent;
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
    void Scene::Remove(Entity* in_entity)
    {
        CS_ASSERT(in_entity != nullptr, "Cannot remove a null entity");
        CS_ASSERT(in_entity->GetScene() == this, "Cannot add an entity without a pre-exisitng scene");
        CS_ASSERT((in_entity->GetParent() == nullptr || in_entity->GetParent()->GetScene() == this), "Cannot remove an entity from a different scene than it's parent.");
        
        auto searchPredicate = [in_entity](const EntitySPtr& in_entityInList)
        {
            return in_entityInList.get() == in_entity;
        };
        
        auto it = std::find_if(m_entities.begin(), m_entities.end(), searchPredicate);
        
        if(it != m_entities.end())
        {
            if (m_entitiesActive == true)
            {
                if (m_entitiesForegrounded == true)
                {
                    in_entity->OnBackground();
                }
                in_entity->OnSuspend();
            }
            
            in_entity->OnRemovedFromScene();
            in_entity->SetScene(nullptr);
            
            //the iterator may have been invalidated during OnBackground, OnSuspend or OnRemovedFromScene, so re-calculate it
            it = std::find_if(m_entities.begin(), m_entities.end(), searchPredicate);
            
            it->swap(m_entities.back());
            m_entities.pop_back();
        }
    }
    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    void Scene::OnDestroy() noexcept
    {
        m_renderTarget.reset();
    }
    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    Scene::~Scene()
    {
        RemoveAllEntities();
    }
}
