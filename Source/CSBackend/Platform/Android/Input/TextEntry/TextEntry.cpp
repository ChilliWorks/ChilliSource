//
//  TextEntry.cpp
//  Chilli Source
//  Created by Scott Downie on 08/07/2014.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Input/TextEntry/TextEntry.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Input/TextEntry/TextEntryJavaInterface.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>

namespace CSBackend
{
	namespace Android
	{
		//-------------------------------------------------------
		//-------------------------------------------------------
		TextEntry::TextEntry()
		{
			m_textEntryJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<TextEntryJavaInterface>();
			if (m_textEntryJI == nullptr)
			{
				m_textEntryJI = TextEntryJavaInterfaceSPtr(new TextEntryJavaInterface());
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_textEntryJI);
			}

			m_textEntryJI->SetTextAddedDelegate(CSCore::MakeDelegate(this, &TextEntry::OnTextAdded));
			m_textEntryJI->SetTextDeletedDelegate(CSCore::MakeDelegate(this, &TextEntry::OnTextDeleted));
			m_textEntryJI->SetKeyboardDismissedDelegate(CSCore::MakeDelegate(this, &TextEntry::OnKeyboardDismissed));
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::SetTextInputEnabled(bool in_enabled)
		{
			if (in_enabled == true && m_enabled == false)
			{
				m_textEntryJI->Activate();
				m_enabled = true;
			}
			else if (in_enabled == false && m_enabled == true)
			{
				m_textEntryJI->Deactivate();
				m_enabled = false;
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool TextEntry::IsTextInputEnabled() const
		{
			return m_enabled;
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
		void TextEntry::SetType(Type in_type)
		{
			m_textEntryJI->SetKeyboardType(in_type);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::SetCapitalisation(Capitalisation in_capitalisation)
		{
			m_textEntryJI->SetCapitalisationMethod(in_capitalisation);
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextEntry::SetTextBufferChangedDelegate(const TextBufferChangedDelegate& in_delegate)
        {
        	m_textBufferChangedDelegate = in_delegate;
        }
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::OnTextAdded(const std::string& in_text)
		{
            bool acceptText = true;
            auto text = m_text + in_text;

            if(m_textBufferChangedDelegate != nullptr)
            {
                acceptText = m_textBufferChangedDelegate(text);
            }

            if(acceptText == true)
            {
                m_text = text;
            }
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::OnTextDeleted()
		{
            bool acceptText = true;
            std::string text;
			if (m_text.size() > 0)
			{
				s32 newLength = std::max((s32)(CSCore::UTF8StringUtils::CalcLength(m_text.begin(), m_text.end())) - 1, 0);
				text = CSCore::UTF8StringUtils::SubString(m_text, 0, newLength);
			}

            if(m_textBufferChangedDelegate != nullptr)
            {
                acceptText = m_textBufferChangedDelegate(text);
            }

            if(acceptText == true)
            {
                m_text = text;
            }
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::OnKeyboardDismissed()
		{
			if (m_enabled == true)
			{
				m_enabled = false;
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		TextEntry::~TextEntry()
		{
			if (m_textEntryJI != nullptr)
			{
				m_textEntryJI->SetTextAddedDelegate(nullptr);
				m_textEntryJI->SetTextDeletedDelegate(nullptr);
				m_textEntryJI->SetKeyboardDismissedDelegate(nullptr);
				m_textEntryJI.reset();
			}
		}
	}
}

#endif
