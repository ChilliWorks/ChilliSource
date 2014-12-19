//
//  ReferenceProperty.h
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
#ifndef _CHILLISOURCE_CORE_CONTAINER_PROPERTY_REFERENCEPROPERTY_H_
#define _CHILLISOURCE_CORE_CONTAINER_PROPERTY_REFERENCEPROPERTY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/Property.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// A reference property. This does not contain a specific value for the
        /// instead referencing another value accessible though the provided
        /// getter and setter delegates.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        template <typename TType> class ReferenceProperty final : public Property<TType>
        {
        public:
            //-----------------------------------------------------------------
            /// The getter delegate used when calling Get() on the property.
            ///
            /// @author Ian Copland
            ///
            /// @return The current value of the property.
            //-----------------------------------------------------------------
            using GetterDelegate = std::function<TType()>;
            //-----------------------------------------------------------------
            /// The setter delegate used when calling Set() on the property.
            ///
            /// @author Ian Copland
            ///
            /// @param The new value for the property.
            //-----------------------------------------------------------------
            using SetterDelegate = std::function<void(TType)>;
            //-----------------------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param The type of property.
            /// @param The getter delegate.
            /// @param The setter delegate.
            //-----------------------------------------------------------------
            ReferenceProperty(const PropertyType<TType>* in_type, const GetterDelegate& in_getter, const SetterDelegate& in_setter);
            //-----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The value provided by calling the getter delegate.
            //-----------------------------------------------------------------
            TType Get() const override;
            //-----------------------------------------------------------------
            /// Sets the property value of the property by calling the setter
            /// delegate.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------------------
            void Set(const TType& in_value) override;
            
        private:
            GetterDelegate m_getter;
            SetterDelegate m_setter;
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
        template <typename TType> ReferenceProperty<TType>::ReferenceProperty(const PropertyType<TType>* in_type, const GetterDelegate& in_getter, const SetterDelegate& in_setter)
        : Property<TType>(in_type), m_getter(in_getter), m_setter(in_setter)
        {
            CS_ASSERT(m_getter != nullptr, "Property getter delegate cannot be null.");
            CS_ASSERT(m_setter != nullptr, "Property setter delegate cannot be null.");
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> void ReferenceProperty<TType>::Set(const TType& in_value)
        {
            m_setter(in_value);
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        template <typename TType> TType ReferenceProperty<TType>::Get() const
        {
            return m_getter();
        }
    }
}

#endif