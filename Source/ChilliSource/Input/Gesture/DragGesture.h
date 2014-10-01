//
//  DragGesture.h
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

#ifndef _CHILLISOURCE_INPUT_GESTURE_DRAGGESTURE_H_
#define _CHILLISOURCE_INPUT_GESTURE_DRAGGESTURE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Input/Gesture/Gesture.h>

#include <functional>

namespace ChilliSource
{
    namespace Input
    {
        //----------------------------------------------------------
        /// A gesture for receiving drag input events. This incudes
        /// support for multi-finger events such as a two finger
        /// drag.
        ///
        /// A drag gesture is started when the required pointer
        /// count is reached.
        ///
        /// Drag moved events will be sent when any movement occurs
        /// with exactly the correct number of pointers on the screen.
        ///
        /// A drag will end when all fingers are lifted from the
        /// screen.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------
        class DragGesture final : public Gesture
        {
        public:
            CS_DECLARE_NAMEDTYPE(DragGesture);
            //----------------------------------------------------
            /// A container for information on a single drag event.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            struct DragInfo final
            {
                Core::Vector2 m_position;        //The screen space location of the gesture.
                Core::Vector2 m_displacement;   //The displacement since the gesture started. Displacement caused by the currently tracked pointer changing is ignored.
                Core::Vector2 m_delta;          //The change in displacement since the last event. Displacement caused by the currently tracked pointer changing is ignored.
            };
            //----------------------------------------------------
            /// A delegate called for any of the drag gesture
            /// event.
            ///
            /// @author Ian Copland
            ///
            /// @param A pointer to the drag gesture.
            /// @param Information on the drag.
            //----------------------------------------------------
            using Delegate = std::function<void(const DragGesture*, const DragInfo&)>;
            //----------------------------------------------------
            /// Constructor. Constructs the drag gesture with the
            /// given settings.
            ///
            /// @author Ian Copland
            ///
            /// @param The number of pointers that should be
            /// down per tap. Defaults to 1.
            /// @param The input type this gesture should listen
            /// for. Defaults to using the default input type.
            //----------------------------------------------------
			DragGesture(u32 in_numPointers = 1, Pointer::InputType in_inputType = Pointer::GetDefaultInputType());
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
            /// gesture.
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
            /// @return An event that can be used to listen for the
            /// start of a drag gesture.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetDragStartedEvent();
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to list for
            /// movement events within a drag gesture.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetDragMovedEvent();
            //----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event that can be used to list for
            /// the end of a drag gesture.
            //----------------------------------------------------
            Core::IConnectableEvent<Delegate>& GetDragEndedEvent();
        private:
            //----------------------------------------------------
            /// Information on a single pointer within the gesture.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            struct PointerInfo
            {
                Pointer::Id m_pointerId;
                Core::Vector2 m_initialPosition;
                Core::Vector2 m_currentPosition;
                bool m_isDrag = false;
                bool m_active = false;
            };
            //--------------------------------------------------------
            /// Tries to activate or un-pause a drag gesture.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer.
            //--------------------------------------------------------
            void TryStart(const Pointer& in_pointer);
            //----------------------------------------------------
            /// Calculates the current position of the gesture.
            /// This will be the centre position between all pointers
            /// involved in the gesture.
            ///
            /// @author Ian Copland
            ///
            /// @return The calculated position.
            //----------------------------------------------------
            Core::Vector2 CalculatePosition() const;
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
            
            u32 m_requiredPointerCount;
            Pointer::InputType m_requiredInputType;
            Core::Event<Delegate> m_dragStartedEvent;
            Core::Event<Delegate> m_dragMovedEvent;
            Core::Event<Delegate> m_dragEndedEvent;

            f32 m_minDisplacementSquared = 0.0f;
            
            std::vector<PointerInfo> m_pendingPointers;
            DragInfo m_currentDragInfo;
            bool m_paused = false;
        };
    }
}

#endif
