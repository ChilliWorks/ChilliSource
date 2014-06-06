//
//  Keyboard.mm
//  Chilli Source
//  Created by Scott Downie on 26/11/2010
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_IOS

#include <ChilliSource/Backend/Platform/iOS/Input/Keyboard/Keyboard.h>

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Backend/Platform/iOS/Input/Keyboard/VirtualKeyboardDelegate.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <UIKit/UIKit.h>

namespace ChilliSource
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(Keyboard);
        //-------------------------------------------------------
        //-------------------------------------------------------
        Keyboard::Keyboard()
        : m_enabled(false), m_textView(nil), m_delegate(nil)
        {
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Keyboard::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return (in_interfaceID == Input::Keyboard::InterfaceID || in_interfaceID == Keyboard::InterfaceID);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Keyboard::OnInit()
        {
            m_delegate = [[VirtualKeyboardDelegate alloc] initWithKeyboard:this];
            
            //Just create it with garbage size as we will never see it
            m_textView = [[UITextField alloc] initWithFrame:CGRectMake(200, 10, 100, 200)];
            [m_textView setReturnKeyType:UIReturnKeyDone];
            m_textView.autocorrectionType = UITextAutocorrectionTypeNo;
            m_textView.autocapitalizationType = UITextAutocapitalizationTypeNone;
            m_textView.keyboardType = UIKeyboardTypeASCIICapable;
            m_textView.delegate = m_delegate;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Keyboard::SetTextInputEnabled(bool in_enabled)
        {
            if (in_enabled == true && m_enabled == false && [m_textView canBecomeFirstResponder])
            {
                //Show the keyboard!
                [[UIApplication sharedApplication].keyWindow.rootViewController.view addSubview:m_textView];
				m_textView.hidden = YES;
				[m_textView becomeFirstResponder];
				m_enabled = true;
				
				//Notify our listeners
				m_textInputEnabledDelegate.NotifyConnections();
            }
            else if (in_enabled == false && m_enabled == true)
            {
                [m_textView resignFirstResponder];
				[m_textView removeFromSuperview];
				m_enabled = false;
				
				//Notify our listeners
				m_textInputDisabledDelegate.NotifyConnections();
				
				m_text.clear();
				m_textView.text = @"";
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
            NSString* text = [NSStringUtils newNSStringWithUTF8String:m_text];
            m_textView.text = text;
            [text release];
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Keyboard::SetType(Type in_type)
        {
            if(m_textView != nullptr)
            {
                switch (in_type)
                {
                    case Input::Keyboard::Type::k_text:
                        m_textView.keyboardType = UIKeyboardTypeASCIICapable;
                        break;
                    case Input::Keyboard::Type::k_numeric:
                        m_textView.keyboardType = UIKeyboardTypeNumberPad;
                        break;
                    default:
                        CS_LOG_ERROR("Invalid keyboard type passed to keyboard!");
                        m_textView.keyboardType = UIKeyboardTypeASCIICapable;
                        break;
                }
            }
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Keyboard::SetCapitalisation(Capitalisation in_capitalisation)
        {
            if(m_textView != nullptr)
            {
                switch (in_capitalisation)
                {
                    case Input::Keyboard::Capitalisation::k_none:
                        m_textView.autocapitalizationType = UITextAutocapitalizationTypeNone;
                        break;
                    case Input::Keyboard::Capitalisation::k_words:
                        m_textView.autocapitalizationType = UITextAutocapitalizationTypeWords;
                        break;
                    case Input::Keyboard::Capitalisation::k_sentences:
                        m_textView.autocapitalizationType = UITextAutocapitalizationTypeSentences;
                        break;
                    case Input::Keyboard::Capitalisation::k_all:
                        m_textView.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
                        break;
                    default:
                        CS_LOG_WARNING("Unknown Capitalisation Method");
                        m_textView.autocapitalizationType = UITextAutocapitalizationTypeNone;
                        break;
                }
            }
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Core::IConnectableEvent<Keyboard::KeyboardEventDelegate>& Keyboard::GetTextInputEnabledEvent()
        {
            return m_textInputEnabledDelegate;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Core::IConnectableEvent<Keyboard::TextInputEventDelegate>& Keyboard::GetTextInputReceivedEvent()
        {
            return m_textInputReceivedDelegate;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Core::IConnectableEvent<Keyboard::KeyboardEventDelegate>& Keyboard::GetTextInputDisabledEvent()
        {
            return m_textInputDisabledDelegate;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Keyboard::OnTextUpdated(NSString* in_text)
        {
            //Inform subscripted to this event and receive if text was rejected lower down
            bool rejectText = false;
            
            auto text = [NSStringUtils newUTF8StringWithNSString:in_text];
            m_textInputReceivedDelegate.NotifyConnections(text, &rejectText);
            
            if(rejectText == false)
            {
                m_text = text;
            }
            
            return !rejectText;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Keyboard::OnDestroy()
        {
            if(m_textView != nullptr)
			{
				[m_textView release];
				m_textView = nil;
			}
            
            if(m_delegate != nullptr)
            {
                [m_delegate release];
                m_delegate = nil;
            }
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Keyboard::~Keyboard()
        {
            OnDestroy();
        }
	}
}

#endif