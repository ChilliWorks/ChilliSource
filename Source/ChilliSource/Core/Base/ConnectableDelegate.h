//
//  ConnectableDelegate.h
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

#ifndef _CHILLISOURCE_CORE_BASE_CONNECTABLEDELEGATE_H_
#define _CHILLISOURCE_CORE_BASE_CONNECTABLEDELEGATE_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/Base/DelegateConnection.h>

#include <functional>
#include <mutex>
#include <vector>

namespace ChilliSource
{
	namespace Core
	{
        //------------------------------------------------------------------
        /// Wrapper around a delegate that allows 3rd parties to establish
        /// connections. While these connections are open the 3rd parties
        /// can safely execute the delegate
        ///
        /// @author S Downie
        //------------------------------------------------------------------
        template <typename TReturnType, typename... TArgTypes> class ConnectableDelegate
        {
        public:
            
            CS_DECLARE_NOCOPY(ConnectableDelegate);
            
            //------------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            ConnectableDelegate() = default;
            //------------------------------------------------------------------
            /// Signature of the delegate function
            ///
            /// @uathor S Downie
            ///
            /// @params Variadic
            ///
            /// @return TReturnType
            //------------------------------------------------------------------
            using Delegate = std::function<TReturnType(TArgTypes...)>;
            //------------------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Delegate to wrap
            //------------------------------------------------------------------
            ConnectableDelegate(const Delegate& in_delegate)
            : m_delegate(in_delegate)
            {
                
            }
            //------------------------------------------------------------------
            /// Open a new connection to the delegate. While the connection
            /// remains the open the returned object can be used to invoke the
            /// delegate
            ///
            /// @author S Downie
            ///
            /// @return New connection
            //------------------------------------------------------------------
            DelegateConnectionUPtr<TReturnType, TArgTypes...> OpenConnection()
            {
                DelegateConnectionUPtr<TReturnType, TArgTypes...> connection(new DelegateConnection<TReturnType, TArgTypes...>(this));
                
                std::unique_lock<std::mutex> lock(m_mutex);
                m_connections.push_back(connection.get());
                lock.unlock();
                
                return connection;
            }
            //------------------------------------------------------------------
            /// Close all the open connections and prevent this delegate from
            /// being executed in the future
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            void CloseAllConnections()
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                
                for(auto connection : m_connections)
                {
                    connection->Close();
                }
                
                m_connections.clear();
            }
            //------------------------------------------------------------------
            /// Closes all open connections
            ///
            /// @author S Downie
            //------------------------------------------------------------------
            ~ConnectableDelegate()
            {
                //Close the connection if this is being destructed from the owner
                CloseAllConnections();
            }
            
        private:
            
            friend TReturnType DelegateConnection<TReturnType, TArgTypes...>::Execute(TArgTypes...);
            //------------------------------------------------------------------
            /// Executes the delegate if the connection is open
            ///
            /// @author S Downie
            ///
            /// @param Variadic - matches the delegate signature
            ///
            /// @return TReturnType - matches the delegate signature
            //------------------------------------------------------------------
            TReturnType Execute(TArgTypes... in_args)
            {
                return m_delegate(in_args...);
            }
            
        private:
            
            Delegate m_delegate;
            
            std::mutex m_mutex;
            std::vector<DelegateConnection<TReturnType, TArgTypes...>*> m_connections;
        };
	}
}

#endif