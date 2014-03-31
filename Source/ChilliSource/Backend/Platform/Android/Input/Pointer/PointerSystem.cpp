//
//  PointerSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 27/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Backend/Platform/Android/Input/Pointer/TouchInputJavaInterface.h>
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
			Core::Vector2 touchLocation(in_location.x, Core::Screen::GetOrientedDimensions().y - in_location.y);
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
				Core::Vector2 touchLocation(in_location.x, Core::Screen::GetOrientedDimensions().y - in_location.y);
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
			ChilliSource::Android::TouchInputJavaInterface::SetPointerSystem(this);
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnDestroy()
		{
			ChilliSource::Android::TouchInputJavaInterface::SetPointerSystem(nullptr);
			RemoveAllPointers();
		}
	}
}
