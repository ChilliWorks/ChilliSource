//
//  Component.cpp
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

#include <ChilliSource/UI/Base/Component.h>

#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>


namespace ChilliSource
{
    namespace UI
    {
        CS_DEFINE_NAMEDTYPE(Component);
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        Component::Component(const std::string& in_name)
            : m_widget(nullptr), m_name(in_name)
        {
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const std::string& Component::GetName() const
        {
            return m_name;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        bool Component::HasProperty(const std::string& in_propertyName) const
        {
            std::string lowerPropertyName = in_propertyName;
            Core::StringUtils::ToLowerCase(lowerPropertyName);
            
            auto it = m_properties.find(lowerPropertyName);
            return (it != m_properties.end());
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void Component::SetProperty(const std::string& in_propertyName, const char* in_propertyValue)
        {
            SetProperty(in_propertyName, std::string(in_propertyValue));
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        Widget* Component::GetWidget()
        {
            return m_widget;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const Widget* Component::GetWidget() const
        {
            return m_widget;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void Component::ApplyRegisteredProperties(const Core::PropertyMap& in_properties)
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
        void Component::SetWidget(Widget* in_widget)
        {
            CS_ASSERT(m_propertyRegistrationComplete == true, "Cannot add component to a widget before property registration is complete.");
            CS_ASSERT(m_widget == nullptr, "Cannot change the owning widget on a component.");
            
            m_widget = in_widget;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void Component::SetProperty(const std::string& in_propertyName, const Core::IProperty* in_property)
        {
            CS_ASSERT(m_propertyRegistrationComplete == true, "Cannot set a property on a UI::Component prior to property registration completion.");
            
            std::string lowerPropertyName = in_propertyName;
            Core::StringUtils::ToLowerCase(lowerPropertyName);
            
            auto it = m_properties.find(lowerPropertyName);
            if(it == m_properties.end())
            {
                CS_LOG_FATAL("Cannot find property with name '" + in_propertyName + "' in UI::Component.");
            }
            
            it->second->Set(in_property);
        }
    }
}