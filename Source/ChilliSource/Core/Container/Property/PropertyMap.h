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

#ifndef _CHILLISOURCE_CORE_CONTAINER_PROPERTY_PROPERTYMAP_H_
#define _CHILLISOURCE_CORE_CONTAINER_PROPERTY_PROPERTYMAP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Container/Property/Property.h>

#include <cassert>
#include <unordered_map>
#include <vector>

namespace ChilliSource
{
	namespace Core
	{
        //---------------------------------------------------------------------------------
        /// Simplified version of a map data-structure. The keys can only be set at
        /// construction time as the map has a fixed size and is contiguous. Unlike STL
        /// maps, the property map can handle heterogenous types.
        ///
        /// Properties are created without a value. GetProperty() should only be used on
        /// a property after SetProperty() has been called for it. HasValue() can be used
        /// to check if a value has been set.
        ///
        /// @author S Downie
        //---------------------------------------------------------------------------------
        class PropertyMap final
        {
        public:
            //----------------------------------------------------------------------------------------
            /// Holds the definition of a custom property.
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            struct PropertyDesc
            {
                const IPropertyType* m_type;
                std::string m_name;
            };
            //----------------------------------------------------------------------------------------
            /// Constructor. Creates a property map with no keys.
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
            /// @author Ian Copland
            ///
            /// @return The list of keys in the property map.
            //----------------------------------------------------------------------------------------
            const std::vector<std::string>& GetKeys() const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Property name
            ///
            /// @return Whether the property key exists
            //----------------------------------------------------------------------------------------
            bool HasKey(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The property name
            ///
            /// @return Whether or not property with the given key has a value. This will error if the
            /// property doesn't exist.
            //----------------------------------------------------------------------------------------
            bool HasValue(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// Set the value of the property with the given name. If no property exists with the
            /// name then it will assert.
            ///
            /// @author S Downie
            ///
            /// @param Name
            /// @param Value
            //----------------------------------------------------------------------------------------
            template<typename TType> void SetProperty(const std::string& in_name, TType&& in_value);
            //----------------------------------------------------------------------------------------
            /// Specialisation to store property value for const char* as a std::string
            ///
            /// @author S Downie
            ///
            /// @param Property name
            /// @param Property value
            //----------------------------------------------------------------------------------------
            void SetProperty(const std::string& in_name, const char* in_value);
            //----------------------------------------------------------------------------------------
            /// Sets the property with the given name to the result of parsing the given string.
            /// Not all property types can be parsed depending on whether or not the type specifies
            /// a parse delegate. If not or a property with the given name doesn't exist the app is
            /// considered to be in an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the property.
            /// @param The value to set in string form.
            //----------------------------------------------------------------------------------------
            void ParseProperty(const std::string& in_name, const std::string& in_value);
            //----------------------------------------------------------------------------------------
            /// Get the value of the property with the given name. If no property exists with the name
            /// or the property doesn't yet have a value, with the name then it will assert.
            ///
            /// @author S Downie
            ///
            /// @param Name
            ///
            /// @return Value
            //----------------------------------------------------------------------------------------
            template<typename TType> TType GetProperty(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// Get the value of the property with the given name. If the property has not been set
            /// the default will be returned instead. If the property doesn't exist it will assert.
            ///
            /// @author S Downie
            ///
            /// @param Name
            ///
            /// @return Value (or default)
            //----------------------------------------------------------------------------------------
            template<typename TType> TType GetPropertyOrDefault(const std::string& in_name, TType&& in_default) const;
            //----------------------------------------------------------------------------------------
            /// Specialisation to return property value for const char* which is stored as a std::string
            ///
            /// @author S Downie
            ///
            /// @param Property name
            /// @param Default value
            ///
            /// @return Property value or default if name not found
            //----------------------------------------------------------------------------------------
			std::string GetPropertyOrDefault(const std::string& in_name, const char* in_default) const;
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Property name
            ///
            /// @return The type of the property with the given name
            //----------------------------------------------------------------------------------------
            const IPropertyType* GetType(const std::string& in_name) const;
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The property name.
            ///
            /// @return The underlying property object for the given key. If there is no property
            /// with the given name, or the property has no value this will assert.
            //----------------------------------------------------------------------------------------
            IProperty* GetPropertyObject(const std::string& in_name);
            //----------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The property name.
            ///
            /// @return A constant version of the underlying property object for the given key. If
            /// there is no property with the given name, or the property has no value this will assert.
            //----------------------------------------------------------------------------------------
            const IProperty* GetPropertyObject(const std::string& in_name) const;
            
        private:
            //----------------------------------------------------------------------------------------
            /// A container for the property and a boolean to state whether or not the property has
            /// been given a value.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------------------------------
            struct PropertyContainer
            {
				CS_DECLARE_NOCOPY(PropertyContainer);
				//----------------------------------------------------------------------------------------
				/// Default constructor.
				///
				/// @author Ian Copland
				//----------------------------------------------------------------------------------------
				PropertyContainer() = default;
				//----------------------------------------------------------------------------------------
				/// Move constructor. The implicit move constructor isn't enough on windows so it needs to
				/// be manually specified.
				///
				/// @author Ian Copland
				///
				/// @param The container to move.
				//----------------------------------------------------------------------------------------
				PropertyContainer(PropertyContainer&& in_move);
				//----------------------------------------------------------------------------------------
				/// Move asignment operator. The implicit move assignment isn't enough on windows so it 
				/// needs to be manually specified.
				///
				/// @author Ian Copland
				///
				/// @param The container to move.
				///
				/// @return A reference to this.
				//----------------------------------------------------------------------------------------
				PropertyContainer& operator=(PropertyContainer&& in_move);

                bool m_initialised = false;
                IPropertyUPtr m_property;
            };
    
            std::vector<std::string> m_propertyKeys;
            std::unordered_map<std::string, PropertyContainer> m_properties;
        };
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> void PropertyMap::SetProperty(const std::string& in_name, TType&& in_value)
        {
            typedef typename std::decay<TType>::type TValueType;
            
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            auto entry = m_properties.find(lowerCaseName);
            CS_ASSERT(entry != m_properties.end(), "No property in property map with name: " + in_name);

            Property<TValueType>* property = CS_SMARTCAST(Property<TValueType>*, entry->second.m_property.get(), "Wrong type for property with name: " + in_name);
            property->Set(std::forward<TType>(in_value));
            entry->second.m_initialised = true;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> TType PropertyMap::GetProperty(const std::string& in_name) const
        {
            typedef typename std::decay<TType>::type TValueType;
            
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            auto entry = m_properties.find(lowerCaseName);
            CS_ASSERT(entry != m_properties.end(), "No property in property map with name: " + in_name);
            CS_ASSERT(entry->second.m_initialised == true, "Cannot get the value for an uninitialised property.");
            
            Property<TValueType>* property = CS_SMARTCAST(Property<TValueType>*, entry->second.m_property.get(), "Wrong type for property with name: " + in_name);
            return property->Get();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        template<typename TType> TType PropertyMap::GetPropertyOrDefault(const std::string& in_name, TType&& in_default) const
        {
            typedef typename std::decay<TType>::type TValueType;
            
            std::string lowerCaseName = in_name;
            Core::StringUtils::ToLowerCase(lowerCaseName);
            
            auto entry = m_properties.find(lowerCaseName);
            CS_ASSERT(entry != m_properties.end(), "No property in property map with name: " + in_name);
            
            Property<TValueType>* property = CS_SMARTCAST(Property<TValueType>*, entry->second.m_property.get(), "Wrong type for property with name: " + in_name);
            if (entry->second.m_initialised == true)
            {
                return property->Get();
            }
            
            return in_default;
        }
    }
}

#endif
