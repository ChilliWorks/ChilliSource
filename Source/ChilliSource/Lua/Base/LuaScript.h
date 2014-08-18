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
		class LuaScript final
		{
		public:
            
            //----------------------------------------------------
			/// Creates a new lua script from the given lua file.
            ///
            /// @author S Downie
            ///
            /// @param Location
            /// @param File path
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static LuaScriptUPtr Create(Core::StorageLocation in_location, const std::string& in_filePath);
            //----------------------------------------------------
			/// Creates a new lua script from the given lua.
            ///
            /// @author S Downie
            ///
            /// @param Lua script
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static LuaScriptUPtr Create(const std::string& in_lua);
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
                m_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(m_luaVM, in_functionName, in_function)));
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
                m_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(m_luaVM, in_functionName, Core::MakeDelegate(in_function))));
            }
            //----------------------------------------------------
            /// Register a class instance and its functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            ///
            /// Usage: RegisterClass("className", this, "funcA", &MyClass::FuncA, "funcB", &MyClass::FuncB, etc)
            ///
            /// @author S Downie
            ///
            /// @param Class name as accessed in Lua
            /// @param Instance of class to call into
            /// @param Name of function
            /// @param Functions
            //----------------------------------------------------
            template <typename TObject, typename...TFunctions>
            void RegisterClass(const char* in_objectName, TObject* in_object, const char* in_functionName, TFunctions&&...in_functions)
            {
                std::string tableName(in_objectName + std::string("Table"));
                luaL_newmetatable(m_luaVM, tableName.c_str());

                RegisterClassFunctions(in_object, in_functionName, std::forward<TFunctions>(in_functions)...);
                
                lua_pushvalue(m_luaVM, -1);
                lua_setfield(m_luaVM, -1, "__index");
                lua_setglobal(m_luaVM, in_objectName);
            }
            //----------------------------------------------------
            /// Register a class and its static functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            ///
            /// Usage: RegisterStaticClass("className", "funcA", &MyClass::FuncA, "funcB", &MyClass::FuncB, etc)
            ///
            /// @author S Downie
            ///
            /// @param Class name as accessed in Lua
            /// @param Name of function
            /// @param Functions
            //----------------------------------------------------
            template <typename...TFunctions>
            void RegisterStaticClass(const char* in_objectName, const char* in_functionName, TFunctions&&...in_functions)
            {
                std::string tableName(in_objectName + std::string("Table"));
                luaL_newmetatable(m_luaVM, tableName.c_str());
                
                RegisterClassStaticFunctions(in_functionName, std::forward<TFunctions>(in_functions)...);
                
                lua_pushvalue(m_luaVM, -1);
                lua_setfield(m_luaVM, -1, "__index");
                lua_setglobal(m_luaVM, in_objectName);
            }
            //----------------------------------------------------
            /// Register an enum type that can be accessed in the
            /// lua script
            ///
            /// Usage: RegisterEnum("enumName", "a", MyEnum::A, "b", MyEnum::B, etc)
            ///
            /// @author S Downie
            ///
            /// @param Enum name as accessed in Lua
            /// @param Name of type
            /// @param List of types
            //----------------------------------------------------
            template <typename...TTypes>
            void RegisterEnum(const char* in_enumName, const char* in_typeName, TTypes&&...in_types)
            {
                std::string tableName(in_enumName + std::string("Table"));
                luaL_newmetatable(m_luaVM, tableName.c_str());
                
                RegisterEnumTypes(in_typeName, std::forward<TTypes>(in_types)...);
                
                lua_pushvalue(m_luaVM, -1);
                lua_setfield(m_luaVM, -1, "__index");
                lua_setglobal(m_luaVM, in_enumName);
            }
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
            void RegisterVariable(const char* in_name, TVariable* in_variable)
            {
                LuaUtils::PushValueToVM(m_luaVM, std::forward<TVariable*>(in_variable));
                lua_setglobal(m_luaVM, in_name);
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

            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Lua
            //-------------------------------------------------------
            LuaScript(const std::string& in_lua);
            //----------------------------------------------------
            /// Register C++ function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterClassFunction(TClass* in_object, const char* in_functionName, TResult (TClass::*in_function)(TArgs...))
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaClassFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(m_luaVM, in_functionName, Core::MakeDelegate(in_object, in_function))));
            }
            //----------------------------------------------------
            /// Register C++ const function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterClassFunction(TClass* in_object, const char* in_functionName, TResult (TClass::*in_function)(TArgs...) const)
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaClassFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(m_luaVM, in_functionName, Core::MakeDelegate(in_object, in_function))));
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// functions
            ///
            /// @author S Downie
            ///
            /// @param Instance of class to call into name as called by Lua
            //----------------------------------------------------
            template <typename TObject> void RegisterClassFunctions(TObject* in_object){}
            //----------------------------------------------------
            /// Register a class instance and its functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            /// This function recursively registers each class function
            ///
            /// @author S Downie
            ///
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            /// @param Remaining functions
            //----------------------------------------------------
            template <typename TObject, typename TFunction, typename...TFunctions>
            void RegisterClassFunctions(TObject* in_object, const char* in_functionName, TFunction&& in_function, TFunctions&&...in_functions)
            {
                RegisterClassFunction(in_object, in_functionName, in_function);
                RegisterClassFunctions(in_object, in_functions...);
            }
            //----------------------------------------------------
            /// Register C++ static function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TResult, typename...TArgs> void RegisterClassStaticFunction(const char* in_functionName, TResult (*in_function)(TArgs...))
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaClassFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(m_luaVM, in_functionName, Core::MakeDelegate(in_function))));
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// functions
            ///
            /// @author S Downie
            //----------------------------------------------------
            void RegisterClassStaticFunctions(){}
            //----------------------------------------------------
            /// Register a class and its static functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            /// This function recursively registers each class function
            ///
            /// @author S Downie
            ///
            /// @param Function name as called by Lua
            /// @param Function
            /// @param Remaining functions
            //----------------------------------------------------
            template <typename TFunction, typename...TFunctions>
            void RegisterClassStaticFunctions(const char* in_functionName, TFunction&& in_function, TFunctions&&...in_functions)
            {
                RegisterClassStaticFunction(in_functionName, in_function);
                RegisterClassStaticFunctions(in_functions...);
            }
            //----------------------------------------------------
            /// Register C++ enums to be accessed in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Type name as called by Lua
            /// @param Type
            //----------------------------------------------------
            template <typename TType> void RegisterEnumType(const char* in_typeName, TType&& in_type)
            {
                LuaUtils::PushValueToVM(m_luaVM, (s32)in_type);
                lua_setfield(m_luaVM, -2, in_typeName);
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// enums
            ///
            /// @author S Downie
            //----------------------------------------------------
            void RegisterEnumTypes(){}
            //----------------------------------------------------
            /// Register C++ enums to be accessed in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Type name as called by Lua
            /// @param Type
            /// @param Remainging types
            //----------------------------------------------------
            template <typename TType, typename... TTypes> void RegisterEnumTypes(const char* in_typeName, TType&& in_type, TTypes&&...in_types)
            {
                RegisterEnumType(in_typeName, in_type);
                RegisterEnumTypes(in_types...);
            }
            
        private:
            
            std::vector<ILuaFunctionUPtr> m_functions;
            lua_State* m_luaVM = nullptr;
		};
	}
}

#endif
