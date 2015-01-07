//
//  random_access_iterator.h
//  Chilli Source
//  Created by Ian Copland on 27/12/2014.
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

#ifndef _CHILLISOURCE_CORE_CONTAINERS_RANDOMACCESSITERATOR_H_
#define _CHILLISOURCE_CORE_CONTAINERS_RANDOMACCESSITERATOR_H_

#include <ChilliSource/ChilliSource.h>

#include <iterator>

namespace ChilliSource
{
	namespace Core
	{
		//------------------------------------------------------------------------------
		/// A basic random access iterator. This can be used by any container that is
        /// backed by elements stored contiguously in memory.
		/// 
		/// @author Ian Copland
        //------------------------------------------------------------------------------
        template <typename TType> class random_access_iterator final : public std::iterator<std::random_access_iterator_tag, TType>
        {
        public:

            using value_type = typename std::iterator<std::random_access_iterator_tag, TType>::value_type;
            using difference_type = typename std::iterator<std::random_access_iterator_tag, TType>::difference_type;
            using pointer = typename std::iterator<std::random_access_iterator_tag, TType>::pointer;
            using reference = typename std::iterator<std::random_access_iterator_tag, TType>::reference;
            using iterator_category = typename std::iterator<std::random_access_iterator_tag, TType>::iterator_category;
            
            //------------------------------------------------------------------------------
            /// Default constuctor.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            random_access_iterator() = default;
            //------------------------------------------------------------------------------
            /// Constructor. Creates the iterator from a pointer.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer.
            //------------------------------------------------------------------------------
            random_access_iterator(pointer in_pointer);
            //------------------------------------------------------------------------------
            /// Copy Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param Another random access iterator.
            //------------------------------------------------------------------------------
            random_access_iterator(const random_access_iterator& in_toCopy);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Another random access iterator.
            ///
            /// @return A reference to this.
            //------------------------------------------------------------------------------
            random_access_iterator& operator=(const random_access_iterator& in_toCopy);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The dereferenced value.
            //------------------------------------------------------------------------------
            reference operator*();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The pointer.
            //------------------------------------------------------------------------------
            pointer operator->();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Another random access iterator.
            ///
            /// @return Whether or not this and the given iterator have the same value.
            //------------------------------------------------------------------------------
            bool operator==(const random_access_iterator& in_toTest) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Another random access iterator.
            ///
            /// @return Whether or not this and the given iterator have different values.
            //------------------------------------------------------------------------------
            bool operator!=(const random_access_iterator& in_toTest) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Another random access iterator.
            ///
            /// @return Whether or not this is more than the given iterator.
            //------------------------------------------------------------------------------
            bool operator>(const random_access_iterator& in_toTest) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Another random access iterator.
            ///
            /// @return Whether or not this is more than the given iterator.
            //------------------------------------------------------------------------------
            bool operator<(const random_access_iterator& in_toTest) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Another random access iterator.
            ///
            /// @return Whether or not this is more than the given iterator.
            //------------------------------------------------------------------------------
            bool operator>=(const random_access_iterator& in_toTest) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Another random access iterator.
            ///
            /// @return Whether or not this is more than the given iterator.
            //------------------------------------------------------------------------------
            bool operator<=(const random_access_iterator& in_toTest) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A reference to this after the increment has been performed.
            //------------------------------------------------------------------------------
            random_access_iterator& operator++();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A copy of this before the increment is performed.
            //------------------------------------------------------------------------------
            random_access_iterator operator++(int);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The difference.
            ///
            /// @return A copy of this incremented by the given difference.
            //------------------------------------------------------------------------------
            random_access_iterator operator+(difference_type in_difference);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The difference.
            ///
            /// @return A reference to this incremented by the given difference.
            //------------------------------------------------------------------------------
            random_access_iterator& operator+=(difference_type in_difference);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A reference to this after the decrement has been performed.
            //------------------------------------------------------------------------------
            random_access_iterator& operator--();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A copy of this before the decrement is performed.
            //------------------------------------------------------------------------------
            random_access_iterator operator--(int);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The difference.
            ///
            /// @return A copy of this decremented by the given difference.
            //------------------------------------------------------------------------------
            random_access_iterator operator-(difference_type in_difference);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The difference.
            ///
            /// @return A reference to this decremented by the given difference.
            //------------------------------------------------------------------------------
            random_access_iterator& operator-=(difference_type in_difference);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Another iterator.
            ///
            /// @return The difference.
            //------------------------------------------------------------------------------
            difference_type operator-(const random_access_iterator& in_otherIt) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The difference.
            ///
            /// @return A reference to the element at the given difference.
            //------------------------------------------------------------------------------
            reference operator[](difference_type in_difference);
		private:
            pointer m_pointer;
		};
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType>::random_access_iterator(pointer in_pointer)
            : m_pointer(in_pointer)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType>::random_access_iterator(const random_access_iterator& in_toCopy)
            : m_pointer(in_toCopy.m_pointer)
        {
            
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType>& random_access_iterator<TType>::operator=(const random_access_iterator& in_toCopy)
        {
            m_pointer = in_toCopy.m_pointer;
            return *this;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename random_access_iterator<TType>::reference random_access_iterator<TType>::operator*()
        {
            return *m_pointer;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename random_access_iterator<TType>::pointer random_access_iterator<TType>::operator->()
        {
            return m_pointer;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> bool random_access_iterator<TType>::operator==(const random_access_iterator& in_toTest) const
        {
            return m_pointer == in_toTest.m_pointer;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> bool random_access_iterator<TType>::operator!=(const random_access_iterator& in_toTest) const
        {
            return !(*this == in_toTest);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> bool random_access_iterator<TType>::operator>(const random_access_iterator& in_toTest) const
        {
            return (m_pointer - in_toTest.m_pointer) > 0;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> bool random_access_iterator<TType>::operator<(const random_access_iterator& in_toTest) const
        {
            return (in_toTest.m_pointer - m_pointer) > 0;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> bool random_access_iterator<TType>::operator>=(const random_access_iterator& in_toTest) const
        {
            return !(*this < in_toTest);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> bool random_access_iterator<TType>::operator<=(const random_access_iterator& in_toTest) const
        {
            return !(*this > in_toTest);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType>& random_access_iterator<TType>::operator++()
        {
            m_pointer++;
            return *this;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType> random_access_iterator<TType>::operator++(int)
        {
            auto copy = *this;
            (*this)++;
            return copy;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType> random_access_iterator<TType>::operator+(difference_type in_difference)
        {
            return m_pointer + in_difference;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType>& random_access_iterator<TType>::operator+=(difference_type in_difference)
        {
            m_pointer += in_difference;
            return *this;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType>& random_access_iterator<TType>::operator--()
        {
            m_pointer--;
            return *this;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType> random_access_iterator<TType>::operator--(int)
        {
            auto copy = *this;
            (*this)--;
            return copy;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType> random_access_iterator<TType>::operator-(difference_type in_difference)
        {
            return m_pointer - in_difference;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> random_access_iterator<TType>& random_access_iterator<TType>::operator-=(difference_type in_difference)
        {
            m_pointer -= in_difference;
            return *this;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename random_access_iterator<TType>::difference_type random_access_iterator<TType>::operator-(const random_access_iterator& in_otherIt) const
        {
            return m_pointer - in_otherIt.m_pointer;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename random_access_iterator<TType>::reference random_access_iterator<TType>::operator[](difference_type in_difference)
        {
            return m_pointer[in_difference];
        }
	}
}

#endif