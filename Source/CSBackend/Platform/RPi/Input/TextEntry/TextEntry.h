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

#ifndef _CSBACKEND_PLATFORM_RPI_INPUT_TEXTENTRY_TEXTENTRY_H_
#define _CSBACKEND_PLATFORM_RPI_INPUT_TEXTENTRY_TEXTENTRY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>
#include <ChilliSource/Input/TextEntry/TextEntry.h>
#include <ChilliSource/Input/TextEntry/TextEntryCapitalisation.h>
#include <ChilliSource/Input/TextEntry/TextEntryType.h>
#include <CSBackend/Platform/RPi/ForwardDeclarations.h>

#include <mutex>

namespace CSBackend
{
	namespace RPi
	{
        /// The Raspberry Pi backend to the text entry system. Listens to X keyboard events
		/// and builds a buffer of text input.
		///
		/// NOTE: Does not use any of the capitalisation policies.
        ///
        /// The methods in this class are not thread-safe and must be called
        /// from the main thread.
        ///
		class TextEntry final : public ChilliSource::TextEntry
		{
		public:
			CS_DECLARE_NAMEDTYPE(TextEntry);

			/// @param interfaceId
			///			Id of the interface that we wish to check if this class implements
			///
			/// @return TRUE if the class implements the given interface
			///
			bool IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept override;

			/// Calling this will make the instance listen for text input. Changes to the
			/// input buffer are fed through the given delegate.
			///
			/// @param text
			///		Initial buffer contents
			/// @param type
			///		Keyboard type (not used on RPi)
			/// @param capitalisation
			///		Not used on RPi
			/// @param changeDelegate
			///		Delegate called when the text buffer changes
			/// @param deactivateDelegate
			/// 	Called when text entry is ended
			///
			void Activate(const std::string& text, ChilliSource::TextEntryType type, ChilliSource::TextEntryCapitalisation capitalisation, const TextBufferChangedDelegate& changeDelegate, const TextInputDeactivatedDelegate& deactivateDelegate) noexcept override;

			/// Stops the instance receiving text input events from X server
			///
			void Deactivate() noexcept override;

			/// @return TRUE if text input is currently enabled
			///
			bool IsActive() const noexcept override { return m_active; }

			/// @return Current text input buffer (UTF-8)
			///
			const std::string& GetTextBuffer() const noexcept override;

			/// @param text
			///		Text to override the current input buffer
			///
            void SetTextBuffer(const std::string& text) noexcept override;

		private:
            friend ChilliSource::TextEntryUPtr ChilliSource::TextEntry::Create();

			///
			TextEntry() = default;

			/// Called from X when keyboard input is received. Builds the input buffer.
			///
			/// @param character
			///		Utf8-character
			///
			void OnTextEntered(ChilliSource::UTF8Char character) noexcept;

		private:

            TextBufferChangedDelegate m_textBufferChangedDelegate;
            TextInputDeactivatedDelegate m_textInputDeactivatedDelegate;

            std::mutex m_mutex;

            bool m_active = false;
			std::string m_text;
		};
	}
}

#endif

#endif
