//
//  MakeSharedArray.h
//  Chilli Source
//
//  Created by Ian Copland on 26/02/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
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
