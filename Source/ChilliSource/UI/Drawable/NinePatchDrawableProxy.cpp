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
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Scripting/Lua/LuaSystem.h>
#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Drawable/NinePatchDrawable.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace NinePatchDrawableProxy
        {
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RegisterWithLua(Scripting::LuaSystem* in_system)
            {
                in_system->RegisterStaticClass("NinePatchDrawable",
                                               "getType", NinePatchDrawableProxy::GetType,
                                               "setTexture", NinePatchDrawableProxy::SetTexture,
                                               "setTextureAtlas", NinePatchDrawableProxy::SetTextureAtlas,
                                               "setTextureAtlasId", NinePatchDrawableProxy::SetTextureAtlasId,
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
            void SetTexture(NinePatchDrawable* in_drawable, Core::StorageLocation in_location, const std::string& in_path)
            {
                auto resourcePool = Core::Application::Get()->GetResourcePool();
                in_drawable->SetTexture(resourcePool->LoadResource<Rendering::Texture>(in_location, in_path));
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetTextureAtlas(NinePatchDrawable* in_drawable, Core::StorageLocation in_location, const std::string& in_path)
            {
                auto resourcePool = Core::Application::Get()->GetResourcePool();
                in_drawable->SetTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(in_location, in_path));
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetTextureAtlasId(NinePatchDrawable* in_drawable, const std::string& in_id)
            {
                in_drawable->SetTextureAtlasId(in_id);
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