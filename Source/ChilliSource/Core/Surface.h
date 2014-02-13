//
//  Surface.h
//  MoFlow
//
//  Created by Ian Copland on 09/01/2012.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_RENDERING_SURFACE_H_
#define _MOFLOW_RENDERING_SURFACE_H_

#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/GUI/ForwardDeclarations.h>
#include <ChilliSource/Core/CustomCreator.h>

#include <ChilliSource/Input/ForwardDeclarations.h>
#include <ChilliSource/Input/TouchScreen.h>

namespace moFlo
{
	class ISurface : public ICustomCreator<ISurface>
	{
	public:
        ISurface() : mpInputSystem(NULL){}
		virtual ~ISurface(){};
		//-----------------------------------------------------
		/// Update
		///
		/// @param Time between frames
		//-----------------------------------------------------
		virtual void Update(f32 infDt) = 0;
		//-----------------------------------------------------
		/// Draw
		///
		/// Override this implementation to draw the GUI
		/// using the primitves within canvas renderer i.e.
		/// DrawBox, DrawString etc
		///
		/// @param Canvas renderer
		//-----------------------------------------------------
		virtual void Draw(Rendering::CCanvasRenderer * inpCanvas) = 0;
		//-----------------------------------------------------
		/// Enable User Interaction
		///
		/// Toggle whether the view receives user input
		///
		/// @param Enable/disable
		//-----------------------------------------------------
		virtual void EnableUserInteraction(bool inbEnabled) = 0;
		//-----------------------------------------------------
		/// Set Input System
		///
		/// @param pointer to the input system
		//-----------------------------------------------------
        virtual void SetInputSystem(Input::IInputSystem* inpInputSystem){} 
        //-----------------------------------------------------
		/// Get Input System
		///
		/// @return pointer to the input system
		//-----------------------------------------------------
        Input::IInputSystem* GetInputSystem() 
        {
            return mpInputSystem;
        }
        //-----------------------------------------------------------
		/// Get Touch Began Event
		///
		/// Use this to register for touch notifications that
		/// are not consumed by the GUI
		///
		/// @return Touch began event
		//-----------------------------------------------------------
		IEvent<Input::TouchEventDelegate> & GetTouchBeganEvent()
		{
			return mTouchBeganEvent;
		}
		//-----------------------------------------------------------
		/// Get Touch Moved Event
		///
		/// Use this to register for touch notifications that
		/// are not consumed by the GUI
		///
		/// @return Touch moved event
		//-----------------------------------------------------------
		IEvent<Input::TouchEventDelegate> & GetTouchMovedEvent()
		{
			return mTouchMovedEvent;
		}
		//-----------------------------------------------------------
		/// Get Touch End Event
		///
		/// Use this to register for touch notifications that
		/// are not consumed by the GUI
		///
		/// @return Touch end event
		//-----------------------------------------------------------
		IEvent<Input::TouchEventDelegate> & GetTouchEndEvent()
		{
			return mTouchEndedEvent;
		}
        
    protected:
        
        //These are touch events for non-gui objects
        //and will only be triggered if the GUI does not consume them
        CEvent1<Input::TouchEventDelegate> mTouchBeganEvent;
        CEvent1<Input::TouchEventDelegate> mTouchMovedEvent;			
        CEvent1<Input::TouchEventDelegate> mTouchEndedEvent;
        
        Input::IInputSystem* mpInputSystem;
	};
}

#endif
