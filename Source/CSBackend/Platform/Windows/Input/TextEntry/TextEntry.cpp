//
//  TextEntry.cpp
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

#include <CSBackend/Platform/Windows/Input/TextEntry/TextEntry.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>

namespace CSBackend
{
	namespace Windows
	{
		CS_DEFINE_NAMEDTYPE(TextEntry);

		//-------------------------------------------------------
		//-------------------------------------------------------
		bool TextEntry::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ChilliSource::TextEntry::InterfaceID || in_interfaceId == TextEntry::InterfaceID;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::Activate(const std::string& in_text, Type in_type, Capitalisation in_capitalisation, const TextBufferChangedDelegate& in_changeDelegate, const TextInputDeactivatedDelegate& in_deactivateDelegate)
		{
			if (IsActive() == false)
			{
				m_text = in_text;
				m_textBufferChangedDelegate = in_changeDelegate;
				m_textInputDeactivatedDelegate = in_deactivateDelegate;
				m_textEnteredConnection = SFMLWindow::Get()->GetTextEnteredEvent().OpenConnection(ChilliSource::MakeDelegate(this, &TextEntry::OnTextEntered));
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::Deactivate()
		{
			if (IsActive() == true)
			{
				m_textEnteredConnection.reset();
				if (m_textInputDeactivatedDelegate != nullptr)
				{
					auto delegate = m_textInputDeactivatedDelegate;
					m_textInputDeactivatedDelegate = nullptr;
					delegate();
				}
			}
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool TextEntry::IsActive() const
        {
			return m_textEnteredConnection != nullptr;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const std::string& TextEntry::GetTextBuffer() const
        {
            return m_text;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextEntry::SetTextBuffer(const std::string& in_text)
        {
            m_text = in_text;
        }
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::OnTextEntered(ChilliSource::UTF8Char in_unicodeChar)
		{
			const ChilliSource::UTF8Char k_backspace = 8;

			std::string text;

			if (in_unicodeChar != k_backspace)
			{
				text = ChilliSource::UTF8StringUtils::AppendCopy(in_unicodeChar, m_text);
			}
			else
			{
				s32 length = (s32)ChilliSource::UTF8StringUtils::CalcLength(m_text.begin(), m_text.end());
				length = std::max(length-1, 0);
				text = ChilliSource::UTF8StringUtils::SubString(m_text, 0, (u32)length);
			}

			bool acceptText = true;

			if (m_textBufferChangedDelegate != nullptr)
			{
				acceptText = m_textBufferChangedDelegate(text);
			}

			if (acceptText == true)
			{
				m_text = text;
			}
		}
	}
}

#endif