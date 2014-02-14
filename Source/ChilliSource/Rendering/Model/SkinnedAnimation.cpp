/*
 *  SkinnedAnimation.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>

namespace moFlo
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(CSkinnedAnimation);
		//--------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------
		CSkinnedAnimation::CSkinnedAnimation() : mfFrameTime(0.0f)
		{
		}
		//---------------------------------------------------------------------
		/// Is A
		//---------------------------------------------------------------------
		bool CSkinnedAnimation::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CSkinnedAnimation::InterfaceID;
		}
		//---------------------------------------------------------------------
		/// Get Frame At Index
		//---------------------------------------------------------------------
		SkinnedAnimationFramePtr CSkinnedAnimation::GetFrameAtIndex(u32 indwFrameIndex) const
		{
			if (indwFrameIndex < mFrames.size())
			{
				return mFrames[indwFrameIndex];
			}
			return SkinnedAnimationFramePtr();
		}
		//---------------------------------------------------------------------
		/// Get Frame Time
		//---------------------------------------------------------------------
		f32 CSkinnedAnimation::GetFrameTime() const
		{
			return mfFrameTime;
		}
		//---------------------------------------------------------------------
		/// Get Num Frames
		//---------------------------------------------------------------------
		u32 CSkinnedAnimation::GetNumFrames() const
		{
			return mFrames.size();
		}
		//---------------------------------------------------------------------
		/// Add Frame
		//---------------------------------------------------------------------
		void CSkinnedAnimation::AddFrame(const SkinnedAnimationFramePtr& inpAnimationFrame)
		{
			mFrames.push_back(inpAnimationFrame);
		}
		//---------------------------------------------------------------------
		/// Set Frame Rate
		//---------------------------------------------------------------------
		void CSkinnedAnimation::SetFrameTime(f32 infFrameTime)
		{
			mfFrameTime = infFrameTime;
		}
		//-----------------------------------------------------------------
		/// Destructor
		//-----------------------------------------------------------------
		CSkinnedAnimation::~CSkinnedAnimation()
		{
			mFrames.clear();
		}
	}
}

