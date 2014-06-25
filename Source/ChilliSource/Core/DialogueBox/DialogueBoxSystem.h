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
        /// @author Ian Copland
		//-----------------------------------------------------------
		class DialogueBoxSystem : public AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(DialogueBoxSystem);
            //----------------------------------------------------
			/// An enum describing the possible results from a
            /// dialogue box.
            ///
            /// @author Ian Copland
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
            /// @author Ian Copland
            ///
            /// @param The Id of the dialogue box.
            /// @param The result.
			//----------------------------------------------------
            typedef std::function<void(u32, DialogueResult)> DialogueDelegate;
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
            virtual void ShowSystemDialogue(u32 in_id, const DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm) = 0;
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
            virtual void ShowSystemConfirmDialogue(u32 in_id, const DialogueDelegate& in_delegate, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel) = 0;
            //-----------------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author Ian Copland
            ///
            /// @param The text to display (UTF-8).
            //-----------------------------------------------------
            virtual void MakeToast(const std::string& in_text) = 0;
            //----------------------------------------------------
			/// Destructor.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
            virtual ~DialogueBoxSystem();
            
        protected:
            friend class Application;
            
            //----------------------------------------------------
			/// Creates a new instance of this system.
            ///
            /// @author Ian Copland
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
