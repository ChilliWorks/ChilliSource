//
//  ILuaFunction.h
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

#ifndef _CHILLISOURCE_SCRIPTING_LUA_FUNCTION_ILUAFUNCTION_H_
#define _CHILLISOURCE_SCRIPTING_LUA_FUNCTION_ILUAFUNCTION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Scripting/Lua/LuaLibIncludes.h>
#include <ChilliSource/Scripting/Lua/LuaUtils.h>

namespace ChilliSource
{
    namespace Scripting
    {
        //--------------------------------------------------------
        /// Lua can only call functions that match this signature
        /// and therefore this function is used in conjunction
        /// with user data stored on the Lua stack to route
        /// the call to the actual C++ function
        ///
        /// @author S Downie
        ///
        /// @param Lua VM
        ///
        /// @return Number of values pushed to the Lua stack
        //--------------------------------------------------------
        s32 RouteToFunction(lua_State* in_luaVM);
        
        //--------------------------------------------------------
        /// Class that stores a function that can be called
        /// by Lua. The class consists of a polymorphic interface
        /// and a concrete class that is templated to the function
        /// signature
        ///
        /// @author S Downie
        //--------------------------------------------------------
        class ILuaFunction
        {
        public:
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
            virtual s32 Execute(lua_State* in_luaVM) = 0;
            //--------------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //--------------------------------------------------------
            virtual ~ILuaFunction(){}
        };
    }
}

#endif
