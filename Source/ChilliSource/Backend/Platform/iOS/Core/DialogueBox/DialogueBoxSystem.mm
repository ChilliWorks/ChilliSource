//
//  DialogueBoxSystem.mm
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/DialogueBox/DialogueBoxSystem.h>

#include <ChilliSource/Backend/Platform/iOS/Core/DialogueBox/DialogueBoxListener.h>
#import <ChilliSource/Backend/Platform/iOS/Core/Notification/ToastNotification.h>
#import <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

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
        void DialogueBoxSystem::ShowSystemDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm)
        {
            NSString* title = Core::StringUtils::UTF8StringToNSString(in_title);
            NSString* message = Core::StringUtils::UTF8StringToNSString(in_message);
            NSString* confirm = Core::StringUtils::UTF8StringToNSString(in_confirm);
            
            UIAlertView* pConfirm = [[UIAlertView alloc] initWithTitle:title message:message delegate:m_listener cancelButtonTitle:confirm otherButtonTitles:nil];
            
            pConfirm.tag = in_id;
            [pConfirm show];
            [pConfirm release];
            
            m_activeSysConfirmDelegate = in_delegate;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::ShowSystemConfirmDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm, const Core::UTF8String& in_cancel)
        {
            NSString* title = Core::StringUtils::UTF8StringToNSString(in_title);
            NSString* message = Core::StringUtils::UTF8StringToNSString(in_message);
            NSString* confirm = Core::StringUtils::UTF8StringToNSString(in_confirm);
            NSString* cancel = Core::StringUtils::UTF8StringToNSString(in_cancel);
            
            UIAlertView* pConfirm = [[UIAlertView alloc] initWithTitle:title message:message delegate:m_listener cancelButtonTitle:cancel otherButtonTitles:confirm, nil];
            
            pConfirm.tag = in_id;
            [pConfirm show];
            [pConfirm release];
        
            m_activeSysConfirmDelegate = in_delegate;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::MakeToast(const Core::UTF8String& in_text)
        {
            ToastNotification* pToast = [[ToastNotification alloc] initWithMessage:Core::StringUtils::UTF8StringToNSString(in_text)];
            [[EAGLView sharedInstance] addSubview:pToast];
            [pToast animateIn];
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
