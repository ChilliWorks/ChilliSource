//
//  PointerSystem.h
//  Chilli Source
//  Created by Ian Copland on 26/03/2014.
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

#ifndef _CHILLISOURCE_INPUT_POINTER_POINTERSYSTEM_H_
#define _CHILLISOURCE_INPUT_POINTER_POINTERSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <mutex>
#include <queue>
#include <set>

namespace ChilliSource
{
    namespace Input
    {
        //--------------------------------------------------------
        /// A system that provides a generic API for working with
        /// pointer devices. Pointers include touches on a touch
        /// screen and the mouse on a PC.
        ///
        /// @author I Copland
        //--------------------------------------------------------
        class PointerSystem : public Core::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(PointerSystem);
            //----------------------------------------------------
            /// typedef
            //----------------------------------------------------
            using PointerId = u64;
            //----------------------------------------------------
            /// An enum describing the different types of pointer
            /// press.
            ///
            /// @author I Copland
            //----------------------------------------------------
            enum class InputType
            {
                k_none,
                k_touch,
                k_leftMouseButton,
                k_middleMouseButton,
                k_rightMouseButton
            };
            //----------------------------------------------------
            /// A container for information on a single pointer.
            /// A pointer contains positional information as well
            /// as an index and unique Id. The Id is unique to that
            /// specific pointer and can be used to query for it.
            /// The index describes the active index of the pointer
            /// at the point it was created, so if there were already
            /// two active pointers existing at creation, it will be
            /// have an index of 2.
            ///
            /// @author I Copland
            //----------------------------------------------------
            struct Pointer
            {
                Core::Vector2 m_location;
                Core::Vector2 m_previousLocation;
                u32 m_pointerIndex;
                PointerId m_uniqueId;
                std::set<InputType> m_activeInput;
            };
            //----------------------------------------------------
            /// A delegate that is used to receive pointer down
            /// events. This could be pressing a mouse button or
            /// touching a touch screen.
            ///
            /// @param The pointer.
            /// @param The timestamp of the event.
            /// @param The press type.
            ///
            /// @author I Copland
            //----------------------------------------------------
            using PointerDownDelegate = std::function<void(const Pointer&, f64, InputType)>;
            //----------------------------------------------------
            /// A delegate that is used to receive pointer moved
            /// events. This could be dragging a touch on screen
            /// or moving the mouse cursor.
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            ///
            /// @author I Copland
            //----------------------------------------------------
            using PointerMovedDelegate = std::function<void(const Pointer&, f64)>;
            //----------------------------------------------------
            /// A delegate that is used to receive pointer up
            /// events. This could be releasing a mouse button or
            /// releasing a touch from the touch screen.
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            /// @param The press type.
            ///
            /// @author I Copland
            //----------------------------------------------------
            using PointerUpDelegate = std::function<void(const Pointer&, f64, InputType)>;
            //----------------------------------------------------
            /// Creates a new platfrom specific instance of pointer
            /// system.
            ///
            /// @author I Copland
            ///
            /// @return The new system instance.
            //----------------------------------------------------
            static PointerSystemUPtr Create();
            //----------------------------------------------------
            /// Returns the default press type for the current platform.
            /// This should be used where possible rather than
            /// direct use of the press type to make cross platform
            /// development easier.
            ///
            /// @author I Copland
            ///
            /// @return the default press type.
            //----------------------------------------------------
            static InputType GetDefaultInputType();
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @return The pointer down event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerDownDelegate>& GetPointerDownEvent();
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @return The pointer moved event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerMovedDelegate>& GetPointerMovedEvent();
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @return The pointer up event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerUpDelegate>& GetPointerUpEvent();
            //----------------------------------------------------
            /// Tries to get the pointer with the given unique Id.
            ///
            /// @author I Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param [Out] The output pointer if there was one
            /// with the given Id.
            ///
            /// @return Whether or not there was a pointer with
            /// the given Id.
            //----------------------------------------------------
            bool TryGetPointerWithId(PointerId in_uniqueId, Pointer& out_pointer) const;
            //----------------------------------------------------
            /// Tries to get the pointer with the given index.
            ///
            /// @author I Copland
            ///
            /// @return The pointer with the given unique Id.
            //----------------------------------------------------
            bool TryGetPointerWithIndex(u32 in_index, Pointer& out_pointer) const;
            //----------------------------------------------------
            /// Returns the list of currently active pointers.
            ///
            /// @author I Copland
            ///
            /// @return The pointer up event.
            //----------------------------------------------------
            std::vector<Pointer> GetPointers() const;
            //----------------------------------------------------
            /// Process all input events that have been received
            /// from the OS.
            ///
            /// @author I Copland
            //----------------------------------------------------
            void ProcessQueuedInput();
            //----------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //----------------------------------------------------
            virtual ~PointerSystem() {};
        protected:
            //----------------------------------------------------
            /// Constructor. Declared protected to force the use
            /// of the factory method.
            ///
            /// @author I Copland
            //----------------------------------------------------
            PointerSystem();
            //----------------------------------------------------
            /// Adds a new Create Pointer event. This method is
            /// thread safe and can be called on any thread.
            ///
            /// @author I Copland
            ///
            /// @param The position to create the pointer at.
            ///
            /// @return The unique Id of the new pointer.
            //----------------------------------------------------
            PointerId AddPointerCreateEvent(const Core::Vector2& in_position);
            //----------------------------------------------------
            /// Adds a new pointer down event. This method is thread
            /// safe and can be called on any thread.
            ///
            /// @author I Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param The press type.
            //----------------------------------------------------
            void AddPointerDownEvent(PointerId in_pointerUniqueId, InputType in_inputType);
            //----------------------------------------------------
            /// Adds a new pointer moved event. This method is
            /// threadsafe and can be called on any thread.
            ///
            /// @author I Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param The position it has moved to.
            //----------------------------------------------------
            void AddPointerMovedEvent(PointerId in_pointerUniqueId, const Core::Vector2& in_position);
            //----------------------------------------------------
            /// Adds a new pointer up event. This method is thread
            /// safe and can be called on any thread.
            ///
            /// @author I Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param The press type.
            //----------------------------------------------------
            void AddPointerUpEvent(PointerId in_pointerUniqueId, InputType in_inputType);
            //----------------------------------------------------
            /// Adds a new remove pointer event. This method is thread
            /// safe and can be called on any thread.
            ///
            /// @author I Copland
            ///
            /// @param The unique Id of the pointer that should be
            /// removed.
            //-----------------------------------------------------
            void AddPointerRemoveEvent(PointerId in_pointerUniqueId);
            //----------------------------------------------------
            /// Removes all existing pointers. This must be called
            /// from the main thread.
            ///
            /// @author I Copland
            //-----------------------------------------------------
            void RemoveAllPointers();
        private:
            //----------------------------------------------------
            /// An enum describing the different types of pointer
            /// event.
            ///
            /// @author I Copland
            //----------------------------------------------------
            enum class PointerEventType
            {
                k_add,
                k_down,
                k_move,
                k_up,
                k_remove
            };
            //----------------------------------------------------
            /// A container for information on a single pointer
            /// event.
            ///
            /// @author I Copland
            //----------------------------------------------------
            struct PointerEvent
            {
                PointerEventType m_type;
                PointerId m_pointerUniqueId;
                Core::Vector2 m_position;
                InputType m_InputType;
                f64 m_timestamp;
            };
            //----------------------------------------------------
            /// Creates and adds a new pointer to the list.
            ///
            /// @author I Copland
            ///
            /// @param The unique pointer Id.
            /// @param The initial position.
            //-----------------------------------------------------
            void CreatePointer(PointerId in_uniqueId, const Core::Vector2& in_initialPosition);
            //----------------------------------------------------
            /// Notifies listeners that the pointer with the given
            /// unique id is down.
            ///
            /// @author I Copland
            ///
            /// @param The unique pointer Id.
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------
            void PointerDown(PointerId in_uniqueId, f64 in_timestamp, InputType in_inputType);
            //----------------------------------------------------
            /// Updates the pointer with the given unique id and
            /// notifies listeners that it has moved.
            ///
            /// @author I Copland
            ///
            /// @param The unique pointer Id.
            /// @param The timestamp.
            /// @param The new position.
            //-----------------------------------------------------
            void PointerMoved(PointerId in_uniqueId, f64 in_timestamp, const Core::Vector2& in_newPosition);
            //----------------------------------------------------
            /// Notifies listeners that the pointer with the given
            /// unique id is up.
            ///
            /// @author I Copland
            ///
            /// @param The unique pointer Id.
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------
            void PointerUp(PointerId in_uniqueId, f64 in_timestamp, InputType in_inputType);
            //----------------------------------------------------
            /// Removes the pointer with the given Id.
            ///
            /// @author I Copland
            ///
            /// @param The unique pointer Id.
            //-----------------------------------------------------
            void RemovePointer(PointerId in_uniqueId);
            
            Core::Event<PointerDownDelegate> m_pointerDownEvent;
            Core::Event<PointerMovedDelegate> m_pointerMovedEvent;
            Core::Event<PointerUpDelegate> m_pointerUpEvent;
            std::mutex m_mutex;
            std::vector<Pointer> m_pointers;
            std::queue<PointerEvent> m_eventQueue;
            PointerId m_nextUniqueId;
        };
    }
}

#endif
