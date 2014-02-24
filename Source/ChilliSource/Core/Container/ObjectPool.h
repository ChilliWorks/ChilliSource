//
//  ObjectPool.h
//  MoFlow
//
//  Created by Scott Downie on 23/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_CORE_OBJECT_POOL_H
#define _MOFLOW_CORE_OBJECT_POOL_H

#include <ChilliSource/Core/Base/FastDelegate.h>

#include <new>

namespace ChilliSource
{
    template <typename T> class CObjectPool
    {
        typedef DYNAMIC_ARRAY<T*> Pool;
        typedef SHARED_PTR<T> ObjectSharedPtr;
        typedef T* ObjectPtr;
        
    public:
        
        //---------------------------------------------------------
        /// Constructor
        ///
        /// @param Initial Capacity
        //---------------------------------------------------------
        CObjectPool(u32 inudwInitialCapacity)
        {
            //Fill the pool with blank objects
            maActivePool.reserve(inudwInitialCapacity);
            maReleasePool.reserve(inudwInitialCapacity);
            
            for(u32 i=0; i<inudwInitialCapacity; ++i)
            {
                maReleasePool.push_back(new T());
            }
        }
        //---------------------------------------------------------
        /// Create
        ///
        /// Create or re-cycle an object of type T
        ///
        /// @return Pointer of T
        //---------------------------------------------------------
        ObjectPtr Create() const
        {
            //Find a free object. If none are free
            //then create a new one
            if(maReleasePool.empty())
            {
                maReleasePool.push_back(new T());
                WARNING_LOG("Object Pool - Exceeding initial capacity (" + STRING_CAST((u32)maReleasePool.size() + (u32)maActivePool.size()) + ")");
            }

            T* pObj = maReleasePool.back();
            
            maActivePool.push_back(pObj);
            maReleasePool.pop_back();
            
            //Just call placement new on the object so that we don't alloc any memory
            new (pObj) T();
            
            //Make sure the shared pointer doesn't delete the memory. Give it our own deallocator
            return pObj;
        }
        //---------------------------------------------------------
        /// Create Shared
        ///
        /// Create or re-cycle an object of type T
        ///
        /// @return Shared pointer of T
        //---------------------------------------------------------
        ObjectSharedPtr CreateShared() const
        {
            ObjectPtr pObj = Create();
            
            //Make sure the shared pointer doesn't delete the memory. Give it our own deallocator
            return ObjectSharedPtr(pObj, fastdelegate::FastDelegate1<T*>(const_cast<CObjectPool<T>*>(this), &CObjectPool<T>::Release));
        }
        //----------------------------------------------------------
        /// Release
        ///
        /// Destruct the object and recycle it
        ///
        /// @param Object
        //----------------------------------------------------------
        void Release(T* inpObject)
        {
            typename Pool::iterator it = std::find(maActivePool.begin(), maActivePool.end(), inpObject);
            
            if(it != maActivePool.end())
            {
                *it = maActivePool.back();
                maActivePool.pop_back();
                maReleasePool.push_back(inpObject);
                inpObject->~T();
            }
        }
        //----------------------------------------------------------
        /// Destroy
        ///
        /// Delete all objects and clear the pool cache
        //----------------------------------------------------------
        void Destroy()
        {
            for(typename Pool::iterator it = maActivePool.begin(); it != maActivePool.end(); ++it)
            {
                delete(*it);
            }
            for(typename Pool::iterator it = maReleasePool.begin(); it != maReleasePool.end(); ++it)
            {
                delete(*it);
            }
            
            maActivePool.clear();
            maReleasePool.clear();
        }
        //---------------------------------------------------------
        /// Destructor
        ///
        //---------------------------------------------------------
        ~CObjectPool()
        {
            Destroy();
        }
        
    private:
        
        mutable Pool maActivePool;
        mutable Pool maReleasePool;
    };
}

#endif
