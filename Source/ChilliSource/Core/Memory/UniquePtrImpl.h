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

#ifndef _CHILLISOURCE_CORE_MEMORY_UNIQUEPTRIMPL_H_
#define _CHILLISOURCE_CORE_MEMORY_UNIQUEPTRIMPL_H_

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    template <typename TType, typename... TConstructorArgs> UniquePtr<TType> MakeUnique(IAllocator& allocator, TConstructorArgs&&... constructorArgs) noexcept
    {
        void* memory = allocator.Allocate(sizeof(TType));
        TType* object = new (memory) TType(std::forward<TConstructorArgs>(constructorArgs)...);
        return UniquePtr<TType>(object, [&allocator](const TType* object) noexcept -> void
        {
            object->~TType();
            allocator.Deallocate((void*)(object), sizeof(TType));
        });
    }

    //------------------------------------------------------------------------------
    template <typename TType> UniquePtr<TType[]> MakeUniqueArray(IAllocator& allocator, std::size_t size) noexcept
    {
        auto array = reinterpret_cast<TType*>(allocator.Allocate(sizeof(TType) * size));
        if (!std::is_fundamental<TType>::value)
        {
            for (std::size_t i = 0; i < size; ++i)
            {
                new (array + i) TType();
            }
        }

        return UniquePtr<TType[]>(array, [&allocator, size](const TType* array) noexcept -> void
        {
            if (!std::is_fundamental<TType>::value)
            {
                for (std::size_t i = 0; i < size; ++i)
                {
                    (array + i)->~TType();
                }
            }

            allocator.Deallocate((void*)(array), sizeof(TType) * size);
        });
    }
}

#endif
