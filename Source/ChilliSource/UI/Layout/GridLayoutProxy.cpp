//
//  GridLayoutProxy.cpp
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

#include <ChilliSource/UI/Layout/GridLayoutProxy.h>

#include <ChilliSource/Scripting/Lua/LuaSystem.h>
#include <ChilliSource/UI/Layout/GridLayout.h>
#include <ChilliSource/UI/Layout/LayoutType.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace GridLayoutProxy
        {
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RegisterWithLua(Scripting::LuaSystem* in_system)
            {
                in_system->RegisterStaticClass("GridLayout",
                                               "getType", GridLayoutProxy::GetType,
                                               "setNumRows", GridLayoutProxy::SetNumRows,
                                               "setNumCols", GridLayoutProxy::SetNumCols,
                                               "setRelativeMargins", GridLayoutProxy::SetRelativeMargins,
                                               "setAbsoluteMargins", GridLayoutProxy::SetAbsoluteMargins,
                                               "setRelativeHSpacing", GridLayoutProxy::SetRelativeHSpacing,
                                               "setAbsoluteHSpacing", GridLayoutProxy::SetAbsoluteHSpacing,
                                               "setRelativeVSpacing", GridLayoutProxy::SetRelativeVSpacing,
                                               "setAbsoluteVSpacing", GridLayoutProxy::SetAbsoluteVSpacing
                                               );
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            LayoutType GetType(GridLayout* in_layout)
            {
                return in_layout->GetType();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetNumRows(GridLayout* in_layout, u32 in_numRows)
            {
                in_layout->SetNumRows(in_numRows);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetNumCols(GridLayout* in_layout, u32 in_numCols)
            {
                in_layout->SetNumCols(in_numCols);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetRelativeMargins(GridLayout* in_layout, f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
            {
                in_layout->SetRelativeMargins(in_top, in_right, in_bottom, in_left);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetAbsoluteMargins(GridLayout* in_layout, f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
            {
                in_layout->SetAbsoluteMargins(in_top, in_right, in_bottom, in_left);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetRelativeHSpacing(GridLayout* in_layout, f32 in_spacing)
            {
                in_layout->SetRelativeHSpacing(in_spacing);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetAbsoluteHSpacing(GridLayout* in_layout, f32 in_spacing)
            {
                in_layout->SetAbsoluteHSpacing(in_spacing);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetRelativeVSpacing(GridLayout* in_layout, f32 in_spacing)
            {
                in_layout->SetRelativeVSpacing(in_spacing);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetAbsoluteVSpacing(GridLayout* in_layout, f32 in_spacing)
            {
                in_layout->SetAbsoluteVSpacing(in_spacing);
            }
        }
    }
}