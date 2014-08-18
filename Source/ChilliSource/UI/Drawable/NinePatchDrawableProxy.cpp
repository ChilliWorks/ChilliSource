//
//  NinePatchDrawableProxy.cpp
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

#include <ChilliSource/UI/Drawable/NinePatchDrawableProxy.h>

#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Lua/Base/LuaScript.h>
#include <ChilliSource/UI/Drawable/NinePatchDrawable.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace NinePatchDrawableProxy
        {
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RegisterWithLuaScript(Lua::LuaScript* in_script)
            {
                in_script->RegisterStaticClass("NinePatchDrawable",
                                               "getType", NinePatchDrawableProxy::GetType,
                                               //"setTexture", NinePatchDrawableProxy::SetTexture,
                                               "setUVs", NinePatchDrawableProxy::SetUVs,
                                               "setInsets", NinePatchDrawableProxy::SetInsets,
                                               "getPreferredSize", NinePatchDrawableProxy::GetPreferredSize
                                               );
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            DrawableType GetType(NinePatchDrawable* in_drawable)
            {
                return in_drawable->GetType();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetTexture(NinePatchDrawable* in_drawable, const Rendering::TextureCSPtr& in_texture)
            {
                in_drawable->SetTexture(in_texture);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetUVs(NinePatchDrawable* in_drawable, const Rendering::UVs& in_UVs)
            {
                in_drawable->SetUVs(in_UVs);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetInsets(NinePatchDrawable* in_drawable, f32 in_left, f32 in_right, f32 in_top, f32 in_bottom)
            {
                in_drawable->SetInsets(in_left, in_right, in_top, in_bottom);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPreferredSize(NinePatchDrawable* in_drawable)
            {
                return in_drawable->GetPreferredSize();
            }
        }
    }
}