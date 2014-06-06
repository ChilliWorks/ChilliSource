//
//  MakeSharedArray.h
//  Chilli Source
//  Created by Ian Copland on 26/02/2014.
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

#ifndef _CHILLISOURCE_CORE_BASE_MAKESHAREDARRAY_H_
#define _CHILLISOURCE_CORE_BASE_MAKESHAREDARRAY_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        //-----------------------------------------------------
        /// A function object for deleting an array. This is
        /// used to safely delete shared pointers to arrays.
        //-----------------------------------------------------
        template <typename T> class ArrayDeleter
        {
        public:
            //----------------------------------------------
            /// Deletes the given array.
            //----------------------------------------------
            void operator () (T* in_arrayObject) const
            {
                delete[] in_arrayObject;
            }
        };
        //-----------------------------------------------------
        /// Creates a new shared pointer to an array of the
        /// given object. Uses a custom deleter to ensure the
        /// array is correctly deleted.
        ///
        /// @author Ian Copland
        ///
        /// @param The size of the array.
        /// @return The new shared pointer.
        //-----------------------------------------------------
        template <typename TType> std::shared_ptr<TType> MakeSharedArray(s32 in_size)
        {
            return std::shared_ptr<TType>(new TType[in_size], ArrayDeleter<TType>());
        }
    }
}

#endif
