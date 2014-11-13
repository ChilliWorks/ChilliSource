//
//  ConstMethodCast.h
//  Chilli Source
//  Created by Ian Copland on 13/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_BASE_CONSTMETHODCAST_H_
#define _CHILLISOURCE_CORE_BASE_CONSTMETHODCAST_H_

#include <ChilliSource/ChilliSource.h>

#include <type_traits>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// Casts the const away from a pointer return value of a const method.
        /// This is used to avoid duplicate code when defining both a non-const and
        /// const version of a method. This should only be used in non-const
        /// methods of the class that contains the const member, its use elsewhere
        /// should be considered just as evil as a standard const_cast!
        ///
        /// @author Ian Copland
        ///
        /// @param The object that contains the const method. This should always
        /// be "this" when called, and cannot be a const instance.
        /// @param The method that should be called.
        /// @param The arguments to the method.
        ///
        /// @return The return value from the method with const removed.
        //------------------------------------------------------------------------
        template <typename TClassType, typename TReturnType, typename... TArgs> TReturnType* ConstMethodCast(TClassType* in_object, const TReturnType*(TClassType::*in_method)(TArgs...) const, TArgs&& ...in_args)
        {
            const TReturnType* constReturn = (static_cast<const TClassType*>(in_object)->*in_method)(std::forward<TArgs>(in_args)...);
            return const_cast<TReturnType*>(constReturn);
        }
        //------------------------------------------------------------------------
        /// Casts the const away from a reference return value of a const method.
        /// This is used to avoid duplicate code when defining both a non-const and
        /// const version of a method. This should only be used in non-const
        /// methods of the class that contains the const member, its use elsewhere
        /// should be considered just as evil as a standard const_cast!
        ///
        /// @author Ian Copland
        ///
        /// @param The object that contains the const method. This should always
        /// be "this" when called, and cannot be a const instance.
        /// @param The method that should be called.
        /// @param The arguments to the method.
        ///
        /// @return The return value from the method with const removed.
        //------------------------------------------------------------------------
        template <typename TClassType, typename TReturnType, typename... TArgs> TReturnType& ConstMethodCast(TClassType* in_object, const TReturnType&(TClassType::*in_method)(TArgs...) const, TArgs&& ...in_args)
        {
            const TReturnType& constReturn = (static_cast<const TClassType*>(in_object)->*in_method)(std::forward<TArgs>(in_args)...);
            return const_cast<TReturnType&>(constReturn);
        }
        //------------------------------------------------------------------------
        /// Casts the const away from a shared pointer return value of a const method.
        /// This is used to avoid duplicate code when defining both a non-const and
        /// const version of a method. This should only be used in non-const
        /// methods of the class that contains the const member, its use elsewhere
        /// should be considered just as evil as a standard const_cast!
        ///
        /// @author Ian Copland
        ///
        /// @param The object that contains the const method. This should always
        /// be "this" when called, and cannot be a const instance.
        /// @param The method that should be called.
        /// @param The arguments to the method.
        ///
        /// @return The return value from the method with const removed.
        //------------------------------------------------------------------------
        template <typename TClassType, typename TReturnType, typename... TArgs> std::shared_ptr<TReturnType> ConstMethodCast(TClassType* in_object, std::shared_ptr<const TReturnType>(TClassType::*in_method)(TArgs...) const, TArgs&& ...in_args)
        {
            std::shared_ptr<const TReturnType> constReturn = (static_cast<const TClassType*>(in_object)->*in_method)(std::forward<TArgs>(in_args)...);
            return std::const_pointer_cast<TReturnType>(constReturn);
        }
    }
}

#endif
