//
//  DelegateConnection.h
//  Chilli Source
//  Created by S Downie on 08/05/2014.
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

#ifndef _CHILLISOURCE_CORE_DELEGATE_DELEGATECONNECTION_H_
#define _CHILLISOURCE_CORE_DELEGATE_DELEGATECONNECTION_H_

#include <ChilliSource/ChilliSource.h>

#include <mutex>

namespace ChilliSource
{
	namespace Core
	{
        //------------------------------------------------------------------
        /// Connection wrapper around a delegate to allow safe callbacks
        /// to member functions. The connection is closed when the connection
        /// or the delegate goes out of scope
        ///
        /// @author S Downie
        //------------------------------------------------------------------
        template <typename TReturnType, typename... TArgTypes> class DelegateConnection<TReturnType(TArgTypes...)> final
        {
        public:
            
            CS_DECLARE_NOCOPY(DelegateConnection);
            
            //------------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Owning delegate
            //------------------------------------------------------------------
            DelegateConnection(ConnectableDelegate<TReturnType(TArgTypes...)>* in_owner)
            : m_owningDelegate(in_owner)
            {
                
            }
            //------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether the connection is open and delegate can be executed
            //------------------------------------------------------------------
            bool IsOpen() const
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                return m_owningDelegate != nullptr;
            }
            //------------------------------------------------------------------
            /// Close the connection and prevent the delegate from being executed
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            void Close()
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                if(m_owningDelegate != nullptr)
                {
                    m_owningDelegate->Close(this);
                    m_owningDelegate = nullptr;
                }
            }
            //------------------------------------------------------------------
            /// Executes the delegate if the connection is open
            ///
            /// @author S Downie
            ///
            /// @param Variadic - matches the delegate signature
            ///
            /// @return TReturnType - matches the delegate signature
            //------------------------------------------------------------------
            TReturnType Call(TArgTypes... in_args)
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                if(m_owningDelegate != nullptr)
                {
                    return m_owningDelegate->Execute(in_args...);
                }
            }
            //------------------------------------------------------------------
            /// Destructor closes the connection when the owner goes out of scope
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            ~DelegateConnection()
            {
                Close();
            }
            
        private:
            
            friend class ConnectableDelegate<TReturnType(TArgTypes...)>;
            //------------------------------------------------------------------
            /// Called by the delegate when it wishes to close the connection
            /// without being told by the connection itself
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            void CloseNoNotify()
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_owningDelegate = nullptr;
            }
            
        private:

            mutable std::mutex m_mutex;
            ConnectableDelegate<TReturnType(TArgTypes...)>* m_owningDelegate;
        };
	}
}

#endif