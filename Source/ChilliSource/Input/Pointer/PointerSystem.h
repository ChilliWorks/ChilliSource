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
    //------------------------------------------------------------------------------
    /// A system that provides a generic API for working with pointer devices.
    /// Pointers include touches on a touch screen and the mouse on a PC.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class PointerSystem : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(PointerSystem);
        //------------------------------------------------------------------------------
        /// A delegate that is used to receive pointer added events.
        ///
        /// On platforms which use a mouse this occurs once: when the mouse is first
        /// created. On platforms with a touch screen this is called everytime a new
        /// touch is added - immediately before the pointer down event.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointer - The pointer.
        /// @param in_timestamp - The timestamp of the event.
        //------------------------------------------------------------------------------
        using PointerAddedDelegate = std::function<void(const Pointer& in_pointer, f64 in_timestamp)>;
        //------------------------------------------------------------------------------
        /// A delegate that is used to receive pointer down events. This could be
        /// pressing a mouse button or touching a touch screen.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointer - The pointer.
        /// @param in_timestamp - The timestamp of the event.
        /// @param in_inputType - The press type.
        //------------------------------------------------------------------------------
        using PointerDownDelegate = std::function<void(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)>;
        //------------------------------------------------------------------------------
        /// A delegate that is used to receive pointer moved events. This could be
        /// dragging a touch on screen or moving the mouse cursor.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointer - The pointer
        /// @param in_timestamp - The timestamp of the event.
        //------------------------------------------------------------------------------
        using PointerMovedDelegate = std::function<void(const Pointer& in_pointer, f64 in_timestamp)>;
        //------------------------------------------------------------------------------
        /// A delegate that is used to receive pointer up events. This could be
        /// releasing a mouse button or releasing a touch from the touch screen.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointer - The pointer
        /// @param in_timestamp - The timestamp of the event.
        /// @param in_inputType - The press type.
        //------------------------------------------------------------------------------
        using PointerUpDelegate = std::function<void(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)>;
        //------------------------------------------------------------------------------
        /// A delegate that is used to receive pointer scroll events (i.e. mouse wheel).
        ///
        /// @author Scott Downie
        ///
        /// @param in_pointer - The pointer
        /// @param in_timestamp - The timestamp of the event.
        /// @param in_delta - The scroll vector (x, y delta)
        //------------------------------------------------------------------------------
        using PointerScrollDelegate = std::function<void(const Pointer& in_pointer, f64 in_timestamp, const Vector2& in_delta)>;
        //------------------------------------------------------------------------------
        /// A delegate that is used to receive pointer removal events.
        ///
        /// On platforms which use a mouse this occurs once: when the mouse is destroyed
        /// on application shutdown. On platforms with a touch screen this is called
        /// everytime an existing touch is removed - immediately after the pointer up
        /// event.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointer - The pointer.
        /// @param in_timestamp - The timestamp of the event.
        //------------------------------------------------------------------------------
        using PointerRemovedDelegate = std::function<void(const Pointer& in_pointer, f64 in_timestamp)>;
        //------------------------------------------------------------------------------
        /// An event that is called whenever a new pointer is added.
        ///
        /// On platforms which use a mouse this occurs once: when the mouse is first
        /// created. On platforms with a touch screen this is called everytime a new
        /// touch is added - immediately before the pointer down event.
        ///
        /// @author Ian Copland
        ///
        /// @return The pointer added event.
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerAddedDelegate>& GetPointerAddedEvent();
        //------------------------------------------------------------------------------
        /// Event that is triggered when the pointer is first down.
        ///
        /// @author Ian Copland
        ///
        /// @return The pointer down event.
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerDownDelegate>& GetPointerDownEvent();
        //------------------------------------------------------------------------------
        /// Event that is triggered when the pointer moves.
        ///
        /// @author Ian Copland
        ///
        /// @return The pointer moved event.
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerMovedDelegate>& GetPointerMovedEvent();
        //------------------------------------------------------------------------------
        /// Event that is triggered when the pointer is first up having been down.
        ///
        /// @author Ian Copland
        ///
        /// @return The pointer up event.
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerUpDelegate>& GetPointerUpEvent();
        //------------------------------------------------------------------------------
        /// Event that is triggered when the scroll wheel ticks.
        ///
        /// @author S Downie
        ///
        /// @return The event triggered on scroll change (i.e. mouse wheel scroll).
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerScrollDelegate>& GetPointerScrollEvent();
        //------------------------------------------------------------------------------
        /// Event that is triggered when the pointer is first down.
        ///
        /// NOTE: Filtered events are not always triggered as they can be consumed by
        /// internal systems such as UI.
        ///
        /// @author S Downie
        ///
        /// @return The pointer down event.
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerDownDelegate>& GetPointerDownEventFiltered();
        //------------------------------------------------------------------------------
        /// Event that is triggered when the pointer is first up having been down.
        ///
        /// NOTE: Filtered events are not always triggered as they can be consumed by
        /// internal systems such as UI.
        ///
        /// @author S Downie
        ///
        /// @return The pointer up event.
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerUpDelegate>& GetPointerUpEventFiltered();
        //------------------------------------------------------------------------------
        /// Event that is triggered when the scroll wheel ticks.
        ///
        /// NOTE: Filtered events are not always triggered as they can be consumed by
        /// internal systems such as UI.
        ///
        /// @author S Downie
        ///
        /// @return The event triggered on scroll change (i.e. mouse wheel scroll).
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerScrollDelegate>& GetPointerScrollEventFiltered();
        //------------------------------------------------------------------------------
        /// An event that is called whenever an existing pointer is removed.
        ///
        /// On platforms which use a mouse this occurs once: when the mouse is first
        /// created. On platforms with a touch screen this is called everytime a new
        /// touch is added - immediately before the pointer down event.
        ///
        /// @author Ian Copland
        ///
        /// @return The pointer removed event.
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerRemovedDelegate>& GetPointerRemovedEvent();
        //------------------------------------------------------------------------------
        /// Tries to get the pointer with the given unique Id.
        ///
        /// @author Ian Copland
        ///
        /// @param in_uniqueId - The unique Id of the pointer.
        /// @param out_pointer - [Out] The output pointer if there was one with the
        /// given Id.
        ///
        /// @return Whether or not there was a pointer with the given Id.
        //------------------------------------------------------------------------------
        bool TryGetPointerWithId(Pointer::Id in_uniqueId, Pointer& out_pointer) const;
        //------------------------------------------------------------------------------
        /// Tries to get the pointer with the given index.
        ///
        /// @author Ian Copland
        ///
        /// @param in_index - The index of the pointer.
        /// @param out_pointer - [Out] The output pointer if there was one with the
        /// given index.
        ///
        /// @return Whether or not there was a pointer with the given index.
        //------------------------------------------------------------------------------
        bool TryGetPointerWithIndex(u32 in_index, Pointer& out_pointer) const;
        //------------------------------------------------------------------------------
        /// Returns the list of currently active pointers.
        ///
        /// @author Ian Copland
        ///
        /// @return The pointer up event.
        //------------------------------------------------------------------------------
        std::vector<Pointer> GetPointers() const;
        //------------------------------------------------------------------------------
        /// Hide the pointer cursor if one exists
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        virtual void HideCursor() = 0;
        //------------------------------------------------------------------------------
        /// Show the pointer cursor if one exists
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        virtual void ShowCursor() = 0;
        //------------------------------------------------------------------------------
        /// Process all input events that have been received from the OS.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void ProcessQueuedInput();
        //------------------------------------------------------------------------------
        /// Destructor
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        virtual ~PointerSystem() {};
        
    protected:
        //------------------------------------------------------------------------------
        /// Constructor. Declared protected to force the use of the factory method.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        PointerSystem();
        //------------------------------------------------------------------------------
        /// Adds a new Create Pointer event.
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @author Ian Copland
        ///
        /// @param in_position - The position to create the pointer at.
        ///
        /// @return The unique Id of the new pointer.
        //------------------------------------------------------------------------------
        Pointer::Id AddPointerCreateEvent(const Vector2& in_position);
        //------------------------------------------------------------------------------
        /// Adds a new pointer down event.
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointerUniqueId - The unique Id of the pointer.
        /// @param in_inputType - The input type.
        //------------------------------------------------------------------------------
        void AddPointerDownEvent(Pointer::Id in_pointerUniqueId, Pointer::InputType in_inputType);
        //------------------------------------------------------------------------------
        /// Adds a new pointer moved event.
        ///
        /// This method is threadsafe and can be called on any thread.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointerUniqueId - The unique Id of the pointer.
        /// @param in_position - The position it has moved to.
        //------------------------------------------------------------------------------
        void AddPointerMovedEvent(Pointer::Id in_pointerUniqueId, const Vector2& in_position);
        //------------------------------------------------------------------------------
        /// Adds a new pointer up event.
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointerUniqueId - The unique Id of the pointer.
        /// @param in_inputType - The input type.
        //------------------------------------------------------------------------------
        void AddPointerUpEvent(Pointer::Id in_pointerUniqueId, Pointer::InputType in_inputType);
        //------------------------------------------------------------------------------
        /// Adds a new pointer scroll event.
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointerUniqueId - The unique Id of the pointer.
        /// @param in_delta - The scroll delta in the x-y plane
        //------------------------------------------------------------------------------
        void AddPointerScrollEvent(Pointer::Id in_pointerUniqueId, const Vector2& in_delta);
        //------------------------------------------------------------------------------
        /// Adds a new remove pointer event.
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @author Ian Copland
        ///
        /// @param in_pointerUniqueId - The unique Id of the pointer that should be
        /// removed.
        //------------------------------------------------------------------------------
        void AddPointerRemoveEvent(Pointer::Id in_pointerUniqueId);
        //------------------------------------------------------------------------------
        /// Removes all existing pointers. This must be called from the main thread.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        void RemoveAllPointers();
        
    private:
        friend class Application;
        friend class GestureSystem;
        friend class Canvas;
        //------------------------------------------------------------------------------
        /// Creates a new platform specific instance of pointer system.
        ///
        /// @author Ian Copland
        ///
        /// @return The new system instance.
        //------------------------------------------------------------------------------
        static PointerSystemUPtr Create();
        //------------------------------------------------------------------------------
        /// An enum describing the different types of pointer event.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        enum class PointerEventType
        {
            k_add,
            k_down,
            k_move,
            k_up,
            k_scroll,
            k_remove
        };
        //------------------------------------------------------------------------------
        /// A container for information on a single pointer event.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        struct PointerEvent
        {
            PointerEventType m_type;
            Pointer::Id m_pointerUniqueId;
            Vector2 m_position;
            Pointer::InputType m_InputType;
            f64 m_timestamp;
        };
        //------------------------------------------------------------------------------
        /// Creates and adds a new pointer to the list.
        ///
        /// @author Ian Copland
        ///
        /// @param in_uniqueId - The unique pointer Id.
        /// @param in_timestamp - The timestamp of the creation event.
        /// @param in_initialPosition - The initial position.
        //------------------------------------------------------------------------------
        void CreatePointer(Pointer::Id in_uniqueId, f64 in_timestamp, const Vector2& in_initialPosition);
        //------------------------------------------------------------------------------
        /// Notifies listeners that the pointer with the given unique id is down.
        ///
        /// @author Ian Copland
        ///
        /// @param in_uniqueId - The unique pointer Id.
        /// @param in_timestamp - The timestamp.
        /// @param in_inputType - The press type.
        //------------------------------------------------------------------------------
        void PointerDown(Pointer::Id in_uniqueId, f64 in_timestamp, Pointer::InputType in_inputType);
        //------------------------------------------------------------------------------
        /// Updates the pointer with the given unique id and notifies listeners that it
        /// has moved.
        ///
        /// @author Ian Copland
        ///
        /// @param in_uniqueId - The unique pointer Id.
        /// @param in_timestamp - The timestamp.
        /// @param in_newPosition - The new position.
        //------------------------------------------------------------------------------
        void PointerMoved(Pointer::Id in_uniqueId, f64 in_timestamp, const Vector2& in_newPosition);
        //------------------------------------------------------------------------------
        /// Notifies listeners that the pointer with the given unique id is up.
        ///
        /// @author Ian Copland
        ///
        /// @param in_uniqueId - The unique pointer Id.
        /// @param in_timestamp - The timestamp.
        /// @param in_inputType - The press type.
        //------------------------------------------------------------------------------
        void PointerUp(Pointer::Id in_uniqueId, f64 in_timestamp, Pointer::InputType in_inputType);
        //------------------------------------------------------------------------------
        /// Updates the pointer with the given unique id and notifies listeners that it
        /// has scrolled.
        ///
        /// @author S Downie
        ///
        /// @param in_uniqueId - The unique pointer Id.
        /// @param in_timestamp - The timestamp.
        /// @param in_delta - The scroll delta.
        //------------------------------------------------------------------------------
        void PointerScrolled(Pointer::Id in_uniqueId, f64 in_timestamp, const Vector2& in_delta);
        //------------------------------------------------------------------------------
        /// Removes the pointer with the given Id.
        ///
        /// @author Ian Copland
        ///
        /// @param in_uniqueId - The unique pointer Id.
        /// @param in_timestamp - The timestamp.
        //------------------------------------------------------------------------------
        void RemovePointer(Pointer::Id in_uniqueId, f64 in_timestamp);
        
        //---These are events used internally by the engine
        //------------------------------------------------------------------------------
        /// A delegate that is used to receive pointer down events. This could be
        /// pressing a mouse button or touching a touch screen.
        ///
        /// Systems listening to this should check whether the event has already been
        /// filtered prior to performing any action.
        ///
        /// Additionally, systems using this should also check whether they received
        /// this event, unfiltered, prior to performing any action from a pointer moved
        /// or pointer up event.
        ///
        /// @author S Downie
        ///
        /// @param in_pointer - The pointer.
        /// @param in_timestamp - The timestamp of the event.
        /// @param in_inputType - The input type.
        /// @param in_filter - The filter.
        //------------------------------------------------------------------------------
        using PointerDownDelegateInternal = std::function<void(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, InputFilter& in_filter)>;
        //------------------------------------------------------------------------------
        /// A delegate that is used to receive pointer scroll events (i.e. mouse wheel).
        ///
        /// @author S Downie
        ///
        /// @param in_pointer - The pointer
        /// @param in_timestamp - The timestamp of the event.
        /// @param in_delta - The scroll vector (x, y delta)
        /// @param in_filter - Filter
        //------------------------------------------------------------------------------
        using PointerScrollDelegateInternal = std::function<void(const Pointer& in_pointer, f64 in_timestamp, const Vector2& in_delta, InputFilter& in_filter)>;
        //------------------------------------------------------------------------------
        /// Event that is triggered when the pointer is first down.
        ///
        /// @author S Downie
        ///
        /// @return The pointer down event.
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerDownDelegateInternal>& GetPointerDownEventInternal();
        //------------------------------------------------------------------------------
        /// Event that is triggered when the scroll wheel ticks.
        ///
        /// @author S Downie
        ///
        /// @return The event triggered on scroll change (i.e. mouse wheel scroll).
        //------------------------------------------------------------------------------
        IConnectableEvent<PointerScrollDelegateInternal>& GetPointerScrollEventInternal();
        
        Event<PointerAddedDelegate> m_pointerAddedEvent;
        Event<PointerDownDelegate> m_pointerDownEvent;
        Event<PointerMovedDelegate> m_pointerMovedEvent;
        Event<PointerUpDelegate> m_pointerUpEvent;
        Event<PointerScrollDelegate> m_pointerScrolledEvent;
        Event<PointerRemovedDelegate> m_pointerRemovedEvent;
        
        Event<PointerDownDelegate> m_pointerDownEventFiltered;
        Event<PointerUpDelegate> m_pointerUpEventFiltered;
        Event<PointerScrollDelegate> m_pointerScrolledEventFiltered;
        
        Event<PointerDownDelegateInternal> m_pointerDownEventInternal;
        Event<PointerScrollDelegateInternal> m_pointerScrolledEventInternal;
        
        std::mutex m_mutex;
        std::vector<Pointer> m_pointers;
        std::queue<PointerEvent> m_eventQueue;
        Pointer::Id m_nextUniqueId;
        
        std::unordered_map<Pointer::Id, std::set<Pointer::InputType>> m_filteredPointerInput;
    };
}

#endif
