//
//  DialogueBoxJavaInterface.h
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


#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_CORE_DIALOGUEBOX_DIALOGUEBOXJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_CORE_DIALOGUEBOX_DIALOGUEBOXJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

namespace CSBackend
{
	namespace Android
	{
		//-----------------------------------------------------
		/// An interface for communicating with Java system
		/// dialogue box functionality
		//-----------------------------------------------------
		class DialogueBoxJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(DialogueBoxJavaInterface);
			//-----------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			//-----------------------------------------------
			DialogueBoxJavaInterface();
			//-----------------------------------------------
			/// @author Ian Copland
			///
			/// @return whether or not this object implements
			/// the given interface.
			//-----------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const;
            //-----------------------------------------------
            /// Display the systems confirmation dialog
            ///
			/// @author Ian Copland
			///
            /// @param ID that is returned in pressed delegate
            /// @param Title text (UTF-8)
            /// @param Message text (UTF-8)
            /// @param Confirm text (UTF-8)
            /// @param Cancel text (UTF-8)
            //-----------------------------------------------
            void ShowSystemConfirmDialogue(s32 in_dialogID, const std::string& in_title, const std::string& in_message, const std::string& in_confirm, const std::string& in_cancel);
            //-----------------------------------------------
            /// Display the systems confirmation dialog
            ///
            /// @author Ian Copland
            ///
            /// @param ID that is returned in pressed delegate
            /// @param Title text (UTF-8)
            /// @param Message text (UTF-8)
            /// @param Confirm text (UTF-8)
            //-----------------------------------------------
            void ShowSystemDialogue(s32 in_dialogID, const std::string& in_title, const std::string& in_message, const std::string& in_confirm);
            //-----------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author Ian Copland
            ///
            /// @param Text (UTF-8)
            //-----------------------------------------------
            void MakeToast(const std::string& in_text);
		};
	}
}

#endif

#endif
