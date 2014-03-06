//
//  DialogueBoxSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Windows/Core/DialogueBox/DialogueBoxSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

#include <windows.h>

namespace ChilliSource
{
	namespace Windows
	{
        CS_DEFINE_NAMEDTYPE(DialogueBoxSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        DialogueBoxSystem::DialogueBoxSystem()
        {
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
			MessageBoxA(nullptr, in_title.ToASCII().c_str(), in_message.ToASCII().c_str(), MB_OK);
			if (in_delegate)
			{
				in_delegate(in_id, DialogueResult::k_confirm);
			}
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::ShowSystemConfirmDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm, const Core::UTF8String& in_cancel)
        {
			if (MessageBoxA(NULL, in_title.ToASCII().c_str(), in_message.ToASCII().c_str(), MB_OKCANCEL) == IDOK)
			{
				if (in_delegate)
				{
					in_delegate(in_id, DialogueResult::k_confirm);
				}
			}
			else
			{
				if (in_delegate)
				{
					in_delegate(in_id, DialogueResult::k_cancel);
				}
			}
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::MakeToast(const Core::UTF8String& in_text)
        {
			CS_LOG_WARNING("Toast not available on Windows");
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        DialogueBoxSystem::~DialogueBoxSystem()
        {
        }
	}
}
