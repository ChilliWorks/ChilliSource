//
//  DialogueBoxJavaInterface.h
//  Chilli Source
//
//  Created by Ian Copland on 04/03/2014
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_DIALOGUEBOX_DIALOGUEBOXJAVAINTERFACE_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_DIALOGUEBOX_DIALOGUEBOXJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterface.h>

namespace ChilliSource
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
			bool IsA(Core::InterfaceIDType in_interfaceID) const;
            //-----------------------------------------------
            /// Display the systems confirmation dialog
            ///
			/// @author I Copland
			///
            /// @param ID that is returned in pressed delegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            /// @param Cancel text
            //-----------------------------------------------
            void ShowSystemConfirmDialogue(s32 in_dialogID, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm, const Core::UTF8String& in_cancel);
            //-----------------------------------------------
            /// Display the systems confirmation dialog
            ///
            /// @author I Copland
            ///
            /// @param ID that is returned in pressed delegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            //-----------------------------------------------
            void ShowSystemDialogue(s32 in_dialogID, const Core::UTF8String& in_title, const Core::UTF8String& in_message, const Core::UTF8String& in_confirm);
            //-----------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author I Copland
            ///
            /// @param Text
            //-----------------------------------------------
            void MakeToast(const Core::UTF8String& in_text);
		};
	}
}

#endif
