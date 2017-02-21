//
//  Scene.h
//  ChilliSource
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

#ifndef _CHILLISOURCE_CORE_SCENE_SCENE_H_
#define _CHILLISOURCE_CORE_SCENE_SCENE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/System/StateSystem.h>
#include <ChilliSource/Core/Volume/VolumeComponent.h>
#include <ChilliSource/Rendering/Target/TargetGroup.h>

namespace ChilliSource
{
    //--------------------------------------------------------------------------------------------------
    /// Holds the entities that form the scene. Entities will be updated from the scene and the
    /// scene is queried by other systems for render components etc.
    ///
    /// @author S Downie
    //--------------------------------------------------------------------------------------------------
    class Scene final : public StateSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(Scene);
        //-------------------------------------------------------
        /// Factory method
        ///
        /// @author S Downie
        ///
        /// @param renderTarget
        ///     The target into which the scene renders, if null renders to screen
        //-------------------------------------------------------
        static SceneUPtr Create(TargetGroupUPtr renderTarget) noexcept;
        
        //-------------------------------------------------------
        /// Destructor
        ///
        /// @author S Downie
        //-------------------------------------------------------
        ~Scene();
        //-------------------------------------------------------
        /// Query interface to determine if the object implements
        /// the interface with the given ID
        ///
        /// @author S Downie
        ///
        /// @param Interface ID
        ///
        /// @return Whether the system has the given interface
        //-------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        
        /// Enable/Disable updating and rendering on the scene
        ///
        /// @param enabled
        ///     True if enabling, False if disabling
        ///
        void SetEnabled(bool enabled) noexcept { m_enabled = enabled; }
        
        /// @return TRUE if the scene is updating and rendering
        ///
        bool IsEnabled() const noexcept { return m_enabled; }
        
