//
//  HashedArray.h
//  MoFlow
//
//  Created by Scott Downie on 16/02/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_CORE_CONTAINERS_HASHED_ARRAY_H_
#define _MOFLOW_CORE_CONTAINERS_HASHED_ARRAY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

#include <functional>

namespace ChilliSource
{
    namespace Core
    {
        //=======================================================================
        /// Generic
        //=======================================================================
        //=======================================================================
        /// Key - K
        //=======================================================================
        template <typename K, typename T> class HashedArray
        {
        public:

            typedef std::function<u32(K)> HashDelegate;
            typedef std::pair<u32, T> KeyValue;
            typedef KeyValue* iterator;
            typedef const KeyValue* const_iterator;
            
            //-------------------------------------------
            /// Constructor
            ///
            /// @param Size/Capcity of the array
            /// @param Hash function
            //-------------------------------------------
            HashedArray(u32 inudwCapacity, const HashDelegate& inDelegate) 
            : mudwCapacity(inudwCapacity), mudwSize(0), mHashDelegate(inDelegate), mbSortCacheValid(true)
            {
                CS_ASSERT(inudwCapacity > 0, "Cannot create an array with capacity zero");
                mpaStorage = new KeyValue[inudwCapacity];
            }
            //-------------------------------------------
            /// Copy Constructor
            ///
            /// @param Copy
            //-------------------------------------------
            HashedArray(const HashedArray& inRHS) 
            : mudwCapacity(inRHS.mudwCapacity), mudwSize(inRHS.mudwSize), mHashDelegate(inRHS.mHashDelegate), mbSortCacheValid(inRHS.mbSortCacheValid)
            {
                mpaStorage = new KeyValue[inRHS.mudwSize];
                
                std::copy(inRHS.begin(), inRHS.end(), this->begin());
            }
            //-------------------------------------------
            /// Copy Assignment
            ///
            /// @param Copy
            //-------------------------------------------
            void operator=(const HashedArray& inRHS)
            {
                mudwCapacity = inRHS.mudwCapacity;
                mudwSize = inRHS.mudwSize;
                mHashDelegate = inRHS.mHashDelegate;
                mbSortCacheValid = inRHS.mbSortCacheValid;
                std::copy(inRHS.begin(), inRHS.end(), this->begin());
            }
            //-------------------------------------------
            /// Insert
            ///
            /// Insert the object into the array
            ///
            /// @param Key
            /// @param Value
            /// @return False if exceeds capacity
            //-------------------------------------------
            bool Insert(const K& inKey, const T& inValue)
            {
                if(mudwSize >= mudwCapacity)
                {
                    return false;
                }
                
                CS_ASSERT(mHashDelegate, "Key type cannot be hashed without providing a custom hash method");
                u32 udwHash = mHashDelegate(inKey);
                mpaStorage[mudwSize].first = udwHash;
                mpaStorage[mudwSize].second = inValue;
                mbSortCacheValid = false;
                mudwSize++;
                return true;
            }
            //-------------------------------------------
            /// Remove 
            ///
            /// Remove the object identified by the
            /// given key
            ///
            /// @param Remove
            //-------------------------------------------
            void Remove(const K& inKey)
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                CS_ASSERT(mHashDelegate, "Key type cannot be hashed without providing a custom hash method");
                
                u32 udwHash = mHashDelegate(inKey);
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    for(s32 i=dwIndex; i<(mudwSize-1); i++)
                    {	
                        mpaStorage[i] = mpaStorage[i+1];
                    }
                    
                    mpaStorage[dwIndex].~KeyValue();
                    
                    mudwSize--;
                }
            }
            //-------------------------------------------
            /// Clear
            ///
            /// Clear all objects
            //-------------------------------------------
            void Clear()
            {
                for(u32 i=0; i<mudwSize; ++i)
                {	
                    mpaStorage[i].~KeyValue();
                }
                
                mudwSize = 0;
            }
            //-------------------------------------------
            /// Find
            ///
            /// @param Key
            /// @return Value
            //-------------------------------------------
            iterator Find(const K& inKey) 
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                CS_ASSERT(mHashDelegate, "Key type cannot be hashed without providing a custom hash method");
                
