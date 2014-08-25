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

#include <ChilliSource/Scripting/Lua/LuaSystem.h>

#include <ChilliSource/Scripting/Lua/LuaScript.h>

namespace ChilliSource
{
	namespace Scripting
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
            m_luaVM = luaL_newstate();
            luaL_openlibs(m_luaVM);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        LuaScriptUPtr LuaSystem::CreateScript(const LuaSourceCSPtr& in_luaSource)
        {
            return LuaScript::Create(m_luaVM, in_luaSource);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void LuaSystem::OnDestroy()
        {
            m_functions.clear();
            lua_close(m_luaVM);
        }
	}
}
