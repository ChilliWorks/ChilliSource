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
            template <> void PushValueToVM(lua_State* in_vm, bool in_arg)
            {
                lua_pushboolean(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, s32 in_arg)
            {
                lua_pushinteger(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, u32 in_arg)
            {
                lua_pushunsigned(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, f32 in_arg)
            {
                lua_pushnumber(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, f64 in_arg)
            {
                lua_pushnumber(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, std::string in_arg)
            {
                lua_pushstring(in_vm, in_arg.c_str());
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> void PushValueToVM(lua_State* in_vm, const char* in_arg)
            {
                lua_pushstring(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> bool PopValueFromVM(lua_State* in_vm)
            {
                //No luaL_checkboolean. Don't know why.
                bool result = lua_toboolean(in_vm, -1);
                
                lua_pop(in_vm, 1);
                return result;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> s32 PopValueFromVM(lua_State* in_vm)
            {
#ifdef CS_ENABLE_DEBUG
                s32 result = luaL_checkinteger(in_vm, -1);
#else
                s32 result = lua_tointeger(in_vm, -1);
#endif
                lua_pop(in_vm, 1);
                return result;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> u32 PopValueFromVM(lua_State* in_vm)
            {
#ifdef CS_ENABLE_DEBUG
                u32 result = luaL_checkunsigned(in_vm, -1);
#else
                u32 result = lua_tounsigned(in_vm, -1);
#endif
                lua_pop(in_vm, 1);
                return result;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> f32 PopValueFromVM(lua_State* in_vm)
            {
#ifdef CS_ENABLE_DEBUG
                f32 result = luaL_checknumber(in_vm, -1);
#else
                f32 result = lua_tonumber(in_vm, -1);
#endif
                lua_pop(in_vm, 1);
                return result;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> f64 PopValueFromVM(lua_State* in_vm)
            {
#ifdef CS_ENABLE_DEBUG
                f64 result = luaL_checknumber(in_vm, -1);
#else
                f64 result = lua_tonumber(in_vm, -1);
#endif
                lua_pop(in_vm, 1);
                return result;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> std::string PopValueFromVM(lua_State* in_vm)
            {
#ifdef CS_ENABLE_DEBUG
                std::string result = luaL_checkstring(in_vm, -1);
#else
                std::string result = lua_tostring(in_vm, -1);
#endif
                lua_pop(in_vm, 1);
                return result;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> const char* PopValueFromVM(lua_State* in_vm)
            {
#ifdef CS_ENABLE_DEBUG
                const char* result = luaL_checkstring(in_vm, -1);
#else
                const char* result = lua_tostring(in_vm, -1);
#endif
                lua_pop(in_vm, 1);
                return result;
            }
        }
    }
}

