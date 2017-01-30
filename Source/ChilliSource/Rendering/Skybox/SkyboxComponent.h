//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_SKYBOX_SKYBOXCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_SKYBOX_SKYBOXCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Rendering/Model/Model.h>

namespace ChilliSource
{
    /// A static skybox component. A 3D model that is rendered behind the rest of the scene
    /// and infinetely far away.
    ///
    /// NOTE: "Skybox" is a convention only the model can be a sphere or dome, etc.
    /// NOTE: Ignores any transformations applied to the entity
    /// NOTE: Model must consist of a single mesh
    ///
    /// This is not thread safe and should not be used on multiple threads at once.
    ///
    class SkyboxComponent final : public Component
    {
    public:
        CS_DECLARE_NAMEDTYPE(SkyboxComponent);
        
        /// Creates a new skybox component with the given model and material. The
        /// material will be applied to all meshes in the model.
        ///
        /// @param model
        ///     The model that should be used.
        /// @param material
        ///     The material that should be applied to each mesh in the model.
        ///
        SkyboxComponent(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept;

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
        
        /// Get the material.
        ///
        /// @return Handle to material
        ///
        const MaterialCSPtr& GetMaterial() const noexcept;
        
        /// Set the material that the model will use. Applies the material to all meshes.
        ///
        /// @param material
        ///     The material that should be used.
        ///
        void SetMaterial(const MaterialCSPtr& material) noexcept;
        
    private:
        
        /// Called during the render snapshot phase. Adds render objects to the scene
        /// describing the model.
        ///
        /// @param renderSnapshot
        ///     The render snapshot.
        /// @param frameAllocator
        ///     Allocate any memory required for rendering stuff from here
        ///
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        ModelCSPtr m_model;
        MaterialCSPtr m_material;
    };
}

#endif
