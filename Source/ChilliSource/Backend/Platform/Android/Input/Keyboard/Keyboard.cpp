//
//  Keyboard.cpp
//  Chilli Source
//
//  Created by Ian Copland on 31/12/2011
//  Copyright (c)2010 Tag Games Limited. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Input/Keyboard/Keyboard.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Input/Keyboard/KeyboardJavaInterface.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Utils.h>

namespace ChilliSource
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
		bool Keyboard::IsA(Core::InterfaceIDType in_interfaceID) const
		{
			return (Input::Keyboard::InterfaceID == in_interfaceID || Keyboard::InterfaceID == in_interfaceID);
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
		const Core::UTF8String& Keyboard::GetText() const
		{
			return m_text;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::SetText(const Core::UTF8String& in_text)
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
		Core::IConnectableEvent<Keyboard::KeyboardEventDelegate>& Keyboard::GetTextInputEnabledEvent()
		{
			return m_textInputEnabledEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::IConnectableEvent<Keyboard::TextInputEventDelegate>& Keyboard::GetTextInputReceivedEvent()
		{
			return m_textInputReceivedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::IConnectableEvent<Keyboard::KeyboardEventDelegate>& Keyboard::GetTextInputDisabledEvent()
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

			m_keyboardJI->SetTextAddedDelegate(Core::MakeDelegate(this, &Keyboard::OnTextAdded));
			m_keyboardJI->SetTextDeletedDelegate(Core::MakeDelegate(this, &Keyboard::OnTextDeleted));
			m_keyboardJI->SetKeyboardDismissedDelegate(Core::MakeDelegate(this, &Keyboard::OnKeyboardDismissed));
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnTextAdded(const Core::UTF8String& in_text)
		{
			Core::UTF8String newText = m_text + in_text;

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
			Core::UTF8String newText = m_text;
			if (newText.size() > 0)
			{
				newText = m_text.substr(0, m_text.length() - 1);
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
