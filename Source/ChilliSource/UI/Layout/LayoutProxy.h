//
//  LayoutProxy.h
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

#ifndef _CHILLISOURCE_UI_LAYOUT_LAYOUTPROXY_H_
#define _CHILLISOURCE_UI_LAYOUT_LAYOUTPROXY_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// Functions from base layout that are exposed to Lua as static functions acting
        /// on the given drawable. This allows Lua to manipulate layouts without owning them
        /// as expensive full data.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        namespace LayoutProxy
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
            LayoutType GetType(ILayout* in_layout);
        }
    }
}

#endif