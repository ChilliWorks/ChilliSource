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

#ifndef _CHILLISOURCE_SCRIPTING_LUA_LUASCRIPT_H_
#define _CHILLISOURCE_SCRIPTING_LUA_LUASCRIPT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Scripting/Lua/LuaUtils.h>
#include <ChilliSource/Scripting/Lua/Function/LuaFunction.h>
#include <ChilliSource/Scripting/Lua/Function/LuaFunctionUtils.h>

#include <vector>

namespace ChilliSource
{
	namespace Scripting
	{
		//-----------------------------------------------------------
		/// A system that creates and manages a Lua VM.
        /// The system is used to call lua functions.
        ///
        /// @author S Downie
		//-----------------------------------------------------------
		class LuaScript final
		{
		public:
            
            //----------------------------------------------------
            /// Describes how execution should behave if an attempt
            /// is made to call a Lua function that doesn't exist
            ///
            /// @author S Downie
            //----------------------------------------------------
            enum class FunctionNotFoundPolicy
            {
                k_failHard,
                k_failSilent
            };
            //----------------------------------------------------
            /// Register an variable that can be accessed in the
            /// lua script
            ///
            /// @author S Downie
            ///
            /// @param Name of variable
            /// @param Pointer to variable
            //----------------------------------------------------
            template <typename TVariable>
            void RegisterVariable(const std::string& in_name, TVariable* in_variable)
            {
                LuaFunctionUtils::RegisterVariable(m_luaVM, m_environment, in_name, in_variable);
            }
            //----------------------------------------------------
            /// This function must be called after registering
            /// any CS objects, functions or data but before
            /// calling any functions. This causes lua to run the
            /// script and as a requirement of that all functions
            /// must exist.
            ///
            /// @author S Downie
            //----------------------------------------------------
            void Run();
            //----------------------------------------------------
            /// Call the lua function on this script that
            /// has the given signature. Lua has the ability
            /// to return multiple values and therefore this function
            /// returns a tuple for multi returns.
            /// Values can be accessed via std::get<idx>.
            ///
            /// NOTE: As this method expects a return value
            /// the Lua function must exist!
            ///
            /// @author S Downie
            ///
            /// @param Function name
            /// @param Parameters
            ///
            /// @return Results
            //----------------------------------------------------
            template <typename... TResults, typename... TArgs>
            typename LuaUtils::StackPopper<sizeof...(TResults), TResults...>::type CallFunction(const std::string& in_functionName, TArgs&&... in_args)
            {
                lua_getfield(m_luaVM, LUA_REGISTRYINDEX, m_environment.c_str());
                lua_getfield(m_luaVM, -1, in_functionName.c_str());
                
                LuaUtils::PushAllToVM(m_luaVM, std::forward<TArgs>(in_args)...);
                
                auto funcResult = lua_pcall(m_luaVM, sizeof...(TArgs), sizeof...(TResults), 0);
                if(funcResult != 0)
                {
                    CS_LOG_FATAL("Error calling Lua function " + std::string(in_functionName) + ": " + std::string(lua_tostring(m_luaVM, -1)));
                }
                
                //LuaUtils::PopAllFromVM can return void and therefore we cannot assign
                //the return value of that function to a variable, pop and then return.
                //Instead we use a simple scoping cheat to return the value and then pop the
                //environment field.
                struct ScopedPop
                {
                    ScopedPop(lua_State* in_vm):m_vm(in_vm){}
                    ~ScopedPop(){lua_pop(m_vm, 1);}
                    
                    lua_State* m_vm;
                };
                ScopedPop popOnReturn(m_luaVM);
                
                return LuaUtils::PopAllFromVM<TResults...>(m_luaVM);
            }
            //----------------------------------------------------
            /// Call the lua function on this script that
            /// has the given signature.
            ///
            /// @author S Downie
            ///
            /// @param Function name
            /// @param Policy that indicates how to handle missing functions
            /// @param Parameters
            //----------------------------------------------------
            template <typename... TArgs>
            void CallFunction(const std::string& in_functionName, FunctionNotFoundPolicy in_notFoundPolicy, TArgs&&... in_args)
            {
                lua_getfield(m_luaVM, LUA_REGISTRYINDEX, m_environment.c_str());
                lua_getfield(m_luaVM, -1, in_functionName.c_str());
                
                if(in_notFoundPolicy == FunctionNotFoundPolicy::k_failSilent && lua_topointer(m_luaVM, -1) == nullptr)
                {
                    lua_pop(m_luaVM, 2);
                    return;
                }
                
                LuaUtils::PushAllToVM(m_luaVM, std::forward<TArgs>(in_args)...);
                
                auto funcResult = lua_pcall(m_luaVM, sizeof...(TArgs), 0, 0);
                if(funcResult != 0)
                {
                    CS_LOG_FATAL("Error calling Lua function " + std::string(in_functionName) + ": " + std::string(lua_tostring(m_luaVM, -1)));
                }
                
                lua_pop(m_luaVM, 1);
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
			/// Creates a new lua script from the given lua.
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Lua source
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static LuaScriptUPtr Create(lua_State* in_vm, const LuaSourceCSPtr& in_luaSource);
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Lua source
            //-------------------------------------------------------
            LuaScript(lua_State* in_vm, const LuaSourceCSPtr& in_luaSource);
            //-------------------------------------------------------
            /// Imports the given lua script into the global table
            /// for the current VM
            ///
            /// @author S Downie
            ///
            /// @param Location as string
            /// @param Path from location
            //-------------------------------------------------------
            void Import(const std::string& in_location, const std::string& in_path);
            
        private:
            
            std::vector<ILuaFunctionUPtr> m_functions;
            std::string m_environment;
            lua_State* m_luaVM = nullptr;
            
            LuaSourceCSPtr m_source;
            
            static u32 s_environmentCounter;
		};
	}
}

#endif
