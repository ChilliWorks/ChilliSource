//
//  IProperty.h
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_PROPERTY_IPROPERTY_H_
#define _CHILLISOURCE_CORE_CONTAINER_PROPERTY_IPROPERTY_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
        //------------------------------------------------------------------------
        /// A container for a value of a type determined by the implementing class.
        /// This is mostly used for type erasuring to store heterogeneous types in
        /// the same container. It also provides indirect method of setting the
        /// value without knowing the erased type, both through string parsing and
        /// using another type erased property.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        class IProperty
        {
        public:
            //-----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The underlying property type description. This can be
            /// used to re-establish the underlying type.
            //-----------------------------------------------------------------
            virtual const IPropertyType* GetType() const = 0;
            //-----------------------------------------------------------------
            /// Parses the given string and sets the property to the parsed
            /// value.
            ///
            /// @author Ian Copland
            ///
            /// @param The string to parse.
            //-----------------------------------------------------------------
            virtual void Parse(const std::string& in_stringValue) = 0;
            //-----------------------------------------------------------------
            /// Sets the value of the property to that of the given property.
            /// The two properties must be of the same type, otherwise the app
            /// is considered to be in an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The property whose value this will be set to.
            //-----------------------------------------------------------------
            virtual void Set(const IProperty* in_property) = 0;
            //-----------------------------------------------------------------
            /// Virtual destructor.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------------------
            virtual ~IProperty() {}
        };
    }
}

#endif
