//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_MATERIALFACTORY_H
#define _CHILLISOURCE_RENDERING_MATERIAL_MATERIALFACTORY_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
    /// A convenience factory for creating new materials of the requested types.
    ///
    /// This is not thread-safe and must be called from the main thread.
    ///
    class MaterialFactory final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(MaterialFactory);

        /// Allows querying of whether or not this system implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// Creates a new material with unlit shaders that can be used with static, animated and
        /// sprites.
        ///
        /// @param uniqueId
        ///     The unique Id that will be used to identify the material in the resource pool. Note
        ///     that Id's which are prefixed with an underscore (_) are reserved from engine use.
        /// @param texture
        ///     The texture that the material will use.
        /// @param isTransparent
        ///     Whether or not transparency is enabled for the material.
        /// @param colour
        ///     (Optional) The colour of the material. Defaults to white.
        ///
        /// @return The newly created material.
        ///
        MaterialCSPtr CreateUnlit(const std::string& uniqueId, const TextureCSPtr& texture, bool isTransparent, const Colour& colour = Colour::k_white) const noexcept;
        
        /// Creates a new material with skybox shaders that can be used with static models
        /// in order to render a skybox behind all other objects in the scene and infinetely far away.
        ///
        /// @param uniqueId
        ///     The unique Id that will be used to identify the material in the resource pool. Note
        ///     that Id's which are prefixed with an underscore (_) are reserved from engine use.
        /// @param cubemap
        ///     The cubemap that the material will use.
        ///
        /// @return The newly created material.
        ///
        MaterialCSPtr CreateSkybox(const std::string& uniqueId, const CubemapCSPtr& cubemap) const noexcept;
        
        /// Creates a new material with blinn shaders that can be used with static and animated meshes.
        ///
        /// @param uniqueId
        ///     The unique Id that will be used to identify the material in the resource pool. Note
        ///     that Id's which are prefixed with an underscore (_) are reserved from engine use.
        /// @param texture
        ///     The texture that the material will use.
        /// @param ambientColour
        ///     The ambient colour.
        /// @param emissiveColour
        ///     The ambient colour.
        /// @param diffuseColour
        ///     The diffuse colour.
        /// @param specularColour
        ///     The specular colour, note that the alpha value is ignored.
        /// @param shininess
        ///     The specular shininess of the material.
        ///
        /// @return The newly created material.
        ///
        MaterialCSPtr CreateBlinn(const std::string& uniqueId, const TextureCSPtr& texture, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour,
                                  const Colour& specularColour, f32 shininess) const noexcept;
        
        /// Creates a new custom material, on which all shader settings can be changed. The shading
        /// type defaults to Unlit, but other types can be set. The types of mesh that the material
        /// can be used with depends on the shading type set.
        ///
        /// The material must have it's load state set manually to loaded before it can be used.
        ///
        /// @param uniqueId
        ///     The unique Id that will be used to identify the material in the resource pool. Note
        ///     that Id's which are prefixed with an underscore (_) are reserved from engine use.
        ///
        /// @return The new empty, mutable material.
        ///
        MaterialSPtr CreateCustom(const std::string& uniqueId) const noexcept;
        
    private:
        friend class Application;
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static MaterialFactoryUPtr Create() noexcept;
        
        MaterialFactory() = default;
    };
}

#endif 
