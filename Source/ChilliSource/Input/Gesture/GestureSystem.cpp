//
//  GestureSystem.cpp
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

#include <ChilliSource/Input/Gesture/GestureSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Input/Base/Filter.h>
#include <ChilliSource/Input/Gesture/Gesture.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>

namespace ChilliSource
{
    namespace Input
    {
        namespace
        {
            //-------------------------------------------------------
            /// Searches the given gesture list for the given gesture
            /// pointer to see if it exists.
            ///
            /// @author Ian Copland
            ///
            /// @param The gesture list.
            /// @param The gesture to look for.
            ///
            /// @return Whether or not it exists.
            //-------------------------------------------------------
            bool GestureExists(Core::concurrent_vector<GestureSPtr>& in_gestureList, const Gesture* in_gesture)
            {
                in_gestureList.lock();
                for (const auto& gesturePair : in_gestureList)
                {
                    if (gesturePair.get() == in_gesture)
                    {
                        return true;
                    }
                }
                in_gestureList.unlock();
                return false;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(GestureSystem);
        //--------------------------------------------------------
        //--------------------------------------------------------
        GestureSystemUPtr GestureSystem::Create()
        {
            return GestureSystemUPtr(new GestureSystem());
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        bool GestureSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (GestureSystem::InterfaceID == in_interfaceId);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::AddGesture(const GestureSPtr& in_gesture)
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            CS_ASSERT(GestureExists(m_gestures, in_gesture.get()) == false, "Cannot add a gesture that has already been added to the Gesture System.");
            in_gesture->SetGestureSystem(this);
            m_gestures.push_back(in_gesture);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::RemoveGesture(const Gesture* in_gesture)
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_gestures.lock();
            
            CS_ASSERT(GestureExists(m_gestures, in_gesture) == true, "Cannot remove a gesture that hasn't been added to the Gesture System.");
            
            for (auto it = m_gestures.begin(); it != m_gestures.end();)
            {
                if (it->get() == in_gesture)
                {
                    (*it)->SetGestureSystem(nullptr);
                    m_gestures.erase(it);
                    break;
                }
                else
                {
                    ++it;
                }
            }
            
            m_gestures.unlock();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::SetConflictResolutionDelegate(const ConflictResolutionDelegate& in_delegate)
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_conflictResolutionDelegate = in_delegate;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        bool GestureSystem::ResolveConflicts(Gesture* in_gesture)
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_gestures.lock();
            
            bool canActivate = true;
            
            if (m_conflictResolutionDelegate != nullptr)
            {
                for (const auto& gesture : m_gestures)
                {
                    if (gesture.get() != in_gesture && gesture->IsActive() == true)
                    {
                        ConflictResult conflictResult = m_conflictResolutionDelegate(gesture.get(), in_gesture);
                        
                        switch (conflictResult)
                        {
                            case ConflictResult::k_neitherGesture:
                                gesture->Cancel();
                                canActivate = false;
                                break;
                            case ConflictResult::k_existingGesture:
                                canActivate = false;
                                break;
                            case ConflictResult::k_newGesture:
                                gesture->Cancel();
                                break;
                            case ConflictResult::k_bothGestures:
                                break;
                        }
                    }
                }
            }
            
            m_gestures.unlock();
            return canActivate;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::OnInit()
        {
            Input::PointerSystem* pointerSystem = Core::Application::Get()->GetSystem<Input::PointerSystem>();
            CS_ASSERT(pointerSystem != nullptr, "Gesture system missing required system: Pointer System");
            
            m_pointerDownConnection = pointerSystem->GetPointerDownEventInternal().OpenConnection(Core::MakeDelegate(this, &GestureSystem::OnPointerDown));
            m_pointerMovedConnection = pointerSystem->GetPointerMovedEvent().OpenConnection(Core::MakeDelegate(this, &GestureSystem::OnPointerMoved));
            m_pointerUpConnection = pointerSystem->GetPointerUpEvent().OpenConnection(Core::MakeDelegate(this, &GestureSystem::OnPointerUp));
            m_pointerScrolledConnection = pointerSystem->GetPointerScrollEventInternal().OpenConnection(Core::MakeDelegate(this, &GestureSystem::OnPointerScrolled));
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::OnUpdate(f32 in_deltaTime)
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_gestures.lock();
            
            for (const auto& gesture : m_gestures)
            {
                gesture->OnUpdate(in_deltaTime);
            }
            
            m_gestures.unlock();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType, Filter& in_filter)
        {
            if (in_filter.IsFiltered() == false)
            {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                m_gestures.lock();
                
                for (const auto& gesture : m_gestures)
                {
                    gesture->OnPointerDown(in_pointer, in_timestamp, in_inputType);
                }
                
                m_gestures.unlock();
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp)
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_gestures.lock();
            
            for (const auto& gesture : m_gestures)
            {
                gesture->OnPointerMoved(in_pointer, in_timestamp);
            }
            
            m_gestures.unlock();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_gestures.lock();
            
            for (const auto& gesture : m_gestures)
            {
                gesture->OnPointerUp(in_pointer, in_timestamp, in_inputType);
            }
            
            m_gestures.unlock();
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::OnPointerScrolled(const Pointer& in_pointer, f64 in_timestamp, const Core::Vector2& in_delta, Filter& in_filter)
        {
            if (in_filter.IsFiltered() == false)
            {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                m_gestures.lock();
                
                for (const auto& gesture : m_gestures)
                {
                    gesture->OnPointerScrolled(in_pointer, in_timestamp, in_delta);
                }
                
                m_gestures.unlock();
            }
        }
        //-------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::OnPointerDownLegacy(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
        {
            Filter filter;
            OnPointerDown(in_pointer, in_timestamp, in_inputType, filter);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void GestureSystem::OnDestroy()
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_gestures.lock();
            
            m_conflictResolutionDelegate = nullptr;
            
            m_pointerScrolledConnection.reset();
            m_pointerUpConnection.reset();
            m_pointerMovedConnection.reset();
            m_pointerDownConnection.reset();
            
            
            for (const auto& gesture : m_gestures)
            {
                gesture->SetGestureSystem(nullptr);
            }
            m_gestures.clear();
            
            m_gestures.unlock();
        }
    }
}
