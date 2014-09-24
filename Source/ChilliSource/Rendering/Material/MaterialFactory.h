//
//  MaterialFactory.h
//  Chilli Source
//  Created by Scott Downie on 5/08/2013.
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
    namespace Rendering
    {
        //-------------------------------------------------------------------------
        /// Factory for creating material resources based on rendering type
        ///
        /// @author S Downie
        //-------------------------------------------------------------------------
        class MaterialFactory final : public Core::AppSystem
        {
        public:
            
            CS_DECLARE_NAMEDTYPE(MaterialFactory);
            
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
            static MaterialFactoryUPtr Create(RenderCapabilities* in_renderCapabilities);
            //---------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID
            ///
            /// @return Whether the object is of the same interface
            //---------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------
            /// Create a material with no shader programs.
            /// The shader programs must be specified by the
            /// user before using the material
            ///
            /// @author S Downie
            ///
            /// @param Unique Id to identify in resource pool
            /// "_" prefix is reserved for engine resources
            ///
            /// @param Empty new material
            //---------------------------------------------------
            MaterialSPtr CreateCustom(const std::string& in_uniqueId) const;
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
            
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Render capabilities used to determine which
            /// materials can be used
            //-------------------------------------------------------
            MaterialFactory(RenderCapabilities* in_renderCapabilities);
            
        private:
            
            RenderCapabilities* m_renderCapabilities;
        };
    }
}

#endif 
