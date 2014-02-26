/*
 *  EntityAnimation.h
 *  SceneLoader
 *
 *  Created by Stuart McGaw on 21/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_ENTITYANIMATION_H_
#define _MOFLO_CORE_ENTITYANIMATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Animation/GenericAnimation.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/Scene/SceneDescription.h>
#include <ChilliSource/Core/Scene/SceneAnimation.h>

namespace ChilliSource
{
	namespace Core
	{
		/*
		 Animation keyframe data. Usually loaded from a scene's animation file. Manually const
		 */
		struct EntityAnimationData
		{
            u32 udwKeyframeCount;
			std::shared_ptr<f32> afKeyframeTimes;
			std::shared_ptr<EntityTransform>	aKeyframeValues;
		};
		
		/*
		 An EntityAnimation object. It extends IAnimation
		 */
		class CEntityAnimation : public IAnimation
		{
		public:
			//----------------------------------------------
			/// CEntityAnimation
			///
			/// 
			/// @param A const pointer to a EntityAnimationData struct. 
			/// The pointer and its contents must be valid through the lifetime of the NodeAnimation
			/// @param Entity which the animation is to manipulate.
			//----------------------------------------------
			CEntityAnimation(const EntityAnimationData* inpAnimData, CEntity* inpTarget, u32 inudwQueryFlags = 0, f32 infInTime = -1, f32 infOutTime = -1);			
            void SetInAndOutTime(f32 inInTime, f32 inOutTime);
            void ToFrame(u32 inudwFrameNumber);
            CEntity* GetTarget();
            
            void SetInterpolationMode(InterpolationType ineType);
		protected:
            void UpdateInternal();
            void Step(u32 inudwLowFrame, u32 inudwHighFrame, f32 infT);
            void Lerp(u32 inudwLowFrame, u32 inudwHighFrame, f32 infT);
        public:
			CEntity*						mpTarget;			// Entity to be animated
        protected:
            
            typedef fastdelegate::FastDelegate3<u32, u32, f32> InterpolateDelegate;
            
            u32 mudwFrameCount;
            const std::shared_ptr<f32> mpafFrameTimes; // Array of pointers to frame times
			const std::shared_ptr<EntityTransform> mpasFrameValues; // Corresponding array of pointers to keyframes
            f32 mfInTime; // Float for the start of the animation
            f32 mfOutTime; // Float for the end of the animation
            InterpolateDelegate mInterpolateDelegate;
		};
		
		typedef std::shared_ptr<CEntityAnimation> EntityAnimationPtr;
        typedef std::weak_ptr<CEntityAnimation> EntityAnimationWPtr;
	}
}


#endif