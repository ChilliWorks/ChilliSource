//
//  DialogueBoxSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_DIALOGUE_DIALOGUESYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_DIALOGUE_DIALOGUESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/DialogueBox/DialogueBoxSystem.h>

namespace ChilliSource
{
	namespace Windows
	{
		//-----------------------------------------------------------
		/// A system used to display platform specific system
        /// dialogue boxes.
		//-----------------------------------------------------------
		class DialogueBoxSystem final : public Core::DialogueBoxSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(DialogueBoxSystem);
            //----------------------------------------------------
            /// Is A
            ///
            /// @return Whether this implements the passed in
            /// interface id.
            //-----------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //-----------------------------------------------------
            /// Display a system dialog with the given ID and delegate
            ///
            /// @author I Copland
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text (UTF-8)
            /// @param Message text (UTF-8)
            /// @param Confirm text (UTF-8)
            //------------------------------------------------------
			void ShowSystemDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm) override;
            //-----------------------------------------------------
            /// Display a system confirmation dialog with the given
            /// ID and delegate.
            ///
            /// @author I Copland
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text (UTF-8)
            /// @param Message text (UTF-8)
            /// @param Confirm text (UTF-8)
            /// @param Cancel text (UTF-8)
            //-----------------------------------------------------
			void ShowSystemConfirmDialogue(u32 in_id, const Core::DialogueBoxSystem::DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel) override;
            //-----------------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author I Copland
            ///
            /// @param The text to display (UTF-8).
            //-----------------------------------------------------
            void MakeToast(const std::string& in_text) override;
            //----------------------------------------------------
			/// Destructor.
            ///
            /// @author I Copland
			//----------------------------------------------------
            ~DialogueBoxSystem();
        private:
			friend Core::DialogueBoxSystemUPtr Core::DialogueBoxSystem::Create();

			//----------------------------------------------------
			/// Constructor
			///
			/// @author I Copland
			//----------------------------------------------------
			DialogueBoxSystem();

		};
	}
}

#endif
