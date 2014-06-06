//
//  DialogueBoxSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <ChilliSource/Backend/Platform/Windows/Core/DialogueBox/DialogueBoxSystem.h>

#include <ChilliSource/Backend/Platform/Windows/Core/String/WindowsStringUtils.h>
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
        bool DialogueBoxSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (DialogueBoxSystem::InterfaceID == in_interfaceId || Core::DialogueBoxSystem::InterfaceID == in_interfaceId);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
		void DialogueBoxSystem::ShowSystemDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm)
        {
			MessageBox(nullptr, WindowsStringUtils::UTF8ToUTF16(in_title).c_str(), WindowsStringUtils::UTF8ToUTF16(in_message).c_str(), MB_OK);
			if (in_delegate)
			{
				in_delegate(in_id, DialogueResult::k_confirm);
			}
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
		void DialogueBoxSystem::ShowSystemConfirmDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel)
        {
			if (MessageBox(NULL, WindowsStringUtils::UTF8ToUTF16(in_title).c_str(), WindowsStringUtils::UTF8ToUTF16(in_message).c_str(), MB_OKCANCEL) == IDOK)
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
		void DialogueBoxSystem::MakeToast(const std::string& in_text)
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

#endif