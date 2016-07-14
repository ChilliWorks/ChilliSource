//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_BASE_LIFECYCLEMANAGER_H_
#define _CHILLISOURCE_CORE_BASE_LIFECYCLEMANAGER_H_

#include <ChilliSource/ChilliSource.h>

#include <atomic>
#include <condition_variable>
#include <thread>

namespace ChilliSource
{
    /// Manages the main thread lifecycle events. The events are retreived from
    /// the system thread and then queued for processing on the main thread.
    ///
    /// This is thread-safe.
    ///
    class LifecycleManager final
    {
    public:
        CS_DECLARE_NOCOPY(LifecycleManager);
        
        /// Creates a new lifecycle manager instance with the given Application.
        /// The initialise event will be sent to the main thread.
        ///
        /// @param application
        ///     The application instance which will receive lifecycle events.
        ///
        LifecycleManager(Application* application) noexcept;
        
        /// Sends the resume event to the main thread. The app must currently be
        /// suspended or this will assert.
        ///
        /// This is thread safe.
        ///
        void Resume() noexcept;
        
        /// Sends the foreground event to the main thread. The app must currently
        /// be backgrounded or this will assert.
        ///
        /// This is thread safe.
        ///
        void Foreground() noexcept;
        
        /// Sends the background event to the main thread. The app must currently
        /// be foregrounded or this will assert.
        ///
        /// This is thread safe.
        ///
        void Background() noexcept;
        
        /// Sends the background event to the main thread. The app must currently
        /// be foregrounded or this will assert.
        ///
        /// This is thread safe.
        ///
        void Suspend() noexcept;
        
        /// Sends the render event to the application, rendering to the screen.
        ///
        /// This is not thread safe, and must be called from the render thread.
        ///
        void Render() noexcept;
        
        /// Executes scheduled system thread tasks.
        ///
        /// This is not thread safe, and must be called from the system thread.
        ///
        void SystemUpdate() noexcept;
        
        /// Tells the resource manager to release all and then sends the memory warning
        /// event to the application.
        ///
        void MemoryWarning() noexcept;
        
        ~LifecycleManager() noexcept;
        
    private:
        /// Describes the lifecycle states of the application.
        ///
        enum class LifecycleState
        {
            k_uninitialised,
            k_initialised,
            k_resumed,
            k_foregrounded
        };
        
        /// Evaluates whether or not an init event should occur by comparing the
        /// current and target lifecycle states.
        ///
        /// @param currentLifecycleState
        ///     The current lifecycle state.
        /// @param targetLifecycleState
        ///     The target lifecycle state.
        ///
        /// @return Whether or not an init event should occur.
        ///
        static constexpr bool ShouldInit(LifecycleState currentLifecycleState, LifecycleState targetLifecycleState) noexcept;
        
        /// Evaluates whether or not a resume event should occur by comparing the
        /// current and target lifecycle states.
        ///
        /// @param currentLifecycleState
        ///     The current lifecycle state.
        /// @param targetLifecycleState
        ///     The target lifecycle state.
        ///
        /// @return Whether or not a resume event should occur.
        ///
        static constexpr bool ShouldResume(LifecycleState currentLifecycleState, LifecycleState targetLifecycleState) noexcept;
        
        /// Evaluates whether or not a foreground event should occur by comparing the
        /// current and target lifecycle states.
        ///
        /// @param currentLifecycleState
        ///     The current lifecycle state.
        /// @param targetLifecycleState
        ///     The target lifecycle state.
        ///
        /// @return Whether or not a foreground event should occur.
        ///
        static constexpr bool ShouldForeground(LifecycleState currentLifecycleState, LifecycleState targetLifecycleState) noexcept;
        
        /// Evaluates whether or not an update event should occur by checking the
        /// target lifecycle is either active or foregrounded.
        ///
        /// @param currentLifecycleState
        ///     The current lifecycle state.
        /// @param targetLifecycleState
        ///     The target lifecycle state.
        ///
        /// @return Whether or not an update event should occur.
        ///
        static constexpr bool ShouldUpdate(LifecycleState targetLifecycleState) noexcept;
        
        /// Evaluates whether or not a background event should occur by comparing the
        /// current and target lifecycle states.
        ///
        /// @param currentLifecycleState
        ///     The current lifecycle state.
        /// @param targetLifecycleState
        ///     The target lifecycle state.
        ///
        /// @return Whether or not a background event should occur.
        ///
        static constexpr bool ShouldBackground(LifecycleState currentLifecycleState, LifecycleState targetLifecycleState) noexcept;
        
        /// Evaluates whether or not a suspend event should occur by comparing the
        /// current and target lifecycle states.
        ///
        /// @param currentLifecycleState
        ///     The current lifecycle state.
        /// @param targetLifecycleState
        ///     The target lifecycle state.
        ///
        /// @return Whether or not a suspend event should occur.
        ///
        static constexpr bool ShouldSuspend(LifecycleState currentLifecycleState, LifecycleState targetLifecycleState) noexcept;
        
        /// Evaluates whether or not a destroy event should occur by comparing the
        /// current and target lifecycle states.
        ///
        /// @param currentLifecycleState
        ///     The current lifecycle state.
        /// @param targetLifecycleState
        ///     The target lifecycle state.
        ///
        /// @return Whether or not a destroy event should occur.
        ///
        static constexpr bool ShouldDestroy(LifecycleState currentLifecycleState, LifecycleState targetLifecycleState) noexcept;
        
        /// Processes the main thread lifecycle events which have been received. If
        /// the app is currently resumed or foregrounded, then update will be called,
        /// otherwise this will block until a new event occurs.
        ///
        void ProcessMainThread() noexcept;
        
        /// Blocks the application until a new lifecycle event occurs if the application
        /// is not currently resumed or foregrounded.
        ///
        void BlockIfInactive() noexcept;
        
        /// Called when the raw platform resume event is triggered. This is guaranteed
        /// to happen before the regular OnResume event.
        ///
        void SystemResume() noexcept;
        
        /// Called when the raw platform suspend event is triggered. This is guaranteed
        /// to happen before the regular OnSuspend event.
        ///
        void SystemSuspend() noexcept;
        
        Application* m_application;
        
        LifecycleState m_currentLifecycleState = LifecycleState::k_uninitialised;
        std::atomic<LifecycleState> m_targetLifecycleState;
        
        std::chrono::time_point<std::chrono::system_clock> m_initTime;
        std::chrono::time_point<std::chrono::system_clock> m_lastUpdateTime;
        
        std::thread m_mainThread;
        std::mutex m_mainThreadMutex;
        std::condition_variable m_activeCondition;
        
        std::mutex m_initialisedMutex;
        bool m_initialised = false;
        std::condition_variable m_initialisedCondition;
    };
}

#endif