                u32 udwHash = mHashDelegate(inKey);
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    return &mpaStorage[dwIndex];
                }
             
                return end();
            }
            //-------------------------------------------
            /// Find
            ///
            /// @param Key
            /// @return Value
            //-------------------------------------------
            const_iterator Find(const K& inKey) const 
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                CS_ASSERT(mHashDelegate, "Key type cannot be hashed without providing a custom hash method");
                
                u32 udwHash = mHashDelegate(inKey);
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    return &mpaStorage[dwIndex];
                }
                
                return end();
            }
            //-------------------------------------------
            /// Get Length
            ///
            /// The number of elements that have been
            /// added to the array
            ///
            /// @return Length
            //-------------------------------------------
            inline u32 GetLength() const
            {
                return mudwSize;
            }
            //-------------------------------------------
            /// Get Capacity
            ///
            /// @return Size/Capcity of the array
            //-------------------------------------------
            inline u32 GetCapacity() const
            {
                return mudwCapacity;
            }
            //-------------------------------------------
            /// Begin
            ///
            /// @return Iterator pointing to the first
            /// object in the structure
            //-------------------------------------------
            inline iterator begin() 
            {
                return mpaStorage;
            }
            //-------------------------------------------
            /// End
            ///
            /// @return Iterator pointing beyond the last
            /// object in the structure
            //-------------------------------------------
            inline iterator end() 
            {
                return (mpaStorage + mudwSize);
            }
            //-------------------------------------------
            /// Begin
            ///
            /// @return Iterator pointing to the first
            /// object in the structure
            //-------------------------------------------
            inline const_iterator begin() const
            {
                return mpaStorage;
            }
            //-------------------------------------------
            /// End
            ///
            /// @return Iterator pointing beyond the last
            /// object in the structure
            //-------------------------------------------
            inline const_iterator end() const
            {
                return (mpaStorage + mudwSize);
            }
            
        private:
            
            //---Sort Predicate
            //-------------------------------------------
            /// Less Than
            ///
            /// @param LHS
            /// @param RHS
            /// @return Whether LHS is less than RHS
            //-------------------------------------------
            static bool LessThanSortPredicate(const KeyValue& inLHS, const KeyValue& inRHS)
            {
                return inLHS.first < inRHS.first;
            }
            //--------------------------------------------
            /// Find Index
            ///
            /// Performs a binary search looking for the
            /// index of the object with the given key
            ///
            /// @param Key
            /// @return Index of object identified by key
            /// or -1 if not found
            //--------------------------------------------
            s32 FindIndex(u32 inudwKey) const
            {
                if(mudwSize == 0)
                {
                    return -1;
                }
                
                u32 udwLow = 0;
                s32 dwHigh = mudwSize-1;
                u32 udwMidpoint = 0;
                
                while((s32)udwLow <= dwHigh)
                {
                    udwMidpoint = (dwHigh + udwLow)/ 2;
                    if(inudwKey == mpaStorage[udwMidpoint].first)
                    {
                        return udwMidpoint;
                    }
                    else if(inudwKey < mpaStorage[udwMidpoint].first)
                    {
                        dwHigh = udwMidpoint - 1;
                    }
                    else
                    {
                        udwLow = udwMidpoint + 1;
                    }
                }
                
                return -1;
            }
            
        private:
            
            HashDelegate mHashDelegate;
            
            u32 mudwSize;
            u32 mudwCapacity;
            
            mutable bool mbSortCacheValid;
            
            KeyValue* mpaStorage; 
        };
        
        //=======================================================================
        /// Specialisations
        //=======================================================================
        //=======================================================================
        /// Key - String
        //=======================================================================
        template <typename T> class HashedArray <std::string, T>
        {
        public:
            
            typedef std::string K;
            typedef std::function<u32(K)> HashDelegate;
            typedef std::pair<u32, T> KeyValue;
            typedef KeyValue* iterator;
            typedef const KeyValue* const_iterator;
            
            //-------------------------------------------
            /// Constructor
            ///
            /// @param Size/Capcity of the array
            //-------------------------------------------
            HashedArray(u32 inudwCapacity) 
            : mudwCapacity(inudwCapacity), mudwSize(0), mbSortCacheValid(true)
            {
                CS_ASSERT(inudwCapacity > 0, "Cannot create an array with capacity zero");
                mpaStorage = new KeyValue[inudwCapacity];
            }
            //-------------------------------------------
            /// Copy Constructor
            ///
            /// @param Copy
            //-------------------------------------------
            HashedArray(const HashedArray& inRHS) 
            : mudwCapacity(inRHS.mudwCapacity), mudwSize(inRHS.mudwSize), mbSortCacheValid(inRHS.mbSortCacheValid)
            {
                mpaStorage = new KeyValue[inRHS.mudwSize];
                
                std::copy(inRHS.begin(), inRHS.end(), this->begin());
            }
            //-------------------------------------------
            /// Copy Assignment
            ///
            /// @param Copy
            //-------------------------------------------
            void operator=(const HashedArray& inRHS)
            {
                mudwCapacity = inRHS.mudwCapacity;
                mudwSize = inRHS.mudwSize;
                mbSortCacheValid = inRHS.mbSortCacheValid;
                std::copy(inRHS.begin(), inRHS.end(), this->begin());
            }
            //-------------------------------------------
            /// Insert
            ///
            /// Insert the object into the array
            ///
            /// @param Key
            /// @param Value
            /// @return False if exceeds capacity
            //-------------------------------------------
            bool Insert(const K& inKey, const T& inValue)
            {
                if(mudwSize >= mudwCapacity)
                {
                    return false;
                }
                
                u32 udwHash = CHashCRC32::GenerateHashCode(inKey);
                mpaStorage[mudwSize].first = udwHash;
                mpaStorage[mudwSize].second = inValue;
                mbSortCacheValid = false;
                mudwSize++;
                return true;
            }
            //-------------------------------------------
            /// Remove 
            ///
            /// Remove the object identified by the
            /// given key
            ///
            /// @param Remove
            //-------------------------------------------
            void Remove(const K& inKey)
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                u32 udwHash = CHashCRC32::GenerateHashCode(inKey);
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    for(s32 i=dwIndex; i<(mudwSize-1); ++i)
                    {	
                        mpaStorage[i] = mpaStorage[i+1];
                    }
                    
                    mpaStorage[dwIndex].~KeyValue();
                    
                    mudwSize--;
                }
            }
            //-------------------------------------------
            /// Clear
            ///
            /// Clear all objects
            //-------------------------------------------
            void Clear()
            {
                for(u32 i=0; i<mudwSize; ++i)
                {	
                    mpaStorage[i].~KeyValue();
                }
                
                mudwSize = 0;
            }
            //-------------------------------------------
            /// Find
            ///
            /// @param Key
            /// @return Value
            //-------------------------------------------
            iterator Find(const K& inKey) 
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                u32 udwHash = CHashCRC32::GenerateHashCode(inKey);
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    return &mpaStorage[dwIndex];
                }
                
                return end();
            }
            //-------------------------------------------
            /// Find
            ///
            /// @param Key
            /// @return Value
            //-------------------------------------------
            const_iterator Find(const K& inKey) const 
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                u32 udwHash = CHashCRC32::GenerateHashCode(inKey);
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    return &mpaStorage[dwIndex];
                }
                
                return end();
            }
            //-------------------------------------------
            /// Get Length
            ///
            /// The number of elements that have been
            /// added to the array
            ///
            /// @return Length
            //-------------------------------------------
            inline u32 GetLength() const
            {
                return mudwSize;
            }
            //-------------------------------------------
            /// Get Capacity
            ///
            /// @return Size/Capcity of the array
            //-------------------------------------------
            inline u32 GetCapacity() const
            {
                return mudwCapacity;
            }
            //-------------------------------------------
            /// Begin
            ///
            /// @return Iterator pointing to the first
            /// object in the structure
            //-------------------------------------------
            inline iterator begin()
            {
                return mpaStorage;
            }
            //-------------------------------------------
            /// End
            ///
            /// @return Iterator pointing beyond the last
            /// object in the structure
            //-------------------------------------------
            inline iterator end()
            {
                return (mpaStorage + mudwSize);
            }
            //-------------------------------------------
            /// Begin
            ///
            /// @return Iterator pointing to the first
            /// object in the structure
            //-------------------------------------------
            inline const_iterator begin() const
            {
                return mpaStorage;
            }
            //-------------------------------------------
            /// End
            ///
            /// @return Iterator pointing beyond the last
            /// object in the structure
            //-------------------------------------------
            inline const_iterator end() const
            {
                return (mpaStorage + mudwSize);
            }
            
        private:
            
            //---Sort Predicate
            //-------------------------------------------
            /// Less Than
            ///
            /// @param LHS
            /// @param RHS
            /// @return Whether LHS is less than RHS
            //-------------------------------------------
            static bool LessThanSortPredicate(const KeyValue& inLHS, const KeyValue& inRHS)
            {
                return inLHS.first < inRHS.first;
            }
            
            //--------------------------------------------
            /// Find Index
            ///
            /// Performs a binary search looking for the
            /// index of the object with the given key
            ///
            /// @param Key
            /// @return Index of object identified by key
            /// or -1 if not found
            //--------------------------------------------
            s32 FindIndex(u32 inudwKey) const
            {
                if(mudwSize == 0)
                {
                    return -1;
                }
                
                u32 udwLow = 0;
                s32 dwHigh = mudwSize-1;
                u32 udwMidpoint = 0;
                
                while((s32)udwLow <= dwHigh)
                {
                    udwMidpoint = (dwHigh + udwLow)/ 2;
                    if(inudwKey == mpaStorage[udwMidpoint].first)
                    {
                        return udwMidpoint;
                    }
                    else if(inudwKey < mpaStorage[udwMidpoint].first)
                    {
                        dwHigh = udwMidpoint - 1;
                    }
                    else
                    {
                        udwLow = udwMidpoint + 1;
                    }
                }
                
                return -1;
            }
            
        private:
            
            u32 mudwSize;
            u32 mudwCapacity;
            
            mutable bool mbSortCacheValid;
            
            KeyValue* mpaStorage; 
        };
        
        //=======================================================================
        /// Key - u32
        //=======================================================================
        template <typename T> class HashedArray <u32, T>
        {
        public:
            
            typedef u32 K;
            typedef std::function<u32(K)> HashDelegate;
            typedef std::pair<u32, T> KeyValue;
            typedef KeyValue* iterator;
            typedef const KeyValue* const_iterator;
            
            //-------------------------------------------
            /// Constructor
            ///
            /// @param Size/Capcity of the array
            //-------------------------------------------
            HashedArray(u32 inudwCapacity) 
            : mudwCapacity(inudwCapacity), mudwSize(0), mbSortCacheValid(true)
            {
                CS_ASSERT(inudwCapacity > 0, "Cannot create an array with capacity zero");
                mpaStorage = new KeyValue[inudwCapacity];
            }
            //-------------------------------------------
            /// Copy Constructor
            ///
            /// @param Copy
            //-------------------------------------------
            HashedArray(const HashedArray& inRHS) 
            : mudwCapacity(inRHS.mudwCapacity), mudwSize(inRHS.mudwSize), mbSortCacheValid(inRHS.mbSortCacheValid)
            {
                mpaStorage = new KeyValue[inRHS.mudwSize];
                
                std::copy(inRHS.begin(), inRHS.end(), this->begin());
            }
            //-------------------------------------------
            /// Copy Assignment
            ///
            /// @param Copy
            //-------------------------------------------
            void operator=(const HashedArray& inRHS)
            {
                mudwCapacity = inRHS.mudwCapacity;
                mudwSize = inRHS.mudwSize;
                mbSortCacheValid = inRHS.mbSortCacheValid;
                std::copy(inRHS.begin(), inRHS.end(), this->begin());
            }
            //-------------------------------------------
            /// Insert
            ///
            /// Insert the object into the array
            ///
            /// @param Key
            /// @param Value
            /// @return False if exceeds capacity
            //-------------------------------------------
            bool Insert(K inKey, const T& inValue)
            {
                if(mudwSize >= mudwCapacity)
                {
                    return false;
                }
                
                mpaStorage[mudwSize].first = inKey;
                mpaStorage[mudwSize].second = inValue;
                mbSortCacheValid = false;
                mudwSize++;
                return true;
            }
            //-------------------------------------------
            /// Remove 
            ///
            /// Remove the object identified by the
            /// given key
            ///
            /// @param Remove
            //-------------------------------------------
            void Remove(K inKey)
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                u32 udwHash = inKey;
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    for(s32 i=dwIndex; i<(mudwSize-1); i++)
                    {	
                        mpaStorage[i] = mpaStorage[i+1];
                    }
                    
                    mpaStorage[dwIndex].~KeyValue();
                    
                    mudwSize--;
                }
            }
            //-------------------------------------------
            /// Clear
            ///
            /// Clear all objects
            //-------------------------------------------
            void Clear()
            {
                for(u32 i=0; i<mudwSize; ++i)
                {	
                   mpaStorage[i].~KeyValue();
                }
                
                mudwSize = 0;
            }
            //-------------------------------------------
            /// Find
            ///
            /// @param Key
            /// @return Value
            //-------------------------------------------
            iterator Find(K inKey)  
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                u32 udwHash = inKey;
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    return &mpaStorage[dwIndex];
                }
                
                return end();
            }
            //-------------------------------------------
            /// Find
            ///
            /// @param Key
            /// @return Value
            //-------------------------------------------
            const_iterator Find(K inKey) const 
            {
                if(!mbSortCacheValid)
                {
                    std::sort(mpaStorage, mpaStorage + mudwSize, HashedArray::LessThanSortPredicate);
                    mbSortCacheValid = true;
                }
                
                u32 udwHash = inKey;
                s32 dwIndex = FindIndex(udwHash);
                
                if(dwIndex >= 0)
                {
                    return &mpaStorage[dwIndex];
                }
                
                return end();
            }
            //-------------------------------------------
            /// Get Length
            ///
            /// The number of elements that have been
            /// added to the array
            ///
            /// @return Length
            //-------------------------------------------
            inline u32 GetLength() const
            {
                return mudwSize;
            }
            //-------------------------------------------
            /// Get Capacity
            ///
            /// @return Size/Capcity of the array
            //-------------------------------------------
            inline u32 GetCapacity() const
            {
                return mudwCapacity;
            }
            //-------------------------------------------
            /// Begin
            ///
            /// @return Iterator pointing to the first
            /// object in the structure
            //-------------------------------------------
            inline iterator begin()
            {
                return mpaStorage;
            }
            //-------------------------------------------
            /// End
            ///
            /// @return Iterator pointing beyond the last
            /// object in the structure
            //-------------------------------------------
            inline iterator end()
            {
                return (mpaStorage + mudwSize);
            }
            //-------------------------------------------
            /// Begin
            ///
            /// @return Iterator pointing to the first
            /// object in the structure
            //-------------------------------------------
            inline const_iterator begin() const
            {
                return mpaStorage;
            }
            //-------------------------------------------
            /// End
            ///
            /// @return Iterator pointing beyond the last
            /// object in the structure
            //-------------------------------------------
            inline const_iterator end() const
            {
                return (mpaStorage + mudwSize);
            }
            
        private:
            
            //---Sort Predicate
            //-------------------------------------------
            /// Less Than
            ///
            /// @param LHS
            /// @param RHS
            /// @return Whether LHS is less than RHS
            //-------------------------------------------
            static bool LessThanSortPredicate(const KeyValue& inLHS, const KeyValue& inRHS)
            {
                return inLHS.first < inRHS.first;
            }
            
            //--------------------------------------------
            /// Find Index
            ///
            /// Performs a binary search looking for the
            /// index of the object with the given key
            ///
            /// @param Key
            /// @return Index of object identified by key
            /// or -1 if not found
            //--------------------------------------------
            s32 FindIndex(u32 inudwKey) const
            {
                if(mudwSize == 0)
                {
                    return -1;
                }
                
                u32 udwLow = 0;
                s32 dwHigh = mudwSize-1;
                u32 udwMidpoint = 0;
                
                while((s32)udwLow <= dwHigh)
                {
                    udwMidpoint = (dwHigh + udwLow)/ 2;
                    if(inudwKey == mpaStorage[udwMidpoint].first)
                    {
                        return udwMidpoint;
                    }
                    else if(inudwKey < mpaStorage[udwMidpoint].first)
                    {
                        dwHigh = udwMidpoint - 1;
                    }
                    else
                    {
                        udwLow = udwMidpoint + 1;
                    }
                }
                
                return -1;
            }
            
        private:
            
            u32 mudwSize;
            u32 mudwCapacity;
            
            mutable bool mbSortCacheValid;
            
            KeyValue* mpaStorage; 
        };
    }
}

#endif

