//
//  ValueProperty.h
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_PROPERTY_VALUEPROPERTY_H_
#define _CHILLISOURCE_CORE_CONTAINER_PROPERTY_VALUEPROPERTY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/Property.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// A property that contains an instance of the given type. This is exposed
        /// via the Get() and Set() methods.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        template <typename TType> class ValueProperty final : public Property<TType>
        {
        public:
            //-----------------------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param The type of the property.
            //-----------------------------------------------------------------
            ValueProperty(const PropertyType<TType>* in_type);
            //-----------------------------------------------------------------
            /// Directly sets the underlying property value.
            ///
            /// @author Ian Copland
            ///
            /// @param The new value for the property.
            //-----------------------------------------------------------------
            void Set(const TType& in_value) override;
            //-----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The value of the underlying property.
            //-----------------------------------------------------------------
            TType Get() const override;
            
        private:
            TType m_value;
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
        template <typename TType> ValueProperty<TType>::ValueProperty(const PropertyType<TType>* in_type)
            : Property<TType>(in_type), m_value(in_type->GetDefaultValue())
        {
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> void ValueProperty<TType>::Set(const TType& in_value)
        {
            m_value = in_value;
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> TType ValueProperty<TType>::Get() const
        {
            return m_value;
        }
    }
}

#endif