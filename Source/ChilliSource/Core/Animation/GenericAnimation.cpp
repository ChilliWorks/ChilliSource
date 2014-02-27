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

namespace ChilliSource
{
    namespace Core
    {
        Animation::Animation(f32 infDuration, f32 infStartDelay)
        :mfAnimPos(0), mfDuration(infDuration),mfTimeScaleFactor(1.0f), mfStartDelay(infStartDelay )
        {
            mePlayMode = AnimationPlayMode::k_once;
            mbForward = true;
            mbPlaying = false;
            mbFinished = false;
            mbLooped = false;
            mbStarted = false;
        }
        
        Animation::~Animation(){}
        
        void Animation::SetOnCompleteDelegate(OnCompleteDelegate inDelegate)
        {
            OnComplete = inDelegate;
        }
        
        void Animation::SetOnStartDelegate(OnStartDelegate inDelegate)
        {
            OnStart = inDelegate;
        }
        
        void Animation::SetStartDelay(f32 infStartDelay)
        {
            mfStartDelay = infStartDelay;
        }
        
        f32 Animation::GetStartDelay()
        {
            return mfStartDelay;
        }
        
        f32 Animation::GetDelayRemaining()
        {
            return mfAnimDelayRemaining;
        }
        
        void Animation::SetDuration(f32 infValue)
        {
            mfDuration = infValue;
        }
        f32 Animation::GetDuration() const
        {
            return mfDuration;
        }
        
        void Animation::SetTimeScaleFactor(f32 infValue)
        {
            mfTimeScaleFactor = std::max(0.0f,infValue);
        }
        f32 Animation::GetTimeScaleFactor(f32 infValue) const{
            return mfTimeScaleFactor;
        }
        
        AnimationPlayMode Animation::GetPlayMode() const{
            return mePlayMode;
        }
        
        void Animation::Play(AnimationPlayMode inePlayMode)
        {
            switch (inePlayMode)
            {
                case AnimationPlayMode::k_once :
                case AnimationPlayMode::k_looping :
                case AnimationPlayMode::k_pingPong :
                case AnimationPlayMode::k_pingPongLooping :
                case AnimationPlayMode::k_stopHalf:
                    mfAnimPos = 0.0f;
                    mbForward = true;
                    break;
                    
                case AnimationPlayMode::k_onceReverse :
                case AnimationPlayMode::k_loopingReverse :
                case AnimationPlayMode::k_pingPongReverse :
                case AnimationPlayMode::k_pingPongLoopingReverse :
                    mfAnimPos = mfDuration;
                    mbForward = false;
                    break;

                default :
                    CS_FATAL_LOG("Cannot play animation with given mode");
                    break;
            }

            mfAnimDelayRemaining = mfStartDelay;
            
            mbPlaying = true;
            mePlayMode = inePlayMode;
            mbFinished = false;
        }
        
        void Animation::Stop()
        {
            mbPlaying = false;
        }
        
        bool Animation::IsPlaying() const
        {
            return mbPlaying;
        }
        bool Animation::IsForward() const
        {
            return mbForward;
        }
        void Animation::Update(f32 infDT)
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
            if (mePlayMode == AnimationPlayMode::k_once && fProgression >= 1.0f)
            {
                mbPlaying = false;
                mbFinished = true;
                bTriggerComplete = true;
            }
            else if (mePlayMode == AnimationPlayMode::k_looping && fProgression >= 1.0f)
            {
                mfAnimPos = 0;
                mbLooped = true;
            }
            else if ((mePlayMode == AnimationPlayMode::k_pingPong || mePlayMode == AnimationPlayMode::k_pingPongLooping) && fProgression >= 1.0f && mbForward == true)
            {
                mbForward = false;				
                mfAnimPos = mfDuration;
            }
            else if ((mePlayMode == AnimationPlayMode::k_pingPong || mePlayMode == AnimationPlayMode::k_pingPongLooping) && fProgression <= 0.0f && mbForward == false)
            {
                mbForward = true;
                mfAnimPos = 0;
                if(mePlayMode == AnimationPlayMode::k_pingPong)
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
            else if (mePlayMode == AnimationPlayMode::k_onceReverse && fProgression <= 0.0f)
            {
                mbPlaying = false;
                mbFinished = true;
                bTriggerComplete = true;
            } 
            else if (mePlayMode == AnimationPlayMode::k_loopingReverse && fProgression <= 0.0f)
            {
                mfAnimPos = mfDuration;
                mbLooped = true;
            } 
            else if ((mePlayMode == AnimationPlayMode::k_pingPongReverse || mePlayMode == AnimationPlayMode::k_pingPongLoopingReverse) && fProgression <= 0.0f && mbForward == false)
            {
                mbForward = !mbForward;				
                mfAnimPos = 0.0f;
            } 
            else if ((mePlayMode == AnimationPlayMode::k_pingPongReverse || mePlayMode == AnimationPlayMode::k_pingPongLoopingReverse) && fProgression >= 1.0f && mbForward == true)
            {
                mbForward = !mbForward;
                mfAnimPos = mfDuration;
                if(mePlayMode == AnimationPlayMode::k_pingPongReverse)
                {
                    mbPlaying = false;
                    mbFinished = true;
                    bTriggerComplete = true;
                }
            }
            else if (mePlayMode == AnimationPlayMode::k_stopHalf && fProgression > 0.5f)
            {
                mfAnimPos = mfDuration*0.5f;
            }
            
            mfAnimPos = Core::MathUtils::Clamp(mfAnimPos, 0.0f, mfDuration);
            
            UpdateInternal();
            
            if(bTriggerComplete && OnComplete)
            {
                OnComplete(this);
            }
        }
        
        f32 Animation::GetProgression() const
        {
            return (mfDuration==0.0f)? 1.0f : mfAnimPos/mfDuration;
        }
        
        void Animation::SetProgression(f32 infProgress)
        {
            f32 fProgress = Core::MathUtils::Clamp(infProgress, 0.0f, 1.0f);
            
            if(!mbForward)
                fProgress = 1.0f - fProgress;
            
            mfAnimPos = fProgress * mfDuration;
        }
        
        bool Animation::IsFinished() const
        {
            return mbFinished;
        }
        
        bool Animation::IsLooped() const
        {
            return mbLooped;
        }
        
        bool Animation::IsStarted() const
        {
            return mbStarted;
        }
    }
}

