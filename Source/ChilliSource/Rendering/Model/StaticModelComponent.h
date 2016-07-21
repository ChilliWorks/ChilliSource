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

#ifndef _CHILLISOURCE_RENDERING_MODEL_STATICMODELCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_MODEL_STATICMODELCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Volume/VolumeComponent.h>
#include <ChilliSource/Rendering/Model/Model.h>

namespace ChilliSource
{
    /// A static model component. This defines a 3D model that can be manipulated, textured
    /// but not animated.
    ///
    /// This is not thread safe and should not be used on multiple threads at once.
    ///
    class StaticModelComponent final : public VolumeComponent
    {
    public:
        CS_DECLARE_NAMEDTYPE(StaticModelComponent);
        
        /// Creates a new static model component with the given model and material. The
        /// material will be applied to all meshes in the model.
        ///
        /// @param model
        ///     The model that should be used.
        /// @param material
        ///     The material that should be applied to each mesh in the model.
        ///
        StaticModelComponent(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept;
        
        /// Creates a new static model component with the given model and a list of materials,
        /// one for each mesh in the model. If the number of materials and meshes is not the
        /// same, this will assert.
        ///
        /// @param model
        ///     The model that should be used.
        /// @param materials
        ///     The list of materials for each mesh in the model.
        ///
        StaticModelComponent(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials) noexcept;

        /// Allows querying of whether or not this component implements the interface described
        /// by the given interface Id. Typically this is not called directly as the templated
        /// equivalent IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;

        /// Calculates and returns the world space AABB of the model. This is cached and only
        /// recomputed when required.
        ///
        /// @return The world space axis-aligned bounding box
        ///
        const AABB& GetAABB() noexcept override;
        
        /// Calculates and returns the world space OOBB of the model. This is cached and only
        /// recomputed when required.
        ///
        /// @return The world space OOBB
        ///
        const OOBB& GetOOBB() noexcept override;
        
        /// Calculates and returns the world space bounding sphere of the model. This is cached
        /// and only recomputed when required.
        ///
        /// @return The world space bounding sphere
        ///
        const Sphere& GetBoundingSphere() noexcept override;
        
        /// @return Whether or not the object should be rendered.
        ///
        bool IsVisible() const noexcept override { return m_isVisible; }
        
        /// @param isVisible
        ///     Whether or not the object should be rendered.
        ///
        void SetVisible(bool isVisible) noexcept { m_isVisible = isVisible; }
        
        /// @return The model that will be rendered.
        ///
        const ModelCSPtr& GetModel() const noexcept;
        
        /// Sets the model the component should use.
        ///
        /// The material currently applied to the first mesh will be applied to the entire model.
        ///
        /// @param model
        ///     The model.
        ///
        void SetModel(const ModelCSPtr& model) noexcept;
        
        /// Sets the model the component should use. The given material will be applied to all meshes.
        ///
        /// @param model
        ///     The model which should be used.
        /// @param material
        ///     The material which should be used.
        ///
        void SetModel(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept;
        
        /// Sets the model the component should use. The given materials will be applied to the
        /// respective meshes.
        ///
        /// @param model
        ///     The model which should be used.
        /// @param materials
        ///     The material for each mesh in the model. Must have the same number of elements as
        ///     there are meshes in the given model.
        ///
        void SetModel(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials) noexcept;
        
        /// Get the material of a single mesh.
        ///
        /// @param meshIndex
        ///     Index to the mesh
        ///
        /// @return Handle to material
        ///
        const MaterialCSPtr& GetMaterialForMesh(u32 meshIndex) const noexcept;
        
        /// Get the material of a single mesh.
        ///
        /// @param meshName
        ///     The name of the mesh.
        ///
        /// @return Handle to material
        ///
        const MaterialCSPtr& GetMaterialForMesh(const std::string& meshName) const noexcept;
        
        /// Set the material that the model will use. Applies the material to all meshes.
        ///
        /// @param material
        ///     The material that should be used.
        ///
        void SetMaterial(const MaterialCSPtr& material) noexcept;
        
        /// Sets the material for a single mesh.
        ///
        /// @param material
        ///     The material that should be used.
        /// @param meshIndex
        ///     The index of the mesh.
        ///
        void SetMaterialForMesh(const MaterialCSPtr& material, u32 meshIndex) noexcept;
        
        /// Sets the material for a single mesh.
        ///
        /// @param material
        ///     The material that should be used.
        /// @param meshName
        ///     The name of the mesh
        ///
        void SetMaterialForMesh(const MaterialCSPtr& material, const std::string& meshName) noexcept;
        
        /// @return Whether the render component casts shadows
        ///
        bool IsShadowCastingEnabled() const noexcept;
        
        /// @param enabled
        ///     Whether the render component casts shadows
        ///
        void SetShadowCastingEnabled(bool enabled) noexcept;
        
    private:
        /// Triggered when the component is attached to an entity on the scene
        ///
        void OnAddedToScene() noexcept override;
        
        /// Delegate called when the owning entities transform changes. This is used to dirty
        /// the bounding volumes
        ///
        void OnEntityTransformChanged() noexcept;
        
        /// Called during the render snapshot phase. Adds render objects to the scene
        /// describing the model.
        ///
        /// @param renderSnapshot
        ///     The render snapshot.
        /// @param frameAllocator
        ///     Allocate any memory required for rendering stuff from here
        ///
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        /// Triggered when the component is removed from an entity on the scene.
        ///
        void OnRemovedFromScene() noexcept override;
        
        ModelCSPtr m_model;
        std::vector<MaterialCSPtr> m_materials;
        AABB m_aabb;
        OOBB m_oobb;
        Sphere m_boundingSphere;
        bool m_shadowCastingEnabled = true;
        bool m_isVisible = true;
        
        bool m_isAABBValid = false;
        bool m_isOOBBValid = false;
        bool m_isBoundingSphereValid = false;
        EventConnectionUPtr m_transformChangedConnection;
    };
}

#endif
