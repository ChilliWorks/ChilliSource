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

#include <ChilliSource/UI/Base/PropertyMap.h>

#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/UI/Base/PropertyType.h>
#include <ChilliSource/UI/Base/SizePolicy.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Type
            ///
            /// @return New property of given type
            //----------------------------------------------------------------------------------------
            PropertyMap::IPropertyUPtr CreateProperty(PropertyType in_type)
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
                    case PropertyType::k_propertyMap:
                        return PropertyMap::IPropertyUPtr(new PropertyMap::Property<PropertyMap>());
                    case PropertyType::k_sizePolicy:
                        return PropertyMap::IPropertyUPtr(new PropertyMap::Property<SizePolicy>());
                    case PropertyType::k_unknown:
                        return nullptr;
                }

				return nullptr;
            }
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Type
            /// @param Source
            /// @param Destination
            //----------------------------------------------------------------------------------------
            void CopyProperty(PropertyType in_type, const PropertyMap::IProperty* in_src, PropertyMap::IProperty* in_dst)
            {
                switch(in_type)
                {
                    case PropertyType::k_bool:
                    {
                        const PropertyMap::Property<bool>* src = (const PropertyMap::Property<bool>*)in_src;
                        PropertyMap::Property<bool>* dst = (PropertyMap::Property<bool>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_float:
                    {
                        const PropertyMap::Property<f32>* src = (const PropertyMap::Property<f32>*)in_src;
                        PropertyMap::Property<f32>* dst = (PropertyMap::Property<f32>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_int:
                    {
                        const PropertyMap::Property<s32>* src = (const PropertyMap::Property<s32>*)in_src;
                        PropertyMap::Property<s32>* dst = (PropertyMap::Property<s32>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_string:
                    {
                        const PropertyMap::Property<std::string>* src = (const PropertyMap::Property<std::string>*)in_src;
                        PropertyMap::Property<std::string>* dst = (PropertyMap::Property<std::string>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_vec2:
                    {
                        const PropertyMap::Property<Core::Vector2>* src = (const PropertyMap::Property<Core::Vector2>*)in_src;
                        PropertyMap::Property<Core::Vector2>* dst = (PropertyMap::Property<Core::Vector2>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_vec3:
                    {
                        const PropertyMap::Property<Core::Vector3>* src = (const PropertyMap::Property<Core::Vector3>*)in_src;
                        PropertyMap::Property<Core::Vector3>* dst = (PropertyMap::Property<Core::Vector3>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_vec4:
                    {
                        const PropertyMap::Property<Core::Vector4>* src = (const PropertyMap::Property<Core::Vector4>*)in_src;
                        PropertyMap::Property<Core::Vector4>* dst = (PropertyMap::Property<Core::Vector4>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_colour:
                    {
                        const PropertyMap::Property<Core::Colour>* src = (const PropertyMap::Property<Core::Colour>*)in_src;
                        PropertyMap::Property<Core::Colour>* dst = (PropertyMap::Property<Core::Colour>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_alignmentAnchor:
                    {
                        const PropertyMap::Property<Rendering::AlignmentAnchor>* src = (const PropertyMap::Property<Rendering::AlignmentAnchor>*)in_src;
                        PropertyMap::Property<Rendering::AlignmentAnchor>* dst = (PropertyMap::Property<Rendering::AlignmentAnchor>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_propertyMap:
                    {
                        const PropertyMap::Property<PropertyMap>* src = (const PropertyMap::Property<PropertyMap>*)in_src;
                        PropertyMap::Property<PropertyMap>* dst = (PropertyMap::Property<PropertyMap>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_sizePolicy:
                    {
                        const PropertyMap::Property<SizePolicy>* src = (const PropertyMap::Property<SizePolicy>*)in_src;
                        PropertyMap::Property<SizePolicy>* dst = (PropertyMap::Property<SizePolicy>*)in_dst;
                        dst->m_value = src->m_value;
                        break;
                    }
                    case PropertyType::k_unknown:
                    {
                        break;
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(const std::vector<PropertyDesc>& in_propertyDefs)
        {
            AllocateKeys(in_propertyDefs);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(PropertyMap&& in_move)
        : m_properties(std::move(in_move.m_properties))
        {
            in_move.m_properties.clear();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(const PropertyMap& in_copy)
        {
            for(auto& pair : in_copy.m_properties)
            {
                PropertyLookup lookup = {pair.second.m_type, CreateProperty(pair.second.m_type)};
                CopyProperty(pair.second.m_type, pair.second.m_property.get(), lookup.m_property.get());
                m_properties.insert(std::make_pair(pair.first, std::move(lookup)));
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap& PropertyMap::operator=(PropertyMap&& in_move)
        {
            m_properties = std::move(in_move.m_properties);
            in_move.m_properties.clear();
            
            return *this;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap& PropertyMap::operator=(const PropertyMap& in_copy)
        {
            for(auto& pair : in_copy.m_properties)
            {
                PropertyLookup lookup = {pair.second.m_type, CreateProperty(pair.second.m_type)};
                CopyProperty(pair.second.m_type, pair.second.m_property.get(), lookup.m_property.get());
                m_properties.insert(std::make_pair(pair.first, std::move(lookup)));
            }
            
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
                PropertyLookup lookup = {propertyDef.m_type, CreateProperty(propertyDef.m_type)};
                m_properties.insert(std::make_pair(hashId, std::move(lookup)));
                SetProperty(propertyDef.m_type, lowerCaseName, propertyDef.m_value);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool PropertyMap::HasProperty(const std::string& in_name) const
        {
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            u32 hashKey = Core::HashCRC32::GenerateHashCode(lowerCaseName);
            
            return m_properties.find(hashKey) != m_properties.end();
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
        const char* PropertyMap::GetProperty(const std::string& in_name) const
        {
            return GetProperty<std::string>(in_name).c_str();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        const char* PropertyMap::GetPropertyOrDefault(const std::string& in_name, const char* in_default) const
        {
            return GetPropertyOrDefault<std::string>(in_name, in_default).c_str();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void PropertyMap::SetProperty(PropertyType in_type, const std::string& in_name, const std::string& in_value)
        {
            switch(in_type)
            {
                case PropertyType::k_bool:
                    SetProperty(in_name, Core::ParseBool(in_value));
                    break;
                case PropertyType::k_int:
                    SetProperty(in_name, Core::ParseS32(in_value));
                    break;
                case PropertyType::k_float:
                    SetProperty(in_name, Core::ParseF32(in_value));
                    break;
                case PropertyType::k_string:
                    SetProperty(in_name, in_value);
                    break;
                case PropertyType::k_vec2:
                    SetProperty(in_name, Core::ParseVector2(in_value));
                    break;
                case PropertyType::k_vec3:
                    SetProperty(in_name, Core::ParseVector3(in_value));
                    break;
                case PropertyType::k_vec4:
                    SetProperty(in_name, Core::ParseVector4(in_value));
                    break;
                case PropertyType::k_colour:
                    SetProperty(in_name, Core::ParseColour(in_value));
                    break;
                case PropertyType::k_alignmentAnchor:
                    SetProperty(in_name, Rendering::ParseAlignmentAnchor(in_value));
                    break;
                case PropertyType::k_sizePolicy:
                    SetProperty(in_name, ParseSizePolicy(in_value));
                    break;
                case PropertyType::k_propertyMap:
                {
                    Json::Reader reader;
                    Json::Value root;
                    
                    if(reader.parse(in_value, root) == false)
                    {
                        CS_LOG_FATAL("Cannot parse PropertyType::k_propertyMap as it is not key-value JSON");
                    }

                    std::vector<PropertyDesc> propertyDescs;
                    for(auto it = root.begin(); it != root.end(); ++it)
                    {
                        PropertyDesc desc;
                        desc.m_name = it.memberName();
                        desc.m_value = (*it).asString();
                        desc.m_type = PropertyType::k_string;
                        propertyDescs.push_back(desc);
                    }
                    SetProperty(in_name, PropertyMap(propertyDescs));
                    break;
                }
                case PropertyType::k_unknown:
                    break;
            }
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
        template<> PropertyType PropertyMap::GetType<PropertyMap>() const
        {
            return PropertyType::k_propertyMap;
        }
    }
}