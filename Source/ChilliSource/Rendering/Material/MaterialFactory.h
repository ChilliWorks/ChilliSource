//
//  MaterialFactory.h
//  MoFlow
//
//  Created by Scott Downie on 05/08/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#ifndef MOFLOW_RENDERING_MATERIALFACTORY_H
#define MOFLOW_RENDERING_MATERIALFACTORY_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>

namespace ChilliSource
{
    namespace Rendering
    {
        class MaterialFactory : public Core::System
        {
        public:
            
            CS_DECLARE_NAMEDTYPE(MaterialFactory);
            
            //---------------------------------------------------
            /// Constructor
            //---------------------------------------------------
            MaterialFactory();
            //---------------------------------------------------
            /// Is A
            ///
            /// @param Interface ID
            /// @return Whether the object is of the same interface
            //---------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //---------------------------------------------------
            /// Create Custom
            ///
            /// Create a material with no shader programs.
            /// The shader programs must be specified by the
            /// user before using the material
            ///
            /// @param Empty new material
            //---------------------------------------------------
            MaterialSPtr CreateCustom() const;
            //---------------------------------------------------
            /// Create GUI
            ///
            /// Create a material to render UI
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateGUI() const;
            //---------------------------------------------------
            /// Create GUI Distance Font
            ///
            /// Create a material to render UI distance fonts
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateGUIDistanceFont() const;
            //---------------------------------------------------
            /// Create GUI Distance Font Outlined
            ///
            /// Create a material to render UI distance fonts with
            /// outlines
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateGUIDistanceFontOutlined() const;
            //---------------------------------------------------
            /// Create Sprite
            ///
            /// Create a material for rendering sprites. Sprites
            /// have no lighting and vertex colours
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateSprite(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static
            ///
            /// Creates a new material for static textured models
            /// that use no lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateStatic(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static
            ///
            /// Creates a new material for static textured models
            /// that use Ambient lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateStaticAmbient(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static Blinn
            ///
            /// Creates a new material for static textured models
            /// that use blinn-phong per pixel lighting.
            ///
            /// @param Texture file location
            /// @param Texture file path
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateStaticBlinn(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static Blinn Shadowed
            ///
            /// Creates a new material for static textured models
            /// that use blinn-phong per pixel lighting and recieved
            /// shadows.
            ///
            /// @param Texture file location
            /// @param Texture file path
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateStaticBlinnShadowed(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static Blinn Per Vertex
            ///
            /// Creates a new material for static textured models
            /// that use blinn-phong per vertex lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateStaticBlinnPerVertex(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static Blinn Per Vertex Shadowed
            ///
            /// Creates a new material for static textured models
            /// that use blinn-phong per vertex lighting and
            /// received shadows.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateStaticBlinnPerVertexShadowed(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static Directional Shadow Map
            ///
            /// Create a material for rendering a static model
            /// to a directional shadow map.
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateStaticDirectionalShadowMap() const;
            //---------------------------------------------------
            /// Create Animated
            ///
            /// Creates a new material for animated textured models
            /// that use no lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateAnimated(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Ambient
            ///
            /// Creates a new material for animated textured models
            /// that use ambient lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateAnimatedAmbient(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Blinn
            ///
            /// Creates a new material for animated textured models
            /// that use blinn-phong per pixel lighting.
            ///
            /// @param Texture file location
            /// @param Texture file path
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateAnimatedBlinn(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Blinn Shadowed
            ///
            /// Creates a new material for animated textured models
            /// that use blinn-phong per pixel lighting and receives
            /// shadows.
            ///
            /// @param Texture file location
            /// @param Texture file path
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateAnimatedBlinnShadowed(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Blinn Per Vertex
            ///
            /// Creates a new material for animated textured models
            /// that use blinn-phong per vertex lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateAnimatedBlinnPerVertex(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Blinn Per Vertex Shadowed
            ///
            /// Creates a new material for animated textured models
            /// that use blinn-phong per vertex lighting and
            /// recieves shadows.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateAnimatedBlinnPerVertexShadowed(const TextureSPtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Directional Shadow Map
            ///
            /// Create a material for rendering an animated model
            /// to a directional shadow map.
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialSPtr CreateAnimatedDirectionalShadowMap() const;
            
        private:
            
            //---------------------------------------------------
            /// Get Texture Manager
            ///
            /// @return Texture manager
            //---------------------------------------------------
            TextureManager* GetTextureManager() const;
            //---------------------------------------------------
            /// Get Shader Manager
            ///
            /// @return Shader manager
            //---------------------------------------------------
            ShaderManager* GetShaderManager() const;
            //---------------------------------------------------
            /// Get Cubemap Manager
            ///
            /// @return Cubemap manager
            //---------------------------------------------------
            CubemapManager* GetCubemapManager() const;
            //---------------------------------------------------
            /// Get Render Capabilities
            ///
            /// @return The render capabilities.
            //---------------------------------------------------
            RenderCapabilities* GetRenderCapabilities() const;
            
        private:
            
            mutable TextureManager* mpTextureManager;
            mutable ShaderManager* mpShaderManager;
            mutable CubemapManager* mpCubemapManager;
            mutable RenderCapabilities* mpRenderCapabilities;
        };
    }
}

#endif 
