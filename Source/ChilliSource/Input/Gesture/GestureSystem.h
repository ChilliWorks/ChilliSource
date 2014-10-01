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

#ifndef _CHILLISOURCE_INPUT_GESTURE_GESTURESYSTEM_H_
#define _CHILLISOURCE_INPUT_GESTURE_GESTURESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/concurrent_vector.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/System/StateSystem.h>
#include <ChilliSource/Input/Pointer/Pointer.h>

#include <functional>
#include <mutex>

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
            /// An enum describing the possible results from a gesture
            /// conflict.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            enum class ConflictResult
            {
                k_neitherGesture,
                k_existingGesture,
                k_newGesture,
                k_bothGestures
            };
            //--------------------------------------------------------
            /// A delegate which will be called when the activation of
            /// a gesture conflicts with another. The delegate should
            /// return a Conflict Result decribing which gesture
            /// should continue/activate.
            ///
            /// @author Ian Copland
            ///
            /// @param The existing gesture that is already active.
            /// @param The new gesture trying to activate.
            ///
            /// @return The result of the collision. The value returned
            /// should reflect which gestures will be allowed to start
            /// or continue.
            //---------------------------------------------------------
            using ConflictResolutionDelegate = std::function<ConflictResult(const Gesture*, const Gesture*)>;
            //--------------------------------------------------------
            /// Queries whether or not this implements the interface
            /// with the given Id. This is threadsafe.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the inteface is implemented.
            //---------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------
            /// Adds a gesture to the gesture system. When added to the
            /// system a gesture will receive input events. This is
            /// threadsafe.
            ///
            /// @author Ian Copland
            ///
            /// @param The gesture to add to the system.
            //---------------------------------------------------------
			void AddGesture(const GestureSPtr& in_gesture);
            //--------------------------------------------------------
            /// Removes a gesture from the system. When removed from
            /// the system a gesture will no longer recieve input
            /// events. This is threadsafe.
            ///
            /// @author Ian Copland
            ///
            /// @param The gesture which should be removed from the
            /// system.
            //---------------------------------------------------------
			void RemoveGesture(const Gesture* in_gesture);
            //--------------------------------------------------------
            /// Sets a delegate that should be used to handle gesture
            /// conflict resolution. If two gestures are active at
            /// the same time the delegate will be called. The conflict
            /// result enum describes what the result of the conflict
            /// should be. If no delegate is set both will always be
            /// allowed. This is threadsafe.
            ///
            /// @author Ian Copland
            ///
            /// @param The delegate. Can be null.
            //---------------------------------------------------------
			void SetConflictResolutionDelegate(const ConflictResolutionDelegate& in_delegate);
        private:

            friend class Core::State;
            friend class Gesture;
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
            /// Default constructor. Declared private to force the use
            /// of the factory method.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            GestureSystem() = default;
            //--------------------------------------------------------
            /// Process all deferred additions and removals to and
            /// from the gesture list.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void ProcessDeferredAddAndRemovals();
            //--------------------------------------------------------
            /// Performs conflict resolution on the gestures and
            /// returns whether or not the given gesture can be activated.
            /// Any gestures that are currently active which should end
            /// on conflict with this gesture will have their Cancel()
            /// method called.
            ///
            /// @author Ian Copland
            ///
            /// @param The gesture to perform conflict detection on.
            ///
            /// @param Whether or not the gesture can activate.
            //--------------------------------------------------------
            bool ResolveConflicts(Gesture* in_gesture);
            //--------------------------------------------------------
            /// Called when the owning state is initialised. This
            /// registers for the input events.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void OnInit() override;
            //--------------------------------------------------------
            /// Called when the owning state is updated. This passes
            /// the event onto all registered gestures.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time since the last update.
            //--------------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //--------------------------------------------------------
            /// Called when a pointer down event is received from the
            /// pointer system. This will be relayed onto each
            /// active gesture in this gesture system.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer.
            /// @param The timestamp of the event.
            /// @param The press type.
            /// @param The filter, allowing exclusion from the filtered
            /// input event.
            //--------------------------------------------------------
            void OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter);
            //--------------------------------------------------------
            /// Called when a pointer moved event is received from the
            /// pointer system. This will be relayed onto each active
            /// gesture in this gesture system.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            //--------------------------------------------------------
            void OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp);
            //--------------------------------------------------------
            /// Called when a pointer up event is received from the
            /// pointer system. This will be relayed onto each active
            /// gesture in this system.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            /// @param The press type.
            //--------------------------------------------------------
            void OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType);
            //--------------------------------------------------------
            /// Called when a pointer scrolled event is received from
            /// the pointer system. This will be relayed onto each
            /// active gesture in this system.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer
            /// @param The timestamp of the event.
            /// @param The scroll vector (x, y delta)
            /// @param The filter, allowing exclusion from the filtered
            /// input event.
            //--------------------------------------------------------
            void OnPointerScrolled(const Pointer& in_pointer, f64 in_timestamp, const Core::Vector2& in_delta, Filter& in_filter);
            //-------------------------------------------------------
            /// A proxy method for calling GestureSystem::OnPointerDown()
            /// using the old GUI consumption model. This simply relays
            /// on the information to the new method with a unfiltered
            /// filter.
            ///
            /// @author Ian Copland
            ///
            /// @param The pointer.
            /// @param The timestamp of the event.
            /// @param The press type.
            //--------------------------------------------------------
            void OnPointerDownLegacy(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType);
            //--------------------------------------------------------
            /// Called when the owning state is destroyed. This
            /// -de-registers all of the input events.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void OnDestroy() override;
            
            std::recursive_mutex m_mutex;
            Core::concurrent_vector<GestureSPtr> m_gestures;
            
            ConflictResolutionDelegate m_conflictResolutionDelegate;

            Core::EventConnectionUPtr m_pointerDownConnection;
            Core::EventConnectionUPtr m_pointerMovedConnection;
            Core::EventConnectionUPtr m_pointerUpConnection;
            Core::EventConnectionUPtr m_pointerScrolledConnection;
        };
    }
}

#endif
