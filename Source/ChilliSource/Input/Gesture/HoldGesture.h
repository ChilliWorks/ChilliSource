//
//  HoldGesture.h
//  Chilli Source
//  Created by Ian Copland on 19/09/2014.
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

#ifndef _CHILLISOURCE_INPUT_GESTURE_HOLDGESTURE_H_
#define _CHILLISOURCE_INPUT_GESTURE_HOLDGESTURE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Input/Gesture/Gesture.h>

#include <functional>

namespace ChilliSource
{
    namespace Input
    {
        //----------------------------------------------------------
        /// A gesture for receiving hold input events. Allows for
        /// multi-finger events.
        ///
        /// A hold gesture requires that all intended pointers be
        /// pressed down within a short period of each other. These
        /// must then stay down in the same position until the hold
        /// timer elapses.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        class HoldGesture final : public Gesture
        {
        public:
            CS_DECLARE_NAMEDTYPE(HoldGesture);
            //----------------------------------------------------
            /// A delegate called when a hold gesture is activated.
            ///
            /// @author Ian Copland
            ///
            /// @param A pointer to the hold gesture that was
            /// activated.
            /// @param The position at which the tap occurred.
            //----------------------------------------------------
            using Delegate = std::function<void(const HoldGesture*, const Core::Vector2&)>;
            //----------------------------------------------------
            /// Constructor. Constructs the hold gesture with the
            /// given settings.
            ///
            /// @author Ian Copland
            ///
            /// @param The number of pointers that should be
            /// down per tap. Defaults to 1.
            /// @param The input type this gesture should listen
            /// for. Defaults to using the default input type.
            //----------------------------------------------------
			HoldGesture(u32 in_numPointers = 1, Pointer::InputType in_inputType = Pointer::GetDefaultInputType());
            //----------------------------------------------------
            /// Queries whether or not this implements the gesture
            /// interface with the given Id.
            ///
            /// @author Ian Copland
            ///
            /// @param The gesture interface Id.
            ///
            /// @return Whether or not the gesture inteface is
            /// implemented.
            //----------------------------------------------------
			bool IsA(Core::InterfaceIDType in_gestureInterfaceId) const override;
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of pointers required by this
            /// Gesture.
            //----------------------------------------------------
            u32 GetNumPointers() const;
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The input type this gesture will listen for.
            //----------------------------------------------------
            Pointer::InputType GetInputType() const;
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// ocurrances of the gesture.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetHeldEvent();
        private:
            //----------------------------------------------------
            /// Information on the initial state of a single
            /// pointer within a tap.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            struct PointerInfo final
            {
                Core::Vector2 m_initialPosition;
                Pointer::Id m_pointerId;
            };
            //--------------------------------------------------------
            /// Resets the gesture if one is active.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void Reset();
            //--------------------------------------------------------
            /// Updates the hold gesture. Tests to see if the hold
            /// time has exceeded.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time since the last update.
            //--------------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //--------------------------------------------------------
            /// Called when a pointer down event occurs.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer.
            /// @param The timestamp of the event.
            /// @param The press type.
            //--------------------------------------------------------
            void OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType) override;
            //--------------------------------------------------------
            /// Called when a pointer moved event occurs.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            //--------------------------------------------------------
            void OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp) override;
            //--------------------------------------------------------
            /// Called when a pointer up event occurs.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            /// @param The press type.
            //--------------------------------------------------------
            void OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType) override;
            
            u32 m_requiredPointerCount;
            Pointer::InputType m_requiredInputType;
            Core::Event<Delegate> m_heldEvent;
            
            f32 m_maxDisplacementSquared = 0.0f;
            
            bool m_holdPending = false;
            std::vector<PointerInfo> m_pendingPointers;
            f64 m_currentStartTimestamp = 0.0;
            f64 m_lastEndTimestamp = 0.0;
        };
    }
}

#endif
