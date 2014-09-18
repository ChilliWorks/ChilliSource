//
//  VListLayoutProxy.h
//  Chilli Source
//  Created by Scott Downie on 18/09/2014.
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

#ifndef _CHILLISOURCE_UI_LAYOUT_VLISTLAYOUTPROXY_H_
#define _CHILLISOURCE_UI_LAYOUT_VLISTLAYOUTPROXY_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// Functions from vertical list layout that are exposed to Lua as static functions acting
        /// on the given drawable. This allows Lua to manipulate layouts without owning them
        /// as expensive full data.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        namespace VListLayoutProxy
        {
            //----------------------------------------------------------------------------------------
            /// Register all the proxy functions with Lua
            ///
            /// @author S Downie
            ///
            /// @param Lua system
            //----------------------------------------------------------------------------------------
            void RegisterWithLua(Scripting::LuaSystem* in_system);
            //----------------------------------------------------------------------------------------
            /// Proxy function to allow calling on an instance from Lua script
            ///
            /// @author S Downie
            ///
            /// @param Layout on which to operate
            ///
            /// @return The type of this layout instance
            //----------------------------------------------------------------------------------------
            LayoutType GetType(VListLayout* in_layout);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Layout on which to operate
            /// @param The number of cells that make up the list.
            //----------------------------------------------------------------------------------------
            void SetNumCells(VListLayout* in_layout, u32 in_numCells);
            //----------------------------------------------------------------------------------------
            /// Set the relative border size margins for each edge. These are relative to
            /// the overall layout size (i.e owning widget size).
            ///
            /// @author S Downie
            ///
            /// @param Layout on which to operate
            /// @param Top
            /// @param Right
            /// @param Bottom
            /// @param Left
            //----------------------------------------------------------------------------------------
            void SetRelativeMargins(VListLayout* in_layout, f32 in_top, f32 in_right, f32 in_bottom, f32 in_left);
            //----------------------------------------------------------------------------------------
            /// Set the absolute border size margins for each edge.
            ///
            /// @author S Downie
            ///
            /// @param Layout on which to operate
            /// @param Top
            /// @param Right
            /// @param Bottom
            /// @param Left
            //----------------------------------------------------------------------------------------
            void SetAbsoluteMargins(VListLayout* in_layout, f32 in_top, f32 in_right, f32 in_bottom, f32 in_left);
            //----------------------------------------------------------------------------------------
            /// Set the relative spacing between items. This is relative to the overall layout size
            /// (i.e. the owning widget)
            ///
            /// @author S Downie
            ///
            /// @param Layout on which to operate
            /// @param Spacing
            //----------------------------------------------------------------------------------------
            void SetRelativeSpacing(VListLayout* in_layout, f32 in_spacing);
            //----------------------------------------------------------------------------------------
            /// Set the absolute spacing between items.
            ///
            /// @author S Downie
            ///
            /// @param Layout on which to operate
            /// @param Spacing
            //----------------------------------------------------------------------------------------
            void SetAbsoluteSpacing(VListLayout* in_layout, f32 in_spacing);
        }
    }
}

#endif