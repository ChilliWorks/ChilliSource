//
//  GestureSystem.h
//  Chilli Source
//  Created by Ian Copland on 17/09/2014.
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

#ifndef _CHILLISOURCE_INPUT_GESTURES_GESTURESYSTEM_H_
#define _CHILLISOURCE_INPUT_GESTURES_GESTURESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/StateSystem.h>

namespace ChilliSource
{
    namespace Input
    {
        //---------------------------------------------------------------
        /// A system for managing all input gestures. All gestures must
        /// be registered with this system before they will recieve input
        /// events. Gesture collision resolution can also be handled
        /// through this system.
        ///
        /// @author Ian Copland
        //---------------------------------------------------------------
        class GestureSystem final : public Core::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(GestureSystem);
            //--------------------------------------------------------
            /// Queries whether or not this implements the interface
            /// with the given Id.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the inteface is implemented.
            //---------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------
            /// Queries whether or not this implements the interface
            /// with the given Id.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the inteface is implemented.
            //---------------------------------------------------------
			bool AddGesture();
        private:

            friend class Core::State;
            //--------------------------------------------------------
            /// Creates a new instance of this system. This shou
            ///
            /// @author Ian Copland
            ///
            /// @param The content downloader that should be used.
            ///
            /// @return The new system instance.
            //--------------------------------------------------------
			static GestureSystemUPtr Create();
            //--------------------------------------------------------
            /// Constructor. Declared private to force the use of the
            /// factory method.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            GestureSystem();
        };
    }
}

#endif
