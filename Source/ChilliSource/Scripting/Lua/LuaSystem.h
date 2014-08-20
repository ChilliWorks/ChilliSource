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
		};
	}
}

#endif
