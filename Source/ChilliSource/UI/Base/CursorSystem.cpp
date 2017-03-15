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

#include <ChilliSource/UI/Base/CursorSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/AppConfig.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Event/IConnectableEvent.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>
#include <ChilliSource/UI/Base/WidgetFactory.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(CursorSystem);
    
    //------------------------------------------------------------------------------
    CursorSystemUPtr CursorSystem::Create() noexcept
    {
        return CursorSystemUPtr(new CursorSystem());
    }
    
    //------------------------------------------------------------------------------
    bool CursorSystem::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return interfaceId == CursorSystem::InterfaceID;
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::ShowCursor()
    {
        m_enabled = true;
        m_cursor->SetVisible(m_pointerSystem->GetNumPointers() > 0);
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::HideCursor()
    {
        m_enabled = false;
        m_cursor->SetVisible(false);
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::OnInit() noexcept
    {
        auto appConfig = Application::Get()->GetAppConfig();
        m_enabled = appConfig->GetCursorType() == CursorType::k_nonSystem;
        
        WidgetFactory* widgetFactory = Application::Get()->GetWidgetFactory();
        
        auto resourcePool = CS::Application::Get()->GetResourcePool();
        auto pointerTemplate = resourcePool->LoadResource<CS::WidgetTemplate>(appConfig->GetDefaultCursorUILocation(), appConfig->GetDefaultCursorUIPath());
        m_cursor = widgetFactory->Create(pointerTemplate);
        m_cursor->SetCanvas(m_cursor.get());
        
        m_pointerSystem = Application::Get()->GetSystem<PointerSystem>();
        
        Pointer pointer;
        bool pointerExists = m_pointerSystem->TryGetPointerWithIndex(0, pointer);
        m_cursor->SetAbsolutePosition(pointer.GetPosition());
        m_cursor->SetVisible(m_enabled && pointerExists);
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::OnPointerAdded(const Pointer& pointer, f64 timestamp) noexcept
    {
        m_cursor->OnPointerAdded(pointer, timestamp);
        m_cursor->SetVisible(m_enabled);
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::OnPointerMoved(const Pointer& pointer, f64 timestamp) noexcept
    {
        m_cursor->OnPointerMoved(pointer, timestamp);
        m_cursor->SetAbsolutePosition(pointer.GetPosition());
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::OnPointerRemoved(const Pointer& pointer, f64 timestamp) noexcept
    {
        m_cursor->OnPointerRemoved(pointer, timestamp);
        
        if(m_pointerSystem->GetNumPointers() == 0)
        {
            m_cursor->SetVisible(false);
        }
    }

    //------------------------------------------------------------------------------
    void CursorSystem::OnResume() noexcept
    {
        m_pointerAddedConnection = m_pointerSystem->GetPointerAddedEvent().OpenConnection(MakeDelegate(this, &CursorSystem::OnPointerAdded));
        m_pointerMovedConnection = m_pointerSystem->GetPointerMovedEvent().OpenConnection(MakeDelegate(this, &CursorSystem::OnPointerMoved));
        m_pointerRemovedConnection = m_pointerSystem->GetPointerRemovedEvent().OpenConnection(MakeDelegate(this, &CursorSystem::OnPointerRemoved));
        
        m_cursor->OnResume();
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::OnForeground() noexcept
    {
        m_cursor->OnForeground();
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::OnUpdate(f32 timeSinceLastUpdate) noexcept
    {
        m_cursor->OnUpdate(timeSinceLastUpdate);
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::Draw(CanvasRenderer* renderer) const noexcept
    {
        m_cursor->OnDraw(renderer);
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::OnBackground() noexcept
    {
        m_cursor->OnBackground();
    }
    
    //------------------------------------------------------------------------------
    void CursorSystem::OnSuspend() noexcept
    {
        m_cursor->OnSuspend();
        
        m_pointerAddedConnection.reset();
        m_pointerMovedConnection.reset();
        m_pointerRemovedConnection.reset();
    }

    //------------------------------------------------------------------------------
    void CursorSystem::OnDestroy() noexcept
    {
        m_cursor.reset();
        m_pointerRemovedConnection.reset();
        m_pointerMovedConnection.reset();
        m_pointerAddedConnection.reset();
    }
}
