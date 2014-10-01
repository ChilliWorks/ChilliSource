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
#include <ChilliSource/Input/Pointer/Pointer.h>

#include <mutex>
#include <queue>
#include <set>
#include <unordered_map>

namespace ChilliSource
{
    namespace Input
    {
        //--------------------------------------------------------
        /// A system that provides a generic API for working with
        /// pointer devices. Pointers include touches on a touch
        /// screen and the mouse on a PC.
        ///
        /// @author Ian Copland
        //--------------------------------------------------------
        class PointerSystem : public Core::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(PointerSystem);

            //----------------------------------------------------
            /// A delegate that is used to receive pointer down
            /// events. This could be pressing a mouse button or
            /// touching a touch screen.
            ///
            /// @param The pointer.
            /// @param The timestamp of the event.
            /// @param The press type.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            using PointerDownDelegate = std::function<void(const Pointer&, f64, Pointer::InputType)>;
            //----------------------------------------------------
            /// A delegate that is used to receive pointer moved
            /// events. This could be dragging a touch on screen
            /// or moving the mouse cursor.
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            ///
            /// @author Ian Copland
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
            /// @author Ian Copland
            //----------------------------------------------------
            using PointerUpDelegate = std::function<void(const Pointer&, f64, Pointer::InputType)>;
            //----------------------------------------------------
            /// A delegate that is used to receive pointer scroll events
            /// (i.e. mouse wheel).
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            /// @param The scroll vector (x, y delta)
            ///
            /// @author Scott Downie
            //----------------------------------------------------
            using PointerScrollDelegate = std::function<void(const Pointer&, f64, const Core::Vector2&)>;
            //----------------------------------------------------
            /// Creates a new platfrom specific instance of pointer
            /// system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new system instance.
            //----------------------------------------------------
            static PointerSystemUPtr Create();
            //----------------------------------------------------
            /// Event that is triggered when the pointer is first down.
            ///
            /// @author Ian Copland
            ///
            /// @return The pointer down event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerDownDelegate>& GetPointerDownEvent();
            //----------------------------------------------------
            /// Event that is triggered when the pointer moves.
            ///
            /// @author Ian Copland
            ///
            /// @return The pointer moved event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerMovedDelegate>& GetPointerMovedEvent();
            //----------------------------------------------------
            /// Event that is triggered when the pointer is first up
            /// having been down.
            ///
            /// @author Ian Copland
            ///
            /// @return The pointer up event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerUpDelegate>& GetPointerUpEvent();
            //----------------------------------------------------
            /// Event that is triggered when the scroll wheel ticks.
            ///
            /// @author S Downie
            ///
            /// @return The event triggered on scroll change (i.e. mouse wheel scroll).
            //----------------------------------------------------
            Core::IConnectableEvent<PointerScrollDelegate>& GetPointerScrollEvent();
            //----------------------------------------------------
            /// Event that is triggered when the pointer is first down.
            /// NOTE: Filtered events are not always triggered as they
            /// can be consumed by internal systems such as UI.
            ///
            /// @author S Downie
            ///
            /// @return The pointer down event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerDownDelegate>& GetPointerDownEventFiltered();
            //----------------------------------------------------
            /// Event that is triggered when the pointer is first up
            /// having been down.
            /// NOTE: Filtered events are not always triggered as they
            /// can be consumed by internal systems such as UI.
            ///
            /// @author S Downie
            ///
            /// @return The pointer up event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerUpDelegate>& GetPointerUpEventFiltered();
            //----------------------------------------------------
            /// Event that is triggered when the scroll wheel ticks.
            /// NOTE: Filtered events are not always triggered as they
            /// can be consumed by internal systems such as UI.
            ///
            /// @author S Downie
            ///
            /// @return The event triggered on scroll change (i.e.
            /// mouse wheel scroll).
            //----------------------------------------------------
            Core::IConnectableEvent<PointerScrollDelegate>& GetPointerScrollEventFiltered();
            //----------------------------------------------------
            /// Tries to get the pointer with the given unique Id.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param [Out] The output pointer if there was one
            /// with the given Id.
            ///
            /// @return Whether or not there was a pointer with
            /// the given Id.
            //----------------------------------------------------
            bool TryGetPointerWithId(Pointer::Id in_uniqueId, Pointer& out_pointer) const;
            //----------------------------------------------------
            /// Tries to get the pointer with the given index.
            ///
            /// @author Ian Copland
            ///
            /// @return The pointer with the given unique Id.
            //----------------------------------------------------
            bool TryGetPointerWithIndex(u32 in_index, Pointer& out_pointer) const;
            //----------------------------------------------------
            /// Returns the list of currently active pointers.
            ///
            /// @author Ian Copland
            ///
            /// @return The pointer up event.
            //----------------------------------------------------
            std::vector<Pointer> GetPointers() const;
            //----------------------------------------------------
            /// Hide the pointer cursor if one exists
            ///
            /// @author S Downie
            //----------------------------------------------------
            virtual void HideCursor() = 0;
            //----------------------------------------------------
            /// Show the pointer cursor if one exists
            ///
            /// @author S Downie
            //----------------------------------------------------
            virtual void ShowCursor() = 0;
            //----------------------------------------------------
            /// Process all input events that have been received
            /// from the OS.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            void ProcessQueuedInput();
            //----------------------------------------------------
            /// Destructor
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            virtual ~PointerSystem() {};
            
