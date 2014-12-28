//
//  dynamic_array.h
//  Chilli Source
//  Created by Ian Copland on 20/10/2014.
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

#ifndef _CHILLISOURCE_CORE_CONTAINERS_DYNAMICARRAY_H_
#define _CHILLISOURCE_CORE_CONTAINERS_DYNAMICARRAY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/random_access_iterator.h>

namespace ChilliSource
{
	namespace Core
	{
		//------------------------------------------------------------------------------
		/// A fixed size array with a size that is decided at runtime. This is heavily
		/// based on std::dynarray that was initially intended to be included in the 
		/// C++14 standard library. This will likely be removed if std::dynarray is 
		/// ever introduced as a standard part of the language.
		/// 
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		template <typename TType> class dynamic_array final
		{
		public:
            
            using value_type = TType;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using iterator = random_access_iterator<value_type>;
            using const_iterator = random_access_iterator<const value_type>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;
            
			//------------------------------------------------------------------------------
			/// Constructor. Initialises the array with the given number of elements.
			///
			/// @author Ian Copland
			///
			/// @param The size of the array.
			//------------------------------------------------------------------------------
			dynamic_array(size_type in_size);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The size of the array.
            //------------------------------------------------------------------------------
            size_type size() const;
            //------------------------------------------------------------------------------
            /// Sets all elements in the array to the given value.
            ///
            /// @author Ian Copland
            ///
            /// @param The value to set.
            //------------------------------------------------------------------------------
            void fill(const_reference in_value);
			//------------------------------------------------------------------------------
			/// Returns a reference to the object at the given index in the array.
			///
			/// @author Ian Copland
			///
			/// @param The index.
			///
			/// @param The reference.
			//------------------------------------------------------------------------------
			reference at(size_type in_index);
			//------------------------------------------------------------------------------
			/// Returns a const reference to the object at the given index in the array.
			///
			/// @author Ian Copland
			///
			/// @param The index.
			///
			/// @param The const reference.
			//------------------------------------------------------------------------------
			const_reference at(size_type in_index) const;
			//------------------------------------------------------------------------------
			/// Returns a reference to the object at the given index in the array.
			///
			/// @author Ian Copland
			///
			/// @param The index.
			///
			/// @param The reference.
			//------------------------------------------------------------------------------
			reference operator[](size_type in_index);
			//------------------------------------------------------------------------------
			/// Returns a const reference to the object at the given index in the array.
			///
			/// @author Ian Copland
			///
			/// @param The const reference.
			//------------------------------------------------------------------------------
			const_reference operator[](size_type in_index) const;
			//------------------------------------------------------------------------------
			/// Returns a pointer to the start of the array.
			///
			/// @author Ian Copland
			///
			/// @param The pointer to the start of the array.
			//------------------------------------------------------------------------------
			pointer data();
            //------------------------------------------------------------------------------
            /// Returns a const pointer to the start of the array.
            ///
            /// @author Ian Copland
            ///
            /// @param The const pointer to the start of the array.
            //------------------------------------------------------------------------------
            const_pointer data() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The element at the front of the array.
            //------------------------------------------------------------------------------
            reference front();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const version of the element at the front of the array.
            //------------------------------------------------------------------------------
            const_reference front() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The element at the back of the array.
            //------------------------------------------------------------------------------
            reference back();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const version of the element at the back of the array.
            //------------------------------------------------------------------------------
            const_reference back() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An iterator at the beginning of the array.
            //------------------------------------------------------------------------------
            iterator begin();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const iterator at the beginning of the array.
            //------------------------------------------------------------------------------
            const_iterator begin() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const iterator at the beginning of the array.
            //------------------------------------------------------------------------------
            const_iterator cbegin() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An iterator at the end of the array.
            //------------------------------------------------------------------------------
            iterator end();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const iterator at the end of the array.
            //------------------------------------------------------------------------------
            const_iterator end() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const iterator at the end of the array.
            //------------------------------------------------------------------------------
            const_iterator cend() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A reverse iterator at the beginning of the array.
            //------------------------------------------------------------------------------
            reverse_iterator rbegin();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const reverse iterator at the beginning of the array.
            //------------------------------------------------------------------------------
            const_reverse_iterator rbegin() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const reverse iterator at the beginning of the array.
            //------------------------------------------------------------------------------
            const_reverse_iterator crbegin() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A reverse iterator at the end of the array.
            //------------------------------------------------------------------------------
            reverse_iterator rend();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const reverse iterator at the end of the array.
            //------------------------------------------------------------------------------
            const_reverse_iterator rend() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const reverse iterator at the end of the array.
            //------------------------------------------------------------------------------
            const_reverse_iterator crend() const;
		private:
			const size_type m_size;
			std::unique_ptr<value_type[]> m_array;
		};
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TType> dynamic_array<TType>::dynamic_array(size_type in_size)
			: m_size(in_size)
		{
			CS_ASSERT(m_size > 0, "Cannot create dynamic_array with size of 0 or below.");

			m_array = std::unique_ptr<TType[]>(new TType[in_size]);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TType> typename dynamic_array<TType>::size_type dynamic_array<TType>::size() const
		{
			return m_size;
		}
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> void dynamic_array<TType>::fill(const_reference in_value)
        {
            for (auto& val : *this)
            {
                val = in_value;
            }
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TType> typename dynamic_array<TType>::reference dynamic_array<TType>::at(size_type in_index)
		{
			CS_ASSERT(in_index >= 0, "Index into a dynamic_array cannot be below zero.");
			CS_ASSERT(in_index < m_size, "Index into a dynamic_array cannot be beyond the length of the array.");

			return m_array[in_index];
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TType> typename dynamic_array<TType>::const_reference dynamic_array<TType>::at(size_type in_index) const
		{
			CS_ASSERT(in_index >= 0, "Index into a dynamic_array cannot be below zero.");
			CS_ASSERT(in_index < m_size, "Index into a dynamic_array cannot be beyond the length of the array.");

			return m_array[in_index];
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TType> typename dynamic_array<TType>::reference dynamic_array<TType>::operator[](size_type in_index)
		{
			return at(in_index);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TType> typename dynamic_array<TType>::const_reference dynamic_array<TType>::operator[](size_type in_index) const
		{
			return at(in_index);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TType> typename dynamic_array<TType>::pointer dynamic_array<TType>::data()
		{
			return m_array.get();
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TType> typename dynamic_array<TType>::const_pointer dynamic_array<TType>::data() const
		{
			return m_array.get();
		}
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::reference dynamic_array<TType>::front()
        {
            return at(0);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_reference dynamic_array<TType>::front() const
        {
            return at(0);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::reference dynamic_array<TType>::back()
        {
            return at(size() - 1);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_reference dynamic_array<TType>::back() const
        {
            return at(size() - 1);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::iterator dynamic_array<TType>::begin()
        {
            return iterator(&front());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_iterator dynamic_array<TType>::begin() const
        {
			return const_iterator(&front());
        }
        //---------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_iterator dynamic_array<TType>::cbegin() const
        {
			return const_iterator(&front());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::iterator dynamic_array<TType>::end()
        {
            return iterator((&back()) + 1);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_iterator dynamic_array<TType>::end() const
        {
			return const_iterator((&back()) + 1);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_iterator dynamic_array<TType>::cend() const
        {
			return const_iterator((&back()) + 1);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::reverse_iterator dynamic_array<TType>::rbegin()
        {
			return reverse_iterator(&back());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_reverse_iterator dynamic_array<TType>::rbegin() const
        {
			return const_reverse_iterator(&back());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_reverse_iterator dynamic_array<TType>::crbegin() const
        {
			return const_reverse_iterator(&back());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::reverse_iterator dynamic_array<TType>::rend()
        {
			return reverse_iterator((&front() - 1));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_reverse_iterator dynamic_array<TType>::rend() const
        {
			return const_reverse_iterator((&front() - 1));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> typename dynamic_array<TType>::const_reverse_iterator dynamic_array<TType>::crend() const
        {
			return const_reverse_iterator((&front() - 1));
        }
	}
}

#endif