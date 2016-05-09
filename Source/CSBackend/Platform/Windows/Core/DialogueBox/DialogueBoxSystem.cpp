//
//  DialogueBoxSystem.cpp
//  Chilli Source
//  Created by Ian Copland on 04/03/2014.
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Core/DialogueBox/DialogueBoxSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>

#include <windows.h>

namespace CSBackend
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
        bool DialogueBoxSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
            return (DialogueBoxSystem::InterfaceID == in_interfaceId || ChilliSource::DialogueBoxSystem::InterfaceID == in_interfaceId);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
		void DialogueBoxSystem::ShowSystemDialogue(u32 in_id, const ChilliSource::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm)
        {
			MessageBox(SFMLWindow::Get()->GetWindowHandle(), WindowsStringUtils::UTF8ToUTF16(in_message).c_str(), WindowsStringUtils::UTF8ToUTF16(in_title).c_str(), MB_OK);
			if (in_delegate)
			{
				in_delegate(in_id, DialogueResult::k_confirm);
			}
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
		void DialogueBoxSystem::ShowSystemConfirmDialogue(u32 in_id, const ChilliSource::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel)
        {
			if (MessageBox(SFMLWindow::Get()->GetWindowHandle(), WindowsStringUtils::UTF8ToUTF16(in_message).c_str(), WindowsStringUtils::UTF8ToUTF16(in_title).c_str(), MB_OKCANCEL) == IDOK)
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