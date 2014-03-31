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
			CGPoint uitouchLocation = [in_touch locationInView:in_touch.view];
            Core::Vector2 touchLocation(uitouchLocation.x * m_densityScale, Core::Screen::GetOrientedDimensions().y - uitouchLocation.y * m_densityScale);
			u64 pointerId = AddPointerCreateEvent(touchLocation);
            AddPointerDownEvent(pointerId, InputType::k_touch);
			m_touchToIdMap.emplace(in_touch, pointerId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::OnTouchMoved(UITouch* in_touch)
        {
            auto it = m_touchToIdMap.find(in_touch);
			if (it != m_touchToIdMap.end())
			{
                CGPoint uitouchLocation = [in_touch locationInView:in_touch.view];
                Core::Vector2 touchLocation(uitouchLocation.x * m_densityScale, Core::Screen::GetOrientedDimensions().y - uitouchLocation.y * m_densityScale);
				AddPointerMovedEvent(it->second, touchLocation);
			}
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::OnTouchEnded(UITouch* in_touch)
        {
            auto it = m_touchToIdMap.find(in_touch);
			if (it != m_touchToIdMap.end())
			{
				AddPointerUpEvent(it->second, InputType::k_touch);
                AddPointerRemoveEvent(it->second);
                m_touchToIdMap.erase(it);
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
