//
//  Drawble.h
//  Chilli Source
//  Created by Scott Downie on 17/04/2014.
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


#ifndef _CHILLISOURCE_UI_DRAWABLE_DRAWABLE_H_
#define _CHILLISOURCE_UI_DRAWABLE_DRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------
        /// Interface for rendering widgets
        ///
        /// @author S Downie
        //------------------------------------------------------------------------
        class Drawable : public Core::QueryableInterface
        {
        public:
            CS_DECLARE_NAMEDTYPE(Drawable);
            CS_DECLARE_NOCOPY(Drawable);
            //----------------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            Drawable() = default;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The preferred size that the drawable wishes to de drawn at. This is usually
            /// based on an underlying image
            //----------------------------------------------------------------------------------------
            virtual Core::Vector2 GetPreferredSize() const = 0;
            //----------------------------------------------------------------------------------------
            /// Set the texture that should be used in subsequent draws
            ///
            /// @author S Downie
            ///
            /// @param Texture
            //----------------------------------------------------------------------------------------
            virtual void SetTexture(const Rendering::TextureCSPtr& in_texture) = 0;
            //----------------------------------------------------------------------------------------
            /// Set the texture atlas that should be used in subsequent draws. This will
            /// potentially affect the UVs of a drawable.
            ///
            /// @author S Downie
            ///
            /// @param Texture atlas
            //----------------------------------------------------------------------------------------
            virtual void SetTextureAtlas(const Rendering::TextureAtlasCSPtr& in_atlas) = 0;
            //----------------------------------------------------------------------------------------
            /// Set the texture atlas frame Id that should be used in subsequent draws. This will
            /// potentially affect the UVs of a drawable.
            ///
            /// NOTE: An atlas must have been set prior to calling this
            ///
            /// @author S Downie
            ///
            /// @param Frame Id
            //----------------------------------------------------------------------------------------
            virtual void SetTextureAtlasId(const std::string& in_atlasId) = 0;
            //----------------------------------------------------------------------------------------
            /// Set the UVs that should be used in subsequent draws. UVs are relative to the
            /// frame and not the overall atlas
            ///
            /// @author S Downie
            ///
            /// @param Rectangle containing U, V, S, T
            //----------------------------------------------------------------------------------------
            virtual void SetUVs(const Rendering::UVs& in_UVs) = 0;
            //----------------------------------------------------------------------------------------
            /// Sets the colour of the drawable. The final colour of the drawable takes into account
            /// the owning widgets colour and this colour.
            ///
            /// @author Ian Copland
            ///
            /// @param The colour.
            //----------------------------------------------------------------------------------------
            virtual void SetColour(const Core::Colour& in_colour) = 0;
            //----------------------------------------------------------------------------------------
            /// Render the widget using the canvas renderer
            ///
            /// @author S Downie
            ///
            /// @param Renderer
            /// @param Absolute screen space transform
            /// @param Asbolute screen size
            /// @param Absolute colour
            //----------------------------------------------------------------------------------------
            virtual void Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour) = 0;
            //----------------------------------------------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            virtual ~Drawable(){};
        };
    }
}

#endif