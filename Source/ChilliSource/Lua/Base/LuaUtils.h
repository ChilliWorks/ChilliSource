//
//  LuaUtils.h
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

#ifndef _CHILLISOURCE_LUA_BASE_LUAUTILS_H_
#define _CHILLISOURCE_LUA_BASE_LUAUTILS_H_

#include <ChilliSource/ChilliSource.h>

extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
}

#include <tuple>

namespace ChilliSource
{
    namespace Lua
    {
        namespace LuaUtils
        {
            //---------------------------------------------------------
            /// Push the value to the Lua VM stack. This is specialised
            /// for each supported value type and will assert if
            /// unsupported type is used.
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Value
            //---------------------------------------------------------
            template <typename TArg> void PushValueToVM(lua_State* in_vm, TArg in_arg)
            {
                static_assert(std::is_pointer<TArg>::value, "Lua type not supported");
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, bool in_arg);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, s32 in_arg);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, u32 in_arg);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, f32 in_arg);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, f64 in_arg);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, std::string in_arg);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, const char* in_arg);
            //---------------------------------------------------------
            /// Pop the value from the index of the Lua VM stack. This is specialised
            /// for each supported value type and will assert if
            /// unsupported type is used.
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Index in the stack
            ///
            /// @return Value
            //---------------------------------------------------------
            template <typename TResult> TResult ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
                static_assert(std::is_pointer<TResult>::value, "Lua type not supported");
                return TResult();
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> bool ReadValueFromVM(lua_State* in_vm, s32 in_index);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> s32 ReadValueFromVM(lua_State* in_vm, s32 in_index);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> u32 ReadValueFromVM(lua_State* in_vm, s32 in_index);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> f32 ReadValueFromVM(lua_State* in_vm, s32 in_index);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> f64 ReadValueFromVM(lua_State* in_vm, s32 in_index);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> std::string ReadValueFromVM(lua_State* in_vm, s32 in_index);
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> const char* ReadValueFromVM(lua_State* in_vm, s32 in_index);
            //---------------------------------------------------------
            /// Specialised function for pushing no values to the
            /// Lua stack.
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            //---------------------------------------------------------
            void PushAllToVM(lua_State* in_vm);
            //---------------------------------------------------------
            /// Helper function for pushing values of different types
            /// to the Lua stack using template recursion
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            /// @param Value
            /// @param Rest of values
            //---------------------------------------------------------
            template <typename TValue, typename... TValues> void PushAllToVM(lua_State* in_vm, TValue&& in_value, TValues&&... in_restValues)
            {
                PushValueToVM(in_vm, std::forward<TValue>(in_value));
                PushAllToVM(in_vm, std::forward<TValues>(in_restValues)...);
            }
            //---------------------------------------------------------
            /// Performs recursive popping of the Lua VM stack
            /// into a tuple
            ///
            /// @author S Downie
            //---------------------------------------------------------
            template <size_t, typename... TResults> struct Popper
            {
                using type = std::tuple<TResults...>;
                
                //---------------------------------------------------------
                /// Creates a tuple that contains a single element
                /// of template type with the value of the given index
                /// in the Lua stack
                ///
                /// @author S Downie
                ///
                /// @param Lua VM
                /// @param Index in Lua stack
                ///
                /// @return Tuple
                //---------------------------------------------------------
                template <typename TElementType> static std::tuple<TElementType> ReadValue(lua_State* in_vm, s32 in_index)
                {
                    return std::make_tuple(ReadValueFromVM<TElementType>(in_vm, in_index));
                }
                //---------------------------------------------------------
                /// Creates a tuple from the values starting at the
                /// given index in the Lua stack.
                ///
                /// @author S Downie
                ///
                /// @param Lua VM
                /// @param Index in Lua stack to start from
                ///
                /// @return Tuple
                //---------------------------------------------------------
                template <typename TElementType1, typename TElementType2, typename... TElementTypeRest>
                static std::tuple<TElementType1, TElementType2, TElementTypeRest...> ReadValue(lua_State* in_vm, s32 in_index)
                {
                    std::tuple<TElementType1> head = std::make_tuple(ReadValueFromVM<TElementType1>(in_vm, in_index));
                    return std::tuple_cat(head, ReadValue<TElementType2, TElementTypeRest...>(in_vm, in_index + 1));
                }
                //---------------------------------------------------------
                /// Entry function that builds a tuple from
                /// the contents of the VM stack. The results
                /// are built in reverse order with the top of the
                /// stack the last element in the tuple
                ///
                /// @author S Downie
                ///
                /// @param Lua VM
                ///
                /// @return Tuple
                //---------------------------------------------------------
                static type PopRecursive(lua_State* in_vm)
                {
                    auto ret = ReadValue<TResults...>(in_vm, 1);
                    lua_pop(in_vm, sizeof...(TResults));
                    return ret;
                }
            };
            //---------------------------------------------------------
            /// Special case for zero results which will specify void
            /// return type
            ///
            /// @author S Downie
            //---------------------------------------------------------
            template <typename... TResults> struct Popper<0, TResults...>
            {
                using type = void;
                
                //---------------------------------------------------------
                /// Special case for no return results which peforms
                /// no action
                ///
                /// @author S Downie
                ///
                /// @param Lua VM
                ///
                /// @return Void
                //---------------------------------------------------------
                static type PopRecursive(lua_State* in_vm) {}
            };
            //---------------------------------------------------------
            /// Helper function for popping all the values from the
            /// Lua VM stack into a tuple or void.
            ///
            /// NOTE: The results are built in reverse order with the
            /// top of the stack the last element in the tuple
            ///
            /// @author S Downie
            ///
            /// @param Lua VM
            ///
            /// @return Tuple or void depending on num results
            //---------------------------------------------------------
            template <typename... TResults> typename Popper<sizeof...(TResults), TResults...>::type PopAllFromVM(lua_State* in_vm)
            {
                return Popper<sizeof...(TResults), TResults...>::PopRecursive(in_vm);
            }
        }
    }
}

#endif
