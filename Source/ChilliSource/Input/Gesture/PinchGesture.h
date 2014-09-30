//
//  PinchGesture.h
//  Chilli Source
//  Created by Ian Copland on 22/09/2014.
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

#ifndef _CHILLISOURCE_INPUT_GESTURE_PINCHGESTURE_H_
#define _CHILLISOURCE_INPUT_GESTURE_PINCHGESTURE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Input/Gesture/Gesture.h>

#include <functional>

namespace ChilliSource
{
    namespace Input
    {
        //----------------------------------------------------------
        /// A gesture for receiving pinch input events.
        ///
        /// A pinch gesture will start when at least two pointers
        /// have moved.
        ///
        /// The gesture will update whenever there are two active
        /// pointers and one of them moves. Other pointers will be
        /// ignored.
        ///
        /// The gesture will end when all pointers have been
        /// removed.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        class PinchGesture final : public Gesture
        {
        public:
            CS_DECLARE_NAMEDTYPE(PinchGesture);
            //----------------------------------------------------
            /// A container for information on a single pinch event.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            struct PinchInfo final
            {
                Core::Vector2 m_position;   //The screen space location of the gesture. This is the centroid of the two active pointers.
                f32 m_scale = 0.0f;         //The fraction difference between the initial pinch distance to the current. Change caused by the currently tracked pointer changing is ignored.
            };
            //----------------------------------------------------
            /// A delegate called when a pinch gesture is activated.
            ///
            /// @author Ian Copland
            ///
            /// @param A pointer to the pinch gesture that was
            /// activated.
            /// @param Information on the pinch gesture.
            //----------------------------------------------------
            using Delegate = std::function<void(const PinchGesture*, const PinchInfo&)>;
            //----------------------------------------------------
            /// Constructor. Constructs the pinch gesture with the
            /// given settings.
            ///
            /// @author Ian Copland
            ///
            /// @param The input type this gesture should listen
            /// for. Defaults to using the default input type.
            //----------------------------------------------------
            PinchGesture(Pointer::InputType in_inputType = Pointer::GetDefaultInputType());
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
            /// @return The input type this gesture will listen for.
            //----------------------------------------------------
            Pointer::InputType GetInputType() const;
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// start of a pinch.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetPinchStartedEvent();
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// movement in a pinch
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetPinchMovedEvent();
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// end of a pinch.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetPinchEndedEvent();
        private:
            //----------------------------------------------------
            /// Information on a single pointer within the gesture.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            struct PointerInfo final
            {
                Pointer::Id m_pointerId;
                Core::Vector2 m_initialPosition;
                Core::Vector2 m_currentPosition;
                bool m_isDrag = false;
                bool m_active = false;
            };
            //--------------------------------------------------------
            /// Tries to activate or un-pause a pinch gesture.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer.
            //--------------------------------------------------------
            void TryStart(const Pointer& in_pointer);
            //----------------------------------------------------
            /// Calculates the current position of the gesture.
            /// This will be the centre position between the pointers
            /// involved in the gesture.
            ///
            /// @author Ian Copland
            ///
            /// @return The calculated position.
            //----------------------------------------------------
            Core::Vector2 CalculatePosition() const;
            //----------------------------------------------------
            /// Calculates the current distance between the pointers
            /// in the gesture.
            ///
            /// @author Ian Copland
            ///
            /// @return The calculated scale.
            //----------------------------------------------------
            f32 CalculateDistance() const;
            //----------------------------------------------------
            /// Calculates the current fraction of the initial
            /// distance to the current.
            ///
            /// @author Ian Copland
            ///
            /// @return The calculated scale.
            //----------------------------------------------------
            f32 CalculateScale() const;
            //--------------------------------------------------------
            /// This will be called if the gesture is currently active
            /// and conflict resolution deduces that it should end.
            /// This will de-activate the current gesture and set all
            /// pointers back to inactive.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void Cancel() override;
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
            
            Pointer::InputType m_requiredInputType;
            Core::Event<Delegate> m_pinchStartedEvent;
            Core::Event<Delegate> m_pinchMovedEvent;
            Core::Event<Delegate> m_pinchEndedEvent;
            
            f32 m_minDisplacementSquared = 0.0f;
            
            std::vector<PointerInfo> m_pendingPointers;
            f32 m_initialDistance = 0.0f;
            PinchInfo m_currentPinchInfo;
            bool m_paused = false;
        };
    }
}

#endif
