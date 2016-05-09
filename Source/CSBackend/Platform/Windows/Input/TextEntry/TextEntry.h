//
//  TextEntry.h
//  Chilli Source
//  Created by Scott Downie on 08/07/2014
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#ifndef _CSBACKEND_PLATFORM_WINDOWS_INPUT_TEXTENTRY_TEXTENTRY_H_
#define _CSBACKEND_PLATFORM_WINDOWS_INPUT_TEXTENTRY_TEXTENTRY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>
#include <ChilliSource/Input/TextEntry/TextEntry.h>
#include <CSBackend/Platform/Windows/ForwardDeclarations.h>

namespace CSBackend
{
	namespace Windows
	{
        //----------------------------------------------------------------
        /// The iOS backend to the text entry system. This provides access
        /// to the iOS virtual keyboard functionality via a text buffer
        ///
        /// @author S Downie
        //----------------------------------------------------------------
		class TextEntry final : public ChilliSource::TextEntry
		{
		public:
			CS_DECLARE_NAMEDTYPE(TextEntry);
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface ID
			///
			/// @return Whether this system implements the given inteface
			//-------------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
			//-------------------------------------------------------
			/// The system will now receive text input.
			///
			/// @author S Downie
			///
			/// @param Initial buffer contents
			/// @param Preferred keyboard type
			/// @param Preferred capitalisation method
			/// @param Text changed delegate
			/// @param Deactivate delegate
			//-------------------------------------------------------
			void Activate(const std::string& in_text, Type in_type, Capitalisation in_capitalisation, const TextBufferChangedDelegate& in_changeDelegate, const TextInputDeactivatedDelegate& in_deactivateDelegate) override;
			//-------------------------------------------------------
			/// The system will no longer receive text input.
			///
			/// @author S Downie
			//-------------------------------------------------------
			void Deactivate() override;
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether or not text input is currently
			/// enabled.
			//-------------------------------------------------------
			bool IsActive() const override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The current text input buffer (UTF-8).
            //-------------------------------------------------------
			const std::string& GetTextBuffer() const override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The new text input buffer (UTF-8).
            //-------------------------------------------------------
            void SetTextBuffer(const std::string& in_text) override;

		private:
            friend ChilliSource::TextEntryUPtr ChilliSource::TextEntry::Create();
        
            //-------------------------------------------------------
            /// Constructor. Declared private to force the use of the
            /// factory method.
            ///
            /// @author S Downie
            //-------------------------------------------------------
			TextEntry() = default;
			//-------------------------------------------------------
			/// Called when a text character is entered
			///
			/// @author S Downie
			///
			/// @param UTF-8 character
			//-------------------------------------------------------
			void OnTextEntered(ChilliSource::UTF8Char in_unicodeChar);

		private:
            
            TextBufferChangedDelegate m_textBufferChangedDelegate;
            TextInputDeactivatedDelegate m_textInputDeactivatedDelegate;

			ChilliSource::EventConnectionUPtr m_textEnteredConnection;

			std::string m_text;
		};
	}
}

#endif

#endif