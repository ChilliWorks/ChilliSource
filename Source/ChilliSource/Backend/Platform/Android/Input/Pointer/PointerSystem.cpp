//
//  PointerSystem.cpp
//  Chilli Source
//  Created by Ian Copland on 27/03/2014.
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

#include <ChilliSource/Backend/Platform/Android/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(PointerSystem);
		//----------------------------------------------------
		//----------------------------------------------------
		bool PointerSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (Input::PointerSystem::InterfaceID == in_interfaceId || PointerSystem::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void PointerSystem::OnTouchDown(s32 in_systemId, const Core::Vector2& in_location)
		{
			Core::Vector2 touchLocation(in_location.x, m_screen->GetResolution().y - in_location.y);
			PointerId pointerId = AddPointerCreateEvent(touchLocation);
			AddPointerDownEvent(pointerId, InputType::k_touch);
			m_systemIdToPointerIdMap.emplace(in_systemId, pointerId);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void PointerSystem::OnTouchMoved(s32 in_systemId, const Core::Vector2& in_location)
		{
			auto it = m_systemIdToPointerIdMap.find(in_systemId);
			if (it != m_systemIdToPointerIdMap.end())
			{
				Core::Vector2 touchLocation(in_location.x, m_screen->GetResolution().y - in_location.y);
				AddPointerMovedEvent(it->second, touchLocation);
			}
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void PointerSystem::OnTouchUp(s32 in_systemId)
		{
			auto it = m_systemIdToPointerIdMap.find(in_systemId);
			if (it != m_systemIdToPointerIdMap.end())
			{
				AddPointerUpEvent(it->second, InputType::k_touch);
				AddPointerRemoveEvent(it->second);
				m_systemIdToPointerIdMap.erase(it);
			}
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnInit()
		{
			m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
			CS_ASSERT(m_screen != nullptr, "Cannot find required system for PointerSystem: Screen.");

			TouchInputJavaInterface::SetPointerSystem(this);
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnDestroy()
		{
			TouchInputJavaInterface::SetPointerSystem(nullptr);
			RemoveAllPointers();
			m_screen = nullptr;
		}
	}
}
