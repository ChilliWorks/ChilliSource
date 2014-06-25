//
//  SpriteComponent.h
//  Chilli Source
//  Created by Scott Downie on 29/09/2010.
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

#ifndef _CHILLISOURCE_RENDERING_SPRITE_SPRITECOMPONENT_H_
#define _CHILLISOURCE_RENDERING_SPRITE_SPRITECOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Rendering/Sprite/SpriteBatch.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

#include <functional>

namespace ChilliSource
{
	namespace Rendering
	{
		//---------------------------------------------------------------
        /// A render component that draws a 2D textured quad. Sprites
        /// are rendered using a "Sprite" material and can be coloured,
        /// aligned and have thier UVs changed manually or via a texture
        /// atlas.
        ///
        /// @author S Downie
		//---------------------------------------------------------------
		class SpriteComponent final : public RenderComponent
		{
		public: 
			
			CS_DECLARE_NAMEDTYPE(SpriteComponent);
            //----------------------------------------------------------------------------------------
            /// Identifiers for functions that maintain the aspect ratio of the sprite based on
            /// current size and preferred image size
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class SizePolicy
            {
                k_none,
                k_usePreferredSize,
                k_useWidthMaintainingAspect,
                k_useHeightMaintainingAspect,
                k_fitMaintainingAspect,
                k_fillMaintainingAspect,
                k_totalNum
            };
            //----------------------------------------------------------------------------------------
            /// Delegate for size policy functions.
            ///
            /// @author S Downie
            ///
            /// @param Original size
            /// @param Preferred size
            ///
            /// @return New size with function applied
            //----------------------------------------------------------------------------------------
            using SizePolicyDelegate = std::function<Core::Vector2(const Core::Vector2&, const Core::Vector2&)>;
            //----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //----------------------------------------------------------
			SpriteComponent();
			//----------------------------------------------------------
			/// @author S Downie
            ///
			/// @param Comparison Type
            ///
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------
            /// The axis aligned bounding box is positioned in
            /// world space but the orientation is aligned to
            /// the x-y plane. This is cached and recomputed when
            /// tranform is changed.
            ///
            /// @author S Downie
            ///
			/// @return AABB in world space
			//----------------------------------------------------
            const Core::AABB& GetAABB() override;
			//----------------------------------------------------
            /// The object oriented bounding box complete with transform to
            /// transform from local space to world space. This is cached
            /// and recomputed when tranform is changed.
            ///
            /// @author S Downie
            ///
			/// @return OOBB in local space with world transform
			//----------------------------------------------------
			const Core::OOBB& GetOOBB() override;
			//----------------------------------------------------
			/// All render objects have an bounding sphere for
			/// culling. This is cached and recomputed when tranform
            /// is changed. The bounding sphere encompasses
            /// the diagonal axis of the sprite
            ///
            /// @author S Downie
            ///
			/// @return world space bounding sphere
			//----------------------------------------------------
			const Core::Sphere& GetBoundingSphere() override;
			//-----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Vector containing width and height of sprite in
            /// local space
			//-----------------------------------------------------------
			void SetSize(const Core::Vector2& in_size);
			//-----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Width in local space
			/// @param Height in local space
			//-----------------------------------------------------------
			void SetSize(f32 in_width, f32 in_height);
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Size policy that governs how the set size and
            /// the image size are accommodated. Usually by maintaining
            /// the aspect ratio of the image.
            //-----------------------------------------------------------
            void SetSizePolicy(SizePolicy in_sizePolicy);
			//-----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Size after the size policy has been applied
			//-----------------------------------------------------------
            Core::Vector2 GetSize() const;
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Texture atlas
            //-----------------------------------------------------------
            void SetTextureAtlas(const TextureAtlasCSPtr& in_atlas);
            //-----------------------------------------------------------
            /// Set the id of the frame that will be used to texture
            /// the sprite. This will supercede and UVs that have been
            /// set on the sprite
            ///
            /// @author S Downie
            ///
            /// @param Texture atlas id
            //-----------------------------------------------------------
            void SetTextureAtlasId(const std::string& in_atlasId);
			//-----------------------------------------------------------
			/// Set the texture co-ordinates
            ///
            /// @author S Downie
			///
			/// @param Rect containing uv, st
			//-----------------------------------------------------------
			void SetUVs(const Rendering::UVs& in_uvs);
			//-----------------------------------------------------------
			/// Set the UV texture coordinates
            ///
            /// @author S Downie
			///
			/// @param U
			/// @param V
			/// @param S
			/// @param T
			//-----------------------------------------------------------
			void SetUVs(f32 in_u, f32 in_v, f32 in_s, f32 in_t);
			//-----------------------------------------------------------
			/// Set the RGBA colour of the sprite. This is independent
            /// of the material colour and is applied on a per sprite
            /// basis
            ///
            /// @author S Downie
			///
			/// @param Colour containing RGBA
			//-----------------------------------------------------------
			void SetColour(const Core::Colour& in_colour);
			//-----------------------------------------------------------
			/// Set the RGBA colour of the sprite. This is independent
            /// of the material colour and is applied on a per sprite
            /// basis
            ///
            /// @author S Downie
			///
			/// @param Red
			/// @param Green
			/// @param Blue
			/// @param Alpha
			//-----------------------------------------------------------
			void SetColour(f32 in_r, f32 in_g, f32 in_b, f32 in_a);
			//-----------------------------------------------------------
			/// Get Colour
			///
			/// @return Sprite colour
			//-----------------------------------------------------------
			const Core::Colour& GetColour() const;
			//-----------------------------------------------------------
			/// Flip the sprite UVs about it's local x-axis
            ///
            /// @author S Downie
			///
			/// @param Whether to flip or not
			//-----------------------------------------------------------
			void SetFlippedHorizontally(bool in_flip);
			//-----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether sprite is flipped about it's local x-axis
			//-----------------------------------------------------------
			bool IsFlippedHorizontally() const;
			//-----------------------------------------------------------
			/// Flip the sprite UVs about it's local y-axis
            ///
            /// @author S Downie
			///
			/// @param Whether to flip or not
			//-----------------------------------------------------------
			void SetFlippedVertically(bool in_flip);
			//-----------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether sprite is flipped about it's local y-axis
			//-----------------------------------------------------------
			bool IsFlippedVertically() const;
			//-----------------------------------------------------------
            /// Controls the origin of the sprite relative to its position.
            /// An origin of middle centre will draw the sprite with its
            /// centre at its world position. An origin of bottom centre
            /// will draw the sprite with its base at its world position
            ///
            /// @author S Downie
			///
			/// @param Alignment (middle-centre by default)
			//-----------------------------------------------------------
			void SetOriginAlignment(AlignmentAnchor in_alignment);
			//-----------------------------------------------------------
            /// @author S Downie
            ///
			/// @return Returns alignment of this sprite's origin
			//-----------------------------------------------------------
			AlignmentAnchor GetOriginAlignment() const;
            
