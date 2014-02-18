/*
 *  GenericAnimation.cpp
 *  MoFlow
 *
 *  Created by Stuart McGaw on 04/04/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Core/Animation/GenericAnimation.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace moFlo{
	IAnimation::IAnimation(f32 infDuration, f32 infStartDelay)
	:mfAnimPos(0), mfDuration(infDuration),mfTimeScaleFactor(1.0f), mfStartDelay(infStartDelay )
	{
		mePlayMode = ANIM_PLAYONCE;
		mbForward = true;
		mbPlaying = false;
		mbFinished = false;
		mbLooped = false;
        mbStarted = false;
	}
	
	IAnimation::~IAnimation(){}
	
	void IAnimation::SetOnCompleteDelegate(OnCompleteDelegate inDelegate)
    {
		OnComplete = inDelegate;
	}
    
	void IAnimation::SetOnStartDelegate(OnStartDelegate inDelegate)
    {
		OnStart = inDelegate;
	}
	
    void IAnimation::SetStartDelay(f32 infStartDelay)
    {
        mfStartDelay = infStartDelay;
    }
    
    f32 IAnimation::GetStartDelay()
    {
        return mfStartDelay;
    }
    
    f32 IAnimation::GetDelayRemaining()
    {
        return mfAnimDelayRemaining;
    }
    
	void IAnimation::SetDuration(f32 infValue)
	{
		mfDuration = infValue;
	}
	f32 IAnimation::GetDuration() const
	{
		return mfDuration;
	}
	
	void IAnimation::SetTimeScaleFactor(f32 infValue)
	{
		mfTimeScaleFactor = std::max(0.0f,infValue);
	}
	f32 IAnimation::GetTimeScaleFactor(f32 infValue) const{
		return mfTimeScaleFactor;
	}
	
	AnimationPlayMode IAnimation::GetPlayMode() const{
		return mePlayMode;
	}
	
	void IAnimation::Play(AnimationPlayMode inePlayMode)
	{
		enum AnimationPlayMode{ANIM_PLAYONCE, ANIM_PLAYLOOPING, ANIM_PINGPONG, ANIM_PINGPONG_LOOPING,
            ANIM_PLAYONCE_REVERSE, ANIM_PLAYLOOPING_REVERSE, ANIM_PINGPONG_REVERSE, ANIM_PINGPONG_LOOPING_REVERSE, ANIM_PLAYHALF_KEEP_MID_FRAME};
        
		switch (inePlayMode)
		{
			case ANIM_PLAYONCE :
			case ANIM_PLAYLOOPING :
			case ANIM_PINGPONG :
			case ANIM_PINGPONG_LOOPING :
            case ANIM_PLAYHALF_KEEP_MID_FRAME:
				mfAnimPos = 0.0f;
				mbForward = true;
				break;
				
			case ANIM_PLAYONCE_REVERSE :
			case ANIM_PLAYLOOPING_REVERSE :
			case ANIM_PINGPONG_REVERSE :
			case ANIM_PINGPONG_LOOPING_REVERSE :
				mfAnimPos = mfDuration;
				mbForward = false;
				break;

			default :
                FATAL_LOG("Cannot play animation with given mode");
				break;
		}

        mfAnimDelayRemaining = mfStartDelay;
        
		mbPlaying = true;
		mePlayMode = inePlayMode;
		mbFinished = false;
	}
	
	void IAnimation::Stop()
	{
		mbPlaying = false;
	}
	
	bool IAnimation::IsPlaying() const
	{
		return mbPlaying;
	}
	bool IAnimation::IsForward() const
	{
		return mbForward;
	}
	void IAnimation::Update(f32 infDT)
	{
#if DEBUG
        if(infDT > 0.2f)
            infDT = 0.2f;
#endif
		//Return if not playing
		if (!mbPlaying)
		{
			return;
		}
        
        if(mfAnimDelayRemaining > 0)
        {
            mfAnimDelayRemaining -= infDT;
            if(mfAnimDelayRemaining > 0)
                return;
            
            infDT = -mfAnimDelayRemaining;
            mfAnimDelayRemaining = 0;
        }
        
        if(mfAnimDelayRemaining == 0 && !mbStarted)
        {
			if (OnStart)
			{
				OnStart(this);
			}
            
            mbStarted = true;
        }
		
		mfAnimPos += ((mbForward)?infDT:-infDT);
        
		mbLooped = false;
		
		f32 fProgression = GetProgression();
		
        bool bTriggerComplete = false;
		if (mePlayMode == ANIM_PLAYONCE && fProgression >= 1.0f)
		{
			mbPlaying = false;
			mbFinished = true;
            bTriggerComplete = true;
		}
		else if (mePlayMode == ANIM_PLAYLOOPING && fProgression >= 1.0f)
		{
			mfAnimPos = 0;
			mbLooped = true;
		}
		else if ((mePlayMode == ANIM_PINGPONG || mePlayMode == ANIM_PINGPONG_LOOPING) && fProgression >= 1.0f && mbForward == true)
		{
			mbForward = false;				
			mfAnimPos = mfDuration;
		}
		else if ((mePlayMode == ANIM_PINGPONG || mePlayMode == ANIM_PINGPONG_LOOPING) && fProgression <= 0.0f && mbForward == false)
		{
			mbForward = true;
			mfAnimPos = 0;
			if(mePlayMode == ANIM_PINGPONG)
			{
				mbPlaying = false;
				mbFinished = true;
                bTriggerComplete = true;
			}
			else
			{
				mbLooped = true;
			}
		}
		else if (mePlayMode == ANIM_PLAYONCE_REVERSE && fProgression <= 0.0f)
		{
			mbPlaying = false;
			mbFinished = true;
            bTriggerComplete = true;
		} 
		else if (mePlayMode == ANIM_PLAYLOOPING_REVERSE && fProgression <= 0.0f)
		{
			mfAnimPos = mfDuration;
			mbLooped = true;
		} 
		else if ((mePlayMode == ANIM_PINGPONG_REVERSE || mePlayMode == ANIM_PINGPONG_LOOPING_REVERSE) && fProgression <= 0.0f && mbForward == false)
		{
			mbForward = !mbForward;				
			mfAnimPos = 0.0f;
		} 
		else if ((mePlayMode == ANIM_PINGPONG_REVERSE || mePlayMode == ANIM_PINGPONG_LOOPING_REVERSE) && fProgression >= 1.0f && mbForward == true)
		{
			mbForward = !mbForward;
			mfAnimPos = mfDuration;
			if(mePlayMode == ANIM_PINGPONG_REVERSE)
			{
				mbPlaying = false;
				mbFinished = true;
                bTriggerComplete = true;
			}
		}
        else if (mePlayMode == ANIM_PLAYHALF_KEEP_MID_FRAME && fProgression > 0.5f)
        {
            mfAnimPos = mfDuration*0.5f;
        }
		
		mfAnimPos = Core::CMathUtils::Clamp(mfAnimPos, 0.0f, mfDuration);
		
		UpdateInternal();
        
        if(bTriggerComplete && OnComplete)
        {
            OnComplete(this);
        }
	}
	
	f32 IAnimation::GetProgression() const
	{
		return (mfDuration==0.0f)? 1.0f : mfAnimPos/mfDuration;
	}
	
	void IAnimation::SetProgression(f32 infProgress)
	{
		f32 fProgress = Core::CMathUtils::Clamp(infProgress, 0.0f, 1.0f);
		
		if(!mbForward)
			fProgress = 1.0f - fProgress;
		
		mfAnimPos = fProgress * mfDuration;
	}
	
	bool IAnimation::IsFinished() const
	{
		return mbFinished;
	}
	
	bool IAnimation::IsLooped() const
	{
		return mbLooped;
	}
    
	bool IAnimation::IsStarted() const
	{
		return mbStarted;
	}
}

