//
//  LuaClassFunction.h
//  Chilli Source
//  Created by Scott Downie on 14/08/2014.
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

#ifndef _CHILLISOURCE_LUA_FUNCTION_LUACLASSFUNCTION_H_
#define _CHILLISOURCE_LUA_FUNCTION_LUACLASSFUNCTION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Lua/Function/ILuaFunction.h>


namespace ChilliSource
{
    namespace Lua
    {
        //--------------------------------------------------------
        /// Stores a member function of a class and registers it
        /// to be accessed in Lua
        ///
        /// @author S Downie
        //--------------------------------------------------------
        template <s32 TNumResults, typename TResult, typename...TArgs>  class LuaClassFunction : public ILuaFunction
        {
        public:
            using FuncType = std::function<TResult(TArgs...)>;
            
            //--------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Name to register function against
            /// @param Function to register with Lua
            //--------------------------------------------------------
            LuaClassFunction(lua_State* in_luaVM, const char* in_functionName, const FuncType& in_function)
            : m_function(in_function)
            {
                //Register this function pointer with Lua VM so we can call into it from
                //the routing function
                lua_pushlightuserdata(in_luaVM, (void*)this);
                
                //Register the routing function as it has the signature required by Lua
                //and can route back to us
                lua_pushcclosure(in_luaVM, RouteToFunction, 1);
                
                //Bind the function to the given name
                lua_setfield(in_luaVM, -2, in_functionName);
            }
            //--------------------------------------------------------
            /// Calls the stored function with the parameters from
            /// the Lua stack and pushes the results back onto
            /// the Lua stack
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            ///
            /// @return Number of parameters pushed onto Lua stack
            //--------------------------------------------------------
            s32 Execute(lua_State* in_luaVM) override
            {
                //Pull the arguments from the Lua stack and call the function, pushing the result back to the Lua stack
                LuaUtils::PushAllToVM(in_luaVM, LuaUtils::LiftToParamPack(m_function, LuaUtils::ReadAllFromVM<TArgs...>(in_luaVM)));
                
                return TNumResults;
            }
            
        private:
            
            FuncType m_function;
        };
        //--------------------------------------------------------
        /// Stores a member function of a class and registers it
        /// to be accessed in Lua (Specialised for void return)
        ///
        /// @author S Downie
        //--------------------------------------------------------
        template <typename...TArgs>  class LuaClassFunction<0, void, TArgs...> : public ILuaFunction
        {
        public:
            using FuncType = std::function<void(TArgs...)>;
            
            //--------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Name to register function against
            /// @param Function to register with Lua
            //--------------------------------------------------------
            LuaClassFunction(lua_State* in_luaVM, const char* in_functionName, const FuncType& in_function)
            : m_function(in_function)
            {
                //Register this function pointer with Lua VM so we can call into it from
                //the routing function
                lua_pushlightuserdata(in_luaVM, (void*)this);
                
                //Register the routing function as it has the signature required by Lua
                //and can route back to us
                lua_pushcclosure(in_luaVM, RouteToFunction, 1);
                
                //Bind the function to the given name
                lua_setfield(in_luaVM, -2, in_functionName);
            }
            //--------------------------------------------------------
            /// Calls the stored function with the parameters from
            /// the Lua stack and pushes the results back onto
            /// the Lua stack
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            ///
            /// @return Number of parameters pushed onto Lua stack
            //--------------------------------------------------------
            s32 Execute(lua_State* in_luaVM) override
            {
                //Pull the arguments from the Lua stack and call the function.
                LuaUtils::LiftToParamPack(m_function, LuaUtils::ReadAllFromVM<TArgs...>(in_luaVM));
                
                return 0;
            }
            
        private:
            
            FuncType m_function;
        };
    }
}

#endif
