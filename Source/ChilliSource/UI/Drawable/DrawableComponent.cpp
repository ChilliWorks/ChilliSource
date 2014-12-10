//
//  DrawableComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 28/11/2014.
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

#include <ChilliSource/UI/Drawable/DrawableComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>
#include <ChilliSource/UI/Drawable/DrawableDesc.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const char k_drawableKey[] = "Drawable";
            
            const std::vector<Core::PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyTypes::DrawableDesc(), k_drawableKey},
            };
        }
        
        CS_DEFINE_NAMEDTYPE(DrawableComponent);
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const std::vector<Core::PropertyMap::PropertyDesc>& DrawableComponent::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        DrawableComponent::DrawableComponent(const std::string& in_componentName, const Core::PropertyMap& in_properties)
            : Component(in_componentName)
        {
            RegisterProperty<IDrawableSPtr>(PropertyTypes::Drawable(), k_drawableKey, Core::MakeDelegate(this, &DrawableComponent::GetDrawable), Core::MakeDelegate(this, &DrawableComponent::SetDrawable));
            ApplyRegisteredProperties(in_properties);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        bool DrawableComponent::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Component::InterfaceID == in_interfaceId || DrawableComponent::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        const IDrawableSPtr& DrawableComponent::GetDrawable() const
        {
            return m_drawable;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void DrawableComponent::SetDrawable(const IDrawableSPtr& in_drawable)
        {
            CS_ASSERT(in_drawable != nullptr, "Cannot set null drawable on a drawable component.");
            
            m_drawable = in_drawable;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void DrawableComponent::OnDraw(Rendering::CanvasRenderer* in_renderer, const Core::Matrix3& in_transform, const Core::Vector2& in_absSize, const Core::Colour& in_absColour)
        {
            CS_ASSERT(m_drawable != nullptr, "Must set a drawable on a drawable component.");
            
            m_drawable->Draw(in_renderer, in_transform, in_absSize, in_absColour);
        }
    }
}