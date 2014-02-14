//
//  VirtualKeyboard.h
//  moFlow
//
//  Created by Scott Downie on 26/11/2010
//  Copyright �2010 Tag Games Limited. All rights reserved.
//

#ifndef _MO_FLO_PLATFORM_IOS_VIRTUAL_KEYBOARD_H_
#define _MO_FLO_PLATFORM_IOS_VIRTUAL_KEYBOARD_H_

#include <UIKit/UIKit.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Main/EAGLView.h>

#include <ChilliSource/Core/Main/FastDelegate.h>
#include <ChilliSource/Core/Main/GenericEvent.h>

#include <ChilliSource/Input/VirtualKeyboard.h>

namespace moFlo
{
	namespace iOSPlatform
	{
		class CVirtualKeyboard : public Input::IVirtualKeyboard
		{
		public:
			CVirtualKeyboard();
			virtual ~CVirtualKeyboard();
            //-------------------------------------------
            /// Show 
            ///
            /// Display the keyboard
            /// Triggers OnKeyboardShow event
            ///
            /// @param Default text 
            //-------------------------------------------
			void Show();
            //-------------------------------------------
            /// Hide 
            ///
            /// Remove the keyboard
            /// Triggers OnKeyboardHide event
            //-------------------------------------------
			void Hide();
            //-------------------------------------------
            /// Update Text
            ///
            /// @param New Text
            /// @return Whether to accept the text
            //-------------------------------------------
            bool UpdateText(NSString* inpstrText);
            //-------------------------------------------
            /// Set Text 
            ///
            /// @param Text cache for keyboard
            //-------------------------------------------
            void SetText(const UTF8String& instrText);
            //-------------------------------------------
            /// SetKeyboardType
            ///
            /// @param Requests keyboard type to show
            //-------------------------------------------
            void SetKeyboardType(Input::KeyboardType ineKeyboardType);
            //------------------------
            /// Set Capitalisation Type
            ///
            /// @param Requests keyboard Capitalisation Method to show
            //------------------------
            void SetCapitalisationMethod(Input::KeyboardCapitalisation ineKeyboardCapitalisation);
			
		private:
			
			UITextField* mpTextView;
		};
	}
}

#endif


