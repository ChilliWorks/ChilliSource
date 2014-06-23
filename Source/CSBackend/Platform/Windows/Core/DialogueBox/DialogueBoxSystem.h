//
//  DialogueBoxSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#ifndef _CSBACKEND_PLATFORM_WINDOWS_CORE_DIALOGUE_DIALOGUESYSTEM_H_
#define _CSBACKEND_PLATFORM_WINDOWS_CORE_DIALOGUE_DIALOGUESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/DialogueBox/DialogueBoxSystem.h>

namespace CSBackend
{
	namespace Windows
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
            //----------------------------------------------------
			/// Destructor.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
            ~DialogueBoxSystem();
        private:
			friend CSCore::DialogueBoxSystemUPtr CSCore::DialogueBoxSystem::Create();

			//----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			//----------------------------------------------------
			DialogueBoxSystem();

		};
	}
}

#endif

#endif
