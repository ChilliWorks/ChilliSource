//
//  VListLayoutProxy.cpp
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

#include <ChilliSource/UI/Layout/VListLayoutProxy.h>

#include <ChilliSource/Scripting/Lua/LuaSystem.h>
#include <ChilliSource/UI/Layout/LayoutType.h>
#include <ChilliSource/UI/Layout/VListLayout.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace VListLayoutProxy
        {
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RegisterWithLua(Scripting::LuaSystem* in_system)
            {
                in_system->RegisterStaticClass("VListLayout",
                                               "getType", VListLayoutProxy::GetType,
                                               "setNumCells", VListLayoutProxy::SetNumCells,
                                               "setRelativeMargins", VListLayoutProxy::SetRelativeMargins,
                                               "setAbsoluteMargins", VListLayoutProxy::SetAbsoluteMargins,
                                               "setRelativeSpacing", VListLayoutProxy::SetRelativeSpacing,
                                               "setAbsoluteSpacing", VListLayoutProxy::SetAbsoluteSpacing
                                               );
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            LayoutType GetType(VListLayout* in_layout)
            {
                return in_layout->GetType();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetNumCells(VListLayout* in_layout, u32 in_numCells)
            {
                in_layout->SetNumCells(in_numCells);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetRelativeMargins(VListLayout* in_layout, f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
            {
                in_layout->SetRelativeMargins(in_top, in_right, in_bottom, in_left);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetAbsoluteMargins(VListLayout* in_layout, f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
            {
                in_layout->SetAbsoluteMargins(in_top, in_right, in_bottom, in_left);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetRelativeSpacing(VListLayout* in_layout, f32 in_spacing)
            {
                in_layout->SetRelativeSpacing(in_spacing);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetAbsoluteSpacing(VListLayout* in_layout, f32 in_spacing)
            {
                in_layout->SetAbsoluteSpacing(in_spacing);
            }
        }
    }
}