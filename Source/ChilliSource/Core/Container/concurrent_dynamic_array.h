//
//  concurrent_dynamic_array.h
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_CONCURRENTDYNAMICARRAY_H_
#define _CHILLISOURCE_CORE_CONTAINER_CONCURRENTDYNAMICARRAY_H_

#include <ChilliSource/ChilliSource.h>

#include <mutex>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------------
		/// A concurrent, fixed size array with a size that is decided at runtime. 
		/// This is essentially a wrapper around std::unique_ptr<TType[]> which 
		/// keeps track of the array count and provides locking for thread safety.
		/// Before using the array it should first be locked using lock(). Once
		/// finished unlock() should be called. 
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		//TODO: Finish implementing!
		template <typename TType> class concurrent_dynamic_array final
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
			concurrent_dynamic_array(std::size_t in_size);
			//----------------------------------------------------------------
			/// Returns the size of the array. As the size is const, this can
			/// safely be called without locking the array.
			/// 
			/// @author Ian Copland
			///
			/// @param The size of the array.
			//----------------------------------------------------------------
			std::size_t size() const;
			//----------------------------------------------------------------
			/// Locks the array so that only the current thread can asset it.
			/// This should be called prior to any element read or write
			/// operations. This can be called recursively--each lock will 
			/// increment the lock count, unlock() will decrement it. The mutex
			/// is only release when the lock count reaches 0.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void lock() const;
			//----------------------------------------------------------------
			/// Returns a reference to the object at the given index in the
			/// array. The array should be locked whenever this is called, else 
			/// it will assert.
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
			/// the array. The array should be locked whenever this is called, 
			/// else it will assert.
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
			/// array. The array should be locked whenever this is called, else 
			/// it will assert.
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
			/// the array. The array should be locked whenever this is called, 
			/// else it will assert.
			///
			/// @author Ian Copland
			///
			/// @param The const reference.
			//----------------------------------------------------------------
			const TType& operator[](std::size_t in_index) const;
			//----------------------------------------------------------------
			/// Returns a pointer to the start of the array. The array should 
			/// be locked whenever this is called, else it will assert.
			///
			/// @author Ian Copland
			///
			/// @param The pointer to the start of the array.
			//----------------------------------------------------------------
			TType* data();
			//----------------------------------------------------------------
			/// Returns a const pointer to the start of the array. The array 
			/// should be locked whenever this is called, else it will assert.
			///
			/// @author Ian Copland
			///
			/// @param The const pointer to the start of the array.
			//----------------------------------------------------------------
			const TType* data() const;
			//----------------------------------------------------------------
			/// Unlocks the array allowing other thread access to it again. This 
			/// should be called once finished operating on elements in the
			/// array. This can be called recursively--each lock will increment 
			/// the lock count, unlock() will decrement it. The mutex is only
			/// release when the lock count reaches 0.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void unlock() const;
			//----------------------------------------------------------------
			/// Destructor.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			~concurrent_dynamic_array() {};
		private:
			const std::size_t m_size;
			std::unique_ptr<TType[]> m_array;

			mutable std::recursive_mutex m_mutex;
			mutable std::unique_lock<std::recursive_mutex> m_lock;
			mutable std::size_t m_lockCount = 0;
		};
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> concurrent_dynamic_array<TType>::concurrent_dynamic_array(std::size_t in_size)
			: m_size(in_size), m_mutex(), m_lock(m_mutex, std::defer_lock)
		{
			CS_ASSERT(m_size > 0, "Cannot create concurrent_dynamic_array with size of 0 or below.");

			m_array = std::unique_ptr<TType[]>(new TType[in_size]);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> std::size_t concurrent_dynamic_array<TType>::size() const
		{
			return m_size;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> void concurrent_dynamic_array<TType>::lock() const
		{
			std::unique_lock<std::recursive_mutex> scopedLock(m_mutex);

			if (m_lockCount == 0)
			{
				m_lock.lock();
			}

			m_lockCount++;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> TType& concurrent_dynamic_array<TType>::at(std::size_t in_index)
		{
			CS_ASSERT(m_lockCount > 0, "concurrent_dynamic_array should be locked.");
			CS_ASSERT(in_index >= 0, "Index into a concurrent_dynamic_array cannot be below zero.");
			CS_ASSERT(in_index < m_size, "Index into a concurrent_dynamic_array cannot be beyond the length of the array.");

			return m_array[in_index];
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> const TType& concurrent_dynamic_array<TType>::at(std::size_t in_index) const
		{
			CS_ASSERT(m_lockCount > 0, "concurrent_dynamic_array should be locked.");
			CS_ASSERT(in_index >= 0, "Index into a concurrent_dynamic_array cannot be below zero.");
			CS_ASSERT(in_index < m_size, "Index into a concurrent_dynamic_array cannot be beyond the length of the array.");

			return m_array[in_index];
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> TType& concurrent_dynamic_array<TType>::operator[](std::size_t in_index)
		{
			return at(in_index);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> const TType& concurrent_dynamic_array<TType>::operator[](std::size_t in_index) const
		{
			return at(in_index);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> TType* concurrent_dynamic_array<TType>::data()
		{
			CS_ASSERT(m_lockCount > 0, "concurrent_dynamic_array should be locked.");
			return m_array.get();
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> const TType* concurrent_dynamic_array<TType>::data() const
		{
			CS_ASSERT(m_lockCount > 0, "concurrent_dynamic_array should be locked.");
			return m_array.get();
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		template <typename TType> void concurrent_dynamic_array<TType>::unlock() const
		{
			std::unique_lock<std::recursive_mutex> scopedLock(m_mutex);

			m_lockCount--;

			if (m_lockCount == 0)
			{
				m_lock.unlock();
			}
		}
	}
}

#endif