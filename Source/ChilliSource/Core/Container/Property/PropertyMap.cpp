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

#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Base/HorizontalTextJustification.h>
#include <ChilliSource/Rendering/Base/VerticalTextJustification.h>
#include <ChilliSource/UI/Base/PropertyType.h>
#include <ChilliSource/UI/Base/SizePolicy.h>
#include <ChilliSource/UI/Drawable/DrawableDesc.h>
#include <ChilliSource/UI/Layout/LayoutDesc.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(const std::vector<PropertyDesc>& in_propertyDefs)
        {
            AllocateKeys(in_propertyDefs);
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
            for(auto& pair : in_copy.m_properties)
            {
                PropertyLookup lookup;
                lookup.m_type = pair.second.m_type;
                lookup.m_property = CreateProperty(pair.second.m_type);
                lookup.m_property->CopyFrom(pair.second.m_property.get());
                m_properties.insert(std::make_pair(pair.first, std::move(lookup)));
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
            for(auto& pair : in_copy.m_properties)
            {
                PropertyLookup lookup;
                lookup.m_type = pair.second.m_type;
                lookup.m_property = CreateProperty(pair.second.m_type);
                lookup.m_property->CopyFrom(pair.second.m_property.get());
                m_properties.insert(std::make_pair(pair.first, std::move(lookup)));
            }
            
            m_propertyKeys = in_copy.m_propertyKeys;
            
            return *this;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void PropertyMap::AllocateKeys(const std::vector<PropertyDesc>& in_propertyDefs)
        {
            m_properties.clear();
            
            for(const auto& propertyDef : in_propertyDefs)
            {
                CS_ASSERT(propertyDef.m_type != PropertyType::k_unknown, "Unsupported property type for property: " + propertyDef.m_name);
                
                std::string lowerCaseName = propertyDef.m_name;
                Core::StringUtils::ToLowerCase(lowerCaseName);
                
                u32 hashId = Core::HashCRC32::GenerateHashCode(lowerCaseName);
                PropertyLookup lookup;
                lookup.m_type = propertyDef.m_type;
                lookup.m_property = CreateProperty(propertyDef.m_type);
                m_properties.insert(std::make_pair(hashId, std::move(lookup)));
                
                m_propertyKeys.push_back(propertyDef.m_name);
            }
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
            u32 hashKey = Core::HashCRC32::GenerateHashCode(lowerCaseName);
            
            return m_properties.find(hashKey) != m_properties.end();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool PropertyMap::HasValue(const std::string& in_name) const
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            u32 hashKey = Core::HashCRC32::GenerateHashCode(lowerCaseName);
            
            auto it = m_properties.find(hashKey);
            if (it == m_properties.end())
            {
                CS_LOG_FATAL("Querying whether a non-existant property has a value.");
                return false;
            }
            
            return it->second.m_property->IsInitialised();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyType PropertyMap::GetType(const std::string& in_name) const
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            u32 hashKey = Core::HashCRC32::GenerateHashCode(lowerCaseName);
            
            auto entry = m_properties.find(hashKey);
            CS_ASSERT(entry != m_properties.end(), "No UI property with name: " + in_name);
            return entry->second.m_type;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void PropertyMap::SetProperty(const std::string& in_name, const char* in_value)
        {
            SetProperty<std::string>(in_name, in_value);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::string PropertyMap::GetPropertyOrDefault(const std::string& in_name, const char* in_default) const
        {
            return GetPropertyOrDefault<std::string>(in_name, in_default);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<bool>() const
        {
            return PropertyType::k_bool;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<s32>() const
        {
            return PropertyType::k_int;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<std::string>() const
        {
            return PropertyType::k_string;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const char*>() const
        {
            return PropertyType::k_string;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<f32>() const
        {
            return PropertyType::k_float;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::Vector2>() const
        {
            return PropertyType::k_vec2;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::Vector3>() const
        {
            return PropertyType::k_vec3;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::Vector4>() const
        {
            return PropertyType::k_vec4;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::Colour>() const
        {
            return PropertyType::k_colour;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Rendering::AlignmentAnchor>() const
        {
            return PropertyType::k_alignmentAnchor;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<SizePolicy>() const
        {
            return PropertyType::k_sizePolicy;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Rendering::HorizontalTextJustification>() const
        {
            return PropertyType::k_horizontalTextJustification;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Rendering::VerticalTextJustification>() const
        {
            return PropertyType::k_verticalTextJustification;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Rendering::TextureCSPtr>() const
        {
            return PropertyType::k_texture;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Rendering::TextureAtlasCSPtr>() const
        {
            return PropertyType::k_textureAtlas;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Rendering::FontCSPtr>() const
        {
            return PropertyType::k_font;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::LocalisedTextCSPtr>() const
        {
            return PropertyType::k_localisedText;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<DrawableDesc>() const
        {
            return PropertyType::k_drawableDesc;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<LayoutDesc>() const
        {
            return PropertyType::k_layoutDesc;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::IPropertyUPtr PropertyMap::CreateProperty(PropertyType in_type) const
        {
            switch(in_type)
            {
                case PropertyType::k_bool:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<bool>());
                case PropertyType::k_float:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<f32>());
                case PropertyType::k_int:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<s32>());
                case PropertyType::k_string:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<std::string>());
                case PropertyType::k_vec2:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Core::Vector2>());
                case PropertyType::k_vec3:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Core::Vector3>());
                case PropertyType::k_vec4:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Core::Vector4>());
                case PropertyType::k_colour:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Core::Colour>());
                case PropertyType::k_alignmentAnchor:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Rendering::AlignmentAnchor>());
                case PropertyType::k_sizePolicy:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<SizePolicy>());
                case PropertyType::k_horizontalTextJustification:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Rendering::HorizontalTextJustification>());
                case PropertyType::k_verticalTextJustification:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Rendering::VerticalTextJustification>());
                case PropertyType::k_texture:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Rendering::TextureCSPtr>());
                case PropertyType::k_textureAtlas:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Rendering::TextureAtlasCSPtr>());
                case PropertyType::k_font:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Rendering::FontCSPtr>());
                case PropertyType::k_localisedText:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<Core::LocalisedTextCSPtr>());
                case PropertyType::k_drawableDesc:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<DrawableDesc>());
                case PropertyType::k_layoutDesc:
                    return PropertyMap::IPropertyUPtr(new PropertyMap::Property<LayoutDesc>());
                case PropertyType::k_unknown:
                    return nullptr;
            }
            
            return nullptr;
        }
    }
}