//
//  TextEntry.h
//  Chilli Source
//  Created by Scott Downie on 08/07/2014
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

#ifndef _CHILLISOURCE_INPUT_TEXTENTRY_TEXTENTRY_H_
#define _CHILLISOURCE_INPUT_TEXTENTRY_TEXTENTRY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace Input
	{
        //---------------------------------------------------------------
        /// A system for receiving textual from both virtual and hardware
        /// keyboards depending on the platform.
        ///
        /// @author S Downie
        //---------------------------------------------------------------
		class TextEntry : public Core::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(TextEntry);
            
            //-------------------------------------------------------
            /// An enum describing the different types of keyboard.
            ///
            /// @author S Downie
            //-------------------------------------------------------
            enum class Type
            {
                k_text,
                k_numeric
            };
            //-------------------------------------------------------
            /// An enum describing the various methods of keyboard
            /// capitalisation that can be used.
            ///
            ///  - None: will not capitalise any thing.
            ///  - Words: will capitalise the first letter
            ///	   of each word.
            ///  - Sentences: will capitalise the first letter
            ///	   of each sentence.
            ///  - All: will capitalise every letter.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            enum class Capitalisation
            {
                k_none,
                k_words,
                k_sentences,
                k_all
            };
            //-------------------------------------------------------
            /// A delegate used for recieving text input events.
            ///
            /// @author S Downie
            ///
            /// @param The new updated version of the text.
            ///
            /// @return Whether or not the change to the text
            /// has been rejected.
            //-------------------------------------------------------
            typedef std::function<bool(const std::string&)> TextBufferChangedDelegate;
            //-------------------------------------------------------
            /// A delegate used for recieving events when input is disabled.
            ///
            /// @author S Downie
            //-------------------------------------------------------
            typedef std::function<void()> TextInputDeactivatedDelegate;
            //-------------------------------------------------------
            /// Factory method from creating a new platform specific
            /// instance of the keyboard system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance of the system.
            //-------------------------------------------------------
			static TextEntryUPtr Create();
            //-------------------------------------------------------
            /// The system will now receive text input. This will also
            /// show the virtual keyboard if required
            ///
            /// @author S Downie
            ///
            /// @param Initial buffer contents
            /// @param Preferred keyboard type
            /// @param Preferred capitalisation method
            /// @param Text changed delegate
            /// @param Deactivate delegate
            //-------------------------------------------------------
			virtual void Activate(const std::string& in_text, Type in_type, Capitalisation in_capitalisation, const TextBufferChangedDelegate& in_changeDelegate, const TextInputDeactivatedDelegate& in_deactivateDelegate) = 0;
            //-------------------------------------------------------
            /// The system will no longer receive text input. This
            /// will also hide the virtual keyboard if required
            ///
            /// @author S Downie
            //-------------------------------------------------------
			virtual void Deactivate() = 0;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether or not text input is currently
            /// enabled.
            //-------------------------------------------------------
			virtual bool IsActive() const = 0;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The current text input buffer (UTF-8).
            //-------------------------------------------------------
			virtual const std::string& GetTextBuffer() const = 0;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The new text input buffer (UTF-8).
            //-------------------------------------------------------
            virtual void SetTextBuffer(const std::string& in_text) = 0;
		};
	}
}

#endif


