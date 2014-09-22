//
//  ConcurrentVectorConstReverseIterator.h
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_CONCURRENTVECTORCONSTREVERSEITERATOR_H_
#define _CHILLISOURCE_CORE_CONTAINER_CONCURRENTVECTORCONSTREVERSEITERATOR_H_

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------
        /// Reverse iterator for the concurrent vector class that is read only
        ///
        /// @author S Downie
        //------------------------------------------------------------------------
        template <typename TType> class ConcurrentVectorConstReverseIterator
        {
        public:
            
            //------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Data structure to iterate over
            /// @param Mutex used to protect the underlying iterable
            //------------------------------------------------------------------------
            ConcurrentVectorConstReverseIterator(std::vector<std::pair<TType, bool>>& in_iterable, std::recursive_mutex& in_iterableMutex)
            : m_iterable(in_iterable), m_iterableMutex(in_iterableMutex)
            {
                m_iterableIndex = FindPreviousOccupiedIndex(m_iterable.size());
            }
            //------------------------------------------------------------------------
            /// Moves the iterator to point to the previous element in the vector
            ///
            /// @author S Downie
            ///
            /// @return Updated iterator
            //------------------------------------------------------------------------
            ConcurrentVectorConstReverseIterator& operator++()
            {
                m_iterableIndex = FindPreviousOccupiedIndex(m_iterableIndex);
                return *this;
            }
            //------------------------------------------------------------------------
            /// Moves the iterator to point to the element in the vector at the given
            /// negative offset from the current iterator
            ///
            /// @author S Downie
            ///
            /// @param Offset
            ///
            /// @return Updated iterator
            //------------------------------------------------------------------------
            ConcurrentVectorConstReverseIterator& operator+=(s32 in_stride)
            {
                m_iterableIndex = FindPreviousOccupiedIndex(m_iterableIndex - in_stride);
                return *this;
            }
            //------------------------------------------------------------------------
            /// Create an iterator that points to the element at the given offset from
            /// this iterator
            ///
            /// @author S Downie
            ///
            /// @param Offset
            ///
            /// @return New iterator
            //------------------------------------------------------------------------
            ConcurrentVectorConstReverseIterator operator+(s32 in_stride)
            {
                auto iterableIndex = FindPreviousOccupiedIndex(m_iterableIndex - in_stride);
                return ConcurrentVectorConstReverseIterator(m_iterable, m_iterableMutex, iterableIndex);
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Pointer to the object pointed to by the iterator
            //------------------------------------------------------------------------
            const TType* operator->() const
            {
                std::unique_lock<std::recursive_mutex> scopedLock(m_iterableMutex);
                return &m_iterable[m_iterableIndex].first;
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The object pointed to by the iterator
            //------------------------------------------------------------------------
            const TType& operator*() const
            {
                std::unique_lock<std::recursive_mutex> scopedLock(m_iterableMutex);
                return m_iterable[m_iterableIndex].first;
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Iterator to compare with
            ///
            /// @return Whether the iterators are considered equal
            //------------------------------------------------------------------------
            bool operator==(const ConcurrentVectorConstReverseIterator& in_toCompare) const
            {
                return m_iterableIndex == in_toCompare.m_iterableIndex;
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Iterator to compare with
            ///
            /// @return Whether the iterators are considered unequal
            //------------------------------------------------------------------------
            bool operator!=(const ConcurrentVectorConstReverseIterator& in_toCompare) const
            {
                return m_iterableIndex != in_toCompare.m_iterableIndex;
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Iterator to compare with
            ///
            /// @return Whether this iterator points to an element after the given iterator
            //------------------------------------------------------------------------
            bool operator>(const ConcurrentVectorConstReverseIterator& in_toCompare) const
            {
                return m_iterableIndex > in_toCompare.m_iterableIndex;
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Iterator to compare with
            ///
            /// @return Whether this iterator points to an element after or the same as the given iterator
            //------------------------------------------------------------------------
            bool operator>=(const ConcurrentVectorConstReverseIterator& in_toCompare) const
            {
                return m_iterableIndex >= in_toCompare.m_iterableIndex;
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Iterator to compare with
            ///
            /// @return Whether this iterator points to an element before the given iterator
            //------------------------------------------------------------------------
            bool operator<(const ConcurrentVectorConstReverseIterator& in_toCompare) const
            {
                return m_iterableIndex < in_toCompare.m_iterableIndex;
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Iterator to compare with
            ///
            /// @return Whether this iterator points to an element before or the same as the given iterator
            //------------------------------------------------------------------------
            bool operator<=(const ConcurrentVectorConstReverseIterator& in_toCompare) const
            {
                return m_iterableIndex <= in_toCompare.m_iterableIndex;
            }
            //------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Element index that the iterate currently points to
            //------------------------------------------------------------------------
            s32 GetIndex() const
            {
                return m_iterableIndex;
            }
            
        private:
            //------------------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Data structure to iterate over
            /// @param Mutex used to protect the underlying iterable
            /// @param Initial index
            //------------------------------------------------------------------------
            ConcurrentVectorConstReverseIterator(std::vector<std::pair<TType, bool>>& in_iterable, std::recursive_mutex& in_iterableMutex, s32 in_initialIndex)
            : m_iterable(in_iterable), m_iterableMutex(in_iterableMutex), m_iterableIndex(in_initialIndex)
            {

            }
            //------------------------------------------------------------------------
            /// Find the previous element index (exclusive of given index) that is not
            /// flagged for removal
            ///
            /// @author S Downie
            ///
            /// @param Index to begin at (exclusive)
            ///
            /// @return Index of previous element (or start if none)
            //-----------------------------------------------------------------------
            s32 FindPreviousOccupiedIndex(s32 in_beginIndex) const
            {
                if(in_beginIndex == 0)
                {
                    return -1;
                }
                
                in_beginIndex--;
                
                std::unique_lock<std::recursive_mutex> scopedLock(m_iterableMutex);
                for(s32 i=in_beginIndex; i>=0; --i)
                {
                    if(m_iterable[i].second == false)
                    {
                        return i;
                    }
                }
                
                return -1;
            }
            
        private:
            
            s32 m_iterableIndex = 0;
            std::vector<std::pair<TType, bool>>& m_iterable;
            std::recursive_mutex& m_iterableMutex;
        };
    }
}

#endif
