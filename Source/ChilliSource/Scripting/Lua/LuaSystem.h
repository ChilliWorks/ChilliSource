//
//  LuaSystem.h
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

#ifndef _CHILLISOURCE_SCRIPTING_LUA_LUASYSTEM_H_
#define _CHILLISOURCE_SCRIPTING_LUA_LUASYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Scripting/Lua/Function/LuaFunctionUtils.h>

namespace ChilliSource
{
	namespace Scripting
	{
		//-----------------------------------------------------------
        /// The system is used to create Lua scripts.
        ///
        /// @author S Downie
		//-----------------------------------------------------------
		class LuaSystem final : public Core::AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(LuaSystem);
            
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID to check
            ///
            /// @return Whether this object implements the given interface
            //----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
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
                LuaFunctionUtils::RegisterFunction(m_luaVM, LuaFunctionUtils::k_globalEnvName, m_functions, in_functionName, in_function);
            }
            //----------------------------------------------------
            /// Register free function to be called in the Lua
            /// script. This must be performed prior to calling
            /// run or the script will error
            ///
            /// @author S Downie
            ///
            /// @param Function name as called by Lua
            /// @param Object to bind function to
            /// @param Function
            //----------------------------------------------------
            template <typename TClass, typename TResult, typename...TArgs> void RegisterFunction(const std::string& in_functionName, TClass* in_object, TResult (TClass::*in_function)(TArgs...))
            {
                LuaFunctionUtils::RegisterFunction(m_luaVM, LuaFunctionUtils::k_globalEnvName, m_functions, in_functionName, in_object, in_function);
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
                LuaFunctionUtils::RegisterClass(m_luaVM, LuaFunctionUtils::k_globalEnvName, m_functions, in_objectName, in_object, in_functionName, std::forward<TFunctions>(in_functions)...);
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
                LuaFunctionUtils::RegisterStaticClass(m_luaVM, LuaFunctionUtils::k_globalEnvName, m_functions, in_objectName, in_functionName, std::forward<TFunctions>(in_functions)...);
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
                LuaFunctionUtils::RegisterEnum(m_luaVM, LuaFunctionUtils::k_globalEnvName, in_enumName, in_typeName, std::forward<TTypes>(in_types)...);
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
                LuaFunctionUtils::RegisterVariable(m_luaVM, LuaFunctionUtils::k_globalEnvName, in_name, in_variable);
            }
            //----------------------------------------------------
            /// Creates the lua script instance from the given source code
            ///
            /// @author S Downie
            ///
            /// @param Lua source
            ///
            /// @return Script
            //----------------------------------------------------
            LuaScriptUPtr CreateScript(const LuaSourceCSPtr& in_luaSource);
            
        private:
            friend class Core::Application;
            
            //----------------------------------------------------
			/// Creates a new instance of this system.
            ///
            /// @author S Downie
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static LuaSystemUPtr Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            LuaSystem() = default;
            //-------------------------------------------------------
            /// Creates the Lua virtual machine
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void OnInit() override;
            //-------------------------------------------------------
            /// Destroys the Lua virtual machine
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void OnDestroy() override;
            
        private:
            
            std::vector<ILuaFunctionUPtr> m_functions;
            
            lua_State* m_luaVM;
		};
	}
}

#endif
