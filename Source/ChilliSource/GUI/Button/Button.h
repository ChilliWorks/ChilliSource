//
//  Button.h
//  Chilli Source
//  Created by Scott Downie on 26/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_GUI_BUTTON_BUTTON_H_
#define _CHILLISOURCE_GUI_BUTTON_BUTTON_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

#ifdef CS_ENABLE_REFLECTION
#include <ChilliSource/Core/Reflection/Reflection.h>
#else

#endif

namespace ChilliSource
{
    namespace GUI
    {
        typedef std::function<void(Button*)> ButtonEventDelegate;
        
        class Button : public GUIView
        {
        public:

			DECLARE_META_CLASS(Button)

            Button(){}
            Button(const Core::ParamDictionary& insParams) : GUIView(insParams){}
            
            //--------------------------------------------------
            /// Get Activated Event
            ///
            /// @return Event triggered when the button is 
            /// activated
            //--------------------------------------------------
            Core::IConnectableEvent<ButtonEventDelegate>& GetActivatedEvent();
            
        protected:
            
            Core::Event<ButtonEventDelegate> mOnButtonActivatedEvent;
        };
    }
}

#endif