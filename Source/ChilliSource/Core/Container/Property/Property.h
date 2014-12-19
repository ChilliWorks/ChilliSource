//
//  Property.h
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
#ifndef _CHILLISOURCE_CORE_CONTAINER_PROPERTY_PROPERTY_H_
#define _CHILLISOURCE_CORE_CONTAINER_PROPERTY_PROPERTY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/IProperty.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// A specific type of property. This implements the IProperty interface
        /// and exposes the specific type of the underlying value. Methods for
        /// setting and getting the value are provided while abtracting the
        /// literal storage of the value.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        template <typename TType> class Property : public IProperty
        {
        public:
            //-----------------------------------------------------------------
            /// Constructor. Constructs the property with the given type
            /// description.
            ///
            /// @author Ian Copland
            ///
            /// @param The type description.
            //-----------------------------------------------------------------
            Property(const PropertyType<TType>* in_type);
            //-----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The property type description.
            //-----------------------------------------------------------------
            const IPropertyType* GetType() const override;
            //-----------------------------------------------------------------
            /// A basic getter for the value of the property. The literal storage
            /// of the value is determined by the implementing type.
            ///
            /// @author Ian Copland
            ///
            /// @param The value to set the property to.
            //-----------------------------------------------------------------
            virtual TType Get() const = 0;
            //-----------------------------------------------------------------
            /// Parses the given string and sets the property to the parsed
            /// value.
            ///
            /// @author Ian Copland
            ///
            /// @param The string to parse.
            //-----------------------------------------------------------------
            void Parse(const std::string& in_stringValue) override;
            //-----------------------------------------------------------------
            /// Sets the value of the property to that of the given property.
            /// The given property must be of the same type, otherwise the app
            /// is considered to be in an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The property whose value this will be set to.
            //-----------------------------------------------------------------
            void Set(const IProperty* in_property) override;
            //-----------------------------------------------------------------
            /// A basic setter for the value of the property. The literal storage
            /// of the value is determined by the implementing type.
            ///
            /// @author Ian Copland
            ///
            /// @param The value to set the property to.
            //-----------------------------------------------------------------
            virtual void Set(const TType& in_value) = 0;
            
        private:
            const PropertyType<TType>* m_type;
        };
    }
}

//------------------------------------------------------------------------
// These are included here to avoid circular inclusion issues. At this
// stage the class has been defined which is enough for the classes
// included to use it.
//------------------------------------------------------------------------
#include <ChilliSource/Core/Container/Property/PropertyType.h>

namespace ChilliSource
{
    namespace Core
    {
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> Property<TType>::Property(const PropertyType<TType>* in_type)
            : m_type(in_type)
        {
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> const IPropertyType* Property<TType>::GetType() const
        {
            return m_type;
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> void Property<TType>::Parse(const std::string& in_string)
        {
            auto parser = m_type->GetParseDelegate();
            CS_ASSERT(parser != nullptr, "Cannot parse this property type from string.");
            
            TType value = parser(in_string);
            Set(value);
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> void Property<TType>::Set(const IProperty* in_property)
        {
            CS_ASSERT(in_property->GetType() == GetType(), "Cannot set using the contents of another property created from a different Property Type.");
            
            const Property<TType>* castProperty = CS_SMARTCAST(const Property<TType>*, in_property, "Cannot set a property of a different property type.");
            Set(castProperty->Get());
        }
    }
}

#endif