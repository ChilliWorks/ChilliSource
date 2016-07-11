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

#ifndef _CHILLISOURCE_CORE_MEMORY_MEMORYUTILSIMPL_H_
#define _CHILLISOURCE_CORE_MEMORY_MEMORYUTILSIMPL_H_

#include <algorithm>

namespace ChilliSource
{
    namespace MemoryUtils
    {
        //------------------------------------------------------------------------------
        template <typename TValueType, typename TAlignmentType> TValueType* Align(TValueType* value, TAlignmentType alignment) noexcept
        {
            static_assert(std::is_integral<TAlignmentType>::value, "Alignment must be integral type.");
            static_assert(std::is_unsigned<TAlignmentType>::value, "Alignment must be unsigned.");

            CS_ASSERT(IsPowerOfTwo(alignment), "Must be a power of two.");
            
            std::uintptr_t valueIntPtr = reinterpret_cast<std::uintptr_t>(value);
            std::uintptr_t alignmentIntPtr = static_cast<std::uintptr_t>(alignment);

            return reinterpret_cast<TValueType*>((valueIntPtr + alignmentIntPtr - 1) & ~(alignmentIntPtr - 1));
        }

        //------------------------------------------------------------------------------
        template <typename TValueType, typename TAlignmentType> TValueType Align(TValueType value, TAlignmentType alignment) noexcept
        {
            static_assert(std::is_integral<TValueType>::value, "Value must be integral type.");
            static_assert(std::is_unsigned<TValueType>::value, "Value must be unsigned.");
            static_assert(std::is_integral<TAlignmentType>::value, "Alignment must be integral type.");
            static_assert(std::is_unsigned<TAlignmentType>::value, "Alignment must be unsigned.");

            std::uintptr_t valueIntPtr = static_cast<std::uintptr_t>(value);
            std::uintptr_t alignmentIntPtr = static_cast<std::uintptr_t>(alignment);

            return static_cast<TValueType>((valueIntPtr + alignmentIntPtr - 1) & ~(alignmentIntPtr - 1));
        }

        //------------------------------------------------------------------------------
        template <typename TValueType, typename TAlignmentType> TValueType IsAligned(TValueType value, TAlignmentType alignment) noexcept
        {
            static_assert(std::is_integral<TAlignmentType>::value, "Alignment must be integral type.");
            static_assert(std::is_unsigned<TAlignmentType>::value, "Alignment must be unsigned.");

            return static_cast<TValueType>((static_cast<TAlignmentType>(value) & (alignment - 1)) == 0);
        }

        //------------------------------------------------------------------------------
        template <typename TType> constexpr bool IsPowerOfTwo(TType value) noexcept
        {
            static_assert(std::is_integral<TType>::value, "Value must be integral type.");
            static_assert(std::is_unsigned<TType>::value, "Value must be unsigned.");

            return (value > 0) && ((value & (~value + 1)) == value);
        }

        //------------------------------------------------------------------------------
        template <typename TType> TType NextPowerofTwo(TType value) noexcept
        {
            static_assert(std::is_integral<TType>::value, "Value must be integral type.");
            static_assert(std::is_unsigned<TType>::value, "Value must be unsigned.");

            value--;
            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            value++;

            return value;
        }

        //------------------------------------------------------------------------------
        template <typename TType> std::size_t CalcShift(TType value) noexcept
        {
            CS_ASSERT(IsPowerOfTwo(value), "Must be a power of two.");

            TType output = 0;
            while (value > 1)
            {
                output++;
                value >>= 1;
            }

            return output;
        }

        //------------------------------------------------------------------------------
        template <typename TTypeA, typename TTypeB> std::uintptr_t GetPointerOffset(TTypeA* pointer, TTypeB* relativeTo) noexcept
        {
            auto pointerInt = reinterpret_cast<std::uintptr_t>(pointer);
            auto relativeToInt = reinterpret_cast<std::uintptr_t>(relativeTo);

            CS_ASSERT(pointerInt >= relativeToInt, "Pointer must greater than it's parent.");
            return (pointerInt - relativeToInt);
        }

        //------------------------------------------------------------------------------
        template <typename TObject> constexpr std::size_t GetBlockSize() noexcept
        {
            return std::max(sizeof(std::intptr_t) * 2, MemoryUtils::Align(sizeof(TObject), sizeof(std::intptr_t)));
        }
    }
}

#endif