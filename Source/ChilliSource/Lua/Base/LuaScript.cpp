//
//  LuaScript.cpp
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

#include <ChilliSource/Lua/Base/LuaScript.h>

#include <ChilliSource/Core/Base/Utils.h>

extern "C"
{
#include <lua/lualib.h>
}

namespace ChilliSource
{
	namespace Lua
	{
        //----------------------------------------------------
        //----------------------------------------------------
        LuaScriptUPtr LuaScript::Create(Core::StorageLocation in_location, const std::string& in_filePath)
        {
            return LuaScriptUPtr(new LuaScript(in_location, in_filePath));
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        LuaScript::LuaScript(Core::StorageLocation in_location, const std::string& in_filePath)
        {
            std::string luaContents;
            Core::Utils::FileToString(in_location, in_filePath, luaContents);
            
            m_luaVM = luaL_newstate();
            luaL_openlibs(m_luaVM);
            
            auto loadResult = luaL_loadstring(m_luaVM, luaContents.c_str());
            if(loadResult != 0)
            {
                CS_LOG_FATAL("Error loading LUA file: " + std::string(lua_tostring(m_luaVM, -1)));
            }
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void LuaScript::Run()
        {
            auto runResult = lua_pcall(m_luaVM, 0, 0, 0);
            if(runResult != 0)
            {
                CS_LOG_FATAL("Error compiling LUA file: " + std::string(lua_tostring(m_luaVM, -1)));
            }
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        LuaScript::~LuaScript()
        {
            //Functions must be cleared before the VM is destroyed
            //as they unbind themselves from the Lua VM.
            m_functions.clear();
            lua_close(m_luaVM);
        }
	}
}
