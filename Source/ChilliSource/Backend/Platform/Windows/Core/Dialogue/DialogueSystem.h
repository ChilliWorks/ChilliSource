//
//  DialogueSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_DIALOGUE_DIALOGUESYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_DIALOGUE_DIALOGUESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/Dialogue/DialogueSystem.h>

namespace ChilliSource
{
	namespace Windows
	{
		//-----------------------------------------------------------
		/// A system used to display platform specific system
        /// dialogue boxes.
		//-----------------------------------------------------------
		class DialogueSystem final : public Core::DialogueSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(DialogueSystem);
            //----------------------------------------------------
			/// Constructor
            ///
            /// @author I Copland
			//----------------------------------------------------
            DialogueSystem();
            //----------------------------------------------------
            /// Is A
            ///
            /// @return Whether this implements the passed in
            /// interface id.
            //-----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceID) const override;
            //-----------------------------------------------------
            /// Display a system dialog with the given ID and delegate
            ///
            /// @author I Copland
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            //------------------------------------------------------
            void ShowSystemDialogue(u32 in_id, const Core::DialogueSystem::DialogueDelegate& in_delegate, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm) override;
            //-----------------------------------------------------
            /// Display a system confirmation dialog with the given
            /// ID and delegate.
            ///
            /// @author I Copland
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            /// @param Cancel text
            //-----------------------------------------------------
            void ShowSystemConfirmDialogue(u32 in_id, const Core::DialogueSystem::DialogueDelegate& in_delegate, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm, const Core::UTF8String& in_cancel) override;
            //-----------------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author I Copland
            ///
            /// @param The text to display.
            //-----------------------------------------------------
            void MakeToast(const Core::UTF8String& in_text) override;
            //------------------------------------------------------
            /// Triggered from a system dialog confirmation event
            ///
            /// @author I Copland
            ///
            /// @param ID
            /// @param Result
            //------------------------------------------------------
            void OnSystemConfirmDialogResult(u32 in_id, Core::DialogueSystem::DialogueResult in_result);
            //----------------------------------------------------
			/// Destructor.
            ///
            /// @author I Copland
			//----------------------------------------------------
            ~DialogueSystem();
        private:
            
			Core::DialogueSystem::DialogueDelegate m_activeSysConfirmDelegate;
		};
	}
}

#endif
