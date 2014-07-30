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
            /// @return Size of type in bytes
            //----------------------------------------------------------------------------------------
            u32 GetTypeSize(PropertyType in_type)
            {
                switch(in_type)
                {
                    case PropertyType::k_bool:
                        return sizeof(bool);
                    case PropertyType::k_int:
                        return sizeof(s32);
                    case PropertyType::k_float:
                        return sizeof(f32);
                    case PropertyType::k_string:
                        return sizeof(std::string);
                    case PropertyType::k_vec2:
                        return sizeof(Core::Vector2);
                    case PropertyType::k_vec3:
                        return sizeof(Core::Vector3);
                    case PropertyType::k_vec4:
                        return sizeof(Core::Vector4);
                    case PropertyType::k_colour:
                        return sizeof(Core::Colour);
                    case PropertyType::k_alignmentAnchor:
                        return sizeof(Rendering::AlignmentAnchor);
                    case PropertyType::k_sizePolicy:
                        return sizeof(SizePolicy);
                    case PropertyType::k_propertyMap:
                        return sizeof(PropertyMap);
                    case PropertyType::k_unknown:
                        return 0;
                }
                
                return 0;
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(const std::vector<PropertyDesc>& in_customPropertyDefs)
        {
            AllocateKeys(in_customPropertyDefs);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(PropertyMap&& in_move)
        : m_propertyBlob(in_move.m_propertyBlob), m_blobOffsets(std::move(in_move.m_blobOffsets)), m_blobSize(in_move.m_blobSize)
        {
            in_move.m_blobSize = 0;
            in_move.m_propertyBlob = nullptr;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap::PropertyMap(const PropertyMap& in_copy)
        : m_blobOffsets(in_copy.m_blobOffsets), m_blobSize(in_copy.m_blobSize)
        {
            if(m_blobSize > 0)
            {
                m_propertyBlob = new u8[m_blobSize];
                std::copy(in_copy.m_propertyBlob, in_copy.m_propertyBlob + m_blobSize, m_propertyBlob);
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap& PropertyMap::operator=(PropertyMap&& in_move)
        {
            m_blobSize = in_move.m_blobSize;
            m_propertyBlob = in_move.m_propertyBlob;
            m_blobOffsets = std::move(in_move.m_blobOffsets);
            in_move.m_blobSize = 0;
            in_move.m_propertyBlob = nullptr;
            
            return *this;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyMap& PropertyMap::operator=(const PropertyMap& in_copy)
        {
            if(in_copy.m_blobSize > 0)
            {
                m_blobSize = in_copy.m_blobSize;
                m_propertyBlob = new u8[m_blobSize];
                std::copy(in_copy.m_propertyBlob, in_copy.m_propertyBlob + m_blobSize, m_propertyBlob);
                m_blobOffsets = in_copy.m_blobOffsets;
            }
            
            return *this;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void PropertyMap::AllocateKeys(const std::vector<PropertyDesc>& in_customPropertyDefs)
        {
            if(m_propertyBlob != nullptr)
            {
                m_blobSize = 0;
                CS_SAFEDELETE_ARRAY(m_propertyBlob);
                m_blobOffsets.clear();
            }
            
            u32 currentOffset = 0;
            for(const auto& customPropertyDef : in_customPropertyDefs)
            {
                CS_ASSERT(customPropertyDef.m_type != PropertyType::k_unknown, "Unsupported property type for property: " + customPropertyDef.m_name);
                
                PropertyLookup lookup = {customPropertyDef.m_type, currentOffset};
                m_blobOffsets.insert(std::make_pair(Core::HashCRC32::GenerateHashCode(customPropertyDef.m_name), lookup));
                
                u32 typeSize = GetTypeSize(customPropertyDef.m_type);
                currentOffset += typeSize;
            }
            
            m_blobSize = currentOffset;
            
            if(m_blobSize > 0)
            {
                m_propertyBlob = new u8[m_blobSize];
            }
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        bool PropertyMap::HasProperty(const std::string& in_name) const
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            return m_blobOffsets.find(hashKey) != m_blobOffsets.end();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        PropertyType PropertyMap::GetType(const std::string& in_name) const
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            auto entry = m_blobOffsets.find(hashKey);
            CS_ASSERT(entry != m_blobOffsets.end(), "No UI property with name: " + in_name);
            return entry->second.m_type;
        }
        //----------------------------------------------------------------------------------------
        /// This is a specialisation to handle inserting strings into the blob.
        //----------------------------------------------------------------------------------------
        template<> void PropertyMap::SetProperty(const std::string& in_name, std::string in_value)
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            auto entry = m_blobOffsets.find(hashKey);
            CS_ASSERT(entry != m_blobOffsets.end(), "No UI property with name: " + in_name);
            CS_ASSERT(entry->second.m_type == PropertyType::k_string, "Wrong type for property with name " + in_name);
            
            std::string* property = (std::string*)(m_propertyBlob + entry->second.m_offset);
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
                    CS_LOG_FATAL("Cannot parse PropertyType::k_propertyMap as it is a complex object");
                    break;
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
        PropertyMap::~PropertyMap()
        {
            CS_SAFEDELETE_ARRAY(m_propertyBlob);
        }
    }
}