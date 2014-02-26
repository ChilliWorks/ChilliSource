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
        class MaterialFactory : public Core::ISystem
        {
        public:
            
            DECLARE_NAMED_INTERFACE(MaterialFactory);
            
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
            bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //---------------------------------------------------
            /// Create Custom
            ///
            /// Create a material with no shader programs.
            /// The shader programs must be specified by the
            /// user before using the material
            ///
            /// @param Empty new material
            //---------------------------------------------------
            MaterialPtr CreateCustom() const;
            //---------------------------------------------------
            /// Create GUI
            ///
            /// Create a material to render UI
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateGUI() const;
            //---------------------------------------------------
            /// Create GUI Distance Font
            ///
            /// Create a material to render UI distance fonts
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateGUIDistanceFont() const;
            //---------------------------------------------------
            /// Create GUI Distance Font Outlined
            ///
            /// Create a material to render UI distance fonts with
            /// outlines
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateGUIDistanceFontOutlined() const;
            //---------------------------------------------------
            /// Create Sprite
            ///
            /// Create a material for rendering sprites. Sprites
            /// have no lighting and vertex colours
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateSprite(const TexturePtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static
            ///
            /// Creates a new material for static textured models
            /// that use no lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateStatic(const TexturePtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static
            ///
            /// Creates a new material for static textured models
            /// that use Ambient lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateStaticAmbient(const TexturePtr& inpTex) const;
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
            MaterialPtr CreateStaticBlinn(const TexturePtr& inpTex) const;
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
            MaterialPtr CreateStaticBlinnShadowed(const TexturePtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static Blinn Per Vertex
            ///
            /// Creates a new material for static textured models
            /// that use blinn-phong per vertex lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateStaticBlinnPerVertex(const TexturePtr& inpTex) const;
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
            MaterialPtr CreateStaticBlinnPerVertexShadowed(const TexturePtr& inpTex) const;
            //---------------------------------------------------
            /// Create Static Directional Shadow Map
            ///
            /// Create a material for rendering a static model
            /// to a directional shadow map.
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateStaticDirectionalShadowMap() const;
            //---------------------------------------------------
            /// Create Animated
            ///
            /// Creates a new material for animated textured models
            /// that use no lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateAnimated(const TexturePtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Ambient
            ///
            /// Creates a new material for animated textured models
            /// that use ambient lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateAnimatedAmbient(const TexturePtr& inpTex) const;
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
            MaterialPtr CreateAnimatedBlinn(const TexturePtr& inpTex) const;
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
            MaterialPtr CreateAnimatedBlinnShadowed(const TexturePtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Blinn Per Vertex
            ///
            /// Creates a new material for animated textured models
            /// that use blinn-phong per vertex lighting.
            ///
            /// @param Texture
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateAnimatedBlinnPerVertex(const TexturePtr& inpTex) const;
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
            MaterialPtr CreateAnimatedBlinnPerVertexShadowed(const TexturePtr& inpTex) const;
            //---------------------------------------------------
            /// Create Animated Directional Shadow Map
            ///
            /// Create a material for rendering an animated model
            /// to a directional shadow map.
            ///
            /// @return New material
            //---------------------------------------------------
            MaterialPtr CreateAnimatedDirectionalShadowMap() const;
            
        private:
            
            //---------------------------------------------------
            /// Get Texture Manager
            ///
            /// @return Texture manager
            //---------------------------------------------------
            ITextureManager* GetTextureManager() const;
            //---------------------------------------------------
            /// Get Shader Manager
            ///
            /// @return Shader manager
            //---------------------------------------------------
            IShaderManager* GetShaderManager() const;
            //---------------------------------------------------
            /// Get Cubemap Manager
            ///
            /// @return Cubemap manager
            //---------------------------------------------------
            ICubemapManager* GetCubemapManager() const;
            //---------------------------------------------------
            /// Get Render Capabilities
            ///
            /// @return The render capabilities.
            //---------------------------------------------------
            RenderCapabilities* GetRenderCapabilities() const;
            
        private:
            
            mutable ITextureManager* mpTextureManager;
            mutable IShaderManager* mpShaderManager;
            mutable ICubemapManager* mpCubemapManager;
            mutable RenderCapabilities* mpRenderCapabilities;
        };
    }
}

#endif 
