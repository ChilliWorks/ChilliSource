//
//  MaterialFactory.h
//  Chilli Source
//
//  Created by Scott Downie on 05/08/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
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
            /// @param Shader manager used to load material shaders
            /// @param Render capabilities used to determine which
            /// materials can be used
            ///
            /// @return New instance with ownership transferred
            //-------------------------------------------------------
            static MaterialFactoryUPtr Create(ShaderManager* in_shaderMgr, RenderCapabilities* in_renderCapabilities);
            //---------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID
            /// @return Whether the object is of the same interface
            //---------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceID) const override;
            //---------------------------------------------------
            /// Create a material with no shader programs.
            /// The shader programs must be specified by the
            /// user before using the material
            ///
            /// @author S Downie
            ///
            /// @param Empty new material
            //---------------------------------------------------
            MaterialUPtr CreateCustom() const;
            //---------------------------------------------------
            /// Create a material to render UI
            ///
            /// @author S Downie
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateGUI() const;
            //---------------------------------------------------
            /// Create a material to render UI distance fonts
            ///
            /// @author S Downie
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateGUIDistanceFont() const;
            //---------------------------------------------------
            /// Create a material to render UI distance fonts with
            /// outlines
            ///
            /// @author S Downie
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateGUIDistanceFontOutlined() const;
            //---------------------------------------------------
            /// Create a material for rendering sprites. Sprites
            /// have no lighting and vertex colours
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateSprite(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for static textured models
            /// that use no lighting.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateStatic(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for static textured models
            /// that use Ambient lighting.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateStaticAmbient(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for static textured models
            /// that use blinn-phong per pixel lighting.
            ///
            /// @author S Downie
            ///
            /// @param Texture file location
            /// @param Texture file path
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateStaticBlinn(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for static textured models
            /// that use blinn-phong per pixel lighting and recieved
            /// shadows.
            ///
            /// @author S Downie
            ///
            /// @param Texture file location
            /// @param Texture file path
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateStaticBlinnShadowed(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for static textured models
            /// that use blinn-phong per vertex lighting.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateStaticBlinnPerVertex(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for static textured models
            /// that use blinn-phong per vertex lighting and
            /// received shadows.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateStaticBlinnPerVertexShadowed(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Create a material for rendering a static model
            /// to a directional shadow map.
            ///
            /// @author S Downie
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateStaticDirectionalShadowMap() const;
            //---------------------------------------------------
            /// Creates a new material for animated textured models
            /// that use no lighting.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateAnimated(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for animated textured models
            /// that use ambient lighting.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateAnimatedAmbient(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for animated textured models
            /// that use blinn-phong per pixel lighting.
            ///
            /// @author S Downie
            ///
            /// @param Texture file location
            /// @param Texture file path
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateAnimatedBlinn(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for animated textured models
            /// that use blinn-phong per pixel lighting and receives
            /// shadows.
            ///
            /// @author S Downie
            ///
            /// @param Texture file location
            /// @param Texture file path
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateAnimatedBlinnShadowed(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for animated textured models
            /// that use blinn-phong per vertex lighting.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateAnimatedBlinnPerVertex(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Creates a new material for animated textured models
            /// that use blinn-phong per vertex lighting and
            /// recieves shadows.
            ///
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateAnimatedBlinnPerVertexShadowed(const TextureSPtr& in_texture) const;
            //---------------------------------------------------
            /// Create a material for rendering an animated model
            /// to a directional shadow map.
            ///
            /// @author S Downie
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialUPtr CreateAnimatedDirectionalShadowMap() const;
            
        private:
            
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Shader manager used to load material shaders
            /// @param Render capabilities used to determine which
            /// materials can be used
            //-------------------------------------------------------
            MaterialFactory(ShaderManager* in_shaderMgr, RenderCapabilities* in_renderCapabilities);
            
        private:
            
            ShaderManager* m_shaderManager;
            RenderCapabilities* m_renderCapabilities;
        };
    }
}

#endif 
