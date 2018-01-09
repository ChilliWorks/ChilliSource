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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_INPUT_POINTER_POINTERSYSTEM_H_
#define _CSBACKEND_PLATFORM_RPI_INPUT_POINTER_POINTERSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>
#include <CSBackend/Platform/RPi/Core/Base/DispmanWindow.h>

namespace CSBackend
{
	namespace RPi
	{
		/// Raspberry Pi mouse/pointer system. Access information on mouse state
		/// and location from X11
		///
		/// The methods in this class are not thread-safe and must be
		/// accessed from the main thread.
		///
		class PointerSystem final : public ChilliSource::PointerSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(PointerSystem);

			/// Queries whether or not this system implements the
			/// interface with the given Id.
			///
			/// @param interfaceId
			///		The id of the interface that we need to check if this system implements
			///
			/// @retrun TRUE if the system 'IsA' interface
			///
			bool IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept override;

			/// As X11 is behind the GL display our RPi implementation has no mouse cursor
			///
			void HideSystemCursor() noexcept override {}

			/// As X11 is behind the GL display our RPi implementation has no mouse cursor
			///
			void ShowSystemCursor() noexcept override {}

		private:
			friend class ChilliSource::PointerSystem;

			///
			PointerSystem() = default;

			/// Starts listening for mouse events
			///
			void OnInit() noexcept override;

			/// @param xPos
			///		Current position X in window coords
			/// @param yPos
			///		Current position Y in window coords
			///
			void OnMouseMoved(s32 xPos, s32 yPos) noexcept;

			/// @param button
			///		Button ID
			/// @param event
			///		Button action (Press/Release)
			///
			void OnMouseButtonEvent(u32 button, DispmanWindow::MouseButtonEvent event) noexcept;

			/// @param delta
			///		Number of ticks scrolled in the y-axis
			///
			void OnMouseWheeled(s32 delta) noexcept;

			/// Stop listening for mouse events
			///
			void OnDestroy() noexcept override;

			ChilliSource::Screen* m_screen;
			ChilliSource::Pointer::Id m_pointerId;
		};
	}
}

#endif

#endif
