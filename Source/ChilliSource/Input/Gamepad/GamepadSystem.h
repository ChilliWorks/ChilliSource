//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifndef _CHILLISOURCE_INPUT_GAMEPAD_GAMEPADSYSTEM_H_
#define _CHILLISOURCE_INPUT_GAMEPAD_GAMEPADSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Input/Gamepad/Gamepad.h>

#include <mutex>
#include <queue>

namespace ChilliSource
{
    /// Provides access to any attached gamepads and has events to listen for changes to gamepad states
    ///
    /// Each platform has its own limitations in terms of number of buttons and axes
    ///
    class GamepadSystem : public AppSystem
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(GamepadSystem);

        /// Delegate signature to subscribe to events for whenever the OS detects that a new gamepad has been attached
        ///
        /// @param gamepad
        ///     The newly attached gamepad
        /// @param timestamp
        ///     Time at which it was attached
        ///
        using GamepadAddedDelegate = std::function<void(const Gamepad& gamepad, f64 timestamp)>;
        
        /// Delegate signature to subscribe to events for whenever the user presses or releases a button on the gamepad
        ///
        /// @param gamepad
        ///     The gamepad thats input changed
        /// @param timestamp
        ///     Time at which input changed
        /// @param buttonIndex
        ///     Index of button thats pressure changed
        /// @param pressure
        ///     Updated pressure on the button
        ///
        using GamepadButtonPressureChangedDelegate = std::function<void(const Gamepad& gamepad, f64 timestamp, u32 buttonIndex, f32 pressure)>;
        
        /// Delegate signature to subscribe to events for whenever the user moves an axis on the gamepad
        ///
        /// @param gamepad
        ///     The gamepad thats input changed
        /// @param timestamp
        ///     Time at which input changed
        /// @param axis
        ///     Axis thats position changed
        /// @param position
        ///     Updated position of the axis
        ///
        using GamepadAxisPositionChangedDelegate = std::function<void(const Gamepad& gamepad, f64 timestamp, GamepadAxis axis, f32 position)>;
        
        /// Delegate signature to subscribe to events for whenever the user presses or releases a button on the gamepad for a registered mapping.
        ///
        /// @param gamepad
        ///     The gamepad thats input changed
        /// @param timestamp
        ///     Time at which input changed
        /// @param actionId
        ///     Id used when mapping was registered
        /// @param pressure
        ///     Updated pressure on the button
        ///
        using GamepadMappedButtonPressureChangedDelegate = std::function<void(const Gamepad& gamepad, f64 timestamp, u32 actionId, f32 pressure)>;
        
        /// Delegate signature to subscribe to events for whenever the user moves an axis on the gamepad for a registered mapping.
        ///
        /// @param gamepad
        ///     The gamepad thats input changed
        /// @param timestamp
        ///     Time at which input changed
        /// @param actionId
        ///     Id used when mapping was registered
        /// @param position
        ///     Updated position of the axis
        ///
        using GamepadMappedAxisPositionChangedDelegate = std::function<void(const Gamepad& gamepad, f64 timestamp, u32 actionId, f32 position)>;

        /// Delegate signature to subscribe to events for whenever the OS detects that a new gamepad has been unattached
        ///
        /// @param gamepad
        ///     The newly disconnected gamepad
        /// @param timestamp
        ///     Time at which it was disconnected
        ///
        using GamepadRemovedDelegate = std::function<void(const Gamepad& gamepad, f64 timestamp)>;
        
        /// Registers/updates a mapping such that whenever the given axis on the given gamepad is moved
        /// an event is fired with the given actionId.
        ///
        /// If mapping for gamepad/axis combo doesn't exist it is created
        /// If mapping for gamepad/axis combo does exist it is overwritten
        ///
        /// For example:    MoveVertically => XBox360 => GamepadMapping::XBox360::k_dpadY
        ///                 MoveVertically => XBox360 => GamepadMapping::XBox360::k_lStickY
        ///                 MoveVertically => PS3 => GamepadMapping::PS3::k_dpadY
        ///                 MoveVertically => PS3 => GamepadMapping::PS3::k_lStickY
        ///
        /// @param actionId
        ///     Identifier that is passed into the axis moved event
        /// @param gamepadName
        ///     Name of the gamepad that this mapping applies to
        /// @param axis
        ///     Axis on the gamepad that triggers this action
        /// @param invert
        ///     If TRUE flip the value returned by the axis (some controllers report +ve y as -1.0 some as 1.0
        ///
        void SetActionMapping(u32 actionId, const std::string& gamepadName, GamepadAxis axis, bool invert = false) noexcept;
        
