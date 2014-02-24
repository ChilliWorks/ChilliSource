//
//  GUIProgressBar.h
//  moFlo
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_PROGRESSBAR_H_
#define _MOFLO_GUI_PROGRESSBAR_H_

#include <ChilliSource/Rendering/GUI/GUIView.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class CProgressBar : public CGUIView
		{
		public:

			DECLARE_META_CLASS(CProgressBar)

			CProgressBar();
			CProgressBar(const Core::ParamDictionary& insParams);
			//------------------------------------------------
			/// Set Progress
			///
			/// Normalised progress value representing how
			/// far the bar will fill
			///
			/// @param Value between 0 and 1
			//------------------------------------------------
			void SetProgress(f32 infProgress);
			//------------------------------------------------
			/// Get Progress
			///
			/// Normalised progress value representing how
			/// far the bar will fill
            ///
            /// If AnimationTime is not 0 the value returned will 
            /// change depending on the progress of the animation
			///
			/// @return Value between 0 and 1
			//------------------------------------------------
			f32 GetProgress() const;
            //------------------------------------------------------------------------
			/// Set the time to animate between previous values and the new value
			///
			/// @param Number of seconds the animation will run for
			//------------------------------------------------------------------------
			void SetAnimatedTimeInSeconds(f32 infAnimate);
            //------------------------------------------------------------------------
			/// Set the time of the animation between previous values and the new value
			///
			/// @param Number of seconds the animation will run for
			//------------------------------------------------------------------------
            f32 GetAnimationTime();
            //------------------------------------------------------------------------
			/// Draw
			///
			/// Draw this view and any subviews
			///
			/// @param Renderer
			//------------------------------------------------------------------------
			virtual void Draw(Rendering::CCanvasRenderer* inpCanvas);
		private:

			f32 mfProgress;
            
            f32 mfAnimationTime;
            f32 mfPreviousValue;
            f32 mfPreviousValueDiff;
            TimeIntervalMs mPreviousValueTimeStamp;
		};
	}
}

#endif