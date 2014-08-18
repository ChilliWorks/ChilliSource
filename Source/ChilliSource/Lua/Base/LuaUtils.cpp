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

#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

namespace ChilliSource
{
    namespace Lua
    {
        namespace LuaUtils
        {
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, bool in_arg)
            {
                lua_pushboolean(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, s32 in_arg)
            {
                lua_pushinteger(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, u32 in_arg)
            {
                lua_pushunsigned(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, f32 in_arg)
            {
                lua_pushnumber(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, f64 in_arg)
            {
                lua_pushnumber(in_vm, in_arg);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, Core::Vector2&& in_arg)
            {
                lua_pushnumber(in_vm, in_arg.x);
                lua_pushnumber(in_vm, in_arg.y);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, Core::Vector3&& in_arg)
            {
                lua_pushnumber(in_vm, in_arg.x);
                lua_pushnumber(in_vm, in_arg.y);
                lua_pushnumber(in_vm, in_arg.z);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, Core::Vector4&& in_arg)
            {
                lua_pushnumber(in_vm, in_arg.x);
                lua_pushnumber(in_vm, in_arg.y);
                lua_pushnumber(in_vm, in_arg.z);
                lua_pushnumber(in_vm, in_arg.w);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, Core::Colour&& in_arg)
            {
                lua_pushnumber(in_vm, in_arg.r);
                lua_pushnumber(in_vm, in_arg.g);
                lua_pushnumber(in_vm, in_arg.b);
                lua_pushnumber(in_vm, in_arg.a);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            void PushValueToVM(lua_State* in_vm, Rendering::UVs&& in_arg)
            {
                lua_pushnumber(in_vm, in_arg.m_u);
                lua_pushnumber(in_vm, in_arg.m_v);
                lua_pushnumber(in_vm, in_arg.m_s);
                lua_pushnumber(in_vm, in_arg.m_t);
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
            template <> Core::Vector2 ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                f32 x = (f32)luaL_checknumber(in_vm, in_index);
                f32 y = (f32)luaL_checknumber(in_vm, in_index+1);
#else
                f32 x = (f32)lua_tonumber(in_vm, in_index);
                f32 y = (f32)lua_tonumber(in_vm, in_index+1);
#endif
                
                return Core::Vector2(x, y);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> Core::Vector3 ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                f32 x = (f32)luaL_checknumber(in_vm, in_index);
                f32 y = (f32)luaL_checknumber(in_vm, in_index+1);
                f32 z = (f32)luaL_checknumber(in_vm, in_index+2);
#else
                f32 x = (f32)lua_tonumber(in_vm, in_index);
                f32 y = (f32)lua_tonumber(in_vm, in_index+1);
                f32 z = (f32)lua_tonumber(in_vm, in_index+2);
#endif
                
                return Core::Vector3(x, y, z);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> Core::Vector4 ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                f32 x = (f32)luaL_checknumber(in_vm, in_index);
                f32 y = (f32)luaL_checknumber(in_vm, in_index+1);
                f32 z = (f32)luaL_checknumber(in_vm, in_index+2);
                f32 w = (f32)luaL_checknumber(in_vm, in_index+3);
#else
                f32 x = (f32)lua_tonumber(in_vm, in_index);
                f32 y = (f32)lua_tonumber(in_vm, in_index+1);
                f32 z = (f32)lua_tonumber(in_vm, in_index+2);
                f32 w = (f32)lua_tonumber(in_vm, in_index+3);
#endif
                
                return Core::Vector4(x, y, z, w);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> Core::Colour ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                f32 r = (f32)luaL_checknumber(in_vm, in_index);
                f32 g = (f32)luaL_checknumber(in_vm, in_index+1);
                f32 b = (f32)luaL_checknumber(in_vm, in_index+2);
                f32 a = (f32)luaL_checknumber(in_vm, in_index+3);
#else
                f32 r = (f32)lua_tonumber(in_vm, in_index);
                f32 g = (f32)lua_tonumber(in_vm, in_index+1);
                f32 b = (f32)lua_tonumber(in_vm, in_index+2);
                f32 a = (f32)lua_tonumber(in_vm, in_index+3);
#endif
                
                return Core::Colour(r, g, b, a);
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            template <> Rendering::UVs ReadValueFromVM(lua_State* in_vm, s32 in_index)
            {
#ifdef CS_ENABLE_DEBUG
                f32 u = (f32)luaL_checknumber(in_vm, in_index);
                f32 v = (f32)luaL_checknumber(in_vm, in_index+1);
                f32 s = (f32)luaL_checknumber(in_vm, in_index+2);
                f32 t = (f32)luaL_checknumber(in_vm, in_index+3);
#else
                f32 u = (f32)lua_tonumber(in_vm, in_index);
                f32 v = (f32)lua_tonumber(in_vm, in_index+1);
                f32 s = (f32)lua_tonumber(in_vm, in_index+2);
                f32 t = (f32)lua_tonumber(in_vm, in_index+3);
#endif
                
                return Rendering::UVs(u, v, s, t);
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

