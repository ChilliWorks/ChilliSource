//
//  NinePatchDrawable.h
//  Chilli Source
//  Created by Scott Downie on 24/07/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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


#ifndef _CHILLISOURCE_UI_DRAWABLE_NINEPATCHDRAWABLE_H_
#define _CHILLISOURCE_UI_DRAWABLE_NINEPATCHDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Texture/UVs.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// Interface for rendering widget with a texture and UVs as a 9 patch. This allows the
        /// widget to be scaled without distorting the edges. The patches are specified by percentage
        /// insets from the edges of the widget.
        ///
        /// The right and left patches should be designed to stretch vertically.
        /// The top and bottom patches should be designed to stretch horizontally.
        /// The centre patch should be designed to stretch horizontally and vertically
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        class NinePatchDrawable final : public IDrawable
        {
        public:

            //----------------------------------------------------------------------------------------
            /// Set the texture that should be used in subsequent draws
            ///
            /// @author S Downie
            ///
            /// @param Texture
            //----------------------------------------------------------------------------------------
            void SetTexture(const Rendering::TextureCSPtr& in_texture);
            //----------------------------------------------------------------------------------------
            /// Set the UVs that should be used in subsequent draws
            ///
            /// @author S Downie
            ///
            /// @param Rectangle containing U, V, S, T
            //----------------------------------------------------------------------------------------
            void SetUVs(const Rendering::UVs& in_UVs);
            //----------------------------------------------------------------------------------------
            /// Set the UV insets that should be used to create the patches. Insets are from the edge
            /// and therefore no negative numbers need to be specified for right and bottom insets.
            ///
            /// NOTE: Insets must compliment each other i.e. left and right cannot sum to more than 1.0
            /// as they would overlap and insets cannot be zero or less.
            ///
            /// @author S Downie
            ///
            /// @param Left inset as normalised fraction (0 - 1)
            /// @param Right inset as normalised fraction (0 - 1)
            /// @param Top inset as normalised fraction (0 - 1)
            /// @param Bottom inset as normalised fraction (0 - 1)
            //----------------------------------------------------------------------------------------
            void SetInsets(f32 in_left, f32 in_right, f32 in_top, f32 in_bottom);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The preferred size that the drawable wishes to de drawn at based on the
            /// texture size
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPreferredSize() const override;
            //----------------------------------------------------------------------------------------
            /// Render the widget using the canvas renderer. The widget has is rendered using the
            /// set texture and UVs.
            ///
            /// @author S Downie
            ///
            /// @param Renderer
            /// @param Absolute screen transform
            /// @param Asbolute screen size
            /// @param Absolute colour
            //----------------------------------------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour) override;
            
        private:
            
            Rendering::TextureCSPtr m_texture;
            Rendering::UVs m_UVs;
            
            f32 m_leftInset = 0.01f;
            f32 m_rightInset = 0.01f;
            f32 m_topInset = 0.01f;
            f32 m_bottomInset = 0.01f;
        };
    }
}

#endif