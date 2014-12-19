//
//  PropertyType.h
//  Chilli Source
//  Created by Ian Copland on 03/12/2014.
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
#ifndef _CHILLISOURCE_CORE_CONTAINER_PROPERTY_PROPERTYTYPE_H_
#define _CHILLISOURCE_CORE_CONTAINER_PROPERTY_PROPERTYTYPE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/IPropertyType.h>

#include <functional>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// A container for a specific type of property. This exposes a method
        /// for parsing instances of the type, the default value for the type and
        /// methods for creating properties of this type. A property type is
        /// immutable after construction.
        ///
        /// Properties retain a pointer to the Property Type that created them
        /// which means the property type must outlive any properties created
        /// through it. Typically Property Types are allocated statically to
        /// guarantee this.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        template <typename TType> class PropertyType : public IPropertyType
        {
        public:
            //-----------------------------------------------------------------
            /// A delegate used to parse objects of the given type from a
            /// string.
            ///
            /// @author Ian Copland
            ///
            /// @param The string to parse.
            ///
            /// @return The parsed value.
            //-----------------------------------------------------------------
            using ParseDelegate = std::function<TType(const std::string& in_stringToParse)>;
            //-----------------------------------------------------------------
            /// The value accessor delegate that will be used to get the value
            /// of a reference property.
            ///
            /// @author Ian Copland
            ///
            /// @return The value of the property.
            //-----------------------------------------------------------------
            using GetterDelegate = std::function<TType()>;
            //-----------------------------------------------------------------
            /// The value accessor delegate that will be used to set the value
            /// of a reference property.
            ///
            /// @author Ian Copland
            ///
            /// @param The value of the property.
            //-----------------------------------------------------------------
            using SetterDelegate = std::function<void(TType)>;
            //-----------------------------------------------------------------
            /// Constructor. Creates a new property type with the given setting.
            /// After construction it is immutable.
            ///
            /// @author Ian Copland
            ///
            /// @param The default value for the property type.
            /// @param The string parsing method for the type. This can be null
            /// if the type cannot be parsed from string.
            //-----------------------------------------------------------------
            PropertyType(TType&& in_defaultValue, const ParseDelegate& in_parseDelegate);
            //-----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The default value for an instance of the property.
            //-----------------------------------------------------------------
            TType GetDefaultValue() const;
            //-----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The delegate which will be used to parse the value of
            /// a property of this type from a string.
            //-----------------------------------------------------------------
            const ParseDelegate& GetParseDelegate() const;
            //-----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A new instance of a property of the type described by
            /// this, set to the default value.
            //-----------------------------------------------------------------
            std::unique_ptr<IProperty> CreateProperty() const override;
            //-----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The getter delegate.
            /// @param The setter delegate.
            ///
            /// @return A new instance of a property of the type described by
            /// this with the given accessor delegates. The default value will
            /// not be used, instead leaving the referenced value as it current
            /// is.
            //-----------------------------------------------------------------
            std::unique_ptr<IProperty> CreateProperty(GetterDelegate&& in_getter, SetterDelegate&& in_setter) const;
            
        private:
            TType m_defaultValue;
            ParseDelegate m_parseDelegate;
        };
    }
}

//------------------------------------------------------------------------
// These are included here to avoid circular inclusion issues. At this
// stage the class has been defined which is enough for the classes
// included to use it.
//------------------------------------------------------------------------
#include <ChilliSource/Core/Container/Property/ReferenceProperty.h>
#include <ChilliSource/Core/Container/Property/ValueProperty.h>

namespace ChilliSource
{
    namespace Core
    {
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> PropertyType<TType>::PropertyType(TType&& in_defaultValue, const ParseDelegate& in_parseDelegate)
            : m_defaultValue(std::forward<TType>(in_defaultValue)), m_parseDelegate(in_parseDelegate)
        {
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> TType PropertyType<TType>::GetDefaultValue() const
        {
            return m_defaultValue;
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> const typename PropertyType<TType>::ParseDelegate& PropertyType<TType>::GetParseDelegate() const
        {
            return m_parseDelegate;
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> IPropertyUPtr PropertyType<TType>::CreateProperty() const
        {
            return IPropertyUPtr(new ValueProperty<TType>(this));
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> IPropertyUPtr PropertyType<TType>::CreateProperty(GetterDelegate&& in_getter, SetterDelegate&& in_setter) const
        {
            return IPropertyUPtr(new ReferenceProperty<TType>(this, std::forward<GetterDelegate>(in_getter), std::forward<SetterDelegate>(in_setter)));
        }
    }
}

#define CS_DECLARE_PROPERTYTYPE(in_type, in_name) \
    const CSCore::PropertyType<in_type>* in_name();

#define CS_DEFINE_PROPERTYTYPE(in_type, in_name, in_defaultValue, in_parseMethod) \
    const CSCore::PropertyType<in_type>* in_name() \
    { \
        static const CSCore::PropertyType<in_type> k_property(in_defaultValue, in_parseMethod); \
        return &k_property; \
    }

#endif