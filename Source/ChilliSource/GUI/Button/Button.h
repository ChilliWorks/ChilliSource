//
//  Button.h
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_BUTTON_H_
#define _MOFLO_GUI_BUTTON_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

#ifdef ENABLE_MOFLOW_REFLECTION
#include <ChilliSource/Core/Reflection/Reflection.h>
#define DECLARE_ACTION 
#else

#endif

namespace ChilliSource
{
    namespace GUI
    {
        typedef fastdelegate::FastDelegate1<Button*> ButtonEventDelegate;
        
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
            Core::IEvent<ButtonEventDelegate>& GetActivatedEvent();
            
        protected:
            
            Core::CEvent1<ButtonEventDelegate> mOnButtonActivatedEvent;
        };
    }
}

#endif