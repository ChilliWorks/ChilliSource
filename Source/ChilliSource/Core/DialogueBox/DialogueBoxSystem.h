//
//  DialogueBoxSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_DIALOGUE_DIALOGUESYSTEM_H_
#define _CHILLISOURCE_CORE_DIALOGUE_DIALOGUESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>

#include <functional>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------
		/// A system used to display platform specific system
        /// dialogue boxes.
		//-----------------------------------------------------------
		class DialogueBoxSystem : public System
		{
		public:
            CS_DECLARE_NAMEDTYPE(DialogueBoxSystem);
            //----------------------------------------------------
			/// An enum describing the possible results from a
            /// dialogue box.
			//----------------------------------------------------
            enum class DialogueResult
            {
                k_confirm,
                k_cancel
            };
            //----------------------------------------------------
			/// Delegates
			//----------------------------------------------------
            typedef std::function<void(u32, DialogueResult)> DialogueDelegate;
            //----------------------------------------------------
			/// Creates a new instance of this system.
            ///
            /// @author I Copland
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static DialogueBoxSystemUPtr Create();
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
            virtual void ShowSystemDialogue(u32 in_id, const DialogueDelegate& in_delegate, const UTF8String& in_title, const UTF8String& in_message, const UTF8String& in_confirm) = 0;
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
            virtual void ShowSystemConfirmDialogue(u32 in_id, const DialogueDelegate& in_delegate, const UTF8String& in_title, const UTF8String& in_message, const UTF8String& in_confirm, const UTF8String& in_cancel) = 0;
            //-----------------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author I Copland
            ///
            /// @param The text to display.
            //-----------------------------------------------------
            virtual void MakeToast(const UTF8String& in_text) = 0;
            //----------------------------------------------------
			/// Destructor.
            ///
            /// @author I Copland
			//----------------------------------------------------
            virtual ~DialogueBoxSystem();
		};
	}
}

#endif
