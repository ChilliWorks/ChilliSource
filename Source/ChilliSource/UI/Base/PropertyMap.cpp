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
            /// @return New memory of given type as void*
            //----------------------------------------------------------------------------------------
            void* New(PropertyType in_type)
            {
                switch(in_type)
                {
                    case PropertyType::k_bool:
                        return new bool();
                    case PropertyType::k_float:
                        return new f32();
                    case PropertyType::k_int:
                        return new s32();
                    case PropertyType::k_string:
                        return new std::string("");
                    case PropertyType::k_vec2:
                        return new Core::Vector2();
                    case PropertyType::k_vec3:
                        return new Core::Vector3();
                    case PropertyType::k_vec4:
                        return new Core::Vector4();
                    case PropertyType::k_colour:
                        return new Core::Colour();
                    case PropertyType::k_alignmentAnchor:
                        return new Rendering::AlignmentAnchor();
                    case PropertyType::k_propertyMap:
                        return new PropertyMap();
                    case PropertyType::k_sizePolicy:
                        return new SizePolicy();
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
            void Copy(PropertyType in_type, const void* in_src, void* in_dst)
            {
                switch(in_type)
                {
                    case PropertyType::k_bool:
                    {
                        const bool* src = (const bool*)in_src;
                        bool* dst = (bool*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_float:
                    {
                        const f32* src = (const f32*)in_src;
                        f32* dst = (f32*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_int:
                    {
                        const s32* src = (const s32*)in_src;
                        s32* dst = (s32*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_string:
                    {
                        const std::string* src = (const std::string*)in_src;
                        std::string* dst = (std::string*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_vec2:
                    {
                        const Core::Vector2* src = (const Core::Vector2*)in_src;
                        Core::Vector2* dst = (Core::Vector2*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_vec3:
                    {
                        const Core::Vector3* src = (const Core::Vector3*)in_src;
                        Core::Vector3* dst = (Core::Vector3*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_vec4:
                    {
                        const Core::Vector4* src = (const Core::Vector4*)in_src;
                        Core::Vector4* dst = (Core::Vector4*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_colour:
                    {
                        const Core::Colour* src = (const Core::Colour*)in_src;
                        Core::Colour* dst = (Core::Colour*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_alignmentAnchor:
                    {
                        const Rendering::AlignmentAnchor* src = (const Rendering::AlignmentAnchor*)in_src;
                        Rendering::AlignmentAnchor* dst = (Rendering::AlignmentAnchor*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_propertyMap:
                    {
                        const PropertyMap* src = (const PropertyMap*)in_src;
                        PropertyMap* dst = (PropertyMap*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_sizePolicy:
                    {
                        const SizePolicy* src = (const SizePolicy*)in_src;
                        SizePolicy* dst = (SizePolicy*)in_dst;
                        *dst = *src;
                        break;
                    }
                    case PropertyType::k_unknown:
                    {
                        break;
                    }
                }
            }
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Type
            /// @param Pointer to delete
            //----------------------------------------------------------------------------------------
            void Delete(PropertyType in_type, void* in_memory)
            {
                switch(in_type)
                {
                    case PropertyType::k_bool:
                    {
                        bool* memory = (bool*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_float:
                    {
                        f32* memory = (f32*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_int:
                    {
                        s32* memory = (s32*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_string:
                    {
                        std::string* memory = (std::string*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_vec2:
                    {
                        Core::Vector2* memory = (Core::Vector2*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_vec3:
                    {
                        Core::Vector3* memory = (Core::Vector3*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_vec4:
                    {
                        Core::Vector4* memory = (Core::Vector4*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_colour:
                    {
                        Core::Colour* memory = (Core::Colour*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_alignmentAnchor:
                    {
                        Rendering::AlignmentAnchor* memory = (Rendering::AlignmentAnchor*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_propertyMap:
                    {
                        PropertyMap* memory = (PropertyMap*)in_memory;
                        delete memory;
                        break;
                    }
                    case PropertyType::k_sizePolicy:
                    {
                        SizePolicy* memory = (SizePolicy*)in_memory;
                        delete memory;
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
                PropertyLookup lookup = {pair.second.m_type, New(pair.second.m_type)};
                Copy(pair.second.m_type, pair.second.m_value, lookup.m_value);
                m_properties.insert(std::make_pair(pair.first, lookup));
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
                PropertyLookup lookup = {pair.second.m_type, New(pair.second.m_type)};
                Copy(pair.second.m_type, pair.second.m_value, lookup.m_value);
                m_properties.insert(std::make_pair(pair.first, lookup));
            }
            
            return *this;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void PropertyMap::AllocateKeys(const std::vector<PropertyDesc>& in_propertyDefs)
        {
            Clear();
            
            for(const auto& propertyDef : in_propertyDefs)
            {
                CS_ASSERT(propertyDef.m_type != PropertyType::k_unknown, "Unsupported property type for property: " + propertyDef.m_name);
                
                u32 hashId = Core::HashCRC32::GenerateHashCode(propertyDef.m_name);
                PropertyLookup lookup = {propertyDef.m_type, New(propertyDef.m_type)};
                m_properties.insert(std::make_pair(hashId, lookup));
                SetProperty(propertyDef.m_type, propertyDef.m_name, propertyDef.m_value);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool PropertyMap::HasProperty(const std::string& in_name) const
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            return m_properties.find(hashKey) != m_properties.end();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyType PropertyMap::GetType(const std::string& in_name) const
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            auto entry = m_properties.find(hashKey);
            CS_ASSERT(entry != m_properties.end(), "No UI property with name: " + in_name);
            return entry->second.m_type;
        }
        //----------------------------------------------------------------------------------------
        /// This is a specialisation to handle inserting strings into the blob.
        //----------------------------------------------------------------------------------------
        template<> void PropertyMap::SetProperty(const std::string& in_name, std::string in_value)
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            auto entry = m_properties.find(hashKey);
            CS_ASSERT(entry != m_properties.end(), "No UI property with name: " + in_name);
            CS_ASSERT(entry->second.m_type == PropertyType::k_string, "Wrong type for property with name " + in_name);
            
            std::string* property = (std::string*)entry->second.m_value;
            *property = std::move(in_value);
        }
        //----------------------------------------------------------------------------------------
        /// This is a specialisation to handle inserting strings into the blob.
        //----------------------------------------------------------------------------------------
        template<> void PropertyMap::SetProperty(const std::string& in_name, const std::string& in_value)
        {
            SetProperty<std::string>(in_name, in_value);
        }
        //----------------------------------------------------------------------------------------
        /// This is a specialisation to handle inserting string literals into the blob
        //----------------------------------------------------------------------------------------
        template<> void PropertyMap::SetProperty(const std::string& in_name, const char* in_value)
        {
            SetProperty<std::string>(in_name, in_value);
        }
        //----------------------------------------------------------------------------------------
        /// This is a specialisation to handle retrieving strings from the blob.
        //----------------------------------------------------------------------------------------
        template<> const char* PropertyMap::GetProperty(const std::string& in_name) const
        {
            return GetProperty<std::string>(in_name).c_str();
        }
        //----------------------------------------------------------------------------------------
        /// This is a specialisation to handle retrieving strings from the blob.
        //----------------------------------------------------------------------------------------
        template<> const char* PropertyMap::GetPropertyOrDefault(const std::string& in_name, const char* in_default) const
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
        template<> PropertyType PropertyMap::GetType<const std::string&>() const
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
        template<> PropertyType PropertyMap::GetType<const Core::Vector2&>() const
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
        template<> PropertyType PropertyMap::GetType<const Core::Vector3&>() const
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
        template<> PropertyType PropertyMap::GetType<const Core::Vector4&>() const
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
        template<> PropertyType PropertyMap::GetType<const Core::Colour&>() const
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
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const PropertyMap&>() const
        {
            return PropertyType::k_propertyMap;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void PropertyMap::Clear()
        {
            for(auto& pair : m_properties)
            {
                Delete(pair.second.m_type, pair.second.m_value);
            }
            
            m_properties.clear();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::~PropertyMap()
        {
            Clear();
        }
    }
}