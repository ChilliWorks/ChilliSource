//
//  IPropertyType.h
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
#ifndef _CHILLISOURCE_CORE_CONTAINER_PROPERTY_IPROPERTYTYPE_H_
#define _CHILLISOURCE_CORE_CONTAINER_PROPERTY_IPROPERTYTYPE_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// A container for information on a specific type of property. The
        /// specific property type is described by the implementing class. This
        /// is typically used for type erasure so that a series of heterogeneous
        /// property types can be stored in the same container. This allows
        /// creation of instances of the property described without knowing the
        /// specific type.
        ///
        /// Properties retain a pointer to the Property Type that created them
        /// which means the property type must outlive any properties created
        /// through it. Typically Property Types are allocated statically to
        /// guarantee this.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        class IPropertyType
        {
        public:
            //-----------------------------------------------------------------
            /// Creates a new instance of the described property type. This
            /// does not require knowledge of the specific property type.
            ///
            /// @author Ian Copland
            ///
            /// @return The string name of the underlying type.
            //-----------------------------------------------------------------
            virtual IPropertyUPtr CreateProperty() const = 0;
            //-----------------------------------------------------------------
            /// Virtual destructor.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------------------
            virtual ~IPropertyType() {};
        };
    }
}

#endif