//
//  DrawableUIComponent.cpp
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

#include <ChilliSource/UI/Drawable/DrawableUIComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Drawable/UIDrawable.h>
#include <ChilliSource/UI/Drawable/UIDrawableDef.h>

namespace ChilliSource
{
    namespace
    {
        const char k_drawableKey[] = "Drawable";
        
        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            {PropertyTypes::UIDrawableDef(), k_drawableKey},
        };
    }
    
    CS_DEFINE_NAMEDTYPE(DrawableUIComponent);
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& DrawableUIComponent::GetPropertyDescs()
    {
        return k_propertyDescs;
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    DrawableUIComponent::DrawableUIComponent(const std::string& in_componentName, const PropertyMap& in_properties)
        : UIComponent(in_componentName)
    {
        RegisterProperty<UIDrawableDefCSPtr>(PropertyTypes::UIDrawableDef(), k_drawableKey, MakeDelegate(this, &DrawableUIComponent::GetDrawableDef), MakeDelegate(this, &DrawableUIComponent::ApplyDrawableDef));
        ApplyRegisteredProperties(in_properties);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    bool DrawableUIComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIComponent::InterfaceID == in_interfaceId || DrawableUIComponent::InterfaceID == in_interfaceId);
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    UIDrawable* DrawableUIComponent::GetDrawable()
    {
        return m_drawable.get();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const UIDrawable* DrawableUIComponent::GetDrawable() const
    {
        return m_drawable.get();
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    void DrawableUIComponent::ApplyDrawableDef(const UIDrawableDefCSPtr& in_drawableDef)
    {
        m_drawable.reset();
        m_drawableDef = in_drawableDef;
        
        if (m_drawableDef != nullptr)
        {
            m_drawable = m_drawableDef->CreateDrawable();
        }
    }
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    const UIDrawableDefCSPtr& DrawableUIComponent::GetDrawableDef() const
    {
        return m_drawableDef;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void DrawableUIComponent::OnDraw(CanvasRenderer* in_renderer, const Matrix3& in_transform, const Vector2& in_absSize, const Colour& in_absColour)
    {
        if (m_drawable != nullptr)
        {
            m_drawable->Draw(in_renderer, in_transform, in_absSize, in_absColour);
        }
    }
}