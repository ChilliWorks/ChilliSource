//
//  LuaFunctionUtils.h
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

#ifndef _CHILLISOURCE_SCRIPTING_LUA_FUNCTION_LUAFUNCTIONUTILS_H_
#define _CHILLISOURCE_SCRIPTING_LUA_FUNCTION_LUAFUNCTIONUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Scripting/Lua/LuaUtils.h>
#include <ChilliSource/Scripting/Lua/Function/LuaFunction.h>

#include <vector>

namespace ChilliSource
{
    namespace Scripting
    {
        //--------------------------------------------------------
        /// Utility methods for registering functions with Lua
        ///
        /// @author S Downie
        //--------------------------------------------------------
        namespace LuaFunctionUtils
        {
            const std::string k_globalEnvName("_G");
            
            //----------------------------------------------------
            /// Register an variable that can be accessed in the
            /// lua script
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Environment name to register against
            /// @param Name of variable
            /// @param Pointer to variable
            //----------------------------------------------------
            template <typename TVariable>
            void RegisterVariable(lua_State* in_vm, const std::string& in_envName, const std::string& in_name, TVariable* in_variable)
            {
                if(in_envName == k_globalEnvName)
                {
                    lua_pushglobaltable(in_vm);
                }
                else
                {
                    lua_getfield(in_vm, LUA_REGISTRYINDEX, in_envName.c_str());
                }
                
                LuaUtils::PushValueToVM(in_vm, std::forward<TVariable*>(in_variable));
                lua_setfield(in_vm, -2, in_name.c_str());
                
                lua_pop(in_vm, 1);
            }
            //----------------------------------------------------
            /// Register C++ function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param [Out] Functions registered
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterClassFunction(lua_State* in_vm, std::vector<ILuaFunctionUPtr>& out_functions, TClass* in_object, const std::string& in_functionName, TResult (TClass::*in_function)(TArgs...))
            {
                out_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(in_vm, in_functionName, Core::MakeDelegate(in_object, in_function))));
            }
            //----------------------------------------------------
            /// Register C++ const function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param [Out] Functions registered
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterClassFunction(lua_State* in_vm, std::vector<ILuaFunctionUPtr>& out_functions, TClass* in_object, const std::string& in_functionName, TResult (TClass::*in_function)(TArgs...) const)
            {
                out_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(in_vm, in_functionName, Core::MakeDelegate(in_object, in_function))));
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// functions
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param [Out] Functions registered
            /// @param Instance of class to call into name as called by Lua
            //----------------------------------------------------
            template <typename TObject> void RegisterClassFunctions(lua_State* in_vm, std::vector<ILuaFunctionUPtr>& out_functions, TObject* in_object){}
            //----------------------------------------------------
            /// Register a class instance and its functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            /// This function recursively registers each class function
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param [Out] Functions registered
            /// @param Instance of class to call into
            /// @param Function name as called by Lua
            /// @param Function
            /// @param Remaining functions
            //----------------------------------------------------
            template <typename TObject, typename TFunction, typename...TFunctions>
            void RegisterClassFunctions(lua_State* in_vm, std::vector<ILuaFunctionUPtr>& out_functions, TObject* in_object, const std::string& in_functionName, TFunction&& in_function, TFunctions&&...in_functions)
            {
                RegisterClassFunction(in_vm, out_functions, in_object, in_functionName, in_function);
                RegisterClassFunctions(in_vm, out_functions, in_object, in_functions...);
            }
            //----------------------------------------------------
            /// Register C++ static function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param [Out] Functions registered
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TResult, typename...TArgs> void RegisterClassStaticFunction(lua_State* in_vm, std::vector<ILuaFunctionUPtr>& out_functions, const std::string& in_functionName, TResult (*in_function)(TArgs...))
            {
                out_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(in_vm, in_functionName, Core::MakeDelegate(in_function))));
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// functions
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param [Out] Functions registered
            //----------------------------------------------------
            void RegisterClassStaticFunctions(lua_State* in_vm, std::vector<ILuaFunctionUPtr>& out_functions);
            //----------------------------------------------------
            /// Register a class and its static functions to be
            /// called in the Lua script.
            /// This must be performed prior to calling run or the script will error
            /// This function recursively registers each class function
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param [Out] Functions registered
            /// @param Function name as called by Lua
            /// @param Function
            /// @param Remaining functions
            //----------------------------------------------------
            template <typename TFunction, typename...TFunctions>
            void RegisterClassStaticFunctions(lua_State* in_vm, std::vector<ILuaFunctionUPtr>& out_functions, const std::string& in_functionName, TFunction&& in_function, TFunctions&&...in_functions)
            {
                RegisterClassStaticFunction(in_vm, out_functions, in_functionName, in_function);
                RegisterClassStaticFunctions(in_vm, out_functions, in_functions...);
            }
            //----------------------------------------------------
            /// Register C++ enums to be accessed in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Type name as called by Lua
            /// @param Type
            //----------------------------------------------------
            template <typename TType> void RegisterEnumType(lua_State* in_vm, const std::string& in_typeName, TType&& in_type)
            {
                LuaUtils::PushValueToVM(in_vm, (s32)in_type);
                lua_setfield(in_vm, -2, in_typeName.c_str());
            }
            //----------------------------------------------------
            /// Specialised to terminate recursion of registering
            /// enums
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            //----------------------------------------------------
            void RegisterEnumTypes(lua_State* in_vm);
            //----------------------------------------------------
            /// Register C++ enums to be accessed in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Type name as called by Lua
            /// @param Type
            /// @param Remainging types
            //----------------------------------------------------
            template <typename TType, typename... TTypes> void RegisterEnumTypes(lua_State* in_vm, const std::string& in_typeName, TType&& in_type, TTypes&&...in_types)
            {
                RegisterEnumType(in_vm, in_typeName, in_type);
                RegisterEnumTypes(in_vm, in_types...);
            }
            //----------------------------------------------------
            /// Register free function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Environment name to register against
            /// @param [Out] Functions registered
            /// @param Function name as called by Lua
            /// @param Function
            //----------------------------------------------------
            template <typename TResult, typename...TArgs> void RegisterFunction(lua_State* in_vm, const std::string& in_envName, std::vector<ILuaFunctionUPtr>& out_functions, const std::string& in_functionName, TResult (*in_function)(TArgs...))
            {
                if(in_envName == k_globalEnvName)
                {
                    lua_pushglobaltable(in_vm);
                }
                else
                {
                    lua_getfield(in_vm, LUA_REGISTRYINDEX, in_envName.c_str());
                }
                
                out_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(in_vm, in_functionName, Core::MakeDelegate(in_function))));
                
                lua_pop(in_vm, 1);
            }
            //----------------------------------------------------
            /// Register free function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Environment name to register against
            /// @param [Out] Functions registered
            /// @param Function name as called by Lua
            /// @param Object to bind function to
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterFunction(lua_State* in_vm, const std::string& in_envName, std::vector<ILuaFunctionUPtr>& out_functions, const std::string& in_functionName, TClass* in_object, TResult (TClass::*in_function)(TArgs...))
            {
                if(in_envName == k_globalEnvName)
                {
                    lua_pushglobaltable(in_vm);
                }
                else
                {
                    lua_getfield(in_vm, LUA_REGISTRYINDEX, in_envName.c_str());
                }
                
                out_functions.push_back(ILuaFunctionUPtr(new LuaFunction<LuaUtils::NumValues<TResult>::value, typename std::decay<TResult>::type, typename std::decay<TArgs>::type...>(in_vm, in_functionName, Core::MakeDelegate(in_object, in_function))));
                
                lua_pop(in_vm, 1);
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
            /// @param Lua VM
            /// @param Environment name to register against
            /// @param [Out] Functions registered
            /// @param Class name as accessed in Lua
            /// @param Instance of class to call into
            /// @param Name of function
            /// @param Functions
            //----------------------------------------------------
            template <typename TObject, typename...TFunctions>
            void RegisterClass(lua_State* in_vm, const std::string& in_envName, std::vector<ILuaFunctionUPtr>& out_functions, const std::string& in_objectName, TObject* in_object, const std::string& in_functionName, TFunctions&&...in_functions)
            {
                if(in_envName == k_globalEnvName)
                {
                    lua_pushglobaltable(in_vm);
                }
                else
                {
                    lua_getfield(in_vm, LUA_REGISTRYINDEX, in_envName.c_str());
                }
                
                s32 envIndex = lua_gettop(in_vm);
                
                std::string tableName(in_objectName + std::string("Table"));
                luaL_newmetatable(in_vm, tableName.c_str());
                
                RegisterClassFunctions(in_vm, out_functions, in_object, in_functionName, std::forward<TFunctions>(in_functions)...);
                
                lua_pushvalue(in_vm, -1);
                lua_setfield(in_vm, -1, "__index");
                lua_setfield(in_vm, envIndex, in_objectName.c_str());
                
                lua_pop(in_vm, 1);
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
            /// @param Lua VM
            /// @param Environment name to register against
            /// @param [Out] Functions registered
            /// @param Class name as accessed in Lua
            /// @param Name of function
            /// @param Functions
            //----------------------------------------------------
            template <typename...TFunctions>
            void RegisterStaticClass(lua_State* in_vm, const std::string& in_envName, std::vector<ILuaFunctionUPtr>& out_functions, const std::string& in_objectName, const std::string& in_functionName, TFunctions&&...in_functions)
            {
                if(in_envName == k_globalEnvName)
                {
                    lua_pushglobaltable(in_vm);
                }
                else
                {
                    lua_getfield(in_vm, LUA_REGISTRYINDEX, in_envName.c_str());
                }
                
                s32 envIndex = lua_gettop(in_vm);
                
                std::string tableName(in_objectName + std::string("Table"));
                luaL_newmetatable(in_vm, tableName.c_str());
                
                RegisterClassStaticFunctions(in_vm, out_functions, in_functionName, std::forward<TFunctions>(in_functions)...);
                
                lua_pushvalue(in_vm, -1);
                lua_setfield(in_vm, -1, "__index");
                lua_setfield(in_vm, envIndex, in_objectName.c_str());
                
                lua_pop(in_vm, 1);
            }
            //----------------------------------------------------
            /// Register an enum type that can be accessed in the
            /// lua script
            ///
            /// Usage: RegisterEnum("enumName", "a", MyEnum::A, "b", MyEnum::B, etc)
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Environment name to register against
            /// @param Enum name as accessed in Lua
            /// @param Name of type
            /// @param List of types
            //----------------------------------------------------
            template <typename...TTypes>
            void RegisterEnum(lua_State* in_vm, const std::string& in_envName, const std::string& in_enumName, const std::string& in_typeName, TTypes&&...in_types)
            {
                if(in_envName == k_globalEnvName)
                {
                    lua_pushglobaltable(in_vm);
                }
                else
                {
                    lua_getfield(in_vm, LUA_REGISTRYINDEX, in_envName.c_str());
                }
                
                s32 envIndex = lua_gettop(in_vm);
                
                std::string tableName(in_enumName + std::string("Table"));
                luaL_newmetatable(in_vm, tableName.c_str());
                
                RegisterEnumTypes(in_vm, in_typeName, std::forward<TTypes>(in_types)...);
                
                lua_pushvalue(in_vm, -1);
                lua_setfield(in_vm, -1, "__index");
                lua_setfield(in_vm, envIndex, in_enumName.c_str());
                
                lua_pop(in_vm, 1);
            }
        }
    }
}

#endif
