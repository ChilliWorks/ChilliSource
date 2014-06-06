//
//  DialogueBoxSystem.mm
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxSystem.h>

#import <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxListener.h>
#import <CSBackend/Platform/iOS/Core/Notification/ToastNotification.h>
#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Base/PlatformSystem.h>

#import <UIKit/UIKit.h>

namespace ChilliSource
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
        bool DialogueBoxSystem::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return (DialogueBoxSystem::InterfaceID == in_interfaceID || Core::DialogueBoxSystem::InterfaceID == in_interfaceID);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::ShowSystemDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm)
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
        void DialogueBoxSystem::ShowSystemConfirmDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel)
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
            NSString* text = [NSStringUtils newNSStringWithUTF8String:in_text];
            ToastNotification* pToast = [[ToastNotification alloc] initWithMessage:text];
            [[UIApplication sharedApplication].keyWindow.rootViewController.view addSubview:pToast];
            [pToast animateIn];
            [text release];
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void DialogueBoxSystem::OnSystemConfirmDialogResult(u32 in_id, Core::DialogueBoxSystem::DialogueResult in_result)
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