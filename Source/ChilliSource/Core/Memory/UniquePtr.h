//
// The MIT License(MIT)
// 
// Copyright(c) 2016 Ian Copland
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef _CHILLISOURCE_CORE_MEMORY_UNIQUEPTR_H_
#define _CHILLISOURCE_CORE_MEMORY_UNIQUEPTR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/IAllocator.h>

#include <functional>
#include <memory>

namespace ChilliSource
{
    template <typename TType> using UniquePtr = std::unique_ptr<TType, std::function<void(typename std::add_const<typename std::remove_all_extents<TType>::type>::type*)>>;

    /// Allocates a new unique pointer from the given Allocator with the given 
    /// constructor parameters. This follows the make_* convention set in the 
    /// standard library.
    ///
    /// @param allocator
    ///     The allocator from which to allocate the requested type.
    /// @param constructorArgs
    ///     The arguments for the constructor if appropriate.
    ///
    /// @return A unique pointer to the allocated instance.
    ///
    template <typename TType, typename... TConstructorArgs> UniquePtr<TType> MakeUnique(IAllocator& allocator, TConstructorArgs&&... constructorArgs) noexcept;

    /// Allocates a new unique pointer to an array from the given Allocator with 
    /// the given size. This follows the make_* convention  set in the standard 
    /// library. Note that, like new[], fundamental types  will not be set to a 
    /// default value. 
    ///
    /// @param allocator
    ///     The allocator from which to allocate the requested type.
    /// @param size
    ///     The size of the array.
    ///
    /// @return A unique pointer to the allocated array.
    ///
    template <typename TType> UniquePtr<TType[]> MakeUniqueArray(IAllocator& allocator, std::size_t size) noexcept;
}

#include "UniquePtrImpl.h"

#endif
