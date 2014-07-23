//
//  PointerSystem.h
//  Chilli Source
//  Created by Ian Copland on 27/03/2014.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_INPUT_POINTER_POINTERSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_INPUT_POINTER_POINTERSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>

#include <unordered_map>

namespace CSBackend
{
    namespace Android
    {
        //------------------------------------------------------------
        /// The Android backend for the pointer system. This provides
        /// access to touch events.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------
        class PointerSystem final : public CSInput::PointerSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(PointerSystem);
            //----------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given ID.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------
			/// Called when a touch down event is received.
            ///
            /// @author Ian Copland
            ///
            /// @param The system Id of the touch.
            /// @param The touch location.
			//----------------------------------------------------
            void OnTouchDown(s32 in_systemId, const CSCore::Vector2& in_location);
            //----------------------------------------------------
			/// Called when a touch moved event is received.
            ///
            /// @author Ian Copland
            ///
            /// @param The system Id of the touch.
            /// @param The touch location.
			//----------------------------------------------------
			void OnTouchMoved(s32 in_systemId, const CSCore::Vector2& in_location);
            //----------------------------------------------------
			/// Called when a touch up event is received.
            ///
            /// @author Ian Copland
            ///
            /// @param The system Id of the touch.
			//----------------------------------------------------
			void OnTouchUp(s32 in_systemId);
            //----------------------------------------------------
            /// Android has no cursor so this does nothing
            ///
            /// @author S Downie
            //----------------------------------------------------
            void HideCursor() override {};
            //----------------------------------------------------
            /// Android has no cursor so this does nothing
            ///
            /// @author S Downie
            //----------------------------------------------------
            void ShowCursor() override {};
        private:
            friend CSInput::PointerSystemUPtr CSInput::PointerSystem::Create();

            //------------------------------------------------
            /// Default constructor. Declared private to force
            /// the use of the factory method.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            PointerSystem() = default;
            //------------------------------------------------
            /// Initialisation method called at a time when
            /// all App Systems have been created. System
            /// initialisation occurs in the order they were
            /// created.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            void OnInit() override;
            //------------------------------------------------
            /// Called when the application is being destroyed.
            /// This should be used to cleanup memory and
            /// references to other systems. System destruction
            /// occurs in the reverse order to which they
            /// were created
            ///
            /// @author Ian Copland
            //------------------------------------------------
            void OnDestroy() override;

            CSCore::Screen* m_screen = nullptr;
            std::unordered_map<s32, CSInput::Pointer::Id> m_systemIdToPointerIdMap;
        };
    }
}

#endif

#endif
