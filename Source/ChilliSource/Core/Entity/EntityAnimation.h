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

#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/GenericAnimation.h>
#include <ChilliSource/Core/GenericEvent.h>
#include <ChilliSource/Core/SceneAnimation/SceneDescription.h>
#include <ChilliSource/Core/SceneAnimation/SceneAnimation.h>

namespace moFlo
{
	namespace Core
	{
		/*
		 Animation keyframe data. Usually loaded from a scene's animation file. Manually const
		 */
		struct EntityAnimationData
		{
            u32                                 udwKeyframeCount;
			SHARED_ARRAY_PTR<f32>               afKeyframeTimes;
			SHARED_ARRAY_PTR<EntityTransform>	aKeyframeValues;
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
            
            u32                                     mudwFrameCount;
            const SHARED_ARRAY_PTR<f32>             mpafFrameTimes;		// Array of pointers to frame times
			const SHARED_ARRAY_PTR<EntityTransform>	mpasFrameValues;	// Corresponding array of pointers to keyframes
            f32                                     mfInTime;           // Float for the start of the animation
            f32                                     mfOutTime;          // Float for the end of the animation
            InterpolateDelegate                     mInterpolateDelegate;
		};
		
		typedef SHARED_PTR<CEntityAnimation> EntityAnimationPtr;
        typedef WEAK_PTR<CEntityAnimation> EntityAnimationWPtr;
	}
}


#endif