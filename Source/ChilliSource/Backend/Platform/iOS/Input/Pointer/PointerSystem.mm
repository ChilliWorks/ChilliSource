//
//  PointerSystem.mm
//  Chilli Source
//
//  Created by Ian Copland on 26/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Core/Base/Screen.h>

#include <UIKit/UIKit.h>

namespace ChilliSource
{
    namespace iOS
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
        void PointerSystem::OnTouchBegan(UITouch* in_touch)
        {
            //Grab the touch location from the OS
			CGPoint touchLocation = [in_touch locationInView:in_touch.view];
			u64 pointerId = AddPointerCreateEvent(Core::Vector2(touchLocation.x * m_densityScale, touchLocation.y * m_densityScale));
            AddPointerDownEvent(pointerId, PressType::k_touch);
			m_touchToIdMap.emplace(in_touch, pointerId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::OnTouchMoved(UITouch* in_touch)
        {
            auto it = m_touchToIdMap.find(in_touch);
			if (it != m_touchToIdMap.end())
			{
				CGPoint touchLocation = [in_touch locationInView:in_touch.view];
				AddPointerMovedEvent(it->second, Core::Vector2(touchLocation.x * m_densityScale, touchLocation.y * m_densityScale));
			}
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::OnTouchEnded(UITouch* in_touch)
        {
            auto it = m_touchToIdMap.find(in_touch);
			if (it != m_touchToIdMap.end())
			{
				AddPointerUpEvent(it->second, PressType::k_touch);
                AddPointerRemoveEvent(it->second);
			}
        }
        //------------------------------------------------
        //------------------------------------------------
        void PointerSystem::OnInit()
        {
            m_densityScale = Core::Screen::GetDensity();
        }
        //------------------------------------------------
        //------------------------------------------------
        void PointerSystem::OnDestroy()
        {
            RemoveAllPointers();
        }
    }
}
