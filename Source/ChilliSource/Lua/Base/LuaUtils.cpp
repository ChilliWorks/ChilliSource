//
//  LuaUtils.cpp
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

#include <ChilliSource/Lua/Base/LuaUtils.h>


namespace ChilliSource
{
    namespace Lua
    {
        namespace LuaUtils
        {
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, bool&& in_arg)
            {
                lua_pushboolean(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, s32&& in_arg)
            {
                lua_pushinteger(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, u32&& in_arg)
            {
                lua_pushunsigned(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, f32&& in_arg)
            {
                lua_pushnumber(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, f64&& in_arg)
            {
                lua_pushnumber(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, std::string&& in_arg)
            {
                lua_pushstring(in_vm, in_arg.c_str());
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, const char* in_arg)
            {
                lua_pushstring(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> bool ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
                //No luaL_checkboolean. Don't know why.
                return lua_toboolean(in_vm, in_index);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> s32 ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                return luaL_checkinteger(in_vm, in_index);
#else
                return lua_tointeger(in_vm, in_index);
#endif
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> u32 ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                return luaL_checkunsigned(in_vm, in_index);
#else
                return lua_tounsigned(in_vm, in_index);
#endif
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> f32 ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                return (f32)luaL_checknumber(in_vm, in_index);
#else
                return (f32)lua_tonumber(in_vm, in_index);
#endif
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> f64 ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                return luaL_checknumber(in_vm, in_index);
#else
                return lua_tonumber(in_vm, in_index);
#endif
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> std::string ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                return luaL_checkstring(in_vm, in_index);
#else
                return lua_tostring(in_vm, in_index);
#endif
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> const char* ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                return luaL_checkstring(in_vm, in_index);
#else
                return lua_tostring(in_vm, in_index);
#endif
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushAllToVM(lua_State* in_vm)
            {
            }
        }
    }
}

