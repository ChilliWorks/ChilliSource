//
//  PropertyMap.h
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

#ifndef _CHILLISOURCE_UI_BASE_PROPERTYMAP_H_
#define _CHILLISOURCE_UI_BASE_PROPERTYMAP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/UI/Base/PropertyType.h>

#include <unordered_map>
#include <vector>

namespace ChilliSource
{
	namespace UI
	{
        //---------------------------------------------------------------------------------
        /// Simplified version of a map data-structure that is specialised for UI property
        /// types. The keys can only be set at construction time as the map has a fixed
        /// size and is contiguous. Unlike STL maps, the property map can handle heterogenous
        /// types
        ///
        /// @author S Downie
        //---------------------------------------------------------------------------------
        class PropertyMap final
        {
        public:
            
            //----------------------------------------------------
            /// Holds the definition of a custom property.
            ///
            /// @author S Downie
            //----------------------------------------------------
            struct PropertyDesc
            {
                PropertyType m_type;
                std::string m_name;
                std::string m_value;
            };
            //----------------------------------------------------------------------------------------
            /// Constructor - Note using this constructor will not initialise the map and needs
            /// to be used with AllocateKeys
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            PropertyMap() = default;
            //----------------------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param List of the key names and types
            //----------------------------------------------------------------------------------------
            PropertyMap(const std::vector<PropertyDesc>& in_propertyDefs);
            //----------------------------------------------------------------------------------------
            /// Move constructor
            ///
            /// @author S Downie
            ///
            /// @param Object to move to this
            //----------------------------------------------------------------------------------------
            PropertyMap(PropertyMap&& in_move);
            //----------------------------------------------------------------------------------------
            /// Copy constructor
            ///
            /// @author S Downie
            ///
            /// @param Object to copy to this
            //----------------------------------------------------------------------------------------
            PropertyMap(const PropertyMap& in_copy);
            //----------------------------------------------------------------------------------------
            /// Move assignment
            ///
            /// @author S Downie
            ///
            /// @param Object to move to this
            ///
            /// @return this
            //----------------------------------------------------------------------------------------
            PropertyMap& operator=(PropertyMap&& in_move);
            //----------------------------------------------------------------------------------------
            /// Copy assignment
            ///
            /// @author S Downie
            ///
            /// @param Object to copy to this
            ///
            /// @return this
            //----------------------------------------------------------------------------------------
            PropertyMap& operator=(const PropertyMap& in_copy);
            //----------------------------------------------------------------------------------------
            /// Set the keys which will allocate space for the values
            ///
            /// @author S Downie
            ///
            /// @param List of the key names and types
            //----------------------------------------------------------------------------------------
            void AllocateKeys(const std::vector<PropertyDesc>& in_propertyDefs);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Property name
            ///
            /// @return Whether the property key exists
            //----------------------------------------------------------------------------------------
            bool HasProperty(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// Property values are often passed around as homogenous strings and this function
            /// will parse the string and assign the property with the correct type
            ///
            /// @author S Downie
            ///
            /// @param Property type
            /// @param Property name
            /// @param Property value to parse
            //----------------------------------------------------------------------------------------
            void SetProperty(PropertyType in_type, const std::string& in_name, const std::string& in_value);
            //----------------------------------------------------------------------------------------
            /// Set the value of the property with the given name. If no property exists
            /// with the name then it will assert.
            ///
            /// @author S Downie
            ///
            /// @param Name
            /// @param Value
            //----------------------------------------------------------------------------------------
            template<typename TType> void SetProperty(const std::string& in_name, TType in_value);
            //----------------------------------------------------------------------------------------
            /// Get the value of the property with the given name. If no property exists
            /// with the name then it will assert.
            ///
            /// @author S Downie
            ///
            /// @param Name
            ///
            /// @return Value
            //----------------------------------------------------------------------------------------
            template<typename TType> TType GetProperty(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Property name
            ///
            /// @return The type of the property with the given name
            //----------------------------------------------------------------------------------------
            PropertyType GetType(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// Get the value of the property with the given name. If no property exists
            /// with the name then the default value will be returned
            ///
            /// @author S Downie
            ///
            /// @param Name
            ///
            /// @return Value (or default)
            //----------------------------------------------------------------------------------------
            template<typename TType> TType GetPropertyOrDefault(const std::string& in_name, TType in_default) const;
            //----------------------------------------------------------------------------------------
            /// Remove all the properties and keys
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            void Clear();
            //----------------------------------------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            ~PropertyMap();
            
        private:
            
            //----------------------------------------------------------------------------------------
            /// Holds the type of the property and the offset into the blob
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            struct PropertyLookup
            {
                PropertyType m_type;
                void* m_value;
            };
            //----------------------------------------------------------------------------------------
            /// Converts the object type to proprty type and throws compiler error for unsupported
            /// types
            ///
            /// @author S Downie
            ///
            /// @return The internal enum property type of the given object type
            //----------------------------------------------------------------------------------------
            template<typename TType> PropertyType GetType() const;
            
            std::unordered_map<u32, PropertyLookup> m_properties;
        };
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> void PropertyMap::SetProperty(const std::string& in_name, TType in_value)
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            auto entry = m_properties.find(hashKey);
            CS_ASSERT(entry != m_properties.end(), "No UI property with name: " + in_name);
            CS_ASSERT(entry->second.m_type == GetType<TType>(), "Wrong type for property with name " + in_name);
            
            TType* property = (TType*)entry->second.m_value;
            *property = std::move(in_value);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> TType PropertyMap::GetProperty(const std::string& in_name) const
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            auto entry = m_properties.find(hashKey);
            CS_ASSERT(entry != m_properties.end(), "No UI property with name: " + in_name);
            CS_ASSERT(entry->second.m_type == GetType<TType>(), "Wrong type for property with name " + in_name);
            
            TType* property = (TType*)entry->second.m_value;
            return *property;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> TType PropertyMap::GetPropertyOrDefault(const std::string& in_name, TType in_default) const
        {
            u32 hashKey = Core::HashCRC32::GenerateHashCode(in_name);
            
            auto entry = m_properties.find(hashKey);
            if(entry == m_properties.end())
            {
                return in_default;
            }
            
            CS_ASSERT(entry->second.m_type == GetType<TType>(), "Wrong type for property with name " + in_name);
            
            TType* property = (TType*)entry->second.m_value;
            return *property;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> PropertyType PropertyMap::GetType() const
        {
            static_assert(std::is_pointer<TType>::value, "Property type not supported");
            return PropertyType::k_unknown;
        }
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for bool
        ///
        /// @author S Downie
        ///
        /// @return Bool prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<bool>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for int
        ///
        /// @author S Downie
        ///
        /// @return Int prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<s32>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return String prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<std::string>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return String prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const std::string&>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for string
        ///
        /// @author S Downie
        ///
        /// @return String prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const char*>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for float
        ///
        /// @author S Downie
        ///
        /// @return Float prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<f32>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for vec2
        ///
        /// @author S Downie
        ///
        /// @return Vec2 prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::Vector2>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for vec2
        ///
        /// @author S Downie
        ///
        /// @return Vec2 prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const Core::Vector2&>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for vec3
        ///
        /// @author S Downie
        ///
        /// @return Vec3 prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::Vector3>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for vec3
        ///
        /// @author S Downie
        ///
        /// @return Vec3 prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const Core::Vector3&>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for vec4
        ///
        /// @author S Downie
        ///
        /// @return Vec4 prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::Vector4>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for vec4
        ///
        /// @author S Downie
        ///
        /// @return Vec4 prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const Core::Vector4&>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for colour
        ///
        /// @author S Downie
        ///
        /// @return Colour prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Core::Colour>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for colour
        ///
        /// @author S Downie
        ///
        /// @return Colour prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const Core::Colour&>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for anchors
        ///
        /// @author S Downie
        ///
        /// @return Anchor prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<Rendering::AlignmentAnchor>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for size policy
        ///
        /// @author S Downie
        ///
        /// @return Size policy prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<SizePolicy>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for property maps
        ///
        /// @author S Downie
        ///
        /// @return Property map prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<PropertyMap>() const;
        //----------------------------------------------------------------------------------------
        /// Specialisation to return property type for property maps
        ///
        /// @author S Downie
        ///
        /// @return Property map prop type
        //----------------------------------------------------------------------------------------
        template<> PropertyType PropertyMap::GetType<const PropertyMap&>() const;
	}
}

#endif
