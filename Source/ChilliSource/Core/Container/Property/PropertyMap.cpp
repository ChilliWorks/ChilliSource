//
//  PropertyMap.cpp
//  Chilli Source
//  Created by Scott Downie on 29/07/2014.
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

#include <ChilliSource/Core/Container/Property/PropertyMap.h>

#include <ChilliSource/Core/Container/Property/PropertyType.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(const std::vector<PropertyDesc>& in_propertyDefs)
        {
            for(const auto& propertyDef : in_propertyDefs)
            {
                CS_ASSERT(m_properties.find(propertyDef.m_name) == m_properties.end(), "Duplicate property name in property map descs: " + propertyDef.m_name);
                
                std::string lowerName = propertyDef.m_name;
                StringUtils::ToLowerCase(lowerName);
                
                PropertyContainer container;
                container.m_initialised = false;
                container.m_property = propertyDef.m_type->CreateProperty();
                m_properties.insert(std::make_pair(lowerName, std::move(container)));
                
                m_propertyKeys.push_back(propertyDef.m_name);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(PropertyMap&& in_move)
        : m_properties(std::move(in_move.m_properties)), m_propertyKeys(std::move(in_move.m_propertyKeys))
        {
            in_move.m_propertyKeys.clear();
            in_move.m_properties.clear();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(const PropertyMap& in_copy)
        {
            for(const auto& pair : in_copy.m_properties)
            {
                PropertyContainer container;
                container.m_initialised = pair.second.m_initialised;
                container.m_property = pair.second.m_property->GetType()->CreateProperty();
                container.m_property->Set(pair.second.m_property.get());
                m_properties.insert(std::make_pair(pair.first, std::move(container)));
            }
            
            m_propertyKeys = in_copy.m_propertyKeys;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap& PropertyMap::operator=(PropertyMap&& in_move)
        {
            m_properties = std::move(in_move.m_properties);
            m_propertyKeys = std::move(in_move.m_propertyKeys);
            
            in_move.m_properties.clear();
            in_move.m_propertyKeys.clear();
            
            return *this;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap& PropertyMap::operator=(const PropertyMap& in_copy)
        {
            m_properties.clear();
            
            for(auto& pair : in_copy.m_properties)
            {
                PropertyContainer container;
                container.m_initialised = pair.second.m_initialised;
                container.m_property = pair.second.m_property->GetType()->CreateProperty();
                container.m_property->Set(pair.second.m_property.get());
                m_properties.insert(std::make_pair(pair.first, std::move(container)));
            }
            
            m_propertyKeys = in_copy.m_propertyKeys;
            
            return *this;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const std::vector<std::string>& PropertyMap::GetKeys() const
        {
            return m_propertyKeys;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool PropertyMap::HasKey(const std::string& in_name) const
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            return m_properties.find(lowerCaseName) != m_properties.end();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool PropertyMap::HasValue(const std::string& in_name) const
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            auto it = m_properties.find(lowerCaseName);
            if (it == m_properties.end())
            {
                CS_LOG_FATAL("Querying whether a non-existant property has a value.");
                return false;
            }
            
            return it->second.m_initialised;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void PropertyMap::SetProperty(const std::string& in_name, const char* in_value)
        {
            SetProperty<std::string>(in_name, in_value);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void PropertyMap::ParseProperty(const std::string& in_name, const std::string& in_value)
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            auto entry = m_properties.find(lowerCaseName);
            CS_ASSERT(entry != m_properties.end(), "No property in property map with name: " + in_name);
            
            IProperty* property = entry->second.m_property.get();
            property->Parse(in_value);
            entry->second.m_initialised = true;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::string PropertyMap::GetPropertyOrDefault(const std::string& in_name, const char* in_default) const
        {
            return GetPropertyOrDefault<std::string>(in_name, in_default);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const IPropertyType* PropertyMap::GetType(const std::string& in_name) const
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            auto entry = m_properties.find(lowerCaseName);
            CS_ASSERT(entry != m_properties.end(), "No property with name: " + in_name);
            return entry->second.m_property->GetType();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        IProperty* PropertyMap::GetPropertyObject(const std::string& in_name)
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            auto entry = m_properties.find(lowerCaseName);
            CS_ASSERT(entry != m_properties.end(), "No property with name: " + in_name);
            return entry->second.m_property.get();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const IProperty* PropertyMap::GetPropertyObject(const std::string& in_name) const
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            auto entry = m_properties.find(lowerCaseName);
            CS_ASSERT(entry != m_properties.end(), "No property with name: " + in_name);
            return entry->second.m_property.get();
        }
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		PropertyMap::PropertyContainer::PropertyContainer(PropertyContainer&& in_move)
		{
			m_initialised = in_move.m_initialised;
			m_property = std::move(in_move.m_property);

			in_move.m_initialised = false;
		}
		//----------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------
		PropertyMap::PropertyContainer& PropertyMap::PropertyContainer::operator=(PropertyContainer&& in_move)
		{
			m_initialised = in_move.m_initialised;
			m_property = std::move(in_move.m_property);

			in_move.m_initialised = false;

			return *this;
		}
    }
}