//
//  PointerSystem.cpp
//  Chilli Source
//
//  Created by I Copland on 26/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Core/Base/Application.h>

#include <thread>
#include <algorithm>

#ifdef CS_TARGETPLATFORM_ANDROID
#   include <ChilliSource/Backend/Platform/Android/Input/Pointer/PointerSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#   include <ChilliSource/Backend/Platform/iOS/Input/Pointer/PointerSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#   include <ChilliSource/Backend/Platform/Windows/Input/Pointer/PointerSystem.h>
#endif

namespace ChilliSource
{
    namespace Input
    {
        CS_DEFINE_NAMEDTYPE(PointerSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        PointerSystemUPtr PointerSystem::Create()
        {
#if defined CS_TARGETPLATFORM_ANDROID
            return PointerSystemUPtr(new Android::PointerSystem());
#elif defined CS_TARGETPLATFORM_IOS
            return PointerSystemUPtr(new iOS::PointerSystem());
#elif defined CS_TARGETPLATFORM_WINDOWS
            return PointerSystemUPtr(new Windows::PointerSystem());
#else
            return nullptr;
#endif
        }
        //----------------------------------------------------
        //----------------------------------------------------
        PointerSystem::InputType PointerSystem::GetDefaultInputType()
        {
#if defined CS_TARGETPLATFORM_ANDROID
            return InputType::k_touch;
#elif defined CS_TARGETPLATFORM_IOS
            return InputType::k_touch;
#elif defined CS_TARGETPLATFORM_WINDOWS
            return InputType::k_leftMouseButton;
#else
            return nullptr;
#endif
        }
        //----------------------------------------------------
        //----------------------------------------------------
        PointerSystem::PointerSystem()
            : m_nextUniqueId(0)
        {
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PointerSystem::PointerDownDelegate>& PointerSystem::GetPointerDownEvent()
        {
            return m_pointerDownEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PointerSystem::PointerMovedDelegate>& PointerSystem::GetPointerMovedEvent()
        {
            return m_pointerMovedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PointerSystem::PointerUpDelegate>& PointerSystem::GetPointerUpEvent()
        {
            return m_pointerUpEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        std::vector<PointerSystem::Pointer> PointerSystem::GetPointers() const
        {
            return m_pointers;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool PointerSystem::TryGetPointerWithId(PointerId in_uniqueId, Pointer& out_pointer) const
        {
            for (const Pointer& pointer : m_pointers)
            {
                if (pointer.m_uniqueId == in_uniqueId)
                {
                    out_pointer = pointer;
                    return true;
                }
            }
            
            return false;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool PointerSystem::TryGetPointerWithIndex(u32 in_index, Pointer& out_pointer) const
        {
            for (const Pointer& pointer : m_pointers)
            {
                if (pointer.m_pointerIndex == in_index)
                {
                    out_pointer = pointer;
                    return true;
                }
            }
            
            return false;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::ProcessQueuedInput()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            while (m_eventQueue.empty() == false)
            {
                PointerEvent& event = m_eventQueue.front();
                
                switch (event.m_type)
                {
                    case PointerEventType::k_add:
                        CreatePointer(event.m_pointerUniqueId, event.m_position);
                        break;
                    case PointerEventType::k_down:
                        PointerDown(event.m_pointerUniqueId, event.m_timestamp, event.m_InputType);
                        break;
                    case PointerEventType::k_move:
                        PointerMoved(event.m_pointerUniqueId, event.m_timestamp, event.m_position);
                        break;
                    case PointerEventType::k_up:
                        PointerUp(event.m_pointerUniqueId, event.m_timestamp, event.m_InputType);
                        break;
                    case PointerEventType::k_remove:
                        RemovePointer(event.m_pointerUniqueId);
                        break;
                    default:
                        CS_LOG_FATAL("Something has gone very wrong while processing buffered input");
                        break;
                }
                
                m_eventQueue.pop();
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        PointerSystem::PointerId PointerSystem::AddPointerCreateEvent(const Core::Vector2& in_position)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_add;
            event.m_pointerUniqueId = m_nextUniqueId++;
            event.m_InputType = InputType::k_none;
            event.m_position = in_position;
            event.m_timestamp = 0.0;
            
            m_eventQueue.push(event);
            
            return event.m_pointerUniqueId;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::AddPointerDownEvent(PointerId in_pointerUniqueId, InputType in_inputType)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_down;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = in_inputType;
            event.m_position = Core::Vector2::ZERO;
            event.m_timestamp = ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0;
            
            m_eventQueue.push(event);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::AddPointerMovedEvent(PointerId in_pointerUniqueId, const Core::Vector2& in_position)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_move;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = InputType::k_none;
            event.m_position = in_position;
            event.m_timestamp = ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0;
            
            m_eventQueue.push(event);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::AddPointerUpEvent(PointerId in_pointerUniqueId, InputType in_inputType)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_up;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = in_inputType;
            event.m_position = Core::Vector2::ZERO;
            event.m_timestamp = ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0;
            
            m_eventQueue.push(event);
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::AddPointerRemoveEvent(PointerId in_pointerUniqueId)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_remove;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = InputType::k_none;
            event.m_position = Core::Vector2::ZERO;
            event.m_timestamp = 0.0;
            
            m_eventQueue.push(event);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::RemoveAllPointers()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            while (m_eventQueue.empty() == false)
            {
                m_eventQueue.pop();
            }
            
            lock.unlock();
            
            m_pointers.clear();
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::CreatePointer(PointerId in_uniqueId, const Core::Vector2& in_initialPosition)
        {
            Pointer pointer;
            pointer.m_uniqueId = in_uniqueId;
            pointer.m_location = in_initialPosition;
            pointer.m_previousLocation = in_initialPosition;
            pointer.m_pointerIndex = m_pointers.size();
            m_pointers.push_back(pointer);
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::PointerDown(PointerId in_uniqueId, f64 in_timestamp, InputType in_inputType)
        {
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.m_uniqueId);
            });
            
            if (pointerIt != m_pointers.end())
            {
                pointerIt->m_activeInput.insert(in_inputType);
                Pointer copy = *pointerIt;
                m_pointerDownEvent.NotifyConnections(copy, in_timestamp, in_inputType);
            }
            else
            {
                CS_LOG_ERROR("PointerSystem: Received pointer down event for unknown pointer Id.");
            }
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::PointerMoved(PointerId in_uniqueId, f64 in_timestamp, const Core::Vector2& in_newPosition)
        {
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.m_uniqueId);
            });
            
            if (pointerIt != m_pointers.end())
            {
                pointerIt->m_previousLocation = pointerIt->m_location;
                pointerIt->m_location = in_newPosition;
                Pointer copy = *pointerIt;
                m_pointerMovedEvent.NotifyConnections(copy, in_timestamp);
            }
            else
            {
                CS_LOG_ERROR("PointerSystem: Received pointer moved event for unknown pointer Id.");
            }
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::PointerUp(PointerId in_uniqueId, f64 in_timestamp, InputType in_inputType)
        {
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.m_uniqueId);
            });
            
            if (pointerIt != m_pointers.end())
            {
                pointerIt->m_activeInput.erase(in_inputType);
                Pointer copy = *pointerIt;
                m_pointerUpEvent.NotifyConnections(copy, in_timestamp, in_inputType);
            }
            else
            {
                CS_LOG_ERROR("PointerSystem: Received pointer down event for unknown pointer Id.");
            }
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::RemovePointer(PointerId in_uniqueId)
        {
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.m_uniqueId);
            });
            
            if (pointerIt != m_pointers.end())
            {
                m_pointers.erase(pointerIt);
            }
            else
            {
                CS_LOG_ERROR("PointerSystem: Received remove pointer event for unknown pointer Id.");
            }
        }
    }
}
