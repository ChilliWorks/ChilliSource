//
// The MIT License(MIT)
// 
// Copyright(c) 2015 Ian Copland
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

#ifndef _CHILLISOURCE_CORE_MEMORY_MEMORYUTILS_H_
#define _CHILLISOURCE_CORE_MEMORY_MEMORYUTILS_H_

#include <ChilliSource/ChilliSource.h>

#include <cstdint>
#include <cassert>
#include <type_traits>

namespace ChilliSource
{
    namespace MemoryUtils
    {
        /// Aligns the given pointer to the given alignment. The alignment should be a power
        /// of two.
        ///
        /// @param value
        ///     The value to align.
        /// @param alignment
        ///     The alignment.
        ///
        /// @return The aligned pointer.
        ///
        template <typename TValueType, typename TAlignmentType> TValueType* Align(TValueType* value, TAlignmentType alignment) noexcept;

        /// Aligns the given integer to the given alignment. The alignment should be a power
        /// of two.
        ///
        /// @param value
        ///     The value to align.
        /// @param alignment
        ///     The alignment.
        ///
        /// @return The aligned integer.
        ///
        template <typename TValueType, typename TAlignmentType> TValueType Align(TValueType value, TAlignmentType alignment) noexcept;

        /// @param value
        ///     The value.
        /// @param alignment
        ///     The alignment.
        ///
        /// @return Whether or not the given integer is aligned.
        ///
        template <typename TValueType, typename TAlignmentType> TValueType IsAligned(TValueType value, TAlignmentType alignment) noexcept;

        /// @param value
        ///     The value to check.
        ///
        /// @return Whether or not the given integer is a power of two.
        ///
        template <typename TType> constexpr bool IsPowerOfTwo(TType value) noexcept;

        /// @param value
        ///     The value.
        ///
        /// @return The next power ot two on from the given value. This only supports 32-bit values.
        ///
        template <typename TType> TType NextPowerofTwo(TType value) noexcept;

        /// @param value
        ///     The value. Must be a power of two.
        ///
        /// @return The number of times 0x1 has to be shifted to get the given value. 
        ///
        template <typename TType> std::size_t CalcShift(TType value) noexcept;

        /// @param pointer
        ///     The pointer.
        /// @param relativeTo
        ///     The pointer that the other is relative to. Must have a lower memory address
        ///     than the value.
        ///
        /// @return The offset in bytes between the two pointers. 
        ///
        template <typename TTypeA, typename TTypeB> std::uintptr_t GetPointerOffset(TTypeA* pointer, TTypeB* relativeTo) noexcept;

        /// Calculates the size of block required for the given object type
        ///
        /// @return The block size.
        ///
        template <typename TObject> constexpr std::size_t GetBlockSize() noexcept;
    }
}

#include "MemoryUtilsImpl.h"

#endif