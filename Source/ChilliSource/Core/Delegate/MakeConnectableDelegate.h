//
//  MakeConnectableDelegate.h
//  Chilli Source
//  Created by Scott Downie on 12/05/2014.
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

#ifndef _CHILLISOURCE_CORE_DELEGATE_MAKECONNECTABLEDELEGATE_H_
#define _CHILLISOURCE_CORE_DELEGATE_MAKECONNECTABLEDELEGATE_H_

#include <ChilliSource/Core/Delegate/ConnectableDelegate.h>

#include <functional>

namespace ChilliSource
{
	namespace Core
	{
        //------------------------------------------------------------------
        /// Constructs a connectable delegate to a free function with a signature that
        /// matches the given return and parameter types.
        ///
        /// @author S Downie
        ///
        /// @param Function ptr
        ///
        /// @return Delegate
        //------------------------------------------------------------------
        template <typename TReturnType, typename... TArgTypes>
        ConnectableDelegate<TReturnType(TArgTypes...)> MakeConnectableDelegate(TReturnType (*in_func)(TArgTypes...))
        {
            return ConnectableDelegate<TReturnType(TArgTypes...)>(std::function<TReturnType(TArgTypes...)>(in_func));
        }
        //------------------------------------------------------------------
        /// Constructs a connectable delegate to a static function with a signature that
        /// matches the given return and parameter types.
        ///
        /// @author S Downie
        ///
        /// @param Static function ptr
        ///
        /// @return Delegate
        //------------------------------------------------------------------
        template <typename TDelegate, typename TReturnType, typename... TArgTypes>
        ConnectableDelegate<TReturnType(TArgTypes...)> MakeConnectableDelegate(TReturnType (TDelegate::*in_func)(TArgTypes...))
        {
            return ConnectableDelegate<TReturnType(TArgTypes...)>(std::function<TReturnType(TArgTypes...)>(in_func));
        }
        //------------------------------------------------------------------
        /// Constructs a connectable delegate to a member function with a signature that
        /// matches the given return and parameter types.
        ///
        /// @author S Downie
        ///
        /// @param Instance whose function to call
        /// @param Member function ptr
        ///
        /// @return Delegate
        //------------------------------------------------------------------
        template <typename TDelegate, typename TSender, typename TReturnType, typename... TArgTypes>
        ConnectableDelegate<TReturnType(TArgTypes...)> MakeConnectableDelegate(TSender* in_sender, TReturnType (TDelegate::*in_func)(TArgTypes...))
        {
            return ConnectableDelegate<TReturnType(TArgTypes...)>([=](TArgTypes... in_args) { return (in_sender->*in_func)(in_args...); });
        }
        //------------------------------------------------------------------
        /// Constructs a connectable delegate to a const member function with a signature
        /// that matches the given return and parameter types.
        ///
        /// @author S Downie
        ///
        /// @param Instance whose function to call
        /// @param Const member function ptr
        ///
        /// @return Delegate
        //------------------------------------------------------------------
        template <typename TDelegate, typename TSender, typename TReturnType, typename... TArgTypes>
        ConnectableDelegate<TReturnType(TArgTypes...)> MakeConnectableDelegate(TSender* in_sender, TReturnType (TDelegate::*in_func)(TArgTypes...) const)
        {
            return ConnectableDelegate<TReturnType(TArgTypes...)>([=](TArgTypes... in_args) { return (in_sender->*in_func)(in_args...); });
        }
	}
}

#endif