//
//  ThreePatchDrawableProxy.h
//  Chilli Source
//  Created by Scott Downie on 18/08/2014.
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


#ifndef _CHILLISOURCE_UI_DRAWABLE_THREEPATCHDRAWABLEPROXY_H_
#define _CHILLISOURCE_UI_DRAWABLE_THREEPATCHDRAWABLEPROXY_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// Functions from texture drawable that are exposed to Lua as static functions acting
        /// on the given drawable. This allows Lua to manipulate drawables without owning them
        /// as expensive full data.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        namespace ThreePatchDrawableProxy
        {
            //----------------------------------------------------------------------------------------
            /// Register all the proxy functions with the given Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Lua script
            //----------------------------------------------------------------------------------------
            void RegisterWithLuaScript(Lua::LuaScript* in_script);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return The type of this drawable instance
            //----------------------------------------------------------------------------------------
            DrawableType GetType(ThreePatchDrawable* in_drawable);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the texture that should be used in subsequent draws
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Location of texture
            /// @param Path of texture
            //----------------------------------------------------------------------------------------
            void SetTexture(ThreePatchDrawable* in_drawable, Core::StorageLocation in_location, const std::string& in_path);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the UVs that should be used in subsequent draws
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Rectangle containing U, V, S, T
            //----------------------------------------------------------------------------------------
            void SetUVs(ThreePatchDrawable* in_drawable, const Rendering::UVs& in_UVs);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// Set the UV insets that should be used to create the patches. Insets are from the edge
            /// and therefore no negative numbers need to be specified for right and bottom insets.
            ///
            /// NOTE: Insets must compliment each other i.e. left and right cannot sum to more than 1.0
            /// as they would overlap and insets cannot be zero or less.
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            /// @param Left inset if horizontal 3-patch bottom inset if vertical 3-patch (as normalised fraction 0 - 1)
            /// @param Right inset if horizontal 3-patch top inset if vertical 3-patch (as normalised fraction 0 - 1)
            //----------------------------------------------------------------------------------------
            void SetInsets(ThreePatchDrawable* in_drawable, f32 in_leftOrBottom, f32 in_rightOrTop);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Widget on which to operate
            ///
            /// @return The preferred size that the drawable wishes to de drawn at based on the
            /// texture size
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPreferredSize(ThreePatchDrawable* in_drawable);
        }
    }
}

#endif