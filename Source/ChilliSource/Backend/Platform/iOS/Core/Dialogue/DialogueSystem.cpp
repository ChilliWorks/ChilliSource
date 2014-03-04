//
//  DialogueSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/Dialogue/DialogueSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

namespace ChilliSource
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(DialogueSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        DialogueSystem::DialogueSystem()
        {
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool DialogueSystem::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return (DialogueSystem::InterfaceID == in_interfaceID || Core::DialogueSystem::InterfaceID == in_interfaceID);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueSystem::ShowSystemDialogue(u32 in_id, const Core::DialogueSystem::DialogueDelegate& in_delegate, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm)
        {
            Core::Application::Get()->GetPlatformSystemPtr()->ShowSystemDialog(in_id, in_title, in_message, in_confirm);
            m_activeSysConfirmDelegate = in_delegate;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueSystem::ShowSystemConfirmDialogue(u32 in_id, const Core::DialogueSystem::DialogueDelegate& in_delegate, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm, const Core::UTF8String& in_cancel)
        {
            Core::Application::Get()->GetPlatformSystemPtr()->ShowSystemConfirmDialog(in_id, in_title, in_message, in_confirm, in_cancel);
            m_activeSysConfirmDelegate = in_delegate;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueSystem::MakeToast(const Core::UTF8String& in_text)
        {
            Core::Application::Get()->GetPlatformSystemPtr()->MakeToast(in_text);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void DialogueSystem::OnSystemConfirmDialogResult(u32 in_id, Core::DialogueSystem::DialogueResult in_result)
        {
            if(m_activeSysConfirmDelegate)
        	{
        		m_activeSysConfirmDelegate(in_id, in_result);
        		m_activeSysConfirmDelegate = nullptr;
        	}
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        DialogueSystem::~DialogueSystem()
        {
        }
	}
}
