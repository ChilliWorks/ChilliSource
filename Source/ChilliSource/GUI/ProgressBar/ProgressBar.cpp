//
//  ProgressBar.cpp
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

#include <ChilliSource/GUI/ProgressBar/ProgressBar.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(ProgressBar)
        
        
        ProgressBar::ProgressBar()
		:mfProgress(0.0f)
		,mfAnimationTime(0.0f)
		,mfPreviousValue(0)
		,mPreviousValueTimeStamp(0)
		,mfPreviousValueDiff(0)
        {
            
        }
        
        ProgressBar::ProgressBar(const Core::ParamDictionary& insParams)
		:GUIView(insParams)
		,mfProgress(0.0f)
		,mfAnimationTime(0.0f)
		,mfPreviousValue(0)
		,mPreviousValueTimeStamp(0)
		,mfPreviousValueDiff(0)
        {
            std::string strValue;
            
            //---Animation
            if(insParams.TryGetValue("AnimationTime", strValue))
            {
                SetAnimatedTimeInSeconds(Core::ParseF32(strValue));
                SetProgress(mfProgress);
            }
        }
        //------------------------------------------------
        /// Set Progress
        ///
        /// Normalised progress value representing how
        /// far the bar will fill
        ///
        /// @param Value between 0 and 1
        //------------------------------------------------
        void ProgressBar::SetProgress(f32 infProgress)
        {            
            mfPreviousValue = GetProgress();
            mfProgress = Core::MathUtils::Clamp(infProgress, 0.0f, 1.0f);
			
			// Going backward: jump to it
			if(mfProgress < mfPreviousValue)
			{
				mfPreviousValue = mfProgress;
			}
			// Animate
			mfPreviousValueDiff = mfProgress - mfPreviousValue;
			mPreviousValueTimeStamp = Core::Application::Get()->GetSystemTimeInMilliseconds();
        }
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
        f32 ProgressBar::GetProgress() const
        {
            if(mfAnimationTime != 0)
            {
                f32 fTimeSinceValueSet = (f32)Core::Application::Get()->GetSystemTimeInMilliseconds() - mPreviousValueTimeStamp;
                f32 fTimeRatio = std::min(fTimeSinceValueSet / mfAnimationTime, 1.0f);
                f32 fAnimatedProgress = Core::MathUtils::Clamp(mfPreviousValue + (mfPreviousValueDiff * fTimeRatio), 0.0f, 1.0f);
                return fAnimatedProgress;
                
            }
            return mfProgress;
        }
        //------------------------------------------------------------------------
        /// Set the time to animate between previous values and the new value
        ///
        /// @param Number of seconds the animation will run for
        //------------------------------------------------------------------------
        void ProgressBar::SetAnimatedTimeInSeconds(f32 infAnimationTime)
        {
            mfAnimationTime = infAnimationTime * 1000;
        }
        //------------------------------------------------------------------------
        /// Get the time of the animation between previous values and the new value
        ///
        /// @param Number of seconds the animation will run for
        //------------------------------------------------------------------------
        f32 ProgressBar::GetAnimationTime() 
        { 
            return mfAnimationTime;
        }
        
        void ProgressBar::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            GUIView::Draw(inpCanvas);
        }
    }
}
