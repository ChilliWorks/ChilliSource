/*
 *  SkinnedAnimation.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(SkinnedAnimation);
		//--------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------
		SkinnedAnimation::SkinnedAnimation() : mfFrameTime(0.0f)
		{
		}
		//---------------------------------------------------------------------
		/// Is A
		//---------------------------------------------------------------------
		bool SkinnedAnimation::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == SkinnedAnimation::InterfaceID;
		}
		//---------------------------------------------------------------------
		/// Get Frame At Index
		//---------------------------------------------------------------------
		SkinnedAnimationFrameSPtr SkinnedAnimation::GetFrameAtIndex(u32 indwFrameIndex) const
		{
			if (indwFrameIndex < mFrames.size())
			{
				return mFrames[indwFrameIndex];
			}
			return SkinnedAnimationFrameSPtr();
		}
		//---------------------------------------------------------------------
		/// Get Frame Time
		//---------------------------------------------------------------------
		f32 SkinnedAnimation::GetFrameTime() const
		{
			return mfFrameTime;
		}
		//---------------------------------------------------------------------
		/// Get Num Frames
		//---------------------------------------------------------------------
		u32 SkinnedAnimation::GetNumFrames() const
		{
			return mFrames.size();
		}
		//---------------------------------------------------------------------
		/// Add Frame
		//---------------------------------------------------------------------
		void SkinnedAnimation::AddFrame(const SkinnedAnimationFrameSPtr& inpAnimationFrame)
		{
			mFrames.push_back(inpAnimationFrame);
		}
		//---------------------------------------------------------------------
		/// Set Frame Rate
		//---------------------------------------------------------------------
		void SkinnedAnimation::SetFrameTime(f32 infFrameTime)
		{
			mfFrameTime = infFrameTime;
		}
		//-----------------------------------------------------------------
		/// Destructor
		//-----------------------------------------------------------------
		SkinnedAnimation::~SkinnedAnimation()
		{
			mFrames.clear();
		}
	}
}

