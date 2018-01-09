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

#include <ChilliSource/Input/Gamepad/GamepadSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Input/Base/InputFilter.h>

#include <thread>
#include <algorithm>

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Input/Gamepad/GamepadSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_RPI
#include <CSBackend/Platform/RPi/Input/Gamepad/GamepadSystem.h>
#endif

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(GamepadSystem);
    
    namespace
    {
        /// Performs pairing (i.e. generating a unique number from the 2 given numbers) using the Cantor method.
        ///
        /// NOTE: f(a,b) != f(b,a)
        ///
        /// @param a
        ///     First number to pair
        /// @param b
        ///     Second number to pair
        ///
        /// @return Unique paired result
        ///
        u32 CantorPairing(u32 a, u32 b) noexcept
        {
            return ((a + b) * (a + b + 1)) / 2 + b;
        }
        
        /// Attemtps to get the action id firstly from the named mappings and then the default mappings
        ///
        /// @param gamepadNameHash
        ///     Unique id of the gamepad type
        /// @param eventIndex
        ///     Button or axis
        /// @param namedMappings
        ///     Priority mappings using the gamepad name and button/axis
        /// @param defaultMappings
        ///     Fallback mappings using the button/axis only
        /// @param [OUT] actionId
        ///     Set if the mapping exists
        ///
        /// @return TRUE if actionId is set
        ///
        bool TryGetActionId(u32 gamepadNameHash, u32 eventIndex, const std::vector<std::pair<u32, u32>>& namedMappings, const std::vector<std::pair<u32, u32>>& defaultMappings, u32& out_actionId) noexcept
        {
            //Check if this button is mapped for this gamepad type
            u32 mappingId = CantorPairing(gamepadNameHash, eventIndex);
            auto mappingIt = std::find_if(namedMappings.begin(), namedMappings.end(), [mappingId](const std::pair<u32, u32>& mapping)
            {
                return (mapping.first == mappingId);
            });
            
            if(mappingIt != namedMappings.end())
            {
                out_actionId = mappingIt->second;
                return true;
            }
            
            auto defaultMappingIt = std::find_if(defaultMappings.begin(), defaultMappings.end(), [eventIndex](const std::pair<u32, u32>& mapping)
            {
                return (mapping.first == eventIndex);
            });
            
            if(defaultMappingIt != defaultMappings.end())
            {
                out_actionId = defaultMappingIt->second;
                return true;
            }
            
            return false;
        }
        
        /// Attemtps to get the action id firstly from the named mappings and then the default mappings
        ///
        /// @param gamepadNameHash
        ///     Unique id of the gamepad type
        /// @param eventIndex
        ///     Button or axis
        /// @param namedMappings
        ///     Priority mappings using the gamepad name and button/axis
        /// @param defaultMappings
        ///     Fallback mappings using the button/axis only
        /// @param [OUT] out_actionId
        ///     Set if the mapping exists
        /// @param [OUT] out_scalar
        ///     Set if mapping exists
        ///
        /// @return TRUE if out_actionId is set
        ///
        bool TryGetActionId(u32 gamepadNameHash, u32 eventIndex, const std::vector<std::tuple<u32, u32, f32>>& namedMappings, const std::vector<std::pair<u32, u32>>& defaultMappings, u32& out_actionId, f32& out_scalar) noexcept
        {
            //Check if this button is mapped for this gamepad type
            u32 mappingId = CantorPairing(gamepadNameHash, eventIndex);
            auto mappingIt = std::find_if(namedMappings.begin(), namedMappings.end(), [mappingId](const std::tuple<u32, u32, f32>& mapping)
            {
                return (std::get<0>(mapping) == mappingId);
            });
            
            if(mappingIt != namedMappings.end())
            {
                out_actionId = std::get<1>(*mappingIt);
                out_scalar = std::get<2>(*mappingIt);
                return true;
            }
            
            auto defaultMappingIt = std::find_if(defaultMappings.begin(), defaultMappings.end(), [eventIndex](const std::pair<u32, u32>& mapping)
            {
                return (mapping.first == eventIndex);
            });
            
            if(defaultMappingIt != defaultMappings.end())
            {
                out_actionId = defaultMappingIt->second;
                out_scalar = 1.0f;
                return true;
            }
            
            return false;
        }
    }
    
    //------------------------------------------------------------------------------
    GamepadSystemUPtr GamepadSystem::Create() noexcept
    {
#if defined CS_TARGETPLATFORM_WINDOWS
        return GamepadSystemUPtr(new CSBackend::Windows::GamepadSystem());
#elif defined CS_TARGETPLATFORM_RPI
        return GamepadSystemUPtr(new CSBackend::RPi::GamepadSystem());
#else
        return nullptr;
#endif
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::SetActionMapping(u32 actionId, const std::string& gamepadName, GamepadAxis axis, bool invert) noexcept
    {
        u32 gamepadNameHash = HashCRC32::GenerateHashCode(gamepadName);
        u32 mappingId = CantorPairing(gamepadNameHash, (u32)axis);
        
        auto mappingIt = std::find_if(m_axisMappings.begin(), m_axisMappings.end(), [mappingId](const std::tuple<u32, u32, f32>& mapping)
        {
            return std::get<0>(mapping) == mappingId;
        });
        
        if(mappingIt == m_axisMappings.end())
        {
            m_axisMappings.push_back(std::make_tuple(mappingId, actionId, invert == false ? 1.0f : -1.0f));
        }
        else
        {
            std::get<1>(*mappingIt) = actionId;
            std::get<2>(*mappingIt) = invert == false ? 1.0f : -1.0f;
        }
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::SetActionMapping(u32 actionId, const std::string& gamepadName, u32 buttonIndex) noexcept
    {
        u32 gamepadNameHash = HashCRC32::GenerateHashCode(gamepadName);
        u32 mappingId = CantorPairing(gamepadNameHash, buttonIndex);

        auto mappingIt = std::find_if(m_buttonMappings.begin(), m_buttonMappings.end(), [mappingId](const std::pair<u32, u32>& mapping)
        {
            return (mapping.first == mappingId);
        });
        
        if(mappingIt == m_buttonMappings.end())
        {
            m_buttonMappings.push_back(std::make_pair(mappingId, actionId));
        }
        else
        {
            mappingIt->second = actionId;
        }
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::SetDefaultActionMapping(u32 actionId, GamepadAxis axis) noexcept
    {
        u32 mappingId = (u32)axis;
        
        auto mappingIt = std::find_if(m_defaultAxisMappings.begin(), m_defaultAxisMappings.end(), [mappingId](const std::pair<u32, u32>& mapping)
        {
            return (mapping.first == mappingId);
        });
        
        if(mappingIt == m_defaultAxisMappings.end())
        {
            m_defaultAxisMappings.push_back(std::make_pair(mappingId, actionId));
        }
        else
        {
            mappingIt->second = actionId;
        }
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::SetDefaultActionMapping(u32 actionId, u32 buttonIndex) noexcept
    {
        u32 mappingId = buttonIndex;
        
        auto mappingIt = std::find_if(m_defaultButtonMappings.begin(), m_defaultButtonMappings.end(), [mappingId](const std::pair<u32, u32>& mapping)
        {
            return (mapping.first == mappingId);
        });
        
        if(mappingIt == m_defaultButtonMappings.end())
        {
            m_defaultButtonMappings.push_back(std::make_pair(mappingId, actionId));
        }
        else
        {
            mappingIt->second = actionId;
        }
    }

    //------------------------------------------------------------------------------
    bool GamepadSystem::TryGetGamepadWithId(Gamepad::Id uniqueId, Gamepad& out_gamepad) const noexcept
    {
        for (const auto& gamepad : m_gamepads)
        {
            if (gamepad.GetId() == uniqueId)
            {
                out_gamepad = gamepad;
                return true;
            }
        }
        
        return false;
    }
    
    //------------------------------------------------------------------------------
    bool GamepadSystem::TryGetGamepadWithIndex(u32 index, Gamepad& out_gamepad) const noexcept
    {
        if(index < m_gamepads.size())
        {
            out_gamepad = m_gamepads[index];
            return true;
        }
        
        return false;
    }

    //------------------------------------------------------------------------------
    void GamepadSystem::ProcessQueuedInput() noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        std::queue<GamepadCreateEventData> createEventQueue = m_createEventQueue;
        std::queue<GamepadEventData> eventQueue = m_eventQueue;
        while (m_createEventQueue.empty() == false)
        {
            m_createEventQueue.pop();
        }
        while (m_eventQueue.empty() == false)
        {
            m_eventQueue.pop();
        }
        lock.unlock();
        
        while (createEventQueue.empty() == false)
        {
            const GamepadCreateEventData& event = createEventQueue.front();
            ProcessCreateEvent(event);
            
            createEventQueue.pop();
        }
        
        while (eventQueue.empty() == false)
        {
            const GamepadEventData& event = eventQueue.front();
            
            switch (event.m_type)
            {
                case EventType::k_buttonPressure:
                    ProcessButtonPressureEvent(event);
                    break;
                case EventType::k_axisPosition:
                    ProcessAxisPositionEvent(event);
                    break;
                case EventType::k_remove:
                    ProcessRemoveEvent(event);
                    break;
                default:
                    CS_LOG_FATAL("Something has gone very wrong while processing gamepad queued input");
                    break;
            }
            
            eventQueue.pop();
        }
    }
    
    //------------------------------------------------------------------------------
    Gamepad::Id GamepadSystem::AddGamepadCreateEvent(std::string name, u32 numButtons, u32 supportedAxisFlags) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        auto uniqueId = m_nextUniqueId++;
        m_createEventQueue.push(GamepadCreateEventData(uniqueId, std::move(name), numButtons, supportedAxisFlags, ((f64)Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0));
        
        return uniqueId;
    }

    //------------------------------------------------------------------------------
    void GamepadSystem::AddButtonPressureChangedEvent(Gamepad::Id uniqueId, u32 buttonIndex, f32 pressure) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_eventQueue.push(GamepadEventData(EventType::k_buttonPressure, uniqueId, buttonIndex, pressure, ((f64)Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0));
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::AddAxisPositionChangedEvent(Gamepad::Id uniqueId, GamepadAxis axis, f32 position) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_eventQueue.push(GamepadEventData(EventType::k_axisPosition, uniqueId, (u32)axis, position, ((f64)Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0));
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::AddGamepadRemoveEvent(Gamepad::Id uniqueId) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_eventQueue.push(GamepadEventData(EventType::k_remove, uniqueId, 0, 0.0f, ((f64)Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0));
    }

    //------------------------------------------------------------------------------
    void GamepadSystem::RemoveAllGamepads() noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        while (m_eventQueue.empty() == false)
        {
            m_eventQueue.pop();
        }
        
        lock.unlock();
        
        m_gamepads.clear();
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::ProcessCreateEvent(const GamepadCreateEventData& event) noexcept
    {
        //Find the first free index
        u32 index = 0;
        
        while(true)
        {
            bool indexAvailable = true;
            
            for (const auto& gamepad : m_gamepads)
            {
                if (gamepad.GetIndex() == index)
                {
                    indexAvailable = false;
                    break;
                }
            }
            
            if(indexAvailable == true)
            {
                break;
            }
            
            ++index;
        }
        
        m_gamepads.push_back(Gamepad(event.m_uniqueId, index, event.m_numButtons, event.m_supportedAxisFlags, std::move(event.m_name)));
        
        m_gamepadAddedEvent.NotifyConnections(m_gamepads.back(), event.m_timestamp);
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::ProcessButtonPressureEvent(const GamepadEventData& event) noexcept
    {
        auto gamepadIt = std::find_if(m_gamepads.begin(), m_gamepads.end(), [&](const Gamepad& gamepad)
        {
            return (event.m_uniqueId == gamepad.GetId());
        });
        
        if (gamepadIt != m_gamepads.end())
        {
            gamepadIt->m_buttonStates[event.m_index] = event.m_value;
            m_buttonPressureChangedEvent.NotifyConnections(*gamepadIt, event.m_timestamp, event.m_index, event.m_value);
            
            u32 actionId = 0;
            if(TryGetActionId(gamepadIt->GetNameHash(), event.m_index, m_buttonMappings, m_defaultButtonMappings, actionId) == true)
            {
                m_mappedButtonPressureChangedEvent.NotifyConnections(*gamepadIt, event.m_timestamp, actionId, event.m_value);
            }
        }
        else
        {
            CS_LOG_FATAL("GamepadSystem: Received gamepad event for unknown Id.");
        }
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::ProcessAxisPositionEvent(const GamepadEventData& event) noexcept
    {
        auto gamepadIt = std::find_if(m_gamepads.begin(), m_gamepads.end(), [&](const Gamepad& gamepad)
        {
            return (event.m_uniqueId == gamepad.GetId());
        });
        
        if (gamepadIt != m_gamepads.end())
        {
            gamepadIt->m_axisStates[event.m_index] = event.m_value;
            m_axisPositionChangedEvent.NotifyConnections(*gamepadIt, event.m_timestamp, (GamepadAxis)event.m_index, event.m_value);
            
            u32 actionId = 0;
            f32 scalar = 1.0f;
            if(TryGetActionId(gamepadIt->GetNameHash(), event.m_index, m_axisMappings, m_defaultAxisMappings, actionId, scalar) == true)
            {
                m_mappedAxisPositionChangedEvent.NotifyConnections(*gamepadIt, event.m_timestamp, actionId, event.m_value * scalar);
            }
        }
        else
        {
            CS_LOG_FATAL("GamepadSystem: Received gamepad event for unknown Id.");
        }
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::ProcessRemoveEvent(const GamepadEventData& event) noexcept
    {
        auto gamepadIt = std::find_if(m_gamepads.begin(), m_gamepads.end(), [&](const Gamepad& gamepad)
        {
            return (event.m_uniqueId == gamepad.GetId());
        });
        
        if (gamepadIt != m_gamepads.end())
        {
            auto gamepad = *gamepadIt;
            m_gamepads.erase(gamepadIt);
            m_gamepadRemovedEvent.NotifyConnections(std::move(gamepad), event.m_timestamp);
        }
        else
        {
            CS_LOG_FATAL("GamepadSystem: Received gamepad event for unknown Id.");
        }
    }
}
