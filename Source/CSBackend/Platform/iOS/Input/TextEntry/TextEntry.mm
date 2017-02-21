//
//  TextEntry.mm
//  ChilliSource
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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Input/TextEntry/TextEntry.h>

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#import <CSBackend/Platform/iOS/Input/TextEntry/TextEntryDelegate.h>
#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Threading/TaskScheduler.h>
#import <ChilliSource/Core/String/StringUtils.h>

#import <UIKit/UIKit.h>

namespace CSBackend
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(TextEntry);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        TextEntry::TextEntry()
        {
            m_delegate = [[TextEntryDelegate alloc] initWithTextEntry:this];
            
            //Just create it with garbage size as we will never see it
            m_textView = [[UITextField alloc] initWithFrame:CGRectMake(200, 10, 100, 200)];
            [m_textView setReturnKeyType:UIReturnKeyDone];
            m_textView.autocorrectionType = UITextAutocorrectionTypeNo;
            m_textView.autocapitalizationType = UITextAutocapitalizationTypeNone;
            m_textView.keyboardType = UIKeyboardTypeASCIICapable;
            m_textView.delegate = m_delegate;
            m_textView.hidden = YES;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool TextEntry::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == ChilliSource::TextEntry::InterfaceID || in_interfaceId == TextEntry::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextEntry::Activate(const std::string& in_text, ChilliSource::TextEntryType in_type, ChilliSource::TextEntryCapitalisation in_capitalisation, const TextBufferChangedDelegate& in_changeDelegate, const TextInputDeactivatedDelegate& in_deactivateDelegate)
        {
            m_isActive = true;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                if (m_isViewSetup == false && [m_textView canBecomeFirstResponder])
                {
                    m_isViewSetup = true;
                    
                    SetType(in_type);
                    SetCapitalisation(in_capitalisation);
                    SetTextBuffer(in_text);
                    m_textBufferChangedDelegate = in_changeDelegate;
                    m_textInputDeactivatedDelegate = in_deactivateDelegate;
                    
                    [[UIApplication sharedApplication].keyWindow.rootViewController.view addSubview:m_textView];
                    [m_textView becomeFirstResponder];
                }
            });
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextEntry::Deactivate()
        {
            m_isActive = false;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                if (m_isViewSetup == true)
                {
                    m_isViewSetup = false;
                    
                    [m_textView resignFirstResponder];
                    [m_textView removeFromSuperview];
                    
                    ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
                    {
                        if(m_textInputDeactivatedDelegate != nullptr)
                        {
                            auto delegate = m_textInputDeactivatedDelegate;
                            m_textInputDeactivatedDelegate = nullptr;
                            delegate();
                        }
                    });
                }
            });
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool TextEntry::IsActive() const
        {
            return m_isActive;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const std::string& TextEntry::GetTextBuffer() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            return m_text;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextEntry::SetTextBuffer(const std::string& in_text)
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                m_text = in_text;
                NSString* text = [NSStringUtils newNSStringWithUTF8String:m_text];
                m_textView.text = text;
                [text release];
            });
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextEntry::SetType(ChilliSource::TextEntryType in_type)
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                if(m_textView != nullptr)
                {
                    switch (in_type)
                    {
                        case ChilliSource::TextEntryType::k_text:
                            m_textView.keyboardType = UIKeyboardTypeASCIICapable;
                            break;
                        case ChilliSource::TextEntryType::k_numeric:
                            m_textView.keyboardType = UIKeyboardTypeNumberPad;
                            break;
                        default:
                            CS_LOG_ERROR("Invalid keyboard type passed to keyboard!");
                            m_textView.keyboardType = UIKeyboardTypeASCIICapable;
                            break;
                    }
                }
            });
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextEntry::SetCapitalisation(ChilliSource::TextEntryCapitalisation in_capitalisation)
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                if(m_textView != nullptr)
                {
                    switch (in_capitalisation)
                    {
                        case ChilliSource::TextEntryCapitalisation::k_none:
                            m_textView.autocapitalizationType = UITextAutocapitalizationTypeNone;
                            break;
                        case ChilliSource::TextEntryCapitalisation::k_words:
                            m_textView.autocapitalizationType = UITextAutocapitalizationTypeWords;
                            break;
                        case ChilliSource::TextEntryCapitalisation::k_sentences:
                            m_textView.autocapitalizationType = UITextAutocapitalizationTypeSentences;
                            break;
                        case ChilliSource::TextEntryCapitalisation::k_all:
                            m_textView.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
                            break;
                        default:
                            CS_LOG_WARNING("Unknown Capitalisation Method");
                            m_textView.autocapitalizationType = UITextAutocapitalizationTypeNone;
                            break;
                    }
                }
            });
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void TextEntry::OnTextUpdated(NSString* in_text)
        {
            auto text = [NSStringUtils newUTF8StringWithNSString:in_text];
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
                bool acceptText = true;
                    
                if(m_textBufferChangedDelegate != nullptr)
                {
                    acceptText = m_textBufferChangedDelegate(text);
                }
                
                if(acceptText == true)
                {
                    SetTextBuffer(text);
                }
            });
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        TextEntry::~TextEntry()
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
	}
}

#endif
