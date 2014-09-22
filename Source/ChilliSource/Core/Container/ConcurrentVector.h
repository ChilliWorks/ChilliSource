//
//  ConcurrentVector.h
//  Chilli Source
//  Created by Scott Downie on 19/09/2015.
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_CONCURRENTVECTOR_H_
#define _CHILLISOURCE_CORE_CONTAINER_CONCURRENTVECTOR_H_

#include <ChilliSource/Core/Container/ConcurrentVectorConstForwardIterator.h>
#include <ChilliSource/Core/Container/ConcurrentVectorConstReverseIterator.h>
#include <ChilliSource/Core/Container/ConcurrentVectorForwardIterator.h>
#include <ChilliSource/Core/Container/ConcurrentVectorReverseIterator.h>

#include <vector>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// Concurrent vector is a thread safe dynamic array implementation.
        /// It presevers the integrity of the array when accessing from different
        /// threads and also from changes to the array while iterating
        ///
        /// NOTE: This class syntax mimics STL and therefore doe not use the CS coding
        /// standards.
        ///
        /// @author S Downie
        //------------------------------------------------------------------------
        template <typename TType> class ConcurrentVector
        {
        public:
            
            using iterator = ConcurrentVectorForwardIterator<TType>;
            using const_iterator = ConcurrentVectorConstForwardIterator<TType>;
            using reverse_iterator = ConcurrentVectorReverseIterator<TType>;
            using const_reverse_iterator = ConcurrentVectorConstReverseIterator<TType>;
            
            //--------------------------------------------------------------------
            /// Constructor default
            ///
            /// @author S Downie
            //--------------------------------------------------------------------
            ConcurrentVector();
            //--------------------------------------------------------------------
            /// Construct from initialiser list
            ///
            /// @author S Downie
            ///
            /// @param Initialiser list
            //--------------------------------------------------------------------
            ConcurrentVector(std::initializer_list<TType>&& in_initialObjects);
            //--------------------------------------------------------------------
            /// Copy constructor that creates this as a copy of the given vector
            ///
            /// @author S Downie
            ///
            /// @param Vector to copy
            //--------------------------------------------------------------------
            ConcurrentVector(const ConcurrentVector& in_toCopy);
            //--------------------------------------------------------------------
            /// Copy assignment that creates this as a copy of the given vector
            ///
            /// @author S Downie
            ///
            /// @param Vector to copy
            ///
            /// @return This as a copy
            //--------------------------------------------------------------------
            ConcurrentVector& operator=(const ConcurrentVector& in_toCopy);
            //--------------------------------------------------------------------
            /// Move constructor that transfers ownership from the given vector
            ///
            /// @author S Downie
            ///
            /// @param Vector to move
            //--------------------------------------------------------------------
            ConcurrentVector(ConcurrentVector&& in_toMove);
            //--------------------------------------------------------------------
            /// Move assignment that transfers ownership from the given vector
            ///
            /// @author S Downie
            ///
            /// @param Vector to move
            ///
            /// @return Thishaving taken ownership of the given vector
            //--------------------------------------------------------------------
            ConcurrentVector& operator=(ConcurrentVector&& in_toMove);
            //--------------------------------------------------------------------
            /// Push the object onto the back of the array. Unlike STL this does
            /// not invalidate any iterators
            ///
            /// @author S Downie
            ///
            /// @param Object to add
            //--------------------------------------------------------------------
            void push_back(TType&& in_object);
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The object in the first element of the array (undefined if empty)
            //--------------------------------------------------------------------
            TType& front();
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The object in the first element of the array (undefined if empty)
            //--------------------------------------------------------------------
            const TType& front() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The object in the last element of the array (undefined if empty)
            //--------------------------------------------------------------------
            TType& back();
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The object in the last element of the array (undefined if empty)
            //--------------------------------------------------------------------
            const TType& back() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Index
            ///
            /// @return The object at the given index of the array (undefined if out of bounds)
            //--------------------------------------------------------------------
            TType& at(u32 in_index);
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Index
            ///
            /// @return The object at the given index of the array (undefined if out of bounds)
            //--------------------------------------------------------------------
            const TType& at(u32 in_index) const;
            //--------------------------------------------------------------------
            /// Lock the array which prevents its contents being modified unsafely.
            ///
            /// @author S Downie
            //--------------------------------------------------------------------
            void lock();
            //--------------------------------------------------------------------
            /// Unlock the array.
            ///
            /// @author S Downie
            //--------------------------------------------------------------------
            void unlock();
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The number of items currently in the vector
            //--------------------------------------------------------------------
            u32 size() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the vector is empty or not
            //--------------------------------------------------------------------
            bool empty() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the beginning of the vector
            //--------------------------------------------------------------------
            iterator begin();
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the end of the vector (the end being)
            /// after the last element
            //--------------------------------------------------------------------
            iterator end();
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the beginning of the vector
            //--------------------------------------------------------------------
            const_iterator begin() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the end of the vector (the end being)
            /// after the last element
            //--------------------------------------------------------------------
            const_iterator end() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the beginning of the vector
            //--------------------------------------------------------------------
            const_iterator cbegin() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the end of the vector (the end being)
            /// after the last element
            //--------------------------------------------------------------------
            const_iterator cend() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the last element of the vector
            //--------------------------------------------------------------------
            reverse_iterator rbegin();
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the beginning of the vector (the beginning
            /// in this case being before the first element)
            //--------------------------------------------------------------------
            reverse_iterator rend();
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the last element of the vector
            //--------------------------------------------------------------------
            const_reverse_iterator rbegin() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the beginning of the vector (the beginning
            /// in this case being before the first element)
            //--------------------------------------------------------------------
            const_reverse_iterator rend() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the last element of the vector
            //--------------------------------------------------------------------
            const_reverse_iterator crbegin() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Iterator pointing to the beginning of the vector (the beginning
            /// in this case being before the first element)
            //--------------------------------------------------------------------
            const_reverse_iterator crend() const;
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Index
            ///
            /// @return The object at the given index of the array (undefined if out of bounds)
            //--------------------------------------------------------------------
            TType& operator[](u32 in_index);
            //--------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Index
            ///
            /// @return The object at the given index of the array (undefined if out of bounds)
            //--------------------------------------------------------------------
            const TType& operator[](u32 in_index) const;
            //--------------------------------------------------------------------
            /// Remove the object from the vector that is pointed to by the given
            /// iterator. Unlike STL this does not invalidate any iterators
            ///
            /// @author S Downie
            ///
            /// @param Iterator
            ///
            /// @return Iterator (Same iterator as it is not invalidate but returning
            /// it is consistent with STL)
            //--------------------------------------------------------------------
            template<typename TIter> TIter erase(const TIter& in_itErase);
            //--------------------------------------------------------------------
            /// Clears the vector. Unlike STL this does not invalidate any iterators
            ///
            /// @author S Downie
            //--------------------------------------------------------------------
            void clear();
            
        private:
            
            //--------------------------------------------------------------------
            /// Cleanup any elements that are marked for removal
            ///
            /// @author S Downie
            //--------------------------------------------------------------------
            void GarbageCollect()
            {
                for(auto it = m_container.begin(); it != m_container.end(); )
                {
                    if(it->second == true)
                    {
                        it = m_container.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
                
                m_requiresGC = false;
            }
            
        private:
            
            std::vector<std::pair<TType, bool>> m_container;
            
            std::atomic<u32> m_size;
            bool m_isLocked = false;
            bool m_requiresGC = false;
            
            std::recursive_mutex m_mutex;
            std::unique_lock<std::recursive_mutex> m_lock;
        };
        
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> ConcurrentVector<TType>::ConcurrentVector()
        : m_lock(m_mutex), m_size(0)
        {
            m_lock.unlock();
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> ConcurrentVector<TType>::ConcurrentVector(std::initializer_list<TType>&& in_initialObjects)
        : m_lock(m_mutex), m_size(0)
        {
            m_lock.unlock();
            m_size = in_initialObjects.size();
            m_container.reserve(m_size);
            
            for(const auto& object : in_initialObjects)
            {
                m_container.push_back(std::make_pair(object, false));
            }
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> ConcurrentVector<TType>::ConcurrentVector(const ConcurrentVector& in_toCopy)
        {
            m_size = in_toCopy.m_size.load();
            m_container = in_toCopy.m_container;
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> ConcurrentVector<TType>& ConcurrentVector<TType>::operator=(const ConcurrentVector<TType>& in_toCopy)
        {
            m_size = in_toCopy.m_size.load();
            m_container = in_toCopy.m_container;
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> ConcurrentVector<TType>::ConcurrentVector(ConcurrentVector&& in_toMove)
        {
            m_size = in_toMove.m_size.load();
            in_toMove.m_size = 0;
            m_container = std::move(in_toMove.m_container);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> ConcurrentVector<TType>& ConcurrentVector<TType>::operator=(ConcurrentVector<TType>&& in_toMove)
        {
            m_size = in_toMove.m_size.load();
            in_toMove.m_size = 0;
            m_container = std::move(in_toMove.m_container);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> void ConcurrentVector<TType>::push_back(TType&& in_object)
        {
            std::unique_lock<std::recursive_mutex> scopedLock(m_mutex);
            m_container.push_back(std::make_pair(in_object, false));
            m_size++;
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> TType& ConcurrentVector<TType>::front()
        {
            return at(0);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> const TType& ConcurrentVector<TType>::front() const
        {
            return at(0);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> TType& ConcurrentVector<TType>::back()
        {
            return at(size()-1);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> const TType& ConcurrentVector<TType>::back() const
        {
            return at(size()-1);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> TType& ConcurrentVector<TType>::at(u32 in_index)
        {
            std::unique_lock<std::recursive_mutex> scopedLock(m_mutex);
            if(m_requiresGC == false)
            {
                return m_container.at(in_index).first;
            }
            else
            {
                auto size = m_container.size();
                s32 count = -1;
                u32 index = 0;
                for(u32 i=0; i<size; ++i)
                {
                    if(m_container[i].second == false)
                    {
                        if(++count == in_index)
                        {
                            index = i;
                            break;
                        }
                    }
                }
                
                return m_container[index].first;
            }
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> const TType& ConcurrentVector<TType>::at(u32 in_index) const
        {
            std::unique_lock<std::recursive_mutex> scopedLock(m_mutex);
            if(m_requiresGC == false)
            {
                return m_container.at(in_index).first;
            }
            else
            {
                auto size = m_container.size();
                s32 count = -1;
                u32 index = 0;
                for(u32 i=0; i<size; ++i)
                {
                    if(m_container[i].second == false)
                    {
                        if(++count == in_index)
                        {
                            index = i;
                            break;
                        }
                    }
                }
                
                return m_container[index].first;
            }
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> void ConcurrentVector<TType>::lock()
        {
            m_lock.lock();
            m_isLocked = true;
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> void ConcurrentVector<TType>::unlock()
        {
            GarbageCollect();
            m_isLocked = false;
            m_lock.unlock();
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> u32 ConcurrentVector<TType>::size() const
        {
            return m_size;
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> bool ConcurrentVector<TType>::empty() const
        {
            return m_size == 0;
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::iterator ConcurrentVector<TType>::begin()
        {
            return iterator(m_container, m_mutex);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::iterator ConcurrentVector<TType>::end()
        {
            return iterator(m_container, m_mutex) + size();
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::const_iterator ConcurrentVector<TType>::begin() const
        {
            return const_iterator(m_container, m_mutex);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::const_iterator ConcurrentVector<TType>::end() const
        {
            return const_iterator(m_container, m_mutex) + size();
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::const_iterator ConcurrentVector<TType>::cbegin() const
        {
            return const_iterator(m_container, m_mutex);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::const_iterator ConcurrentVector<TType>::cend() const
        {
            return const_iterator(m_container, m_mutex) + size();
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::reverse_iterator ConcurrentVector<TType>::rbegin()
        {
            return reverse_iterator(m_container, m_mutex);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::reverse_iterator ConcurrentVector<TType>::rend()
        {
            return reverse_iterator(m_container, m_mutex) + size();
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::const_reverse_iterator ConcurrentVector<TType>::rbegin() const
        {
            return const_reverse_iterator(m_container, m_mutex);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::const_reverse_iterator ConcurrentVector<TType>::rend() const
        {
            return const_reverse_iterator(m_container, m_mutex) + size();
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::const_reverse_iterator ConcurrentVector<TType>::crbegin() const
        {
            return const_reverse_iterator(m_container, m_mutex);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> typename ConcurrentVector<TType>::const_reverse_iterator ConcurrentVector<TType>::crend() const
        {
            return const_reverse_iterator(m_container, m_mutex) + size();
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> TType& ConcurrentVector<TType>::operator[](u32 in_index)
        {
            return at(in_index);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> const TType& ConcurrentVector<TType>::operator[](u32 in_index) const
        {
            return at(in_index);
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> template<typename TIter> TIter ConcurrentVector<TType>::erase(const TIter& in_itErase)
        {
            std::unique_lock<std::recursive_mutex> scopedLock(m_mutex);
            if(m_isLocked == false)
            {
                m_container.erase(m_container.begin() + in_itErase.GetIndex());
            }
            else
            {
                m_container[in_itErase.GetIndex()].second = true;
                m_requiresGC = true;
            }
            
            m_size--;
            
            return in_itErase;
        }
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        template <typename TType> void ConcurrentVector<TType>::clear()
        {
            std::unique_lock<std::recursive_mutex> scopedLock(m_mutex);
            if(m_isLocked == false)
            {
                m_container.clear();
                
            }
            else
            {
                for(auto& object : m_container)
                {
                    object.second = true;
                }
                
                m_requiresGC = true;
            }
            
            m_size = 0;
        }
    }
}

#endif
