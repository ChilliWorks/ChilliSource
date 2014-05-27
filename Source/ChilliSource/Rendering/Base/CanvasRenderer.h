//
//  CanvasRenderer.h
//  Chilli Source
//  Created by Scott Downie on 12/01/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_BASE_CANVASRENDERER_H_
#define _CHILLISOURCE_RENDERING_BASE_CANVASRENDERER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
        //----------------------------------------------------------------------------
        /// System that renders simple shapes and text to screen space. Responsible
        /// for rendering the UI system.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------
		class CanvasRenderer : public Core::AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(CanvasRenderer);
            
            //----------------------------------------------------------------------------
            /// Holds the information required to build a sprite for a text character
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            struct DisplayCharacterInfo
            {
                Core::Rectangle m_UVs;
                Core::Vector2 m_size;
                Core::Vector2 m_position;
            };
            //----------------------------------------------------------------------------
            /// Holds the return information for building text including all the characters
            /// and the total width and height
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            struct BuiltText
            {
                std::vector<DisplayCharacterInfo> m_characters;
                
                f32 m_width;
                f32 m_height;
            };
            //----------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID
            ///
            /// @return Whether the class implements the given interface
            //----------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// Recursively descened the view hierarchy drawing each view
			///
			/// @author S Downie
            ///
            /// @param Root view to draw
			//----------------------------------------------------------------------------
			void Render(GUI::GUIView* in_rootView);
            //----------------------------------------------------------------------------
            /// Set the bounds beyond which any subviews will clip
            /// Pushes to a stack which tracks when to enable and
            /// disable scissoring
            ///
            /// @author A Mackie
            ///
            /// @param Position of the bottom left corner of the rect in screen space
            /// @param Size of the clip region in screen space
            //----------------------------------------------------------------------------
            void PushClipBounds(const Core::Vector2& in_blPosition, const Core::Vector2& in_size);
            //----------------------------------------------------------------------------
            /// Pop the scissor tracking stack and disable clipping if the stack is empty
            ///
            /// @author A Mackie
            //----------------------------------------------------------------------------
            void PopClipBounds();
            //----------------------------------------------------------------------------
            /// Build a sprite box and render it to screen
            ///
            /// @param Transform
            /// @param Dimensions
            /// @param Texture
            /// @param UVs
            /// @param Colour
            /// @param Origin anchor
            //----------------------------------------------------------------------------
			void DrawBox(const Core::Matrix3& in_transform, const Core::Vector2& in_size, const TextureCSPtr& in_texture, const Core::Rectangle& in_UVs,
                         const Core::Colour& in_colour, AlignmentAnchor in_anchor);
            //----------------------------------------------------------------------------
            /// Build the descriptions for all characters. The descriptions can then be
            /// passed into the draw method for rendering. The characters will be
            /// build to fit into the given bounds and will wrap and then clip in
            /// order to fit.
            ///
            /// @author S Downie
            ///
            /// @param Text to convert to display characters
            /// @param Font
            /// @param Text scale
            /// @param Line spacing
            /// @param Max bounds
            /// @param Max num lines (ZERO = infinite)
            /// @param Horizontal justification
            /// @param Vertical justufication
            ///
            /// @return Built text struct containing all the character infos
            //----------------------------------------------------------------------------
            BuiltText BuildText(const Core::UTF8String& in_text, const FontCSPtr& in_font, f32 in_textScale, f32 in_lineSpacing,
                                const Core::Vector2& in_bounds, u32 in_numLines, GUI::TextJustification in_horizontal, GUI::TextJustification in_vertical) const;
            //----------------------------------------------------------------------------
            /// Build the sprites for each given character and render them to screen.
            ///
            /// @param Characters in text space
            /// @param Transform to screen space
            /// @param Colour
            /// @param Texture
            //----------------------------------------------------------------------------
			void DrawText(const std::vector<DisplayCharacterInfo>& in_characters, const Core::Matrix3& in_transform, const Core::Colour& in_colour, const TextureCSPtr& in_texture);
			
		private:
			
            friend class Core::Application;
            //----------------------------------------------------------------------------
            /// Creates a new instance of the system.
            ///
            /// @author S Downie
            ///
            /// @return New renderer instance
            //----------------------------------------------------------------------------
            static CanvasRendererUPtr Create();
            //----------------------------------------------------------------------------
            /// Private constructor to enforce use of factory method
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
			CanvasRenderer() = default;
            //----------------------------------------------------------------------------
            /// Called when the system is created
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            void OnInit() override;
            //----------------------------------------------------------------------------
            /// Called when the system is destroyed
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            void OnDestroy() override;
            //----------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Texture
            ///
            /// @return Cached or new GUI material that has the given
            /// texture
            //----------------------------------------------------------------------------
            MaterialCSPtr GetGUIMaterialForTexture(const TextureCSPtr& in_texture);
            
		private:
			
			SpriteComponent::SpriteData m_canvasSprite;

            DynamicSpriteBatchUPtr m_overlayBatcher;
            
			std::vector<Core::Vector2> m_scissorPositions;
            std::vector<Core::Vector2> m_scissorSizes;
            
            std::unordered_map<TextureCSPtr, MaterialCSPtr> m_materialGUICache;
            
            Core::ResourcePool* m_resourcePool;
            Core::Screen* m_screen;
            MaterialFactory* m_materialFactory;
		};
	}
}

#endif