        protected:
            //----------------------------------------------------
            /// Constructor. Declared protected to force the use
            /// of the factory method.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            PointerSystem();
            //----------------------------------------------------
            /// Adds a new Create Pointer event. This method is
            /// thread safe and can be called on any thread.
            ///
            /// @author Ian Copland
            ///
            /// @param The position to create the pointer at.
            ///
            /// @return The unique Id of the new pointer.
            //----------------------------------------------------
            Pointer::Id AddPointerCreateEvent(const Core::Vector2& in_position);
            //----------------------------------------------------
            /// Adds a new pointer down event. This method is thread
            /// safe and can be called on any thread.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param The press type.
            //----------------------------------------------------
            void AddPointerDownEvent(Pointer::Id in_pointerUniqueId, Pointer::InputType in_inputType);
            //----------------------------------------------------
            /// Adds a new pointer moved event. This method is
            /// threadsafe and can be called on any thread.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param The position it has moved to.
            //----------------------------------------------------
            void AddPointerMovedEvent(Pointer::Id in_pointerUniqueId, const Core::Vector2& in_position);
            //----------------------------------------------------
            /// Adds a new pointer up event. This method is thread
            /// safe and can be called on any thread.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param The press type.
            //----------------------------------------------------
            void AddPointerUpEvent(Pointer::Id in_pointerUniqueId, Pointer::InputType in_inputType);
            //----------------------------------------------------
            /// Adds a new pointer scroll event. This method is thread
            /// safe and can be called on any thread.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique Id of the pointer.
            /// @param The scroll delta in the x-y plane
            //----------------------------------------------------
            void AddPointerScrollEvent(Pointer::Id in_pointerUniqueId, const Core::Vector2& in_delta);
            //----------------------------------------------------
            /// Adds a new remove pointer event. This method is thread
            /// safe and can be called on any thread.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique Id of the pointer that should be
            /// removed.
            //-----------------------------------------------------
            void AddPointerRemoveEvent(Pointer::Id in_pointerUniqueId);
            //----------------------------------------------------
            /// Removes all existing pointers. This must be called
            /// from the main thread.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------
            void RemoveAllPointers();
        private:
            friend class GestureSystem;
            //----------------------------------------------------
            /// An enum describing the different types of pointer
            /// event.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            enum class PointerEventType
            {
                k_add,
                k_down,
                k_move,
                k_up,
                k_scroll,
                k_remove
            };
            //----------------------------------------------------
            /// A container for information on a single pointer
            /// event.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            struct PointerEvent
            {
                PointerEventType m_type;
                Pointer::Id m_pointerUniqueId;
                Core::Vector2 m_position;
                Pointer::InputType m_InputType;
                f64 m_timestamp;
            };
            //----------------------------------------------------
            /// Creates and adds a new pointer to the list.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique pointer Id.
            /// @param The initial position.
            //-----------------------------------------------------
            void CreatePointer(Pointer::Id in_uniqueId, const Core::Vector2& in_initialPosition);
            //----------------------------------------------------
            /// Notifies listeners that the pointer with the given
            /// unique id is down.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique pointer Id.
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------
            void PointerDown(Pointer::Id in_uniqueId, f64 in_timestamp, Pointer::InputType in_inputType);
            //----------------------------------------------------
            /// Updates the pointer with the given unique id and
            /// notifies listeners that it has moved.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique pointer Id.
            /// @param The timestamp.
            /// @param The new position.
            //-----------------------------------------------------
            void PointerMoved(Pointer::Id in_uniqueId, f64 in_timestamp, const Core::Vector2& in_newPosition);
            //----------------------------------------------------
            /// Notifies listeners that the pointer with the given
            /// unique id is up.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique pointer Id.
            /// @param The timestamp.
            /// @param The press type.
            //-----------------------------------------------------
            void PointerUp(Pointer::Id in_uniqueId, f64 in_timestamp, Pointer::InputType in_inputType);
            //----------------------------------------------------
            /// Updates the pointer with the given unique id and
            /// notifies listeners that it has scrolled.
            ///
            /// @author S Downie
            ///
            /// @param The unique pointer Id.
            /// @param The timestamp.
            /// @param The scroll delta.
            //-----------------------------------------------------
            void PointerScrolled(Pointer::Id in_uniqueId, f64 in_timestamp, const Core::Vector2& in_delta);
            //----------------------------------------------------
            /// Removes the pointer with the given Id.
            ///
            /// @author Ian Copland
            ///
            /// @param The unique pointer Id.
            //-----------------------------------------------------
            void RemovePointer(Pointer::Id in_uniqueId);
            //---These are events used internally by the engine
            friend class UI::Canvas;
            //----------------------------------------------------
            /// A delegate that is used to receive pointer down
            /// events. This could be pressing a mouse button or
            /// touching a touch screen.
            ///
            /// Systems listening to this should check whether
            /// the event has already been filtered prior to
            /// performing any action.
            ///
            /// Additionally, systems using this should also check
            /// whether they received this event, unfiltered, prior
            /// to performing any action from a pointer moved or
            /// pointer up event.
            ///
            /// @param The pointer.
            /// @param The timestamp of the event.
            /// @param The press type.
            /// @param Filter
            ///
            /// @author S Downie
            //----------------------------------------------------
            using PointerDownDelegateInternal = std::function<void(const Pointer&, f64, Pointer::InputType, Filter&)>;
            //----------------------------------------------------
            /// A delegate that is used to receive pointer scroll
            /// events (i.e. mouse wheel).
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            /// @param The scroll vector (x, y delta)
            /// @param Filter
            ///
            /// @author S Downie
            //----------------------------------------------------
            using PointerScrollDelegateInternal = std::function<void(const Pointer&, f64, const Core::Vector2&, Filter&)>;
            //----------------------------------------------------
            /// Event that is triggered when the pointer is first down.
            ///
            /// @author S Downie
            ///
            /// @return The pointer down event.
            //----------------------------------------------------
            Core::IConnectableEvent<PointerDownDelegateInternal>& GetPointerDownEventInternal();
            //----------------------------------------------------
            /// Event that is triggered when the scroll wheel ticks.
            ///
            /// @author S Downie
            ///
            /// @return The event triggered on scroll change (i.e.
            /// mouse wheel scroll).
            //----------------------------------------------------
            Core::IConnectableEvent<PointerScrollDelegateInternal>& GetPointerScrollEventInternal();
            
            Core::Event<PointerDownDelegate> m_pointerDownEvent;
            Core::Event<PointerMovedDelegate> m_pointerMovedEvent;
            Core::Event<PointerUpDelegate> m_pointerUpEvent;
            Core::Event<PointerScrollDelegate> m_pointerScrolledEvent;
            
            Core::Event<PointerDownDelegate> m_pointerDownEventFiltered;
            Core::Event<PointerUpDelegate> m_pointerUpEventFiltered;
            Core::Event<PointerScrollDelegate> m_pointerScrolledEventFiltered;
            
            Core::Event<PointerDownDelegateInternal> m_pointerDownEventInternal;
            Core::Event<PointerScrollDelegateInternal> m_pointerScrolledEventInternal;
            
            std::mutex m_mutex;
            std::vector<Pointer> m_pointers;
            std::queue<PointerEvent> m_eventQueue;
            Pointer::Id m_nextUniqueId;
            
            std::unordered_map<Pointer::Id, std::set<Pointer::InputType>> m_filteredPointerInput;
        };
    }
}

#endif