		private:
            //-----------------------------------------------------------
            /// Render
            ///
            /// Visited by the render system so we can add ourself
            /// to the sprite batch for rendering and flush the
            /// batch if we have a different material
            ///
            /// @param Render system
            /// @param Active camera component
            /// @param The current shader pass.
            //-----------------------------------------------------------
            void Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass) override;
            //-----------------------------------------------------
            /// Render Shadow Map
            ///
            /// Render the mesh to the shadow map
            ///
            /// @param Render system
            /// @param Active camera component
            /// @param Material to render static shadows with
            /// @param Material to render skinned shadows with
            //-----------------------------------------------------
            void RenderShadowMap(RenderSystem* inpRenderSystem, CameraComponent* inpCam, const MaterialCSPtr& in_staticShadowMap, const MaterialCSPtr& in_animShadowMap) override {};
            //----------------------------------------------------
			/// Triggered when the component is attached to
			/// an entity on the scene
            ///
            /// @author S Downie
			//----------------------------------------------------
			void OnAddedToScene() override;
			//----------------------------------------------------
			/// Triggered when the component is removed from
			/// an entity on the scene
            ///
            /// @author S Downie
			//----------------------------------------------------
			void OnRemovedFromScene() override;
            //------------------------------------------------------------
            /// On Transform Changed
            ///
            /// Our local transform or our parent transform has changed
            /// We must rebuild our sprite data
            //------------------------------------------------------------
            void OnTransformChanged();
            //-----------------------------------------------------------
            /// If the transform cache is invalid we must calculate
            /// the corner positions from the new world transform
            ///
            /// @author S Downie
            //-----------------------------------------------------------
			void UpdateVertexPositions();
            //-----------------------------------------------------------
            /// If the UVs change cache is invalid we must update the
            /// sprite data vertices
            ///
            /// @author S Downie
            ///
            /// @param UVs
            //-----------------------------------------------------------
            void UpdateVertexUVs(const UVs& in_uvs);
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Preferred size based on the current atlas, id or texture
            //-----------------------------------------------------------
            Core::Vector2 GetPreferredSize() const;
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the texture has changed in a manner to
            /// invalidate the size cache
            //-----------------------------------------------------------
            bool IsTextureSizeCacheValid() const;
            //-----------------------------------------------------------
            /// Once the sprite has adjusted to the new texture size
            /// it can flag it as no longer dirty
            ///
            /// @author S Downie
            //-----------------------------------------------------------
            void SetTextureSizeCacheValid();

		private:
        
            Core::EventConnectionUPtr m_transformChangedConnection;
        
            TextureAtlasCSPtr m_textureAtlas;
            u32 m_hashedTextureAtlasId = 0;
        
            SizePolicyDelegate m_sizePolicyDelegate;
            
            SpriteBatch::SpriteData m_spriteData;
        
            Core::Vector2 m_cachedTextureSize;
			Core::Vector2 m_originalSize;
			
			UVs m_uvs;
            
            Core::Colour m_colour;
			
            AlignmentAnchor m_originAlignment = AlignmentAnchor::k_middleCentre;
            
			bool m_flippedHorizontally = false;
			bool m_flippedVertically = false;
            bool m_vertexPositionsValid = false;
            
            bool m_isBSValid = false;
            bool m_isAABBValid = false;
            bool m_isOOBBValid = false;
		};
	}
}

#endif
