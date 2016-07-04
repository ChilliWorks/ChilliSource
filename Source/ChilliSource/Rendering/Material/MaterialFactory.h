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
        
        /// Creates a new material with unlit materials that can be used with static meshes, animated
        /// meshes and sprites.
        ///
        /// @param uniqueId
        ///     The unique Id that will be used to identify the material in the resource pool. Note
        ///     that Id's which are prefixed with an underscore (_) are reserved from engine use.
        /// @param texture
        ///     The texture that the material will use.
        ///
        MaterialCSPtr CreateUnlit(const std::string& uniqueId, const TextureCSPtr& texture) const noexcept;
        
        /// Creates a new material with unlit materials that can be used with static meshes and animated
        /// meshes.
        ///
        /// @param uniqueId
        ///     The unique Id that will be used to identify the material in the resource pool. Note
        ///     that Id's which are prefixed with an underscore (_) are reserved from engine use.
        /// @param texture
        ///     The texture that the material will use.
        ///
        MaterialCSPtr CreateBlinn(const std::string& uniqueId, const TextureCSPtr& texture) const noexcept;
        
        //---------------------------------------------------
        /// Create a material to render UI
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateGUI(const std::string& in_uniqueId) const;
        //---------------------------------------------------
        /// Create a material for rendering sprites. Sprites
        /// have no lighting and vertex colours
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateSprite(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for static textured models
        /// that use no lighting.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateStatic(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for static textured models
        /// that use Ambient lighting.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateStaticAmbient(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for static textured models
        /// that use blinn-phong per pixel lighting.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateStaticBlinn(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for static textured models
        /// that use blinn-phong per pixel lighting and received
        /// shadows.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateStaticBlinnShadowed(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for static textured models
        /// that use blinn-phong per vertex lighting.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateStaticBlinnPerVertex(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for static textured models
        /// that use blinn-phong per vertex lighting and
        /// received shadows.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateStaticBlinnPerVertexShadowed(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Create a material for rendering a static model
        /// to a directional shadow map.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateStaticDirectionalShadowMap(const std::string& in_uniqueId) const;
        //---------------------------------------------------
        /// Creates a new material for animated textured models
        /// that use no lighting.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateAnimated(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for animated textured models
        /// that use ambient lighting.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateAnimatedAmbient(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for animated textured models
        /// that use blinn-phong per pixel lighting.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateAnimatedBlinn(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for animated textured models
        /// that use blinn-phong per pixel lighting and receives
        /// shadows.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateAnimatedBlinnShadowed(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for animated textured models
        /// that use blinn-phong per vertex lighting.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateAnimatedBlinnPerVertex(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Creates a new material for animated textured models
        /// that use blinn-phong per vertex lighting and
        /// recieves shadows.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        /// @param Texture
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateAnimatedBlinnPerVertexShadowed(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const;
        //---------------------------------------------------
        /// Create a material for rendering an animated model
        /// to a directional shadow map.
        ///
        /// @author S Downie
        ///
        /// @param Unique Id to identify in resource pool
        /// "_" prefix is reserved for engine resources
        ///
        /// @return New material
        //---------------------------------------------------
        MaterialSPtr CreateAnimatedDirectionalShadowMap(const std::string& in_uniqueId) const;
        
    private:
        friend class Application;
        //-------------------------------------------------------
        /// Factory method
        ///
        /// @author S Downie
        ///
        /// @param Render capabilities used to determine which
        /// materials can be used
        ///
        /// @return New instance with ownership transferred
        //-------------------------------------------------------
        static MaterialFactoryUPtr Create();
        //-------------------------------------------------------
        /// Private constructor to force use of factory method
        ///
        /// @author S Downie
        //-------------------------------------------------------
        MaterialFactory() = default;
    };
}

#endif 
