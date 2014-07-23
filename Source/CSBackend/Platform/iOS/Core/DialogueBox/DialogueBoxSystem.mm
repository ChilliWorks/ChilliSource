//
//  DialogueBoxSystem.mm
//  Chilli Source
//  Created by Ian Copland on 04/03/2014
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

#import <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxSystem.h>

#import <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxListener.h>
#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Base/PlatformSystem.h>

#import <UIKit/UIKit.h>

namespace CSBackend
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(DialogueBoxSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        DialogueBoxSystem::DialogueBoxSystem()
        {
            m_listener = [[DialogueBoxListener alloc] initWithDialogueBoxSystem:this];
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool DialogueBoxSystem::IsA(CSCore::InterfaceIDType in_interfaceID) const
        {
            return (DialogueBoxSystem::InterfaceID == in_interfaceID || CSCore::DialogueBoxSystem::InterfaceID == in_interfaceID);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::ShowSystemDialogue(u32 in_id, const CSCore::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm)
        {
            NSString* title = [NSStringUtils newNSStringWithUTF8String:in_title];
            NSString* message = [NSStringUtils newNSStringWithUTF8String:in_message];
            NSString* confirm = [NSStringUtils newNSStringWithUTF8String:in_confirm];
            
            UIAlertView* pConfirm = [[UIAlertView alloc] initWithTitle:title message:message delegate:m_listener cancelButtonTitle:confirm otherButtonTitles:nil];
            
            pConfirm.tag = in_id;
            [pConfirm show];
            [pConfirm release];
            
            [title release];
            [message release];
            [confirm release];
            
            m_activeSysConfirmDelegate = in_delegate;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::ShowSystemConfirmDialogue(u32 in_id, const CSCore::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel)
        {
            NSString* title = [NSStringUtils newNSStringWithUTF8String:in_title];
            NSString* message = [NSStringUtils newNSStringWithUTF8String:in_message];
            NSString* confirm = [NSStringUtils newNSStringWithUTF8String:in_confirm];
            NSString* cancel = [NSStringUtils newNSStringWithUTF8String:in_cancel];
            
            UIAlertView* pConfirm = [[UIAlertView alloc] initWithTitle:title message:message delegate:m_listener cancelButtonTitle:cancel otherButtonTitles:confirm, nil];
            
            pConfirm.tag = in_id;
            [pConfirm show];
            [pConfirm release];
        
            [title release];
            [message release];
            [confirm release];
            [cancel release];
            
            m_activeSysConfirmDelegate = in_delegate;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::MakeToast(const std::string& in_text)
        {
            CS_LOG_WARNING("Dialogue Box System: Toast's are not supported on iOS.");
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void DialogueBoxSystem::OnSystemConfirmDialogResult(u32 in_id, CSCore::DialogueBoxSystem::DialogueResult in_result)
        {
            if(m_activeSysConfirmDelegate)
        	{
        		m_activeSysConfirmDelegate(in_id, in_result);
        		m_activeSysConfirmDelegate = nullptr;
        	}
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        DialogueBoxSystem::~DialogueBoxSystem()
        {
            [m_listener release];
        }
	}
}

#endif