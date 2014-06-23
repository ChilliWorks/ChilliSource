//
//  DialogueBoxSystem.h
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

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_PLATFORM_CORE_DIALOGUE_DIALOGUESYSTEM_H_
#define _CSBACKEND_PLATFORM_CORE_DIALOGUE_DIALOGUESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/DialogueBox/DialogueBoxSystem.h>

@class DialogueBoxListener;

namespace CSBackend
{
	namespace iOS
	{
		//-----------------------------------------------------------
		/// A system used to display platform specific system
        /// dialogue boxes.
		//-----------------------------------------------------------
		class DialogueBoxSystem final : public CSCore::DialogueBoxSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(DialogueBoxSystem);
            //----------------------------------------------------
            /// Is A
            ///
            /// @return Whether this implements the passed in
            /// interface id.
            //-----------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //-----------------------------------------------------
            /// Display a system dialog with the given ID and delegate
            ///
            /// @author Ian Copland
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text (UTF-8)
            /// @param Message text (UTF-8)
            /// @param Confirm text (UTF-8)
            //------------------------------------------------------
            void ShowSystemDialogue(u32 in_id, const CSCore::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm) override;
            //-----------------------------------------------------
            /// Display a system confirmation dialog with the given
            /// ID and delegate.
            ///
            /// @author Ian Copland
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text (UTF-8)
            /// @param Message text (UTF-8)
            /// @param Confirm text (UTF-8)
            /// @param Cancel text (UTF-8)
            //-----------------------------------------------------
            void ShowSystemConfirmDialogue(u32 in_id, const CSCore::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel) override;
            //-----------------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author Ian Copland
            ///
            /// @param The text to display (UTF-8).
            //-----------------------------------------------------
            void MakeToast(const std::string& in_text) override;
            //------------------------------------------------------
            /// Triggered from a system dialog confirmation event
            ///
            /// @author Ian Copland
            ///
            /// @param ID
            /// @param Result
            //------------------------------------------------------
            void OnSystemConfirmDialogResult(u32 in_id, CSCore::DialogueBoxSystem::DialogueResult in_result);
            //----------------------------------------------------
			/// Destructor.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
            ~DialogueBoxSystem();
            
        private:
            
            friend CSCore::DialogueBoxSystemUPtr CSCore::DialogueBoxSystem::Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            DialogueBoxSystem();
        private:
            
			CSCore::DialogueBoxSystem::DialogueDelegate m_activeSysConfirmDelegate;
            DialogueBoxListener* m_listener;
		};
	}
}

#endif

#endif