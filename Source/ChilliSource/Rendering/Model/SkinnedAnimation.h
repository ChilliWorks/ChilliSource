/*
 *  SkinnedAnimation.h
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_SKELETALANIMATION_H_
#define _MO_FLO_RENDERING_SKELETALANIMATION_H_

#include <ChilliSource/Rendering/ForwardDeclarations.h>

#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//---------------------------------------------------------------------
		/// AnimationFrame
		///
		/// Contains all relevant data for a single frame of animation.
		//---------------------------------------------------------------------
		struct SkinnedAnimationFrame
		{
			DYNAMIC_ARRAY<Core::CVector3> mNodeTranslations;
			DYNAMIC_ARRAY<Core::CQuaternion> mNodeOrientations;
            DYNAMIC_ARRAY<Core::CVector3> mNodeScalings;
		};
		//---------------------------------------------------------------------
		/// Skeletal Animation Resource.
		//---------------------------------------------------------------------
		class CSkinnedAnimation : public Core::IResource
		{
		public:
			DECLARE_NAMED_INTERFACE(CSkinnedAnimation);
			virtual ~CSkinnedAnimation();
			
			//---------------------------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//---------------------------------------------------------------------
			/// Get Frame At Index
			///
			/// @param the index to the frame
			/// @return the AnimationFramePtr at the given index
			//---------------------------------------------------------------------
			SkinnedAnimationFramePtr GetFrameAtIndex(u32 indwFrameIndex) const;
			//---------------------------------------------------------------------
			/// Get Frame Time
			///
			/// @return the time between frames.
			//---------------------------------------------------------------------
			f32 GetFrameTime() const;
			//---------------------------------------------------------------------
			/// Get Num Frames
			///
			/// @return the number of frames in the animation.
			//---------------------------------------------------------------------
			u32 GetNumFrames() const;
		private:
			//---------------------------------------------------------------------
			/// Constructor
			///
			/// Declared private so that this can only be created by the
			/// Skeletal Animation Manager.
			//---------------------------------------------------------------------
			CSkinnedAnimation();
			//---------------------------------------------------------------------
			/// Add Frame
			///
			/// Adds a new frame to the end of the animation.
			///
			/// @param The new frame.
			//---------------------------------------------------------------------
			void AddFrame(const SkinnedAnimationFramePtr& inpAnimationFrame);
			//---------------------------------------------------------------------
			/// Set Frame Rate
			///
			/// Sets the frame rate of the animation. Do not use this for changing
			/// the speed of an animation. Instead changing the speed through
			/// the animated comoponent.
			///
			/// @param The frame rate
			//---------------------------------------------------------------------
			void SetFrameTime(f32 infFrameTime);
			
			friend class CMoAnimLoader;
			friend class CSkinnedAnimationManager;
		private:
			
			f32 mfFrameTime;
			DYNAMIC_ARRAY<SkinnedAnimationFramePtr> mFrames;
		};
	}
}

#endif