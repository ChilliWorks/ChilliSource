//
//  PointerProxy.h
//  Chilli Source
//  Created by Scott Downie on 18/09/2014.
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

#ifndef _CHILLISOURCE_INPUT_POINTER_POINTERPROXY_H_
#define _CHILLISOURCE_INPUT_POINTER_POINTERPROXY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Input/Pointer/Pointer.h>

namespace ChilliSource
{
    namespace Input
    {
        
        //----------------------------------------------------------------------------------------
        /// Functions from pointer that are exposed to Lua as static functions acting
        /// on the given pointer instance. This allows Lua to access pointer info without owning them
        /// as expensive full data.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        namespace PointerProxy
        {
            //----------------------------------------------------
            /// Register all the proxy functions with Lua
            ///
            /// @author S Downie
            ///
            /// @param Lua system
            //----------------------------------------------------
            void RegisterWithLua(Scripting::LuaSystem* in_system);
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Pointer instance to act on
            ///
            /// @return Current pointer location
            //----------------------------------------------------
            Core::Vector2 GetPosition(Pointer* in_pointer);
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Pointer instance to act on
            ///
            /// @return Previous pointer location
            //----------------------------------------------------
            Core::Vector2 GetPreviousPosition(Pointer* in_pointer);
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Pointer instance to act on
            /// @param Input type
            ///
            /// @return Whether the input type is down
            //----------------------------------------------------
            bool IsInputDown(Pointer* in_pointer, Pointer::InputType in_type);
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Pointer instance to act on
            /// @param Input type
            ///
            /// @return Whether the input type is up
            //----------------------------------------------------
            bool IsInputUp(Pointer* in_pointer, Pointer::InputType in_type);
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Pointer instance to act on
            ///
            /// @return Id
            //----------------------------------------------------
            Pointer::Id GetId(Pointer* in_pointer);
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Pointer instance to act on
            ///
            /// @return Index of pointer in pointer list
            //----------------------------------------------------
            u32 GetIndex(Pointer* in_pointer);
        };
    }
}

#endif
