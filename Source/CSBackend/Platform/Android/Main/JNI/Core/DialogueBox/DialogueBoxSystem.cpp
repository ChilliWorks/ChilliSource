//
//  DialogueBoxSystem.cpp
//  ChilliSource
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Main/JNI/Core/DialogueBox/DialogueBoxSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/DialogueBox/DialogueBoxJavaInterface.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace Android
	{
        CS_DEFINE_NAMEDTYPE(DialogueBoxSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        DialogueBoxSystem::DialogueBoxSystem()
        {
        	m_dialogueBoxJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<DialogueBoxJavaInterface>();
        	if (m_dialogueBoxJI == nullptr)
        	{
        		m_dialogueBoxJI = std::make_shared<DialogueBoxJavaInterface>();
        		JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_dialogueBoxJI);
        	}
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
            CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "System Dialogue requested outside of main thread.");
            m_dialogueBoxJI->ShowSystemDialogue(in_id, in_title, in_message, in_confirm);
            m_activeSysConfirmDelegate = in_delegate;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::ShowSystemConfirmDialogue(u32 in_id, const ChilliSource::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel)
        {
            CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "System Confirm Dialogue requested outside of main thread.");
            m_dialogueBoxJI->ShowSystemConfirmDialogue(in_id, in_title, in_message, in_confirm, in_cancel);
            m_activeSysConfirmDelegate = in_delegate;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void DialogueBoxSystem::MakeToast(const std::string& in_text)
        {
            CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Tried to make toast outside of main thread.");
            m_dialogueBoxJI->MakeToast(in_text);   
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void DialogueBoxSystem::OnSystemConfirmDialogueResult(u32 in_id, ChilliSource::DialogueBoxSystem::DialogueResult in_result)
        {
            if (m_activeSysConfirmDelegate)
            {
                m_activeSysConfirmDelegate(in_id, in_result);
                m_activeSysConfirmDelegate = nullptr;
            }
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        DialogueBoxSystem::~DialogueBoxSystem()
        {
        }
	}
}

#endif
