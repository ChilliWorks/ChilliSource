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

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------------
		/// A fixed size array with a size that is decided at runtime. This is 
		/// essentially a wrapper around std::unique_ptr<TType[]> which also 
		/// keeps track of the array count.
		/// 
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		//TODO: Finish implementing!
		template <typename TType> class dynamic_array final
		{
		public:
			//----------------------------------------------------------------
			/// Constructor. Initialises the array with the given number of 
			/// elements.
			///
			/// @author Ian Copland
			///
			/// @param The size of the array.
			//----------------------------------------------------------------
			dynamic_array(std::size_t in_size);
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The size of the array.
			//----------------------------------------------------------------
			std::size_t size() const;
			//----------------------------------------------------------------
			/// Returns a reference to the object at the given index in the
			/// array. 
			///
			/// @author Ian Copland
			///
			/// @param The index.
			///
			/// @param The reference.
			//----------------------------------------------------------------
			TType& at(std::size_t in_index);
			//----------------------------------------------------------------
			/// Returns a const reference to the object at the given index in
			/// the array.
			///
			/// @author Ian Copland
			///
			/// @param The index.
			///
			/// @param The const reference.
			//----------------------------------------------------------------
			const TType& at(std::size_t in_index) const;
			//----------------------------------------------------------------
			/// Returns a reference to the object at the given index in the
			/// array. 
			///
			/// @author Ian Copland
			///
			/// @param The index.
			///
			/// @param The reference.
			//----------------------------------------------------------------
			TType& operator[](std::size_t in_index);
			//----------------------------------------------------------------
			/// Returns a const reference to the object at the given index in
			/// the array. 
			///
			/// @author Ian Copland
			///
			/// @param The const reference.
			//----------------------------------------------------------------
			const TType& operator[](std::size_t in_index) const;
			//----------------------------------------------------------------
			/// Returns a pointer to the start of the array.
			///
			/// @author Ian Copland
			///
			/// @param The pointer to the start of the array.
			//----------------------------------------------------------------
			TType* data();
			//----------------------------------------------------------------
			/// Returns a const pointer to the start of the array. 
			///
			/// @author Ian Copland
			///
			/// @param The const pointer to the start of the array.
			//----------------------------------------------------------------
			const TType* data() const;
			//----------------------------------------------------------------
			/// Destructor.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			~dynamic_array() {};
		private:
			const std::size_t m_size;
			std::unique_ptr<TType[]> m_array;
		};
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> dynamic_array<TType>::dynamic_array(std::size_t in_size)
			: m_size(in_size)
		{
			CS_ASSERT(m_size > 0, "Cannot create dynamic_array with size of 0 or below.");

			m_array = std::unique_ptr<TType[]>(new TType[in_size]);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> std::size_t dynamic_array<TType>::size() const
		{
			return m_size;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> TType& dynamic_array<TType>::at(std::size_t in_index)
		{
			CS_ASSERT(in_index >= 0, "Index into a dynamic_array cannot be below zero.");
			CS_ASSERT(in_index < m_size, "Index into a dynamic_array cannot be beyond the length of the array.");

			return m_array[in_index];
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> const TType& dynamic_array<TType>::at(std::size_t in_index) const
		{
			CS_ASSERT(in_index >= 0, "Index into a dynamic_array cannot be below zero.");
			CS_ASSERT(in_index < m_size, "Index into a dynamic_array cannot be beyond the length of the array.");

			return m_array[in_index];
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> TType& dynamic_array<TType>::operator[](std::size_t in_index)
		{
			return at(in_index);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> const TType& dynamic_array<TType>::operator[](std::size_t in_index) const
		{
			return at(in_index);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> TType* dynamic_array<TType>::data()
		{
			return m_array.get();
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> const TType* dynamic_array<TType>::data() const
		{
			return m_array.get();
		}
	}
}

#endif