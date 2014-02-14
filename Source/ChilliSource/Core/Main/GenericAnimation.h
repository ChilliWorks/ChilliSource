/** 
 * File: GenericAnimation.h
 * Date: 26/10/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_GENERICANIMATION_H_
#define _MOFLO_CORE_GENERICANIMATION_H_

#include <ChilliSource/Core/Main/FastDelegate.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace moFlo
{
	enum AnimationPlayMode
	{
		ANIM_PLAYONCE, 
		ANIM_PLAYLOOPING, 
		ANIM_PINGPONG, 
		ANIM_PINGPONG_LOOPING, 
		ANIM_PLAYONCE_REVERSE, 
		ANIM_PLAYLOOPING_REVERSE, 
		ANIM_PINGPONG_REVERSE, 
		ANIM_PINGPONG_LOOPING_REVERSE,
        ANIM_PLAYHALF_KEEP_MID_FRAME,
		
		ANIM_TOTAL,
	};

	///<IAnimation is the base class for any kind of playable animation
	class IAnimation
	{
	public:

		typedef fastdelegate::FastDelegate1<IAnimation *> OnCompleteDelegate;
        typedef fastdelegate::FastDelegate1<IAnimation *> OnStartDelegate;

		IAnimation(f32 infDuration = 0.0f, f32 infStartDelay = 0.0f);
		
		virtual ~IAnimation();
		
		void SetOnCompleteDelegate(OnCompleteDelegate inDelegate);
        void SetOnStartDelegate(OnStartDelegate inDelegate);

        void SetStartDelay(f32 infStartDelay);
        f32 GetStartDelay();
        f32 GetDelayRemaining();

		void SetDuration(f32 infValue);
		f32 GetDuration() const;
		
		void SetTimeScaleFactor(f32 infValue);
		f32 GetTimeScaleFactor(f32 infValue) const ;
		
		AnimationPlayMode GetPlayMode() const;
		
		void Play(AnimationPlayMode inePlayMode = ANIM_PLAYONCE);
		
		void Stop();
		
		bool IsPlaying() const;
		bool IsForward() const;
		bool IsFinished() const;
		bool IsLooped() const;
        bool IsStarted() const;
		void Update(f32 infDT);
        
        u32 GetFlags() const { return mudwFlags; }

		f32 GetProgression() const;
		void SetProgression(f32 infProgress);

	protected:
		virtual void UpdateInternal() = 0;

		OnCompleteDelegate OnComplete;
        OnStartDelegate OnStart;
		f32 mfAnimPos;
        f32 mfAnimDelayRemaining;
		f32 mfDuration;
        f32 mfStartDelay;
		f32 mfTimeScaleFactor;
		AnimationPlayMode mePlayMode;
		bool mbPlaying;
		bool mbForward;
        bool mbFinished;
        bool mbLooped;
        bool mbStarted;
		
        u32 mudwFlags;
	};
	
	typedef SHARED_PTR<IAnimation> AnimationPtr;
	
	//<ValueAnimation is a template class which represents the animation of a specific value
	template <typename T> class ValueAnimation : public IAnimation
	{
	public:
		typedef typename fastdelegate::FastDelegate1<const T &> SetCRValueDelegate;
		typedef typename fastdelegate::FastDelegate1<T> SetValueDelegate;
		
		ValueAnimation(f32 infDuration)
		:IAnimation(infDuration)
		{
			
		}
		
		void SetValueCallback(const SetValueDelegate& inDelegate)
		{
			SetValue = inDelegate;
		}
		void SetValueCallback(const SetCRValueDelegate& inDelegate)
		{
			SetCRValue = inDelegate;
		}
		
		virtual T GetValue() const = 0;
		
		void UpdateInternal(){
			//If we have a delegate to call with our value, pass it
			if (SetValue)
			{
				SetValue(GetValue());
			}
			else if (SetCRValue)
			{
				SetCRValue(GetValue());
			} 	
		}
		
	protected:
		SetCRValueDelegate SetCRValue;
		SetValueDelegate SetValue;
	};
	
	template <typename T> class LinearAnimation : public ValueAnimation<T>
	{
	public:
		LinearAnimation()
		:ValueAnimation<T>(0.0f)
		{
			
		}
		LinearAnimation(const T & inStartVal, const T & inEndVal, f32 infDuration = 0.0f)
		:ValueAnimation<T>(infDuration), mStartVal(inStartVal), mEndVal(inEndVal)
		{
			
		}
		
		void SetStartValue(T inValue)
		{
			mStartVal = inValue;
		}
		void SetEndValue(T inValue)
		{
			mEndVal = inValue;
		}
		
		virtual ~LinearAnimation(){}
		
		T GetValue() const
		{
			f32 fProg = this->GetProgression();
			return Core::CMathUtils::Lerp(fProg, mStartVal, mEndVal);
		}
		
	protected:
		T mStartVal;
		T mEndVal;
	};
	
	template <typename T> class BounceAnimation : public ValueAnimation<T>
	{
	public:
		BounceAnimation()
		:ValueAnimation<T>(0.0f), mudwNumBounces(1), mfBounceTotalWidth(0.f)
		{
			
		}
		BounceAnimation(const T & inStartVal, const T & inEndVal, f32 infDuration = 0.0f, f32 infAmplitude = 1.0f)
		:ValueAnimation<T>(infDuration), mStartVal(inStartVal), mEndVal(inEndVal), mudwNumBounces(1), mfBounceTotalWidth(0.f)
		{
			SetAmplitude(infAmplitude);
		}
		
		void SetAmplitude(f32 infAmplitude)
		{
			mfAmplitude = infAmplitude;
		}
		void SetStartValue(T inValue)
		{
			mStartVal = inValue;
		}
		void SetEndValue(T inValue)
		{
			mEndVal = inValue;
		}
        
        /** set the number of desired bounces in the animation, calculates ratio of total distance against initial bounce */
        void SetNumBounces(u32 inudwBounces)
        {
            assert(inudwBounces > 0 && "if overriding bounce animation, must have positive num bounces");
            mudwNumBounces = inudwBounces;
            
            for(int i=1; i < mudwNumBounces+1; ++i)
                mfBounceTotalWidth += 1.f/(f32)i;
        }
		
		virtual ~BounceAnimation(){}
		
		T GetValue() const
		{
			f32 fValue = 0.0f;
			f32 fProg = this->GetProgression();
			
			T valStart = mStartVal;
			T valEnd = mEndVal;
			f32 fAmplitude = mfAmplitude;
            
            // if more than one bounce, get the bounce we're on so we can lower the amplitude
            if(mudwNumBounces > 1)
            {
                if(fProg != 0.f)
                {
                    fAmplitude *= 1.f/( (u32)(fProg*mudwNumBounces)+1);
                }
            }
			
			if (fAmplitude < 0.0f)
			{
				fProg = 1.0f - fProg;
				valEnd = mStartVal;
				valStart = mEndVal;
				fAmplitude = -mfAmplitude;
			}
			
			if (fAmplitude > 1.0f)
			{
				f32 fCeil = moFlo::Core::kPI - asinf(1.0f / fAmplitude);
				fValue = sin(fProg * fCeil * (f32)mudwNumBounces) * fAmplitude;
			}
			
			return valStart + (valEnd - valStart) * fValue;
		}
		
	protected:
		T mStartVal;
		T mEndVal;
		f32 mfAmplitude;
        u32 mudwNumBounces;
        f32 mfBounceTotalWidth;
	};
	
	////////////////////////////////////////////////////////////////////
	// KeyframeAnimation class 
	//
	// This class copies keyframe data it is passed
	////////////////////////////////////////////////////////////////////
	
	template <typename T> class KeyframeAnimation : public ValueAnimation<T>
	{
	public:
		KeyframeAnimation()
		:ValueAnimation<T>(0.0f)
		{
			mpaKeyValues = NULL;
			mpafKeyframeTimes = NULL;
		}
		
		/////////////////////////////////////////////////////////////////////////////////
		// Version using unique keyframe data
		/////////////////////////////////////////////////////////////////////////////////
		KeyframeAnimation(const DYNAMIC_ARRAY<f32>* inpafKeyframeTimes, const DYNAMIC_ARRAY<T>* inpaKeyValues)
		:ValueAnimation<T>(inpafKeyframeTimes->back()), mpafKeyframeTimes(inpafKeyframeTimes), mpaKeyValues(inpaKeyValues), mbOwnsData(false)
		{
			ParameteriseKeyframeTimes();			
		}
		/////////////////////////////////////////////////////////////////////////////////
		// Version copying keyframe data
		/////////////////////////////////////////////////////////////////////////////////
		KeyframeAnimation(const DYNAMIC_ARRAY<f32> & inafKeyframeTimes, const DYNAMIC_ARRAY<T> & inaKeyValues)
		:ValueAnimation<T>(inafKeyframeTimes.back()), mbOwnsData(true)
		{
			SetData(inafKeyframeTimes,inaKeyValues);	
		}
		
		void SetData(const DYNAMIC_ARRAY<f32>  & inafKeyframeTimes, const DYNAMIC_ARRAY<T> & inaKeyValues)
		{
			{
				DYNAMIC_ARRAY<f32> * pLocalKeyFrameTimes = new DYNAMIC_ARRAY<f32>();
				pLocalKeyFrameTimes->resize(inafKeyframeTimes.size());
				std::copy(inafKeyframeTimes.begin(), inafKeyframeTimes.end(),pLocalKeyFrameTimes->begin());
				
				mpafKeyframeTimes = pLocalKeyFrameTimes;
			}
			{
				DYNAMIC_ARRAY<T> * pLocalKeyFrameValues  = new DYNAMIC_ARRAY<T>();
				pLocalKeyFrameValues->resize(inaKeyValues.size());
				std::copy(inaKeyValues.begin(), inaKeyValues.end(),pLocalKeyFrameValues->begin());
				
				mpaKeyValues = pLocalKeyFrameValues;
			}
			mbOwnsData = true;
			
			ParameteriseKeyframeTimes();

			this->SetDuration(inafKeyframeTimes.back());
		}
		
		virtual ~KeyframeAnimation()
		{
			if (mbOwnsData)
			{
				SAFE_DELETE(mpafKeyframeTimes);
				SAFE_DELETE(mpaKeyValues);
			}
		}
		
		T GetValue() const
		{
			f32 fProg = this->GetProgression();
			
			DYNAMIC_ARRAY<f32>::const_iterator pV = std::lower_bound(mpafKeyframeTimes->begin(), mpafKeyframeTimes->end(), fProg);
			
			u32 lowIndex = (&(*pV) - &((*mpafKeyframeTimes)[0]));
			if (lowIndex > 0)
				--lowIndex;
			u32 highIndex = lowIndex + 1;
			
			if (highIndex == mpaKeyValues->size())
			{
				return mpaKeyValues->back();
			}
			else
			{
				f32 fMix = (fProg - (*mpafKeyframeTimes)[lowIndex]) / ((*mpafKeyframeTimes)[highIndex] - (*mpafKeyframeTimes)[lowIndex]);
				
				const T & lowValue = (*mpaKeyValues)[lowIndex];
				const T & highValue = (*mpaKeyValues)[highIndex];
				
				return ((1.0f-fMix) * lowValue) + (fMix * highValue);	
			}
		}
		
		u32 GetCurrentLowerFrameIndex(){
			f32 fProg = this->GetProgression();
			
			DYNAMIC_ARRAY<f32>::const_iterator pV = std::lower_bound(mpafKeyframeTimes->begin(),mpafKeyframeTimes->end(), fProg);
			
			u32 udwLowIndex = (&(*pV) - &((*mpafKeyframeTimes)[0]));
			if (udwLowIndex > 0)
				--udwLowIndex;

			return udwLowIndex;
		}
		
	protected:
		void ParameteriseKeyframeTimes()
		{
			if (mpafKeyframeTimes && mpafKeyframeTimes->back() > 1.0f)
			{
				f32 fScaleFactor = mpafKeyframeTimes->back();
				
				DYNAMIC_ARRAY<f32>* pNonConstTimes = const_cast<DYNAMIC_ARRAY<f32>*>(mpafKeyframeTimes);
				
				for (u32 nKeyframeTime = 0; nKeyframeTime < pNonConstTimes->size(); nKeyframeTime++)
				{
					(*pNonConstTimes)[nKeyframeTime] /=	fScaleFactor;
				}
			}
		}
		
		bool mbOwnsData;
		const DYNAMIC_ARRAY<f32> * mpafKeyframeTimes;
		const DYNAMIC_ARRAY<T> * mpaKeyValues;
	};

}
#endif