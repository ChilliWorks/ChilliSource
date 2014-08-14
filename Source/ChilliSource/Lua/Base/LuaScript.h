//
//  LuaScript.h
//  Chilli Source
//  Created by Scott Downie on 13/08/2014.
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

#ifndef _CHILLISOURCE_LUA_BASE_LUASCRIPT_H_
#define _CHILLISOURCE_LUA_BASE_LUASCRIPT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Lua/Base/LuaUtils.h>

namespace ChilliSource
{
	namespace Lua
	{
		//-----------------------------------------------------------
		/// A system that creates and manages a Lua VM.
        /// The system is used to call lua functions.
        ///
        /// @author S Downie
		//-----------------------------------------------------------
		class LuaScript
		{
		public:
            
            //----------------------------------------------------
            /// Call the lua function on this script that
            /// has the given signature. Lua has the ability
            /// to return multiple values and therefore this function
            /// returns a tuple. Values can be accessed via std::get<idx>.
            ///
            /// @author S Downie
            ///
            /// @param Function name
            /// @param Parameters
            ///
            /// @return Results
            //----------------------------------------------------
            template <typename... TResults, typename... TArgs>
            typename LuaUtils::Popper<sizeof...(TResults), TResults...>::type CallFunction(const char* in_functionName, TArgs&&... in_args)
            {
                lua_getglobal(m_luaVM, in_functionName);
                
                LuaUtils::PushAllToVM(m_luaVM, std::tie(in_args...));
                
                auto funcResult = lua_pcall(m_luaVM, sizeof...(TArgs), sizeof...(TResults), 0);
                if(funcResult != 0)
                {
                    CS_LOG_FATAL("Error calling Lua function " + std::string(in_functionName) + ": " + std::string(lua_tostring(m_luaVM, -1)));
                }
                
                return LuaUtils::PopAllFromVM<TResults...>(m_luaVM);
            }
            //-------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
            ~LuaScript();
            
        private:
            friend class LuaSystem;
            
            //----------------------------------------------------
			/// Creates a new instance of this system.
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static LuaScriptUPtr Create(lua_State* in_luaVM);
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            //-------------------------------------------------------
            LuaScript(lua_State* in_luaVM);
            
        private:
            
            lua_State* m_luaVM = nullptr;
		};
	}
}

#endif
