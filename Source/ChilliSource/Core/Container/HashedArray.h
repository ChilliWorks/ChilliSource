//
//  HashedArray.h
//  Chilli Source
//  Created by Scott Downie on 16/02/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_HASHEDARRAY_H_
#define _CHILLISOURCE_CORE_CONTAINER_HASHEDARRAY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

#include <algorithm>
#include <functional>

namespace ChilliSource
{
    namespace Core
    {
        //TODO: Need to address the mutable member with the new definition of const
        //-------------------------------------------------
        /// Key value array that uses a binary search to
        /// perform lookups on the key. The key is
        /// hashed. Array is statically
        /// sized. Order is not guaranteed. The syntax
        /// mimics std syntax
        ///
        /// @author S Downie
        //-------------------------------------------------
        template <typename TKey, typename TValue> class HashedArray
        {
        public:

            typedef std::function<u32(TKey)> HashDelegate;
            typedef std::pair<u32, TValue> KeyValue;
            typedef KeyValue* iterator;
            typedef const KeyValue* const_iterator;
            
            //-------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Capacity of the array. If this
            /// is zero then the array cannot be added
            /// to until set capacity is called
            /// @param Hash function
            //-------------------------------------------
            HashedArray(u32 in_capacity, const HashDelegate& in_hashFunction)
            : m_capacity(0), m_size(0), m_hashFunction(in_hashFunction), m_isSortCacheValid(true), m_storage(nullptr)
            {
                CS_ASSERT(m_hashFunction, "HashedArray: Key type cannot be hashed without providing a custom hash method");
                
                if(in_capacity > 0)
                {
                    set_capacity(in_capacity);
                }
            }
            //-------------------------------------------
            /// Copy constructor
            ///
            /// @author S Downie
            ///
            /// @param Object to copy
            //-------------------------------------------
            HashedArray(const HashedArray& in_toCopy)
            {
                Clone();
            }
            //-------------------------------------------
            /// Copy assignment
            ///
            /// @author S Downie
            ///
            /// @param Object to copy
            //-------------------------------------------
            HashedArray& operator=(const HashedArray& in_toCopy)
            {
                Clone();
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// Set the initial capacity of the array
            /// if it has already been set then this
            /// will assert
            ///
            /// @param Capacity
            //-------------------------------------------
            void set_capacity(u32 in_capacity)
            {
                CS_ASSERT(in_capacity > 0, "HashedArray: Cannot create an array with capacity zero");
                CS_ASSERT(m_capacity == 0, "HashedArray: Cannot allocate array twice");
                m_storage = new KeyValue[in_capacity];
                m_capacity = in_capacity;
            }
            //-------------------------------------------
            /// Insert the object into the array. Order
            /// is not guaranteed
            ///
            /// @author S Downie
            ///
            /// @param Key
            /// @param Value
            //-------------------------------------------
            void insert(const TKey& in_key, const TValue& in_value)
            {
                CS_ASSERT(m_capacity > 0, "HashedArray: Must allocate capacity");
                CS_ASSERT(m_size < m_capacity, "HashedArray: Exceeds capacity");
                
                u32 hash = m_hashFunction(in_key);
                m_storage[m_size].first = hash;
                m_storage[m_size].second = in_value;
                m_isSortCacheValid = false;
                m_size++;
            }
            //-------------------------------------------
            /// Remove the object identified by the
            /// given key. This doesn't free any memory
            ///
            /// @author S Downie
            ///
            /// @param Key of key and value to remove
            //-------------------------------------------
            void remove(const TKey& in_key)
            {
                u32 hash = m_hashFunction(in_key);
                s32 index = FindIndex(hash);
                
                CS_ASSERT(index >= 0, "HashedArray: Cannot find key to remove");
                
                std::swap(m_storage[index], m_storage[m_size-1]);
                m_storage[m_size-1].~KeyValue();
                
                m_size--;
                m_isSortCacheValid = false;
            }
            //-------------------------------------------
            /// Clear all objects but hold onto the memory
            ///
            /// @author S Downie
            //-------------------------------------------
            void clear()
            {
                for(u32 i=0; i<m_size; ++i)
                {	
                    m_storage[i].~KeyValue();
                }
                
                m_size = 0;
                m_isSortCacheValid = false;
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @param Key
            ///
            /// @return Value at key or end if not found
            //-------------------------------------------
            iterator find(const TKey& in_key)
            {
                u32 hash = m_hashFunction(in_key);
                s32 index = FindIndex(hash);
                
                if(index >= 0)
                {
                    return &m_storage[index];
                }
             
                return end();
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @param Key
            ///
            /// @return Value at key or end if not found
            //-------------------------------------------
            const_iterator find(const TKey& in_key) const
            {
                u32 hash = m_hashFunction(in_key);
                s32 index = FindIndex(hash);
                
                if(index >= 0)
                {
                    return &m_storage[index];
                }
                
                return end();
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @return Number of elements that have been
            /// added
            //-------------------------------------------
            inline u32 length() const
            {
                return m_size;
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the array is empty
            //-------------------------------------------
            inline bool empty() const
            {
                return length() == 0;
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @return Capcaity of the array
            //-------------------------------------------
            inline u32 capacity() const
            {
                return m_capacity;
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the first
            /// object in the structure
            //-------------------------------------------
            inline iterator begin() 
            {
                return m_storage;
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing beyond the last
            /// object in the structure
            //-------------------------------------------
            inline iterator end() 
            {
                return (m_storage + m_size);
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the first
            /// object in the structure
            //-------------------------------------------
            inline const_iterator begin() const
            {
                return m_storage;
            }
            //-------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing beyond the last
            /// object in the structure
            //-------------------------------------------
            inline const_iterator end() const
            {
                return (m_storage + m_size);
            }
            //-------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //-------------------------------------------
            ~HashedArray()
            {
                clear();
                CS_SAFEDELETE_ARRAY(m_storage);
            }
            
        private:
            
            //-------------------------------------------
            /// Sort predicate
            ///
            /// @author S Downie
            ///
            /// @param LHS
            /// @param RHS
            ///
            /// @return Whether LHS is less than RHS
            //-------------------------------------------
            static bool LessThanSortPredicate(const KeyValue& in_lhs, const KeyValue& in_rhs)
            {
                return in_lhs.first < in_rhs.first;
            }
            //-------------------------------------------
            /// Makes a deep copy of the array
            ///
            /// @author S Downie
            ///
            /// @param Array to copy
            //-------------------------------------------
            void Clone(const HashedArray& inRHS)
            {
                m_capacity = inRHS.m_capacity;
                m_size = inRHS.m_size;
                m_hashFunction = inRHS.m_hashFunction;
                m_isSortCacheValid = inRHS.m_isSortCacheValid;
                std::copy(inRHS.begin(), inRHS.end(), this->begin());
            }
            //--------------------------------------------
            /// Performs a binary search looking for the
            /// index of the object with the given key
            ///
            /// @author S Downie
            ///
            /// @param Key
            ///
            /// @return Index of object identified by key
            /// or -1 if not found
            //--------------------------------------------
            s32 FindIndex(u32 in_keyHash) const
            {
                if(m_size == 0)
                {
                    return -1;
                }
                
                if(!m_isSortCacheValid)
                {
                    std::sort(m_storage, m_storage + m_size, HashedArray::LessThanSortPredicate);
                    m_isSortCacheValid = true;
                }
                
                u32 low = 0;
                s32 high = m_size-1;
                u32 mid = 0;
                
                while((s32)low <= high)
                {
                    mid = (high + low)/ 2;
                    if(in_keyHash == m_storage[mid].first)
                    {
                        return mid;
                    }
                    else if(in_keyHash < m_storage[mid].first)
                    {
                        high = mid - 1;
                    }
                    else
                    {
                        low = mid + 1;
                    }
                }
                
                return -1;
            }
            
        private:
            
            HashDelegate m_hashFunction;
            
            u32 m_size;
            u32 m_capacity;
            
            mutable bool m_isSortCacheValid;
            
            KeyValue* m_storage;
        };
    }
}

#endif

