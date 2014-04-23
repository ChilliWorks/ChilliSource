//
//  IDrawble.h
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


#ifndef _CHILLISOURCE_UI_DRAWABLE_IDRAWABLE_H_
#define _CHILLISOURCE_UI_DRAWABLE_IDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// Interface for rendering widgets
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        class IDrawable
        {
        public:
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The preferred size that the drawable wishes to de drawn at. This is usually
            /// based on an underlying image
            //----------------------------------------------------------------------------------------
            virtual Core::Vector2 GetPreferredSize() const = 0;
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
            virtual void Draw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3x3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour) = 0;
        };
    }
}

#endif