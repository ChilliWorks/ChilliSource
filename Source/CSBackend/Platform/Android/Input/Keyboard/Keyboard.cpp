//
//  Keyboard.cpp
//  Chilli Source
//  Created by Ian Copland on 31/12/2011
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <CSBackend/Platform/Android/Input/Keyboard/Keyboard.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Input/Keyboard/KeyboardJavaInterface.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(Keyboard);
		//-------------------------------------------------------
		//-------------------------------------------------------
		Keyboard::Keyboard()
			: m_enabled(false)
		{
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool Keyboard::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (CSInput::Keyboard::InterfaceID == in_interfaceId || Keyboard::InterfaceID == in_interfaceId);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::SetTextInputEnabled(bool in_enabled)
		{
			if (in_enabled == true && m_enabled == false)
			{
				m_keyboardJI->Activate();
				m_enabled = true;
				m_textInputEnabledEvent.NotifyConnections();
			}
			else if (in_enabled == false && m_enabled == true)
			{
				m_keyboardJI->Deactivate();
				m_enabled = false;
				m_textInputDisabledEvent.NotifyConnections();
				m_text.clear();
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool Keyboard::IsTextInputEnabled() const
		{
			return m_enabled;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const std::string& Keyboard::GetText() const
		{
			return m_text;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::SetText(const std::string& in_text)
		{
			m_text = in_text;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::SetType(Type in_type)
		{
			m_keyboardJI->SetKeyboardType(in_type);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::SetCapitalisation(Capitalisation in_capitalisation)
		{
			m_keyboardJI->SetCapitalisationMethod(in_capitalisation);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<Keyboard::KeyboardEventDelegate>& Keyboard::GetTextInputEnabledEvent()
		{
			return m_textInputEnabledEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<Keyboard::TextInputEventDelegate>& Keyboard::GetTextInputReceivedEvent()
		{
			return m_textInputReceivedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<Keyboard::KeyboardEventDelegate>& Keyboard::GetTextInputDisabledEvent()
		{
			return m_textInputDisabledEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnInit()
		{
			m_keyboardJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<KeyboardJavaInterface>();
			if (m_keyboardJI == nullptr)
			{
				m_keyboardJI = KeyboardJavaInterfaceSPtr(new KeyboardJavaInterface());
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_keyboardJI);
			}

			m_keyboardJI->SetTextAddedDelegate(CSCore::MakeDelegate(this, &Keyboard::OnTextAdded));
			m_keyboardJI->SetTextDeletedDelegate(CSCore::MakeDelegate(this, &Keyboard::OnTextDeleted));
			m_keyboardJI->SetKeyboardDismissedDelegate(CSCore::MakeDelegate(this, &Keyboard::OnKeyboardDismissed));
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnTextAdded(const std::string& in_text)
		{
			std::string newText = m_text + in_text;

			bool rejectText = false;
			m_textInputReceivedEvent.NotifyConnections(newText, &rejectText);

			if(!rejectText)
			{
				m_text = newText;
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnTextDeleted()
		{
			std::string newText = m_text;
			if (newText.size() > 0)
			{
				s32 newLength = std::max((s32)(CSCore::UTF8StringUtils::CalcLength(m_text.begin(), m_text.end())) - 1, 0);
				newText = CSCore::UTF8StringUtils::SubString(m_text, 0, newLength);
			}

			bool rejectText = false;
			m_textInputReceivedEvent.NotifyConnections(newText, &rejectText);

			if(!rejectText)
			{
				m_text = newText;
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnKeyboardDismissed()
		{
			if (m_enabled == true)
			{
				m_enabled = false;
				m_textInputDisabledEvent.NotifyConnections();
				m_text.clear();
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnDestroy()
		{
			if (m_keyboardJI != nullptr)
			{
				m_keyboardJI->SetTextAddedDelegate(nullptr);
				m_keyboardJI->SetTextDeletedDelegate(nullptr);
				m_keyboardJI->SetKeyboardDismissedDelegate(nullptr);
				m_keyboardJI.reset();
			}
		}
	}
}

#endif