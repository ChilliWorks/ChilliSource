//
//  LuaSource.h
//  Chilli Source
//  Created by Scott Downie on 20/08/2014.
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

#ifndef _CHILLISOURCE_SCRIPTING_LUA_LUASOURCE_H_
#define _CHILLISOURCE_SCRIPTING_LUA_LUASOURCE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Scripting
	{
        //---------------------------------------------------------------------
        /// A resource that holds lua source
        ///
        /// @author S Downie
        //---------------------------------------------------------------------
		class LuaSource final : public Core::Resource
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(LuaSource);
			//---------------------------------------------------------------------
			/// @author S Downie
            ///
            /// @param Interface ID
			///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------------
            /// @author S Downie
			///
			/// @param Source code
			//---------------------------------------------------------------------
			void Build(const std::string& in_source);
            //---------------------------------------------------------------------
            /// @author S Downie
			///
			/// @return Source code
			//---------------------------------------------------------------------
            const std::string& GetSource() const;
		
        private:
            
            friend class Core::ResourcePool;
            //---------------------------------------------------------------------
            /// Factory method to create empty font resource. Used only by the
            /// resource pool.
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
            static LuaSourceUPtr Create();
			//---------------------------------------------------------------------
            /// Private constructor to enforce the use of factory method
            ///
            /// @author S Downie
            //---------------------------------------------------------------------
            LuaSource() = default;
            
        private:
            
            std::string m_source;
		};
	}
}

#endif
