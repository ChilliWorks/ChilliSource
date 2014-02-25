//
//  MakeDelegate.h
//  ChilliSource
//
//  Created by Scott Downie on 25/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_BASE_MAKEDELEGATE_H_
#define _CHILLISOURCE_CORE_BASE_MAKEDELEGATE_H_

namespace ChilliSource
{
	namespace Core
	{
        //------------------------------------------------------------------
        /// Constructs a delegate to a free function with a signature that
        /// matches the given return and parameter types.
        ///
        /// @author S Downie
        ///
        /// @param Function ptr
        ///
        /// @return Delegate
        //------------------------------------------------------------------
        template <typename TReturnType, typename... TArgTypes>
        std::function<TReturnType(TArgTypes...)> MakeDelegate(TReturnType (*in_func)(TArgTypes...))
        {
            return std::function<TReturnType(TArgTypes...)>(in_func);
        }
        //------------------------------------------------------------------
        /// Constructs a delegate to a static function with a signature that
        /// matches the given return and parameter types.
        ///
        /// @author S Downie
        ///
        /// @param Static function ptr
        ///
        /// @return Delegate
        //------------------------------------------------------------------
        template <typename TDelegate, typename TReturnType, typename... TArgTypes>
        std::function<TReturnType(TArgTypes...)> MakeDelegate(TReturnType (TDelegate::*in_func)(TArgTypes...))
        {
            return std::function<TReturnType(TArgTypes...)>(in_func);
        }
        //------------------------------------------------------------------
        /// Constructs a delegate to a member function with a signature that
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
        std::function<TReturnType(TArgTypes...)> MakeDelegate(TSender* in_sender, TReturnType (TDelegate::*in_func)(TArgTypes...))
        {
            return [=](TArgTypes... in_args) { return (in_sender->*in_func)(in_args...); };
        }
        //------------------------------------------------------------------
        /// Constructs a delegate to a const member function with a signature
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
        std::function<TReturnType(TArgTypes...)> MakeDelegate(TSender* in_sender, TReturnType (TDelegate::*in_func)(TArgTypes...) const)
        {
            return [=](TArgTypes... in_args) { return (in_sender->*in_func)(in_args...); };
        }

	}
}

#endif