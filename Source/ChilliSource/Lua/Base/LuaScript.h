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
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Lua/Base/LuaUtils.h>
#include <ChilliSource/Lua/Function/LuaFunction.h>
#include <ChilliSource/Lua/Function/LuaClassFunction.h>

#include <vector>

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
            /// Register C++ function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TResult, typename...TArgs> void RegisterFunction(const char* in_functionName, typename LuaFunction<1, TResult, TArgs...>::FuncType&& in_function)
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, TResult, TArgs...>(m_luaVM, in_functionName, in_function)));
            }
            //----------------------------------------------------
            /// Register free function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TResult, typename...TArgs> void RegisterFunction(const char* in_functionName, TResult (*in_function)(TArgs...))
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, TResult, TArgs...>(m_luaVM, in_functionName, Core::MakeDelegate(in_function))));
            }
            //----------------------------------------------------
            /// Register a class instance and its functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Class name as accessed in Lua
            /// @param Instance of class to call into
            /// @param Function
            //----------------------------------------------------
            template <typename TObject, typename...TFunctions>
            void RegisterClass(const char* in_objectName, TObject* in_object, const char* in_functionName, TFunctions&&...in_functions)
            {
                std::string tableName(in_objectName + std::string("Table"));
                luaL_newmetatable(m_luaVM, tableName.c_str());

                RegisterClassFunctions(in_objectName, in_object, in_functionName, std::forward<TFunctions>(in_functions)...);
                
                lua_pushvalue(m_luaVM, -1);
                lua_setfield(m_luaVM, -1, "__index");
                lua_setglobal(m_luaVM, in_objectName);
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
                
                LuaUtils::PushAllToVM(m_luaVM, std::forward<TArgs>(in_args)...);
                
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
            //----------------------------------------------------
            /// Register C++ function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Instance name as called by Lua
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterClassFunction(const char* in_className, TClass* in_object, const char* in_functionName, TResult (TClass::*in_function)(TArgs...))
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaClassFunction<LuaUtils::NumValues<TResult>::value, TResult, TArgs...>(m_luaVM, in_functionName, Core::MakeDelegate(in_object, in_function))));
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// functions
            ///
            /// @author S Downie
            ///
            /// @param Instance name as called by Lua
            /// @param Instance of class to call into name as called by Lua
            //----------------------------------------------------
            template <typename TObject> void RegisterClassFunctions(const char* in_objectName, TObject* in_object){}
            //----------------------------------------------------
            /// Register a class instance and its functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            /// This function recursively registers each class function
            ///
            /// @author S Downie
            ///
            /// @param Instance name as accessed in Lua
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            /// @param Functions
            //----------------------------------------------------
            template <typename TObject, typename TFunction, typename...TFunctions>
            void RegisterClassFunctions(const char* in_objectName, TObject* in_object, const char* in_functionName, TFunction&& in_function, TFunctions&&...in_functions)
            {
                RegisterClassFunction(in_objectName, in_object, in_functionName, in_function);
                RegisterClassFunctions(in_objectName, in_object, in_functions...);
            }
            
        private:
            
            std::vector<ILuaFunctionUPtr> m_functions;
            lua_State* m_luaVM = nullptr;
		};
	}
}

#endif
