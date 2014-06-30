//
//  ProgressBar.h
//  Chilli Source
//  Created by Scott Downie on 27/04/2011.
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

#ifndef _CHILLISOURCE_GUI_PROGRESSBAR_H_
#define _CHILLISOURCE_GUI_PROGRESSBAR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

namespace ChilliSource
{
	namespace GUI
	{
		class ProgressBar : public GUIView
		{
		public:

			DECLARE_META_CLASS(ProgressBar)

			ProgressBar();
			ProgressBar(const Core::ParamDictionary& insParams);
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
			virtual void Draw(Rendering::CanvasRenderer* inpCanvas);
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