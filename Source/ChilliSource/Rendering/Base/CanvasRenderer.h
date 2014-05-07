/*
 *  CanvasRenderer.h
 *  moFlo
 *
 *  Created by Scott Downie on 12/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_CANVAS_H_
#define _MO_FLO_RENDERING_CANVAS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Core/Math/Matrix3x3.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
		class CanvasRenderer
		{
		public:

            struct DisplayCharacterInfo
            {
                Core::Rectangle m_UVs;
                Core::Vector2 m_size;
                Core::Vector2 m_position;
            };
            
            //----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Render system
            //----------------------------------------------------------
			CanvasRenderer(RenderSystem* in_renderSystem);
            //----------------------------------------------------------
            /// @author S Downie
            //----------------------------------------------------------
            void Init();
			//----------------------------------------------------------
			/// Render
			///
			/// Draw UI 
			//----------------------------------------------------------
			void Render(GUI::GUIView* inpRootSurface, f32 infNearClipDistance);
            //----------------------------------------------------------
            /// Enable Clipping To Bounds
            ///
            /// Set the bounds beyond which any subviews will clip
            /// Pushes to a stack which tracks when to enable and
            /// disable scissoring
            ///
            /// @param Position of the bottom left corner of the rect
            /// @param Size of the clip region
            //---------------------------------------------------------
            void EnableClippingToBounds(const Core::Vector2& invPosition, const Core::Vector2& invSize);
            //----------------------------------------------------------
            /// Disable Clipping To Bounds
            ///
            /// Pop the scissor tracking stack
            //----------------------------------------------------------                            
            void DisableClippingToBounds();
            //-----------------------------------------------------------
            /// Draw Box
            ///
            /// Build a sprite box and batch it ready for rendering
            ///
            /// @param Transform
            /// @param Dimensions
            /// @param Texture
            /// @param Colour
            /// @param Box origin alignment
            //-----------------------------------------------------------
			void DrawBox(const Core::Matrix3x3& inmatTransform, const Core::Vector2 & invSize, const TextureCSPtr & inpTexture, const Core::Rectangle& inUVs,
                         const Core::Colour & insTintColour, AlignmentAnchor ineAlignment = AlignmentAnchor::k_middleCentre);
            //----------------------------------------------------------------------------
            //----------------------------------------------------------------------------
            std::vector<DisplayCharacterInfo> BuildText(const Core::UTF8String& in_text, const FontCSPtr& in_font, f32 in_textScale, f32 in_lineSpacing, const Core::Vector2& in_bounds, u32 in_numLines) const;
            //----------------------------------------------------------------------------
            //----------------------------------------------------------------------------
			void DrawText(const std::vector<DisplayCharacterInfo>& in_characters, const Core::Matrix3x3& in_transform, const Core::Colour& in_colour, const TextureCSPtr& in_texture);

			
		private:
			
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return Cached or new GUI material that has the given
            /// texture
            //----------------------------------------------------------
            MaterialCSPtr GetGUIMaterialForTexture(const TextureCSPtr& in_texture);
            
			//-----------------------------------------------------
			/// Update Sprite Data
			///
			/// Rebuild the sprite data
			//-----------------------------------------------------
			void UpdateSpriteData(const Core::Matrix4x4& inTransform, const Core::Vector2 & invSize, const Core::Rectangle& inUVs, const Core::Colour & insTintColour, AlignmentAnchor ineAlignment);
            
		private:
			
			SpriteComponent::SpriteData msCachedSprite;

            DynamicSpriteBatch mOverlayBatcher;
            
			std::vector<Core::Vector2> mScissorPos;
            std::vector<Core::Vector2> mScissorSize;
            
            std::unordered_map<TextureCSPtr, MaterialCSPtr> m_materialGUICache;
            
            Core::ResourcePool* m_resourcePool;
            MaterialFactory* m_materialFactory;
            
            f32 mfNearClippingDistance;
		};
	}
}

#endif
