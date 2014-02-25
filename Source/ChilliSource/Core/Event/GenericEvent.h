/*
 *  GenericEvent.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 01/11/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_GENERICEVENT_H_
#define _MOFLO_CORE_GENERICEVENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/GenericEventImpl.h>

namespace ChilliSource
{
    namespace Core
    {
        //===============================================================
        /// IEvent
        ///
        /// An IEvent represents an object which can have multiple listeners
        /// with the given DelegateType. The subclasses CEventX are what
        /// should be instanced by objects as they have an Invoke method.
        /// Objects should though expose the IEvent interface to prevent
        /// other objects invoking the event.
        //===============================================================
        template <typename DelegateType> class IEvent
        {
        public:
            
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            IEvent();
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
            /// += Operator
            ///
            /// Adds a new listener to the event.
            ///
            /// @param The delegate.
            //-----------------------------------------------------------
            void operator +=(DelegateType inDelegate);
            //-----------------------------------------------------------
            /// -= Operator
            ///
            /// Removes an existing listener from the event.
            ///
            /// @param The delegate.
            //-----------------------------------------------------------
            void operator -=(DelegateType inDelegate);
            //-----------------------------------------------------------
            /// Destructor
            //-----------------------------------------------------------
            virtual ~IEvent()
            {
                SAFE_DELETE(mpImpl);
            }
            
        protected:
            
            IEventImpl<DelegateType>* mpImpl;
        };
        //===============================================================
        /// IEvent0
        ///
        /// An event which takes no parameters when invoked
        //===============================================================
        template <typename DelegateType> class CEvent0 : public IEvent<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            CEvent0();
            //-----------------------------------------------------------
            /// Invoke
            ///
            /// Calls all of the delegates associated with this event.
            //-----------------------------------------------------------
            void Invoke();
        };
        //===============================================================
        /// IEvent1
        ///
        /// An event which takes 1 parameters when invoked
        //===============================================================
        template <typename DelegateType> class CEvent1 : public IEvent<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            CEvent1();
            //-----------------------------------------------------------
            /// Invoke
            ///
            /// Calls all of the delegates associated with this event.
            ///
            /// @param parameter 1.
            //-----------------------------------------------------------
            template <typename P1Type> void Invoke(P1Type inP1);
        };
        //===============================================================
        /// IEvent2
        ///
        /// An event which takes 2 parameters when invoked
        //===============================================================
        template <typename DelegateType> class CEvent2 : public IEvent<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            CEvent2();
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
        //===============================================================
        /// IEvent3
        ///
        /// An event which takes 3 parameters when invoked
        //===============================================================
        template <typename DelegateType> class CEvent3 : public IEvent<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            CEvent3();
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
        //===============================================================
        /// IEvent4
        ///
        /// An event which takes 4 parameters when invoked
        //===============================================================
        template <typename DelegateType> class CEvent4 : public IEvent<DelegateType>
        {
        public:
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            CEvent4();
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
        /// Implementation of IEvent
        //===============================================================
        //---------------------------------------------------------------
        /// Constructor
        //---------------------------------------------------------------
        template<typename DelegateType> IEvent<DelegateType>::IEvent()
        {
        }
        //---------------------------------------------------------------
        /// Add Listener
        //---------------------------------------------------------------
        template<typename DelegateType> void IEvent<DelegateType>::AddListener(const DelegateType& inDelegate)
        {
            if(this->mpImpl)
            {
                this->mpImpl->AddListener(inDelegate);
            }
        }
        //---------------------------------------------------------------
        /// Remove Listener
        //---------------------------------------------------------------
        template<typename DelegateType> void IEvent<DelegateType>::RemoveListener(const DelegateType& inDelegate)
        {
            if(this->mpImpl)
            {
                this->mpImpl->RemoveListener(inDelegate);
            }
        }
        //---------------------------------------------------------------
        /// Remove All Listeners
        //---------------------------------------------------------------
        template<typename DelegateType> void IEvent<DelegateType>::RemoveAllListeners()
        {
            if(this->mpImpl)
            {
                this->mpImpl->RemoveAllListeners();
            }
        }
        //-----------------------------------------------------------
        /// Get Num Listeners
        ///
        /// @return The number of listeners subscribed to this event
        //-----------------------------------------------------------
        template<typename DelegateType> u32 IEvent<DelegateType>::GetNumListeners() const
        {
            if(this->mpImpl)
            {
                return this->mpImpl->GetNumListeners();
            }
            
            return 0;
        }
        //---------------------------------------------------------------
        /// += Operator
        //---------------------------------------------------------------
        template<typename DelegateType> void IEvent<DelegateType>::operator+=(DelegateType inDelegate)
        {
            AddListener(inDelegate);
        }
        //---------------------------------------------------------------
        /// -= Operator
        //---------------------------------------------------------------
        template<typename DelegateType> void IEvent<DelegateType>::operator-=(DelegateType inDelegate)
        {
            RemoveListener(inDelegate);
        }
        //===============================================================
        /// Implementation of CEvent0
        //===============================================================
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        template<typename DelegateType> CEvent0<DelegateType>::CEvent0()
        {
            this->mpImpl = new CEvent0Impl<DelegateType>();
        }
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> void CEvent0<DelegateType>::Invoke()
        {
            if(this->mpImpl)
            {
                static_cast<CEvent0Impl<DelegateType>* >(this->mpImpl)->Invoke();
            }
        }
        //===============================================================
        /// Implementation of CEvent1
        //===============================================================
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        template<typename DelegateType> CEvent1<DelegateType>::CEvent1()
        {
            this->mpImpl = new CEvent1Impl<DelegateType>();
        }
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> template <typename P1Type> void CEvent1<DelegateType>::Invoke(P1Type inP1)
        {
            if(this->mpImpl)
            {
                static_cast<CEvent1Impl<DelegateType>* >(this->mpImpl)->Invoke(inP1);
            }
        }
        //===============================================================
        /// Implementation of CEvent2
        //===============================================================
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        template<typename DelegateType> CEvent2<DelegateType>::CEvent2()
        {
            this->mpImpl = new CEvent2Impl<DelegateType>();
        }
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> template <typename P1Type, typename P2Type> void CEvent2<DelegateType>::Invoke(P1Type inP1, P2Type inP2)
        {
            if(this->mpImpl)
            {
                static_cast<CEvent2Impl<DelegateType>* >(this->mpImpl)->Invoke(inP1, inP2);
            }
        }
        //===============================================================
        /// Implementation of CEvent3
        //===============================================================
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        template<typename DelegateType> CEvent3<DelegateType>::CEvent3()
        {
            this->mpImpl = new CEvent3Impl<DelegateType>();
        }
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> template <typename P1Type, typename P2Type, typename P3Type> void CEvent3<DelegateType>::Invoke(P1Type inP1, P2Type inP2, P3Type inP3)
        {
            if(this->mpImpl)
            {
                static_cast<CEvent3Impl<DelegateType>* >(this->mpImpl)->Invoke(inP1, inP2, inP3);
            }
        }
        //===============================================================
        /// Implementation of CEvent4
        //===============================================================
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        template<typename DelegateType> CEvent4<DelegateType>::CEvent4()
        {
            this->mpImpl = new CEvent4Impl<DelegateType>();
        }
        //-----------------------------------------------------------
        /// Invoke
        //-----------------------------------------------------------
        template<typename DelegateType> template <typename P1Type, typename P2Type, typename P3Type, typename P4Type> void CEvent4<DelegateType>::Invoke(P1Type inP1, P2Type inP2, P3Type inP3, P4Type inP4)
        {
            if(this->mpImpl)
            {
                static_cast<CEvent4Impl<DelegateType>* >(this->mpImpl)->Invoke(inP1, inP2, inP3, inP4);
            }
        }
    }
}

#endif