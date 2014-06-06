//
//  DialogueBoxJavaInterface.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_DIALOGUEBOX_DIALOGUEBOXJAVAINTERFACE_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_DIALOGUEBOX_DIALOGUEBOXJAVAINTERFACE_H_

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
			/// @author I Copland
			//-----------------------------------------------
			DialogueBoxJavaInterface();
			//-----------------------------------------------
			/// @author I Copland
			///
			/// @return whether or not this object implements
			/// the given interface.
			//-----------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const;
            //-----------------------------------------------
            /// Display the systems confirmation dialog
            ///
			/// @author I Copland
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
            /// @author I Copland
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
            /// @author I Copland
            ///
            /// @param Text (UTF-8)
            //-----------------------------------------------
            void MakeToast(const std::string& in_text);
		};
	}
}

#endif

#endif