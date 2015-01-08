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
#include <ChilliSource/Rendering/Base/HorizontalTextJustification.h>
#include <ChilliSource/Rendering/Base/VerticalTextJustification.h>
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
            /// A container for text properties for altering the look of built text.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------------------
            struct TextProperties
            {
                f32 m_textScale = 1.0f;
                f32 m_absCharSpacingOffset = 0.0f;
                f32 m_absLineSpacingOffset = 0.0f;
                f32 m_lineSpacingScale = 1.0f;
                u32 m_maxNumLines = 0;
                HorizontalTextJustification m_horizontalJustification = HorizontalTextJustification::k_centre;
                VerticalTextJustification m_verticalJustification = VerticalTextJustification::k_centre;
            };
            //----------------------------------------------------------------------------
            /// Holds the information required to build a sprite for a text character
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            struct DisplayCharacterInfo
            {
                Rendering::UVs m_UVs;
                Core::Vector2 m_position;
                Core::Vector2 m_packedImageSize;
                f32 m_advance = 0.0f;
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
            //----------------------------------------------------------
			/// Render
			///
			/// Draw UI
			//----------------------------------------------------------
			void Render(UI::Canvas* in_canvas);
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
            /// @param Offset
            /// @param Texture
            /// @param UVs
            /// @param Colour
            /// @param Origin anchor
            //----------------------------------------------------------------------------
			void DrawBox(const Core::Matrix3& in_transform, const Core::Vector2& in_size, const Core::Vector2& in_offset, const TextureCSPtr& in_texture, const Rendering::UVs& in_UVs,
                         const Core::Colour& in_colour, AlignmentAnchor in_anchor);
            //----------------------------------------------------------------------------
            /// Build the descriptions for all characters. The descriptions can then be
            /// passed into the draw method for rendering. The characters will be
            /// build to fit into the given bounds and will wrap and then clip in
            /// order to fit.
            ///
            /// @author S Downie
            ///
            /// @param Text to convert to display characters (UTF-8)
            /// @param Font
            /// @param Max bounds
            /// @param The text properties.
            ///
            /// @return Built text struct containing all the character infos
            //----------------------------------------------------------------------------
            BuiltText BuildText(const std::string& in_text, const FontCSPtr& in_font, const Core::Vector2& in_bounds, const TextProperties& in_textProperties) const;
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
            
			SpriteBatch::SpriteData m_canvasSprite;

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
