//
//  UIComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 14/11/2014.
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

#include <ChilliSource/UI/Base/UIComponent.h>

#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(UIComponent);
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    UIComponent::UIComponent(const std::string& in_name)
        : m_widget(nullptr), m_name(in_name)
    {
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    const std::string& UIComponent::GetName() const
    {
        return m_name;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    bool UIComponent::HasProperty(const std::string& in_propertyName) const
    {
        std::string lowerPropertyName = in_propertyName;
        StringUtils::ToLowerCase(lowerPropertyName);
        
        auto it = m_properties.find(lowerPropertyName);
        return (it != m_properties.end());
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void UIComponent::SetProperty(const std::string& in_propertyName, const char* in_propertyValue)
    {
        SetProperty(in_propertyName, std::string(in_propertyValue));
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    Widget* UIComponent::GetWidget()
    {
        return m_widget;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    const Widget* UIComponent::GetWidget() const
    {
        return m_widget;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void UIComponent::ApplyRegisteredProperties(const PropertyMap& in_properties)
    {
        CS_ASSERT(m_propertyRegistrationComplete == false, "Registered properties have already been applied.");
        
        m_propertyRegistrationComplete = true;
        
        for (const auto& key : in_properties.GetKeys())
        {
            if (in_properties.HasValue(key) == true)
            {
                SetProperty(key, in_properties.GetPropertyObject(key));
            }
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void UIComponent::SetWidget(Widget* in_widget)
    {
        CS_ASSERT(m_propertyRegistrationComplete == true, "Cannot add component to a widget before property registration is complete.");
        CS_ASSERT(m_widget == nullptr, "Cannot change the owning widget on a component.");
        
        m_widget = in_widget;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void UIComponent::SetProperty(const std::string& in_propertyName, const IProperty* in_property)
    {
        CS_ASSERT(m_propertyRegistrationComplete == true, "Cannot set a property on a UIComponent prior to property registration completion.");
        
        std::string lowerPropertyName = in_propertyName;
        StringUtils::ToLowerCase(lowerPropertyName);
        
        auto it = m_properties.find(lowerPropertyName);
        if(it == m_properties.end())
        {
            CS_LOG_FATAL("Cannot find property with name '" + in_propertyName + "' in UIComponent.");
        }
        
        it->second->Set(in_property);
    }
}