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
        std::queue<GamepadEventData> eventQueue = m_eventQueue;
        while (m_eventQueue.empty() == false)
        {
            m_eventQueue.pop();
        }
        lock.unlock();
        
        while (eventQueue.empty() == false)
        {
            GamepadEventData& event = eventQueue.front();
            
            switch (event.m_type)
            {
                case EventType::k_add:
                    ProcessCreateEvent(event);
                    break;
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
    Gamepad::Id GamepadSystem::AddGamepadCreateEvent() noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        auto uniqueId = m_nextUniqueId++;
        m_eventQueue.push(GamepadEventData(EventType::k_add, uniqueId, 0, 0.0f, ((f64)Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0));
        
        return uniqueId;
    }

    //------------------------------------------------------------------------------
    void GamepadSystem::AddButtonPressureChangedEvent(Gamepad::Id uniqueId, u32 buttonIndex, f32 pressure) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_eventQueue.push(GamepadEventData(EventType::k_buttonPressure, uniqueId, buttonIndex, pressure, ((f64)Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0));
    }
    
    //------------------------------------------------------------------------------
    void GamepadSystem::AddAxisPositionChangedEvent(Gamepad::Id uniqueId, u32 axisIndex, f32 position) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_eventQueue.push(GamepadEventData(EventType::k_axisPosition, uniqueId, axisIndex, position, ((f64)Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0));
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
    void GamepadSystem::ProcessCreateEvent(const GamepadEventData& event) noexcept
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
        
        m_gamepads.push_back(Gamepad(event.m_uniqueId, index, 32, 8, ""));
        
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
            m_axisPositionChangedEvent.NotifyConnections(*gamepadIt, event.m_timestamp, event.m_index, event.m_value);
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
