//
//  GUIProgressBar.cpp
//  moFlo
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/ProgressBar/ProgressBar.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(CProgressBar)
        
        
        CProgressBar::CProgressBar()
		:mfProgress(0.0f)
		,mfAnimationTime(0.0f)
		,mfPreviousValue(0)
		,mPreviousValueTimeStamp(0)
		,mfPreviousValueDiff(0)
        {
            
        }
        
        CProgressBar::CProgressBar(const Core::ParamDictionary& insParams)
		:CGUIView(insParams)
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
        void CProgressBar::SetProgress(f32 infProgress)
        {            
            mfPreviousValue = GetProgress();
            mfProgress = Core::CMathUtils::Clamp(infProgress, 0.0f, 1.0f);
			
			// Going backward: jump to it
			if(mfProgress < mfPreviousValue)
			{
				mfPreviousValue = mfProgress;
			}
			// Animate
			mfPreviousValueDiff = mfProgress - mfPreviousValue;
			mPreviousValueTimeStamp = Core::CApplication::GetSystemTimeInMilliseconds();
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
        f32 CProgressBar::GetProgress() const
        {
            if(mfAnimationTime != 0)
            {
                f32 fTimeSinceValueSet = Core::CApplication::GetSystemTimeInMilliseconds() - mPreviousValueTimeStamp;
                f32 fTimeRatio = ChilliSource::Core::CMathUtils::Min(fTimeSinceValueSet / mfAnimationTime, 1.0f);
                f32 fAnimatedProgress = Core::CMathUtils::Clamp(mfPreviousValue + (mfPreviousValueDiff * fTimeRatio), 0.0f, 1.0f);
                return fAnimatedProgress;
                
            }
            return mfProgress;
        }
        //------------------------------------------------------------------------
        /// Set the time to animate between previous values and the new value
        ///
        /// @param Number of seconds the animation will run for
        //------------------------------------------------------------------------
        void CProgressBar::SetAnimatedTimeInSeconds(f32 infAnimationTime)
        {
            mfAnimationTime = infAnimationTime * 1000;
        }
        //------------------------------------------------------------------------
        /// Get the time of the animation between previous values and the new value
        ///
        /// @param Number of seconds the animation will run for
        //------------------------------------------------------------------------
        f32 CProgressBar::GetAnimationTime() 
        { 
            return mfAnimationTime;
        }
        
        void CProgressBar::Draw(Rendering::CCanvasRenderer* inpCanvas)
        {
            CGUIView::Draw(inpCanvas);
        }
    }
}
