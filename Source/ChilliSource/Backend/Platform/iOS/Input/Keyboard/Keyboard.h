//
//  Keyboard.h
//  Chilli Source
//
//  Created by S Downie on 26/11/2010
//  Copyright (c)2010 Tag Games Limited. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_INPUT_KEYBOARD_KEYBOARD_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_INPUT_KEYBOARD_KEYBOARD_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Input/Keyboard/Keyboard.h>

@class VirtualKeyboardDelegate;
@class UITextField;
@class NSString;

namespace ChilliSource
{
	namespace iOS
	{
        //----------------------------------------------------------------
        /// The iOS backend to the keyboard system. This provides access
        /// to the iOS virtual keyboard functionality.
        ///
        /// @author S Downie
        //----------------------------------------------------------------
		class Keyboard final : public Input::Keyboard
		{
		public:
            CS_DECLARE_NAMEDTYPE(Keyboard);
            //-------------------------------------------------------
            /// Query whether the object implements an interface
            /// that has the given ID
            ///
            /// @author I Copland
            ///
            /// @param Interface ID
            ///
            /// @return Whether the object implements an interface
            /// that has the given ID
            //-------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceID) const override;
            //-------------------------------------------------------
            /// Sets whether or not the text input is currently
            /// enabled or disabled. If the keyboard is virtual, it
            /// will be shown or hidden when enabled or disabled.
            ///
            /// @author S Downie
            //-------------------------------------------------------
			void SetTextInputEnabled(bool in_enabled) override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether or not text input is currently
            /// enabled.
            //-------------------------------------------------------
			bool IsTextInputEnabled() const override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The currently text input buffer.
            //-------------------------------------------------------
			const Core::UTF8String& GetText() const override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The new text input buffer.
            //-------------------------------------------------------
            void SetText(const Core::UTF8String& in_text) override;
            //-------------------------------------------------------
            /// Sets the type of keyboard to display if using a
            /// virtual keyboard. This must be set prior to the keyboard
            /// being displayed. Note that this is only a suggestion
            /// and the virtual keyboard implementation may choose to
            /// ignore it.
            ///
            /// @author I Copland
            ///
            /// @param The keyboard type
            //-------------------------------------------------------
            void SetType(Type in_type) override;
            //-------------------------------------------------------
            /// Sets capitalisation method to be used for text input.
            ///
            /// @author I Copland
            ///
            /// @param The capitalisation method.
            //-------------------------------------------------------
            void SetCapitalisation(Capitalisation in_capitalisation) override;
            //-------------------------------------------------------
            /// @author I Copland
            ///
            /// @param An event that is called when text input is
            /// enabled.
            //-------------------------------------------------------
			Core::IConnectableEvent<KeyboardEventDelegate>& GetTextInputEnabledEvent() override;
            //-------------------------------------------------------
            /// @author I Copland
            ///
            /// @param An event that is called when text input is
            /// received.
            //-------------------------------------------------------
			Core::IConnectableEvent<TextInputEventDelegate>& GetTextInputReceivedEvent() override;
            //-------------------------------------------------------
            /// @author I Copland
            ///
            /// @param An event that is called when text input is
            /// disabled.
            //-------------------------------------------------------
			Core::IConnectableEvent<KeyboardEventDelegate>& GetTextInputDisabledEvent() override;
            //-------------------------------------------------------
            /// Called when the keyboard text is updated.
            ///
            /// @author S Downie
            ///
            /// @param The new text.
            //-------------------------------------------------------
			bool OnTextUpdated(NSString* in_text);
            //-------------------------------------------------------
            /// Destructor.
            ///
            /// @author S Downie
            //-------------------------------------------------------
			~Keyboard();
		private:
            friend Input::KeyboardUPtr Input::Keyboard::Create();
        
            //-------------------------------------------------------
            /// Constructor. Declared private to force the use of the
            /// factory method.
            ///
            /// @author S Downie
            //-------------------------------------------------------
			Keyboard();
            //-------------------------------------------------------
            /// Initialisation method called at a time when
            /// all App Systems have been created. System
            /// initialisation occurs in the order they were
            /// created.
            ///
            /// @author I Copland
            //-------------------------------------------------------
            void OnInit() override;
            //-------------------------------------------------------
            /// Called when the application is being destroyed.
            /// This should be used to cleanup memory and
            /// references to other systems. System destruction
            /// occurs in the reverse order to which they
            /// were created
            ///
            /// @author I Copland
            //-------------------------------------------------------
            void OnDestroy() override;
            
			UITextField* m_textView;
            VirtualKeyboardDelegate* m_delegate;
            Core::Event<KeyboardEventDelegate> m_textInputEnabledDelegate;
            Core::Event<TextInputEventDelegate> m_textInputReceivedDelegate;
            Core::Event<KeyboardEventDelegate> m_textInputDisabledDelegate;
            bool m_enabled;
            Core::UTF8String m_text;
		};
	}
}

#endif


