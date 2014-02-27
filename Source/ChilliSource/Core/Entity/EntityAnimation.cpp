/*
 *  EntityAnimation.cpp
 *  SceneLoader
 *
 *  Created by Stuart McGaw on 21/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

// INCLUDED FILES
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Entity/EntityAnimation.h>

#include <float.h>

namespace ChilliSource
{
	namespace Core
	{		
		CEntityAnimation::CEntityAnimation(const EntityAnimationData* inpAnimData, CEntity* inpTarget, u32 inudwQueryFlags, f32 infInTime, f32 infOutTime)
		:mpTarget(inpTarget)
        ,mudwFrameCount(inpAnimData->udwKeyframeCount)
		,mpafFrameTimes(inpAnimData->afKeyframeTimes)
		,mpasFrameValues(inpAnimData->aKeyframeValues)
        ,mfInTime(infInTime)
        ,mfOutTime(infOutTime)
		{
            SetInterpolationMode(InterpolationType::k_linear);
            
            if(mfOutTime < 0 && mudwFrameCount > 1)
            {
                mfOutTime = inpAnimData->afKeyframeTimes.get()[mudwFrameCount-1];
            } 
            else if(inpAnimData->udwKeyframeCount == 1)
            {
                mfOutTime = FLT_MAX;
            }
            
            if(mfInTime < 0 && mudwFrameCount > 0)
            {
                mfInTime = inpAnimData->afKeyframeTimes.get()[0];
            } 
            else
            {
                mfInTime = 0.0f;
            }
            
            mudwFlags = inudwQueryFlags;
            
            if(mudwFrameCount > 1)
            {
                SetDuration(mfOutTime - mfInTime);
            }
		}
        
        void CEntityAnimation::SetInAndOutTime(f32 inInTime, f32 inOutTime)
        {
            mfInTime = inInTime;
            mfOutTime = inOutTime;
            SetDuration(inOutTime - mfInTime);
        }
        
        void CEntityAnimation::SetInterpolationMode(InterpolationType ineType)
        {
            switch (ineType)
            {
                case InterpolationType::k_linear:
                    mInterpolateDelegate = Core::MakeDelegate(this, &CEntityAnimation::Lerp);
                    break;
                case InterpolationType::k_step:
                    mInterpolateDelegate = Core::MakeDelegate(this, &CEntityAnimation::Step);
                    break;
            }
        }
        
        CEntity* CEntityAnimation::GetTarget()
        {
            return mpTarget;
        }
        
        void CEntityAnimation::ToFrame(u32 inudwFrameNumber)
        {
            if(inudwFrameNumber < mudwFrameCount)
            {
                mfAnimPos = mpafFrameTimes.get()[inudwFrameNumber] - mfInTime;
                UpdateInternal();
            }
        }
		
 		void CEntityAnimation::UpdateInternal()
		{	
            f32 fAdjustedAnimPos = mfAnimPos + mfInTime;
            
            //Find our upper bound
			f32 * pV = std::lower_bound(mpafFrameTimes.get(), mpafFrameTimes.get() + mudwFrameCount, fAdjustedAnimPos);

			u32 lowIndex = (&(*pV) - mpafFrameTimes.get());
			if (lowIndex > 0)
            {
				lowIndex--;
            }
            
			u32 highIndex = lowIndex + 1;
            highIndex = std::min(highIndex, mudwFrameCount-1);
            
            f32 fMin = mpafFrameTimes.get()[lowIndex];
            f32 fMax = mpafFrameTimes.get()[highIndex];
            f32 fProgression = ( mudwFrameCount == 1 ? 1.0f : CMathUtils::NormalisedRange(fAdjustedAnimPos, fMin, fMax) );
            
            fProgression = CMathUtils::Clamp(fProgression, 0.0f, 1.0f);
            
            if(highIndex == 0 && lowIndex == 0)
                return;
            
            mInterpolateDelegate(lowIndex, highIndex, fProgression);
        }
        
        void CEntityAnimation::Step(u32 inudwLowFrame, u32 inudwHighFrame, f32 infT)
        {
            u32 udwFrame = inudwLowFrame + floor(infT + 0.5f);
            // Interpolate translation
			CVector3 vLerpedTranslation = mpasFrameValues.get()[udwFrame].vTranslation;
            
			// Interpolate scale
			CVector3 vLerpedScale = mpasFrameValues.get()[udwFrame].vScale;
            
			// Interpolate orientation
			CQuaternion qLerpedOrientation = mpasFrameValues.get()[udwFrame].qOrientation;
            
            mpTarget->Transform().SetPositionScaleOrientation(vLerpedTranslation, vLerpedScale, qLerpedOrientation);
            
            //Interpolate Opacity
            f32 fOpacity = mpasFrameValues.get()[udwFrame].fOpacity;
            
            if(mpTarget->Transform().GetLocalOpacity() != fOpacity)
            {
                mpTarget->Transform().FadeTo(fOpacity);
            }
        }
        
        void CEntityAnimation::Lerp(u32 inudwLowFrame, u32 inudwHighFrame, f32 infT)
        {
            // Interpolate translation
			CVector3 vLerpedTranslation = CMathUtils::Lerp(infT, mpasFrameValues.get()[inudwLowFrame].vTranslation, mpasFrameValues.get()[inudwHighFrame].vTranslation);
            
			// Interpolate scale
			CVector3 vLerpedScale = CMathUtils::Lerp(infT, mpasFrameValues.get()[inudwLowFrame].vScale, mpasFrameValues.get()[inudwHighFrame].vScale);
            
			// Interpolate orientation
			CQuaternion qLerpedOrientation = CQuaternion::Slerp(mpasFrameValues.get()[inudwLowFrame].qOrientation, mpasFrameValues.get()[inudwHighFrame].qOrientation, infT);
            
            mpTarget->Transform().SetPositionScaleOrientation(vLerpedTranslation, vLerpedScale, qLerpedOrientation);
            
            //Interpolate Opacity
            f32 fOpacity = CMathUtils::Lerp(infT, mpasFrameValues.get()[inudwLowFrame].fOpacity, mpasFrameValues.get()[inudwHighFrame].fOpacity);
            
            if(mpTarget->Transform().GetLocalOpacity() != fOpacity)
            {
                mpTarget->Transform().FadeTo(fOpacity);
            }
        }
	}
}