//
//  LuaSystem.cpp
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

#include <ChilliSource/Lua/Base/LuaSystem.h>

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Lua/Base/LuaScript.h>

extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace ChilliSource
{
	namespace Lua
	{
        CS_DEFINE_NAMEDTYPE(LuaSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        LuaSystemUPtr LuaSystem::Create()
        {
            return LuaSystemUPtr(new LuaSystem());
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool LuaSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == LuaSystem::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void LuaSystem::OnInit()
        {
            //TODO: Open the Lua VM here when we know how to have multiple environments in
            //a single Lua state.
        }
        //----------------------------------------------------
        //----------------------------------------------------
        LuaScriptUPtr LuaSystem::LoadScript(Core::StorageLocation in_location, const std::string& in_filePath)
        {
            std::string luaContents;
            Core::Utils::FileToString(in_location, in_filePath, luaContents);
            
            auto luaVM = luaL_newstate();
            luaL_openlibs(luaVM);
            
            return LuaScript::Create(luaVM, luaContents);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        LuaScriptUPtr LuaSystem::CreateScript(const std::string& in_luaCode)
        {
            auto luaVM = luaL_newstate();
            luaL_openlibs(luaVM);
            
            return LuaScript::Create(luaVM, in_luaCode);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void LuaSystem::OnDestroy()
        {
            //TODO: Close the Lua VM here when we know how to have multiple environments in
            //a single Lua state
        }
	}
}