        //-------------------------------------------------------
        /// Add an entity to the scene. This entity cannot
        /// exist on another scene prior to adding. The entity
        /// will now be updated from the scene
        ///
        /// @author S Downie
        ///
        /// @param Entity
        //-------------------------------------------------------
        void Add(const EntitySPtr& in_entity);
        //-------------------------------------------------------
        /// Remove all the entities from the scene
        ///
        /// @author S Downie
        //-------------------------------------------------------
        void RemoveAllEntities();
        //-------------------------------------------------------
        /// @author A Mackie
        ///
        /// @return the list of all the entities in the scene.
        //-------------------------------------------------------
        const SharedEntityList& GetEntities() const;
        //------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Screen clear colour
        //------------------------------------------------------
        void SetClearColour(const Colour& in_colour);
        //------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Screen clear colour
        //------------------------------------------------------
        const Colour& GetClearColour() const;
        //------------------------------------------------------
        /// Sets the active camera in the scene. This should be
        /// set by CameraComponent during OnAddedToScene() and
        /// OnRemovedFromScene() to ensure the scene is always
        /// aware of the current camera.
        ///
        /// If a second camera is set while another is active
        /// this will assert.
        ///
        /// @author Ian Copland
        ///
        /// @param in_cameraComponent - The camera component
        /// that should become active. Null should be passed to
        /// deactivate a camera.
        //------------------------------------------------------
        void SetActiveCamera(CameraComponent* in_cameraComponent) noexcept;
        //------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The active camera in this scene, or null if
        /// there isn't one.
        //------------------------------------------------------
        CameraComponent* GetActiveCamera() const noexcept { return m_activeCameraComponent; }
        //------------------------------------------------------
        /// @return The render target, if nullptr then renders to screen
        //------------------------------------------------------
        TargetGroup* GetRenderTarget() const noexcept { return m_renderTarget.get(); }
        //-------------------------------------------------------
        /// Sends the resume event on to the entities.
        ///
        /// @author S Downie
        //-------------------------------------------------------
        void ResumeEntities();
        //-------------------------------------------------------
        /// Sends the foreground event on to the entities.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------
        void ForegroundEntities();
        //-------------------------------------------------------
        /// Updates all entities.
        ///
        /// @author Ian Copland
        ///
        /// @param Time since last update in seconds.
        //-------------------------------------------------------
        void UpdateEntities(f32 in_timeSinceLastUpdate);
        //-------------------------------------------------------
        /// Fixed updates all entities.
        ///
        /// @author Ian Copland
        ///
        /// @param Time since last update in seconds.
        //-------------------------------------------------------
        void FixedUpdateEntities(f32 in_timeSinceLastUpdate);
        //-------------------------------------------------------
        /// Sends the render snapshot event to all entities in
        /// the scene.
        ///
        /// @author Ian Copland
        ///
        /// @param renderSnapshot - The render snapshot object
        /// which contains all snapshotted data.
        /// @param frameAllocator - Allocate any render frame data
        /// from here
        //-------------------------------------------------------
        void RenderSnapshotEntities(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept;
        //-------------------------------------------------------
        /// Sends the background event on to the entities.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------
        void BackgroundEntities();
        //-------------------------------------------------------
        /// Sends the suspend event on to the entities.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------
        void SuspendEntities();
        
        /// Takes a snapshot of the scene and renders it to its
        /// render target during the render stage of the application.
        ///
        /// Should only be called directly by app if performing single shot RTT on a disabled scene
        ///
        /// NOTE: The texture of the target will not be populated until next frame.
        ///
        /// @param scene
        ///     Scene that should be rendered
        /// @param target
        ///     Optional, if wanting to render the scene to a target other than its own
        ///
        void Render(TargetGroup* target = nullptr) noexcept;
        
        //--------------------------------------------------------------------------------------------------
        /// Traverses the contents of the scene and adds any objects that intersect with the ray to the
        /// list. The list order is undefined. Use the query intersection value on the volume component
        /// to sort by depth
        ///
        /// @author S Downie
        ///
        /// @param Ray to check intersection
        /// @param [Out] Container to fill with intersecting components
        //--------------------------------------------------------------------------------------------------
        void QuerySceneForIntersection(const Ray &in_ray, std::vector<VolumeComponent*>& out_volumeComponents);
        //--------------------------------------------------------------------------------------------------
        /// Traverse the scene for the given component type and fill the list with those components
        ///
        /// @author S Downie
        ///
        /// @param [Out] Empty container to be filled
        //--------------------------------------------------------------------------------------------------
        template <typename TComponentType>
        void QuerySceneForComponents(std::vector<TComponentType*>& out_components)
        {
            for(SharedEntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                (*it)->GetComponents<TComponentType>(out_components);
            }
        }
        //--------------------------------------------------------------------------------------------------
        /// Traverse the scene for the given component types and fill the lists with those components
        //
        /// @author S Downie
        ///
        /// @param [Out] Empty container to be filled with T1
        /// @param [Out] Empty container to be filled with T2
        //--------------------------------------------------------------------------------------------------
        template <typename TComponentType1, typename TComponentType2>
        void QuerySceneForComponents(std::vector<TComponentType1*>& out_components1, std::vector<TComponentType2*>& out_components2)
        {
            for(SharedEntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                (*it)->GetComponents<TComponentType1, TComponentType2>(out_components1, out_components2);
            }
        }
        //--------------------------------------------------------------------------------------------------
        /// Traverse the scene for the given component types and fill the lists with those components
        ///
        /// @author S Downie
        ///
        /// @param [Out] Empty container to be filled with T1
        /// @param [Out] Empty container to be filled with T2
        /// @param [Out] Empty container to be filled with T3
        //--------------------------------------------------------------------------------------------------
        template <typename TComponentType1, typename TComponentType2, typename TComponentType3>
        void QuerySceneForComponents(std::vector<TComponentType1*>& out_components1, std::vector<TComponentType2*>& out_components2, std::vector<TComponentType3*>& out_components3)
        {
            for(SharedEntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                (*it)->GetComponents<TComponentType1, TComponentType2, TComponentType3>(out_components1, out_components2, out_components3);
            }
        }
        
    private:
        friend class Entity;
        
        //-------------------------------------------------------
        /// Private to enforce use of factory method
        ///
        /// @author S Downie
        ///
        /// @param renderTarget
        ///     The target into which the scene renders, if null renders to screen
        //-------------------------------------------------------
        Scene(TargetGroupUPtr renderTarget) noexcept;
        //-------------------------------------------------------
        /// Remove the entity from the scene
        ///
        /// @author S Downie
        ///
        /// @param Entity
        //-------------------------------------------------------
        void Remove(Entity* inpEntity);
        
        //------------------------------------------------
        /// Called when the owning state is being destroyed.
        /// Used to release held objects
        ///
        /// @author S Downie
        //------------------------------------------------
        void OnDestroy() noexcept override;
        
    private:
        
        SharedEntityList m_entities;
        Colour m_clearColour;
        bool m_entitiesActive = false;
        bool m_entitiesForegrounded = false;
        bool m_enabled = true;
        CameraComponent* m_activeCameraComponent = nullptr;
        TargetGroupUPtr m_renderTarget;
    };		
}

#endif
