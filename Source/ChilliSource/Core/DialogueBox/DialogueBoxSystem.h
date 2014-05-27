//
//  DialogueBoxSystem.h
//  Chilli Source
//
//  Created by I Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_DIALOGUE_DIALOGUESYSTEM_H_
#define _CHILLISOURCE_CORE_DIALOGUE_DIALOGUESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------
		/// A system used to display platform specific system
        /// dialogue boxes.
        ///
        /// @author I Copland
		//-----------------------------------------------------------
		class DialogueBoxSystem : public AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(DialogueBoxSystem);
            //----------------------------------------------------
			/// An enum describing the possible results from a
            /// dialogue box.
            ///
            /// @author I Copland
			//----------------------------------------------------
            enum class DialogueResult
            {
                k_confirm,
                k_cancel
            };
            //----------------------------------------------------
			/// A delegate called when a dialogue box has been
            /// dismissed.
            ///
            /// @author I Copland
            ///
            /// @param The Id of the dialogue box.
            /// @param The result.
			//----------------------------------------------------
            typedef std::function<void(u32, DialogueResult)> DialogueDelegate;
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
            virtual void ShowSystemDialogue(u32 in_id, const DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm) = 0;
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
            virtual void ShowSystemConfirmDialogue(u32 in_id, const DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel) = 0;
            //-----------------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author I Copland
            ///
            /// @param The text to display (UTF-8).
            //-----------------------------------------------------
            virtual void MakeToast(const std::string& in_text) = 0;
            //----------------------------------------------------
			/// Destructor.
            ///
            /// @author I Copland
			//----------------------------------------------------
            virtual ~DialogueBoxSystem();
            
        protected:
            friend class Application;
            
            //----------------------------------------------------
			/// Creates a new instance of this system.
            ///
            /// @author I Copland
            ///
            /// @return The new instance.
			//----------------------------------------------------
            static DialogueBoxSystemUPtr Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            DialogueBoxSystem() = default;
		};
	}
}

#endif
