//
//  RotationGesture.h
//  Chilli Source
//  Created by Ian Copland on 23/09/2014.
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

#ifndef _CHILLISOURCE_INPUT_GESTURE_ROTATIONGESTURE_H_
#define _CHILLISOURCE_INPUT_GESTURE_ROTATIONGESTURE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Input/Gesture/Gesture.h>

#include <functional>

namespace ChilliSource
{
    namespace Input
    {
        //----------------------------------------------------------
        /// A gesture for receiving rotation input events.
        ///
        /// A rotation gesture will start when at least two pointers
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
        class RotationGesture final : public Gesture
        {
        public:
            CS_DECLARE_NAMEDTYPE(RotationGesture);
            //----------------------------------------------------
            /// A container for information on a single rotation
            /// event.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            struct RotationInfo final
            {
                Core::Vector2 m_position;   //The screen space location of the gesture. This is the centroid of the two active pointers.
                f32 m_rotation = 0.0f;      //The clockwise angle of the gesture from the initial orientation. Change caused by the currently tracked pointer changing is ignored.
                f32 m_rotationDelta = 0.0f; //The change in the rotation since the last event. Change caused by the currently tracked pointer changing is ignored.
            };
            //----------------------------------------------------
            /// A delegate called when a rotation gesture is activated.
            ///
            /// @author Ian Copland
            ///
            /// @param A pointer to the rotation gesture that was
            /// activated.
            /// @param Information on the rotation.
            //----------------------------------------------------
            using Delegate = std::function<void(const RotationGesture*, const RotationInfo&)>;
            //----------------------------------------------------
            /// Constructor. Constructs the rotation gesture with the
            /// given settings.
            ///
            /// @author Ian Copland
            ///
            /// @param The input type this gesture should listen
            /// for. Defaults to using the default input type.
            //----------------------------------------------------
            RotationGesture(Pointer::InputType in_inputType = Pointer::GetDefaultInputType());
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
            /// start of a rotation.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetRotationStartedEvent();
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// movement in a rotation.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetRotationMovedEvent();
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to listen for
            /// end of a rotation.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetRotationEndedEvent();
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
            //----------------------------------------------------
            /// Tries to activate or un-pause a pinch gesture.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer.
            //----------------------------------------------------
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
            /// Calculates the clockwise angle of the gesture.
            ///
            /// @author Ian Copland
            ///
            /// @return The calculated angle.
            //----------------------------------------------------
            f32 CalculateAngle() const;
            //----------------------------------------------------
            /// Calculates the clockwise rotation of the gesture
            /// relative to the initial rotation.
            ///
            /// @author Ian Copland
            ///
            /// @return The calculated rotation angle.
            //----------------------------------------------------
            f32 CalculateRelativeRotation() const;
            //--------------------------------------------------------
            /// This will be called if the gesture is currently active
            /// and conflict resolution deduces that it should end.
            /// This will de-activate the current gesture and set all
            /// pointers back to inactive.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void Cancel() override;
            //----------------------------------------------------
            /// Called when a pointer down event occurs.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer.
            /// @param The timestamp of the event.
            /// @param The press type.
            //----------------------------------------------------
            void OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType) override;
            //----------------------------------------------------
            /// Called when a pointer moved event occurs.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            //----------------------------------------------------
            void OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp) override;
            //----------------------------------------------------
            /// Called when a pointer up event occurs.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            /// @param The press type.
            //----------------------------------------------------
            void OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType) override;
            
            Pointer::InputType m_requiredInputType;
            Core::Event<Delegate> m_rotationStartedEvent;
            Core::Event<Delegate> m_rotationMovedEvent;
            Core::Event<Delegate> m_rotationEndedEvent;
            
            f32 m_minDisplacementSquared = 0.0f;
            
            std::vector<PointerInfo> m_pendingPointers;
            f32 m_initialAngle = 0.0f;
            RotationInfo m_currentRotationInfo;
            bool m_paused = false;
        };
    }
}

#endif
