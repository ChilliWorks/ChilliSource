/*
 *  GenericEventImpl.h
 *  moFlow
 *
 *  Created by Ian Copland on 18/12/2012.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_GENERICEVENTIMPL_H_
#define _MOFLO_CORE_GENERICEVENTIMPL_H_

#include <thread>

namespace ChilliSource
{
    namespace Core
    {
        //==============================================================
        /// IEventImpl
        ///
        /// An implementation of the IEvent interface class.
        //==============================================================
        template <typename DelegateType> class IEventImpl
        {
        public:
            //-----------------------------------------------------------
            /// Delegate Object
            ///
            /// A container for listener delegates.
            //-----------------------------------------------------------
            struct DelegateObject
            {
                DelegateType mDelegate;
                bool mbDestroyed;
                bool mbActive;
            };
            
            //-----------------------------------------------------------
            /// Typedefs
            //-----------------------------------------------------------
            typedef typename DYNAMIC_ARRAY<DelegateObject> DelegateList;
            typedef typename DYNAMIC_ARRAY<DelegateObject>::iterator DelegateListItr;
            
            //-----------------------------------------------------------
            /// Constructor
            ///
            /// Declared private so that instances of IEventImpl have
            /// to be created by the GenericEventManager.
            //-----------------------------------------------------------
            IEventImpl();
            //-----------------------------------------------------------
            /// Add Listener
            ///
            /// Adds a new listener to the event.
            ///
            /// @param The delegate.
            //-----------------------------------------------------------
            void AddListener(const DelegateType& inDelegate);
            //-----------------------------------------------------------
            /// Remove Listener
            ///
            /// Removes an existing delegate from the event.
            ///
            /// @param The delegate.
            //-----------------------------------------------------------
            void RemoveListener(const DelegateType& inDelegate);
            //-----------------------------------------------------------
            /// Remove All Listeners
            ///
            /// Removes all existing listeners from the event.
            //-----------------------------------------------------------
            void RemoveAllListeners();
            //-----------------------------------------------------------
            /// Get Num Listeners
            ///
            /// @return The number of listeners subscribed to this event
            //-----------------------------------------------------------
            u32 GetNumListeners() const;
            //-----------------------------------------------------------
            /// Destructor
            //-----------------------------------------------------------
            virtual ~IEventImpl(){}
        protected:
            //-----------------------------------------------------------
            /// Copy constructor
            ///
            /// This is declared private to ensure that no copies of the
            /// event are made. It doesn't make sense to copy a delegate
            /// and it would also break the object's mutex.
            ///
            /// @param another Event of the same delegate type.
            //-----------------------------------------------------------
            IEventImpl(const IEventImpl<DelegateType>& inEvent){}
            //-----------------------------------------------------------
            /// = operator
            ///
            /// This is declared private to ensure that no copies of the
            /// event are made. It doesn't make sense to copy a delegate
            /// and it would also break the object's mutex.
            ///
            /// @param another event of the same delegate type.
            /// @return an empty event.
            //-----------------------------------------------------------
            IEventImpl<DelegateType> operator =(IEventImpl<DelegateType> inEvent);
            //-----------------------------------------------------------
            /// Find Delegate
            ///
            /// @param the start iterator.
            /// @param the end iterator.
            /// @param The delegate to find.
            //-----------------------------------------------------------
            DelegateListItr FindDelegate(const DelegateListItr& inFirst, const DelegateListItr& inLast, const DelegateType& inValue);
            //-----------------------------------------------------------
            /// Remove Listeners Marked For Destruction
            ///
            /// Iterate through the delegates and erase any that have
            /// been flagged for destruction
            //-----------------------------------------------------------
            void RemoveListenersMarkedForDestruction();
            
        protected:
            
            DelegateList maDelegates;
            
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveMutex mMutex;
#endif
            
            bool mbIsInvoking;
            bool mbAreAnyDelegatesMarkedForRemoval;
        };
        //==============================================================
        /// CEvent0Impl
        ///
        /// An implementation of the IEvent0 interface
        //==============================================================
        template <typename DelegateType> class CEvent0Impl : public IEventImpl<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Invoke
            ///
            /// Calls all of the delegates associated with this event.
            //-----------------------------------------------------------
            void Invoke();
        };
        //==============================================================
        /// CEvent1Impl
        ///
        /// An implementation of the IEvent1 interface
        //==============================================================
        template <typename DelegateType> class CEvent1Impl : public IEventImpl<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Invoke
            ///
            /// Calls all of the delegates associated with this event.
            ///
            /// @param parameter 1.
            //-----------------------------------------------------------
            template <typename P1Type> void Invoke(P1Type inP1);
        };
        //==============================================================
        /// CEvent2Impl
        ///
        /// An implementation of the IEvent2 interface
        //==============================================================
        template <typename DelegateType> class CEvent2Impl : public IEventImpl<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Invoke
            ///
            /// Calls all of the delegates associated with this event.
            ///
            /// @param parameter 1.
            /// @param parameter 2.
            //-----------------------------------------------------------
            template <typename P1Type, typename P2Type> void Invoke(P1Type inP1, P2Type inP2);
        };
        //==============================================================
        /// CEvent3Impl
        ///
        /// An implementation of the IEvent3 interface
        //==============================================================
        template <typename DelegateType> class CEvent3Impl : public IEventImpl<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Invoke
            ///
            /// Calls all of the delegates associated with this event.
            ///
            /// @param parameter 1.
            /// @param parameter 2.
            /// @param parameter 3.
            //-----------------------------------------------------------
            template <typename P1Type, typename P2Type, typename P3Type> void Invoke(P1Type inP1, P2Type inP2, P3Type inP3);
        };
        //==============================================================
        /// CEvent4Impl
        ///
        /// An implementation of the IEvent4 interface
        //==============================================================
        template <typename DelegateType> class CEvent4Impl : public IEventImpl<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Invoke
            ///
            /// Calls all of the delegates associated with this event.
            ///
            /// @param parameter 1.
            /// @param parameter 2.
            /// @param parameter 3.
            /// @param parameter 4.
            //-----------------------------------------------------------
            template <typename P1Type, typename P2Type, typename P3Type, typename P4Type> void Invoke(P1Type inP1, P2Type inP2, P3Type inP3, P4Type inP4);
        };
        //===============================================================
        /// Implementation of IEventImpl
        //===============================================================
        //---------------------------------------------------------------
        /// Add Listener
        //---------------------------------------------------------------
        template<typename DelegateType> void IEventImpl<DelegateType>::AddListener(const DelegateType& inDelegate)
        {
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveScopedLock lock(mMutex);
#endif
            
            DelegateListItr it = FindDelegate(maDelegates.begin(), maDelegates.end(), inDelegate);
            
            if(it != maDelegates.end())
            {
                if(it->mbDestroyed)
                {
                    it->mbDestroyed = false;
                }
                else
                {
                    WARNING_LOG("GenericEvent - Adding a duplicate Listener");
                }
                return;
            }
            
            if(inDelegate)
            {
                DelegateObject sDelegateObject;
                sDelegateObject.mDelegate = inDelegate;
                sDelegateObject.mbDestroyed = false;
                sDelegateObject.mbActive = !mbIsInvoking;
                maDelegates.push_back(sDelegateObject);
            }
        }
        //---------------------------------------------------------------
        /// Remove Listener
        //---------------------------------------------------------------
        template<typename DelegateType> void IEventImpl<DelegateType>::RemoveListener(const DelegateType& inDelegate)
        {
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveScopedLock lock(mMutex);
#endif
            
            DelegateListItr it = FindDelegate(maDelegates.begin(), maDelegates.end(), inDelegate);
            
            if(it != maDelegates.end())
            {
                if(mbIsInvoking)
                {
                    mbAreAnyDelegatesMarkedForRemoval = true;
                    it->mbDestroyed = true;
                }
                else
                {
                    maDelegates.erase(it);
                }
            }
        }
        //---------------------------------------------------------------
        /// Remove All Listeners
        //---------------------------------------------------------------
        template<typename DelegateType> void IEventImpl<DelegateType>::RemoveAllListeners()
        {
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveScopedLock lock(mMutex);
#endif
            
            if(mbIsInvoking)
            {
                mbAreAnyDelegatesMarkedForRemoval = true;
                for(u32 i=0; i<maDelegates.size(); ++i)
                {
                    maDelegates[i].mbDestroyed = true;
                }
            }
            else
            {
                mbAreAnyDelegatesMarkedForRemoval = false;
                maDelegates.clear();
            }
        }
        //-----------------------------------------------------------
        /// Get Num Listeners
        ///
        /// @return The number of listeners subscribed to this event
        //-----------------------------------------------------------
        template<typename DelegateType> u32 IEventImpl<DelegateType>::GetNumListeners() const
        {
            return maDelegates.size();
        }
        //---------------------------------------------------------------
        /// Constructor
        //---------------------------------------------------------------
        template<typename DelegateType> IEventImpl<DelegateType>::IEventImpl() : mbIsInvoking(false), mbAreAnyDelegatesMarkedForRemoval(false)
        {
        }
        //---------------------------------------------------------------
        /// = operator
        //---------------------------------------------------------------
        template<typename DelegateType> IEventImpl<DelegateType> IEventImpl<DelegateType>::operator =(IEventImpl<DelegateType> inEvent)
        {
            return IEventImpl<DelegateType>();
        }
        //---------------------------------------------------------------
        /// Find Delegate
        //---------------------------------------------------------------
        template<typename DelegateType> typename IEventImpl<DelegateType>::DelegateListItr IEventImpl<DelegateType>::FindDelegate(const DelegateListItr& inFirst, const DelegateListItr& inLast, const DelegateType& inValue)
        {
            DelegateListItr it = inFirst;
            for ( ; it != inLast; ++it)
            {
                if ((*it).mDelegate == inValue)
                    return it;
            }
            return inLast;
        }
        //-----------------------------------------------------------
        /// Remove Listeners Marked For Destruction
        ///
        /// Iterate through the delegates and erase any that have
        /// been flagged for destruction
        //-----------------------------------------------------------
        template<typename DelegateType> void IEventImpl<DelegateType>::RemoveListenersMarkedForDestruction()
        {
            if(mbAreAnyDelegatesMarkedForRemoval == false)
                return;
            
            //Clean up destroyed delegates
            for(DelegateListItr it = maDelegates.begin(); it != maDelegates.end(); )
            {
                if(it->mbDestroyed == true)
                {
                    it = maDelegates.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            
            mbAreAnyDelegatesMarkedForRemoval = false;
        }
        //===============================================================
        /// Implementation of CEvent0Impl
        //===============================================================
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> void CEvent0Impl<DelegateType>::Invoke()
        {
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveScopedLock lock(this->mMutex);
#endif
            
            if(this->mbIsInvoking)
            {
                WARNING_LOG("GenericEvent - Nested Invoke code detected");
                return;
            }
            
            this->mbIsInvoking = true;
            
            for(u32 i=0; i<this->maDelegates.size(); ++i)
            {
                if(this->maDelegates[i].mbDestroyed == false)
                {
                    if(this->maDelegates[i].mbActive == true)
                    {
                        this->maDelegates[i].mDelegate();
                    }
                    else
                    {
                        this->maDelegates[i].mbActive = true;
                    }
                }
            }
            
            this->RemoveListenersMarkedForDestruction();
            
            this->mbIsInvoking = false;
        }
        //===============================================================
        /// Implementation of CEvent1Impl
        //===============================================================
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> template <typename P1Type> void CEvent1Impl<DelegateType>::Invoke(P1Type inP1)
        {
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveScopedLock lock(this->mMutex);
#endif
            
            if(this->mbIsInvoking)
            {
                WARNING_LOG("GenericEvent - Nested Invoke code detected");
                return;
            }
            
            this->mbIsInvoking = true;
            
            for(u32 i=0; i<this->maDelegates.size(); ++i)
            {
                if(this->maDelegates[i].mbDestroyed == false)
                {
                    if(this->maDelegates[i].mbActive == true)
                    {
                        this->maDelegates[i].mDelegate(inP1);
                    }
                    else
                    {
                        this->maDelegates[i].mbActive = true;
                    }
                }
            }
            
            this->RemoveListenersMarkedForDestruction();
            
            this->mbIsInvoking = false;
        }
        //===============================================================
        /// Implementation of CEvent2Impl
        //===============================================================
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> template <typename P1Type, typename P2Type> void CEvent2Impl<DelegateType>::Invoke(P1Type inP1, P2Type inP2)
        {
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveScopedLock lock(this->mMutex);
#endif
            
            if(this->mbIsInvoking)
            {
                WARNING_LOG("GenericEvent - Nested Invoke code detected");
                return;
            }
            
            this->mbIsInvoking = true;
            
            for(u32 i=0; i<this->maDelegates.size(); ++i)
            {
                if(this->maDelegates[i].mbDestroyed == false)
                {
                    if(this->maDelegates[i].mbActive == true)
                    {
                        this->maDelegates[i].mDelegate(inP1, inP2);
                    }
                    else
                    {
                        this->maDelegates[i].mbActive = true;
                    }
                }
            }
            
            this->RemoveListenersMarkedForDestruction();
            
            this->mbIsInvoking = false;
        }
        //===============================================================
        /// Implementation of CEvent3Impl
        //===============================================================
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> template <typename P1Type, typename P2Type, typename P3Type> void CEvent3Impl<DelegateType>::Invoke(P1Type inP1, P2Type inP2, P3Type inP3)
        {
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveScopedLock lock(this->mMutex);
#endif
            
            if(this->mbIsInvoking)
            {
                WARNING_LOG("GenericEvent - Nested Invoke code detected");
                return;
            }
            
            this->mbIsInvoking = true;
            
            for(u32 i=0; i<this->maDelegates.size(); ++i)
            {
                if(this->maDelegates[i].mbDestroyed == false)
                {
                    if(this->maDelegates[i].mbActive == true)
                    {
                        this->maDelegates[i].mDelegate(inP1, inP2, inP3);
                    }
                    else
                    {
                        this->maDelegates[i].mbActive = true;
                    }
                }
            }
            
            this->RemoveListenersMarkedForDestruction();
            
            this->mbIsInvoking = false;
        }
        //===============================================================
        /// Implementation of CEvent4Impl
        //===============================================================
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> template <typename P1Type, typename P2Type, typename P3Type, typename P4Type> void CEvent4Impl<DelegateType>::Invoke(P1Type inP1, P2Type inP2, P3Type inP3, P4Type inP4)
        {
#ifdef MOFLOW_THREAD_SAFE_EVENTS
            std::thread::RecursiveScopedLock lock(this->mMutex);
#endif
            
            if(this->mbIsInvoking)
            {
                WARNING_LOG("GenericEvent - Nested Invoke code detected");
                return;
            }
            
            this->mbIsInvoking = true;
            
            for(u32 i=0; i<this->maDelegates.size(); ++i)
            {
                if(this->maDelegates[i].mbDestroyed == false)
                {
                    if(this->maDelegates[i].mbActive == true)
                    {
                        this->maDelegates[i].mDelegate(inP1, inP2, inP3, inP4);
                    }
                    else
                    {
                        this->maDelegates[i].mbActive = true;
                    }
                }
            }
            
            this->RemoveListenersMarkedForDestruction();
            
            this->mbIsInvoking = false;
        }
    }
}

#endif