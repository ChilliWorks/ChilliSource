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

#ifndef _CHILLISOURCE_UI_BASE_CURSOR_H_
#define _CHILLISOURCE_UI_BASE_CURSOR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    /// Used to render a UI (non-system) cursor on top of the active UI canvas.
    /// The cursor uses the PointerSystem to determine the current location to render at.
    ///
    /// NOTE: The cursor will track movement by any pointer. So if you have more than one mouse attached
    /// the cursor will be moved by each mouse
    ///
    class CursorSystem final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(CursorSystem);
        
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;

        /// Makes the cursor visible (if a pointer is attached)
        ///
        void ShowCursor();
        
        /// Makes the cursor invisible
        ///
        void HideCursor();
        
        /// @param cursorWidget
        ///     The widget that will be rendered as the pointer cursor
        ///
        void SetCursorWidget(WidgetUPtr cursorWidget) noexcept { m_cursor = std::move(cursorWidget); }
        
    private:
        friend class Application;
        friend class CanvasRenderer;
        
        ///
        static CursorSystemUPtr Create() noexcept;
        
        ///
        CursorSystem() = default;

        /// Forwarded onto cursor widget. Creates the default cursor
        ///
        void OnInit() noexcept override;

        /// Forwarded onto cursor widget
        ///
        void OnResume() noexcept override;

        /// Forwarded onto cursor widget
        ///
        void OnForeground() noexcept override;

        /// Forwarded onto cursor widget
        ///
        void OnUpdate(f32 timeSinceLastUpdate) noexcept override;

        /// Forwarded onto cursor widget
        ///
        void OnBackground() noexcept override;

        /// Forwarded onto cursor widget
        ///
        void OnSuspend() noexcept override;

        /// Forwarded onto cursor widget
        ///
        void OnDestroy() noexcept override;
        
        /// Draw the cursor widget using the canvas renderer.
        ///
        /// @param renderer
        ///     Canvas renderer used by the widget and children to draw themselves
        ///
        void Draw(CanvasRenderer* renderer) const noexcept;

        /// Called when a new pointer is attached. If this is the first pointer
        /// then the cursor is shown
        ///
        /// @param pointer
        ///     Pointer added
        /// @param timestamp
        ///     Time at which the pointer was added
        ///
        void OnPointerAdded(const Pointer& pointer, f64 timestamp) noexcept;

        /// Will move the cursor to the location of the pointer
        ///
        /// @param pointer
        ///     Pointer moved
        /// @param timestamp
        ///     Time at which the pointer was moved
        ///
        void OnPointerMoved(const Pointer& pointer, f64 timestamp) noexcept;

        /// Called when a pointer is detached. If this is the last pointer
        /// then the cursor is hidden
        ///
        /// @param pointer
        ///     Pointer removed
        /// @param timestamp
        ///     Time at which the pointer was removed
        ///
        void OnPointerRemoved(const Pointer& in_pointer, f64 in_timestamp) noexcept;
        
    private:
        
        WidgetUPtr m_cursor;
        EventConnectionUPtr m_pointerAddedConnection;
        EventConnectionUPtr m_pointerMovedConnection;
        EventConnectionUPtr m_pointerRemovedConnection;
        
        bool m_enabled = false;
        
        PointerSystem* m_pointerSystem;
	};
}

#endif
