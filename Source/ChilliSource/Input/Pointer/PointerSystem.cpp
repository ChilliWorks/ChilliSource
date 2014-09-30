//
//  PointerSystem.cpp
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

#include <ChilliSource/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Input/Base/Filter.h>

#include <thread>
#include <algorithm>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Input/Pointer/PointerSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Input/Pointer/PointerSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Input/Pointer/PointerSystem.h>
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
            return PointerSystemUPtr(new CSBackend::Android::PointerSystem());
#elif defined CS_TARGETPLATFORM_IOS
            return PointerSystemUPtr(new CSBackend::iOS::PointerSystem());
#elif defined CS_TARGETPLATFORM_WINDOWS
            return PointerSystemUPtr(new CSBackend::Windows::PointerSystem());
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
        Core::IConnectableEvent<PointerSystem::PointerScrollDelegate>& PointerSystem::GetPointerScrollEvent()
        {
            return m_pointerScrolledEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PointerSystem::PointerDownDelegate>& PointerSystem::GetPointerDownEventFiltered()
        {
            return m_pointerDownEventFiltered;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PointerSystem::PointerUpDelegate>& PointerSystem::GetPointerUpEventFiltered()
        {
            return m_pointerUpEventFiltered;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PointerSystem::PointerScrollDelegate>& PointerSystem::GetPointerScrollEventFiltered()
        {
            return m_pointerScrolledEventFiltered;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PointerSystem::PointerDownDelegateInternal>& PointerSystem::GetPointerDownEventInternal()
        {
            return m_pointerDownEventInternal;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Core::IConnectableEvent<PointerSystem::PointerScrollDelegateInternal>& PointerSystem::GetPointerScrollEventInternal()
        {
            return m_pointerScrolledEventInternal;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        std::vector<Pointer> PointerSystem::GetPointers() const
        {
            return m_pointers;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool PointerSystem::TryGetPointerWithId(Pointer::Id in_uniqueId, Pointer& out_pointer) const
        {
            for (const Pointer& pointer : m_pointers)
            {
                if (pointer.GetId() == in_uniqueId)
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
                if (pointer.GetIndex() == in_index)
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
            std::queue<PointerEvent> eventQueue = m_eventQueue;
            while (m_eventQueue.empty() == false)
            {
                m_eventQueue.pop();
            }
            lock.unlock();
            
            while (eventQueue.empty() == false)
            {
                PointerEvent& event = eventQueue.front();
                
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
                    case PointerEventType::k_scroll:
                        PointerScrolled(event.m_pointerUniqueId, event.m_timestamp, event.m_position);
                        break;
                    case PointerEventType::k_remove:
                        RemovePointer(event.m_pointerUniqueId);
                        break;
                    default:
                        CS_LOG_FATAL("Something has gone very wrong while processing buffered input");
                        break;
                }
                
                eventQueue.pop();
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Pointer::Id PointerSystem::AddPointerCreateEvent(const Core::Vector2& in_position)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_add;
            event.m_pointerUniqueId = m_nextUniqueId++;
            event.m_InputType = Pointer::InputType::k_none;
            event.m_position = in_position;
            event.m_timestamp = 0.0;
            
            m_eventQueue.push(event);
            
            return event.m_pointerUniqueId;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::AddPointerDownEvent(Pointer::Id in_pointerUniqueId, Pointer::InputType in_inputType)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_down;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = in_inputType;
            event.m_position = Core::Vector2::k_zero;
            event.m_timestamp = ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0;
            
            m_eventQueue.push(event);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::AddPointerMovedEvent(Pointer::Id in_pointerUniqueId, const Core::Vector2& in_position)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_move;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = Pointer::InputType::k_none;
            event.m_position = in_position;
            event.m_timestamp = ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0;
            
            m_eventQueue.push(event);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::AddPointerUpEvent(Pointer::Id in_pointerUniqueId, Pointer::InputType in_inputType)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_up;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = in_inputType;
            event.m_position = Core::Vector2::k_zero;
            event.m_timestamp = ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0;
            
            m_eventQueue.push(event);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::AddPointerScrollEvent(Pointer::Id in_pointerUniqueId, const Core::Vector2& in_delta)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_scroll;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = Pointer::InputType::k_none;
            event.m_timestamp = ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0;
            event.m_position = in_delta;
            
            m_eventQueue.push(event);
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::AddPointerRemoveEvent(Pointer::Id in_pointerUniqueId)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            PointerEvent event;
            event.m_type = PointerEventType::k_remove;
            event.m_pointerUniqueId = in_pointerUniqueId;
            event.m_InputType = Pointer::InputType::k_none;
            event.m_position = Core::Vector2::k_zero;
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
        void PointerSystem::CreatePointer(Pointer::Id in_uniqueId, const Core::Vector2& in_initialPosition)
        {
            //Find the first free index
            u32 index = 0;

            while(true)
            {
                bool indexAvailable = true;
                
                for (const Pointer& pointer : m_pointers)
                {
                    if (pointer.GetIndex() == index)
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
            
            Pointer pointer(in_uniqueId, index, in_initialPosition);
            m_pointers.push_back(pointer);
            
            //create the filtered input set.
            CS_ASSERT(m_filteredPointerInput.find(pointer.m_uniqueId) == m_filteredPointerInput.end(), "Filtered input already exists!");
            m_filteredPointerInput.emplace(pointer.m_uniqueId, std::set<Pointer::InputType>());
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::PointerDown(Pointer::Id in_uniqueId, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.GetId());
            });
            
            if (pointerIt != m_pointers.end())
            {
                pointerIt->m_activeInput.insert(in_inputType);
                Pointer copy = *pointerIt;
                
                //Notify unfiltered connections
                m_pointerDownEvent.NotifyConnections(copy, in_timestamp, in_inputType);
                
                Filter filter;
                //Notify internal engine connections which can filter the input
                m_pointerDownEventInternal.NotifyConnections(copy, in_timestamp, in_inputType, filter);
                
                //Notify filtered connections if the input has not been filtered
                if(filter.IsFiltered() == false)
                {
                    m_pointerDownEventFiltered.NotifyConnections(copy, in_timestamp, in_inputType);
                }
                else
                {
                    //update the filtered input set.
                    auto filteredInputSetIt = m_filteredPointerInput.find(in_uniqueId);
                    CS_ASSERT(filteredInputSetIt != m_filteredPointerInput.end(), "Filtered input doesn't exist!");
                    CS_ASSERT(filteredInputSetIt->second.find(in_inputType) == filteredInputSetIt->second.end(), "Input already filtered!");
                    
                    filteredInputSetIt->second.insert(in_inputType);
                }
            }
            else
            {
                CS_LOG_FATAL("PointerSystem: Received pointer down event for unknown pointer Id.");
            }
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::PointerMoved(Pointer::Id in_uniqueId, f64 in_timestamp, const Core::Vector2& in_newPosition)
        {
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.GetId());
            });
            
            if (pointerIt != m_pointers.end())
            {
                pointerIt->m_previousPosition = pointerIt->m_position;
                pointerIt->m_position = in_newPosition;
                Pointer copy = *pointerIt;

                //Notify all connections
                m_pointerMovedEvent.NotifyConnections(copy, in_timestamp);
            }
            else
            {
                CS_LOG_FATAL("PointerSystem: Received pointer moved event for unknown pointer Id.");
            }
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::PointerUp(Pointer::Id in_uniqueId, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.GetId());
            });
            
            if (pointerIt != m_pointers.end())
            {
                pointerIt->m_activeInput.erase(in_inputType);
                Pointer copy = *pointerIt;
   
                //Notify unfiltered connections
                m_pointerUpEvent.NotifyConnections(copy, in_timestamp, in_inputType);
                
                //Notify filtered connections if the input has not been filtered
                auto filteredInputSetIt = m_filteredPointerInput.find(in_uniqueId);
                CS_ASSERT(filteredInputSetIt != m_filteredPointerInput.end(), "Filtered input doesn't exist!");

                auto filteredInputIt = filteredInputSetIt->second.find(in_inputType);
                if(filteredInputIt == filteredInputSetIt->second.end())
                {
                    m_pointerUpEventFiltered.NotifyConnections(copy, in_timestamp, in_inputType);
                }
                else
                {
                    filteredInputSetIt->second.erase(filteredInputIt);
                }
            }
            else
            {
                CS_LOG_FATAL("PointerSystem: Received pointer down event for unknown pointer Id.");
            }
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::PointerScrolled(Pointer::Id in_uniqueId, f64 in_timestamp, const Core::Vector2& in_delta)
        {
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.GetId());
            });
            
            if (pointerIt != m_pointers.end())
            {
                Pointer copy = *pointerIt;

                //Notify unfiltered connections
                m_pointerScrolledEvent.NotifyConnections(copy, in_timestamp, in_delta);
                
                Filter filter;
                //Notify internal engine connections which can filter the input
                m_pointerScrolledEventInternal.NotifyConnections(copy, in_timestamp, in_delta, filter);
                
                //Notify filtered connections if the input has not been filtered
                if(filter.IsFiltered() == false)
                {
                    m_pointerScrolledEventFiltered.NotifyConnections(copy, in_timestamp, in_delta);
                }
            }
            else
            {
                CS_LOG_FATAL("PointerSystem: Received pointer scroll event for unknown pointer Id.");
            }
        }
        //----------------------------------------------------
        //-----------------------------------------------------
        void PointerSystem::RemovePointer(Pointer::Id in_uniqueId)
        {
            //remove the filtered input set.
            auto filteredInputIt = m_filteredPointerInput.find(in_uniqueId);
            CS_ASSERT(filteredInputIt != m_filteredPointerInput.end(), "Filtered input doesn't exist!");
            
            m_filteredPointerInput.erase(filteredInputIt);
            
            //remove the pointer.
            auto pointerIt = std::find_if(m_pointers.begin(), m_pointers.end(), [in_uniqueId](const Pointer& in_pointer)
            {
                return (in_uniqueId == in_pointer.GetId());
            });
            
            if (pointerIt != m_pointers.end())
            {
                m_pointers.erase(pointerIt);
            }
            else
            {
                CS_LOG_FATAL("PointerSystem: Received remove pointer event for unknown pointer Id.");
            }
        }
    }
}
