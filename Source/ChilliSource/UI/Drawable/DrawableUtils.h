//
//  DrawableUtils.h
//  Chilli Source
//  Created by Scott Downie on 03/10/2014.
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

#ifndef _CHILLISOURCE_UI_DRAWABLE_DRAWABLEUTILS_H_
#define _CHILLISOURCE_UI_DRAWABLE_DRAWABLEUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------
        /// Collection of utility methods shared by drawables
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------
        namespace DrawableUtils
        {
            //----------------------------------------------------------------------------
            /// Creates the texture atlas frame from the given atlas, atlas id, and UVs.
            /// If there is no atlas or atlas id, the frame will be calculated from the
            /// texture and the UVs. If there is also no texture, a zero sized frame
            /// will be created.
            ///
            /// @author Ian Copland
            ///
            /// @param The texture. Can be null.
            /// @param The texture atlas. Can be null.
            /// @param The atlas Id string. Can be null.
            /// @param The virtual UVs.
            ///
            /// @return Atlas frame
            //----------------------------------------------------------------------------
            Rendering::TextureAtlas::Frame BuildFrame(const Rendering::Texture* in_texture, const Rendering::TextureAtlas* in_textureAtlas, const std::string& in_atlasId, const Rendering::UVs& in_virtualUVs);
        }
    }
}

#endif
