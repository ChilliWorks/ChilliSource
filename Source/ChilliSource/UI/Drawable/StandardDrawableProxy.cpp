//
//  StandardDrawableProxy.cpp
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

#include <ChilliSource/UI/Drawable/StandardDrawableProxy.h>

#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Scripting/Lua/LuaSystem.h>
#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Drawable/StandardDrawable.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace StandardDrawableProxy
        {
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void RegisterWithLua(Scripting::LuaSystem* in_system)
            {
                in_system->RegisterStaticClass("StandardDrawable",
                                               "getType", StandardDrawableProxy::GetType,
                                               "setTexture", StandardDrawableProxy::SetTexture,
                                               "setTextureAtlas", StandardDrawableProxy::SetTextureAtlas,
                                               "setTextureAtlasId", StandardDrawableProxy::SetTextureAtlasId,
                                               "setUVs", StandardDrawableProxy::SetUVs,
                                               "getPreferredSize", StandardDrawableProxy::GetPreferredSize
                                               );
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            DrawableType GetType(StandardDrawable* in_drawable)
            {
                return in_drawable->GetType();
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetTexture(StandardDrawable* in_drawable, Core::StorageLocation in_location, const std::string& in_path)
            {
                auto resourcePool = Core::Application::Get()->GetResourcePool();
                in_drawable->SetTexture(resourcePool->LoadResource<Rendering::Texture>(in_location, in_path));
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetTextureAtlas(StandardDrawable* in_drawable, Core::StorageLocation in_location, const std::string& in_path)
            {
                auto resourcePool = Core::Application::Get()->GetResourcePool();
                in_drawable->SetTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(in_location, in_path));
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetTextureAtlasId(StandardDrawable* in_drawable, const std::string& in_id)
            {
                in_drawable->SetTextureAtlasId(in_id);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            void SetUVs(StandardDrawable* in_drawable, const Rendering::UVs& in_UVs)
            {
                in_drawable->SetUVs(in_UVs);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPreferredSize(StandardDrawable* in_drawable)
            {
                return in_drawable->GetPreferredSize();
            }
        }
    }
}