        /// Registers/updates a mapping such that whenever the given button on the given gamepad is pressed or released
        /// an event is fired with the given actionId.
        ///
        /// If mapping for gamepad/button combo doesn't exist it is created
        /// If mapping for gamepad/button combo does exist it is overwritten
        ///
        /// For example:    Fire => XBox360 => GamepadMapping::XBox360::k_rBumper
        ///                 Fire => PS3 => GamepadMapping::PS3::k_r2
        ///
        /// @param actionId
        ///     Identifier that is passed into the button pressure event
        /// @param gamepadName
        ///     Name of the gamepad that this mapping applies to
        /// @param buttonIndex
        ///     Buttin on the gamepad that triggers this action
        ///
        void SetActionMapping(u32 actionId, const std::string& gamepadName, u32 buttonIndex) noexcept;
        
        /// Registers/updates a mapping such that whenever the given axis on a gamepad that is not explicity
        /// registered is moved an event is fired with the given actionId.
        ///
        /// If mapping for axis doesn't exist it is created
        /// If mapping for axis does exist it is overwritten
        ///
        /// For example: MoveVertically => GamepadAxis::k_y
        ///
        /// @param actionId
        ///     Identifier that is passed into the axis moved event
        /// @param axis
        ///     Axis on the gamepad that triggers this action
        ///
        void SetDefaultActionMapping(u32 actionId, GamepadAxis axis) noexcept;
        
        /// Registers/updates a mapping such that whenever the given button on a gamepad not explcity registered
        /// is pressed or released an event is fired with the given actionId.
        ///
        /// If mapping for button doesn't exist it is created
        /// If mapping for button does exist it is overwritten
        ///
        /// For example: Fire => 0
        ///
        /// @param actionId
        ///     Identifier that is passed into the button pressure event
        /// @param buttonIndex
        ///     Buttin on the gamepad that triggers this action
        ///
        void SetDefaultActionMapping(u32 actionId, u32 buttonIndex) noexcept;
        
        /// @return Event triggered when a new gamepad is connected
        ///
        IConnectableEvent<GamepadAddedDelegate>& GetGamepadAddedEvent() noexcept { return m_gamepadAddedEvent; }

        /// @return Event triggered when the user presses or releases a button on the gamepad
        ///
        IConnectableEvent<GamepadButtonPressureChangedDelegate>& GetButtonPressureChangedEvent() noexcept { return m_buttonPressureChangedEvent; }

        /// @return Event triggered when the user moved an axis input on the gamepad
        ///
        IConnectableEvent<GamepadAxisPositionChangedDelegate>& GetAxisPositionChangedEvent() noexcept { return m_axisPositionChangedEvent; }
        
        /// @return Event triggered when the user presses or releases a button on the gamepad if the button was registered against as part of a mapping
        ///
        IConnectableEvent<GamepadMappedButtonPressureChangedDelegate>& GetMappedButtonPressureChangedEvent() noexcept { return m_mappedButtonPressureChangedEvent; }
        
        /// @return Event triggered when the user moved an axis input on the gamepad if the axis was registered against as part of a mapping
        ///
        IConnectableEvent<GamepadMappedAxisPositionChangedDelegate>& GetMappedAxisPositionChangedEvent() noexcept { return m_mappedAxisPositionChangedEvent; }

        /// @return Event triggered when a gamepad is disconnected
        ///
        IConnectableEvent<GamepadRemovedDelegate>& GetGamepadRemovedEvent() noexcept { return m_gamepadRemovedEvent; }

        /// Attempt to find the gamepad with the given id.
        ///
        /// @param uniqueId
        ///     Unique id of the gamepad to find
        /// @param [OUT] out_gamepad
        ///     Populated with found gamepad
        ///
        /// @return TRUE if gamepad is found
        ///
        bool TryGetGamepadWithId(Gamepad::Id uniqueId, Gamepad& out_gamepad) const noexcept;
        
        /// Attempt to find the gamepad with the given index.
        ///
        /// @param index
        ///     Index of gamepad in list if (gamepad 0 is disconnected then gamepad 1 will move to 0 index)
        /// @param [OUT] out_gamepad
        ///     Populated with found gamepad
        ///
        /// @return TRUE if gamepad is found
        ///
        bool TryGetGamepadWithIndex(u32 index, Gamepad& out_gamepad) const noexcept;

        /// @return List of connected gamepads
        ///
        std::vector<Gamepad> GetGamepads() const noexcept { return m_gamepads; }
        
        /// @return Number of attached gamepads
        ///
        u32 GetNumGamepads() const noexcept { return (u32)m_gamepads.size(); }
        
        /// Process all input events that have been received from the OS. This
        /// will fire off events and updated the state of the gamepads
        ///
        void ProcessQueuedInput() noexcept;

        ///
        virtual ~GamepadSystem() {};
        
    protected:

        ///
        GamepadSystem() = default;
        
