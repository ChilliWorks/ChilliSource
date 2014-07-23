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

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(TextEntry);

		//-------------------------------------------------------
		//-------------------------------------------------------
		TextEntry::TextEntry()
		{
			m_textEntryJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<TextEntryJavaInterface>();
			if (m_textEntryJI == nullptr)
			{
				m_textEntryJI = TextEntryJavaInterfaceSPtr(new TextEntryJavaInterface());
				m_textEntryJI->SetTextChangedDelegate(CSCore::MakeDelegate(this, &TextEntry::OnTextChanged));
				m_textEntryJI->SetKeyboardDismissedDelegate(CSCore::MakeDelegate(this, &TextEntry::OnKeyboardDismissed));
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_textEntryJI);
			}
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool TextEntry::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == CSInput::TextEntry::InterfaceID || in_interfaceId == TextEntry::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
		void TextEntry::Activate(const std::string& in_text, Type in_type, Capitalisation in_capitalisation, const TextBufferChangedDelegate& in_changeDelegate, const TextInputDeactivatedDelegate& in_deactivateDelegate)
		{
			if (m_enabled == false)
			{
				m_textBufferChangedDelegate = in_changeDelegate;
	            m_textInputDeactivatedDelegate = in_deactivateDelegate;
				m_textEntryJI->SetCapitalisationMethod(in_capitalisation);
				m_textEntryJI->SetKeyboardType(in_type);
				m_textEntryJI->SetTextBuffer(in_text);

				m_textEntryJI->Activate();
				m_enabled = true;
			}
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
		void TextEntry::Deactivate()
		{
			if (m_enabled == true)
			{
				m_textEntryJI->Deactivate();
				m_enabled = false;

				if(m_textInputDeactivatedDelegate != nullptr)
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
			m_textEntryJI->SetTextBuffer(m_text);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::OnTextChanged(const std::string& in_text)
		{
			if(m_text == in_text)
				return;

            bool acceptText = true;

            if(m_textBufferChangedDelegate != nullptr)
            {
                acceptText = m_textBufferChangedDelegate(in_text);
            }

            if(acceptText == true)
            {
                m_text = in_text;
            }
            else
            {
            	m_textEntryJI->SetTextBuffer(m_text);
            }
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void TextEntry::OnKeyboardDismissed()
		{
			if (m_enabled == true)
			{
				m_enabled = false;

				if(m_textInputDeactivatedDelegate != nullptr)
				{
					auto delegate = m_textInputDeactivatedDelegate;
					m_textInputDeactivatedDelegate = nullptr;
					delegate();
				}
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		TextEntry::~TextEntry()
		{
			m_textEntryJI->SetTextChangedDelegate(nullptr);
			m_textEntryJI->SetKeyboardDismissedDelegate(nullptr);
			m_textEntryJI->Deactivate();
		}
	}
}

#endif
