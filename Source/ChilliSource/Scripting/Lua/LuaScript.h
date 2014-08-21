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
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Scripting/Lua/LuaUtils.h>
#include <ChilliSource/Scripting/Lua/Function/LuaFunction.h>
#include <ChilliSource/Scripting/Lua/Function/LuaClassFunction.h>

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
            /// Register C++ function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TResult, typename...TArgs> void RegisterFunction(const std::string& in_functionName, typename LuaFunction<1, TResult, TArgs...>::FuncType&& in_function)
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
            template <typename TResult, typename...TArgs> void RegisterFunction(const std::string& in_functionName, TResult (*in_function)(TArgs...))
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
            void RegisterClass(const std::string& in_objectName, TObject* in_object, const std::string& in_functionName, TFunctions&&...in_functions)
            {
                std::string tableName(in_objectName + std::string("Table"));
                luaL_newmetatable(m_luaVM, tableName.c_str());
                m_tables.push_back(in_objectName);

                RegisterClassFunctions(tableName.c_str(), in_object, in_functionName, std::forward<TFunctions>(in_functions)...);
                
                lua_pushvalue(m_luaVM, -1);
                lua_setfield(m_luaVM, -1, "__index");
                lua_setglobal(m_luaVM, in_objectName.c_str());
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
            void RegisterStaticClass(const std::string& in_objectName, const std::string& in_functionName, TFunctions&&...in_functions)
            {
                std::string tableName(in_objectName + std::string("Table"));
                luaL_newmetatable(m_luaVM, tableName.c_str());
                m_tables.push_back(in_objectName);
                
                RegisterClassStaticFunctions(tableName.c_str(), in_functionName, std::forward<TFunctions>(in_functions)...);
                
                lua_pushvalue(m_luaVM, -1);
                lua_setfield(m_luaVM, -1, "__index");
                lua_setglobal(m_luaVM, in_objectName.c_str());
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
            void RegisterEnum(const std::string& in_enumName, const std::string& in_typeName, TTypes&&...in_types)
            {
                std::string tableName(in_enumName + std::string("Table"));
                luaL_newmetatable(m_luaVM, tableName.c_str());
                m_tables.push_back(in_enumName);
                
                RegisterEnumTypes(in_typeName, std::forward<TTypes>(in_types)...);
                
                lua_pushvalue(m_luaVM, -1);
                lua_setfield(m_luaVM, -1, "__index");
                lua_setglobal(m_luaVM, in_enumName.c_str());
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
            void RegisterVariable(const std::string& in_name, TVariable* in_variable)
            {
                LuaUtils::PushValueToVM(m_luaVM, std::forward<TVariable*>(in_variable));
                lua_setglobal(m_luaVM, in_name.c_str());
                m_variables.push_back(in_name);
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
            typename LuaUtils::StackPopper<sizeof...(TResults), TResults...>::type CallFunction(const std::string& in_functionName, TArgs&&... in_args)
            {
                lua_getglobal(m_luaVM, in_functionName.c_str());
                
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
            //----------------------------------------------------
            /// Register C++ function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Table name
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterClassFunction(const std::string& in_tableName, TClass* in_object, const std::string& in_functionName, TResult (TClass::*in_function)(TArgs...))
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaClassFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(m_luaVM, in_tableName, in_functionName, Core::MakeDelegate(in_object, in_function))));
            }
            //----------------------------------------------------
            /// Register C++ const function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Table name
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterClassFunction(const std::string& in_tableName, TClass* in_object, const std::string& in_functionName, TResult (TClass::*in_function)(TArgs...) const)
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaClassFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(m_luaVM, in_tableName, in_functionName, Core::MakeDelegate(in_object, in_function))));
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// functions
            ///
            /// @author S Downie
            ///
            /// @param Table name
            /// @param Instance of class to call into name as called by Lua
            //----------------------------------------------------
            template <typename TObject> void RegisterClassFunctions(const std::string& in_tableName, TObject* in_object){}
            //----------------------------------------------------
            /// Register a class instance and its functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            /// This function recursively registers each class function
            ///
            /// @author S Downie
            ///
            /// @param Table name
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            /// @param Remaining functions
            //----------------------------------------------------
            template <typename TObject, typename TFunction, typename...TFunctions>
            void RegisterClassFunctions(const std::string& in_tableName, TObject* in_object, const std::string& in_functionName, TFunction&& in_function, TFunctions&&...in_functions)
            {
                RegisterClassFunction(in_tableName, in_object, in_functionName, in_function);
                RegisterClassFunctions(in_tableName, in_object, in_functions...);
            }
            //----------------------------------------------------
            /// Register C++ static function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Table name
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TResult, typename...TArgs> void RegisterClassStaticFunction(const std::string& in_tableName, const std::string& in_functionName, TResult (*in_function)(TArgs...))
            {
                m_functions.push_back(ILuaFunctionUPtr(new LuaClassFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(m_luaVM, in_tableName, in_functionName, Core::MakeDelegate(in_function))));
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// functions
            ///
            /// @author S Downie
            ///
            /// @param Table name
            //----------------------------------------------------
            void RegisterClassStaticFunctions(const std::string& in_tableName){}
            //----------------------------------------------------
            /// Register a class and its static functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            /// This function recursively registers each class function
            ///
            /// @author S Downie
            ///
            /// @param Table name
            /// @param Function name as called by Lua
            /// @param Function
            /// @param Remaining functions
            //----------------------------------------------------
            template <typename TFunction, typename...TFunctions>
            void RegisterClassStaticFunctions(const std::string& in_tableName, const std::string& in_functionName, TFunction&& in_function, TFunctions&&...in_functions)
            {
                RegisterClassStaticFunction(in_tableName, in_functionName, in_function);
                RegisterClassStaticFunctions(in_tableName, in_functions...);
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
            template <typename TType> void RegisterEnumType(const std::string& in_typeName, TType&& in_type)
            {
                LuaUtils::PushValueToVM(m_luaVM, (s32)in_type);
                lua_setfield(m_luaVM, -2, in_typeName.c_str());
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
            template <typename TType, typename... TTypes> void RegisterEnumTypes(const std::string& in_typeName, TType&& in_type, TTypes&&...in_types)
            {
                RegisterEnumType(in_typeName, in_type);
                RegisterEnumTypes(in_types...);
            }
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
            std::vector<std::string> m_tables;
            std::vector<std::string> m_variables;
            lua_State* m_luaVM = nullptr;
            
            LuaSourceCSPtr m_source;
		};
	}
}

#endif