        /// Called by the concrete system implementation to inject a create event
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @param name
        ///     Device name of the controller
        /// @param numButtons
        ///     Number of buttons on the controller
        /// @param supportedAxisFlags
        ///     Flagged bits stating which axes are supported
        ///
        /// @return The unique Id of the new gamepad.
        ///
        Gamepad::Id AddGamepadCreateEvent(std::string name, u32 numButtons, u32 supportedAxisFlags) noexcept;
        
        /// Called by the concrete system implementation to inject a pressure changed event
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @param uniqueId
        ///     The unique Id of the gamepad on which button pressure has changed.
        /// @param buttonIndex
        ///     Index of button on the gamepad thats pressure has changed
        /// @param pressure
        ///     Updated pressure
        ///
        void AddButtonPressureChangedEvent(Gamepad::Id uniqueId, u32 buttonIndex, f32 pressure) noexcept;
        
        /// Called by the concrete system implementation to inject an axis changed event
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @param uniqueId
        ///     The unique Id of the gamepad on which an axis (analogue stick) has moved.
        /// @param axis
        ///     Axis on the gamepad thats moved
        /// @param position
        ///     Updated position
        ///
        void AddAxisPositionChangedEvent(Gamepad::Id uniqueId, GamepadAxis axis, f32 position) noexcept;
        
        /// Called by the concrete system implementation to inject a remove event
        ///
        /// This method is thread safe and can be called on any thread.
        ///
        /// @param uniqueId
        ///     The unique Id of the gamepad to disconnect.
        ///
        void AddGamepadRemoveEvent(Gamepad::Id uniqueId) noexcept;

        /// Disconnects all gamepads. This must be called from the main thread
        ///
        void RemoveAllGamepads() noexcept;
        
    private:
        friend class Application;

        /// @return New platform specific system
        ///
        static GamepadSystemUPtr Create() noexcept;
        
        enum class EventType
        {
            k_buttonPressure,
            k_axisPosition,
            k_remove
        };
        
        struct GamepadEventData
        {
            GamepadEventData(EventType type, Gamepad::Id uid, u32 index, f32 contextVal, f64 timestamp)
            : m_type(type), m_uniqueId(uid), m_index(index), m_value(contextVal), m_timestamp(timestamp) {}

            EventType m_type;
            Gamepad::Id m_uniqueId;
            u32 m_index;
            f32 m_value;
            f64 m_timestamp;
        };
        
        struct GamepadCreateEventData
        {
            GamepadCreateEventData(Gamepad::Id uid, std::string name, u32 numButtons, u32 supportedAxisFlags, f64 timestamp)
            : m_uniqueId(uid), m_name(std::move(name)), m_numButtons(numButtons), m_supportedAxisFlags(supportedAxisFlags), m_timestamp(timestamp) {}
            
            Gamepad::Id m_uniqueId;
            std::string m_name;
            u32 m_numButtons;
            u32 m_supportedAxisFlags;
            f64 m_timestamp;
        };
        
        /// Update the state and fire off the external events for the given event
        ///
        /// @param event
        ///     Data about the event
        ///
        void ProcessCreateEvent(const GamepadCreateEventData& event) noexcept;
        
        /// Update the state and fire off the external events for the given event
        ///
        /// @param event
        ///     Data about the event
        ///
        void ProcessButtonPressureEvent(const GamepadEventData& event) noexcept;
        
        /// Update the state and fire off the external events for the given event
        ///
        /// @param event
        ///     Data about the event
        ///
        void ProcessAxisPositionEvent(const GamepadEventData& event) noexcept;
        
        /// Update the state and fire off the external events for the given event
        ///
        /// @param event
        ///     Data about the event
        ///
        void ProcessRemoveEvent(const GamepadEventData& event) noexcept;
        
        Event<GamepadAddedDelegate> m_gamepadAddedEvent;
        Event<GamepadButtonPressureChangedDelegate> m_buttonPressureChangedEvent;
        Event<GamepadAxisPositionChangedDelegate> m_axisPositionChangedEvent;
        Event<GamepadMappedButtonPressureChangedDelegate> m_mappedButtonPressureChangedEvent;
        Event<GamepadMappedAxisPositionChangedDelegate> m_mappedAxisPositionChangedEvent;
        Event<GamepadRemovedDelegate> m_gamepadRemovedEvent;
        
        std::mutex m_mutex;
        std::vector<Gamepad> m_gamepads;
        std::queue<GamepadEventData> m_eventQueue;
        std::queue<GamepadCreateEventData> m_createEventQueue;
        std::vector<std::tuple<u32, u32, f32>> m_axisMappings;
        std::vector<std::pair<u32, u32>> m_buttonMappings;
        std::vector<std::pair<u32, u32>> m_defaultAxisMappings;
        std::vector<std::pair<u32, u32>> m_defaultButtonMappings;
        Gamepad::Id m_nextUniqueId = 0;
    };
}

#endif
