//
//  PointerSystem.mm
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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Input/Pointer/PointerSystem.h>

#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Base/Screen.h>

#import <UIKit/UIKit.h>

namespace CSBackend
{
    namespace iOS
    {
        CS_DEFINE_NAMEDTYPE(PointerSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        bool PointerSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (CSInput::PointerSystem::InterfaceID == in_interfaceId || PointerSystem::InterfaceID == in_interfaceId);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void PointerSystem::OnTouchBegan(UITouch* in_touch)
        {
            //Grab the touch location from the OS
			CGPoint uitouchLocation = [in_touch locationInView:in_touch.view];
            CSCore::Vector2 touchLocation(uitouchLocation.x * m_screen->GetDensityScale(), m_screen->GetResolution().y - uitouchLocation.y * m_screen->GetDensityScale());
			u64 pointerId = AddPointerCreateEvent(touchLocation);
            AddPointerDownEvent(pointerId, CSInput::Pointer::InputType::k_touch);
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
                CSCore::Vector2 touchLocation(uitouchLocation.x * m_screen->GetDensityScale(), m_screen->GetResolution().y - uitouchLocation.y * m_screen->GetDensityScale());
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
				AddPointerUpEvent(it->second, CSInput::Pointer::InputType::k_touch);
                AddPointerRemoveEvent(it->second);
                m_touchToIdMap.erase(it);
			}
        }
        //------------------------------------------------
        //------------------------------------------------
        void PointerSystem::OnInit()
        {
            m_screen = CSCore::Application::Get()->GetSystem<CSCore::Screen>();
        }
        //------------------------------------------------
        //------------------------------------------------
        void PointerSystem::OnDestroy()
        {
            RemoveAllPointers();
            m_screen = nullptr;
        }
    }
}

#endif