//
//  SkinnedAnimation.cpp
//  Chilli Source
//
//  Created by Ian Copland on 17/10/2011.
//  Copyright 2011 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(SkinnedAnimation);
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        SkinnedAnimationUPtr SkinnedAnimation::Create()
        {
            return SkinnedAnimationUPtr(new SkinnedAnimation());
        }
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		SkinnedAnimation::SkinnedAnimation()
        : m_frameTime(0.0f)
		{
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		bool SkinnedAnimation::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == SkinnedAnimation::InterfaceID;
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		const SkinnedAnimation::FrameCUPtr& SkinnedAnimation::GetFrameAtIndex(u32 in_index) const
		{
            CS_ASSERT(in_index < m_frames.size(), "Skinned animation frame out of bounds");
            return m_frames[in_index];
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		f32 SkinnedAnimation::GetFrameTime() const
		{
			return m_frameTime;
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		u32 SkinnedAnimation::GetNumFrames() const
		{
			return m_frames.size();
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		void SkinnedAnimation::AddFrame(SkinnedAnimation::FrameCUPtr in_frame)
		{
			m_frames.push_back(std::move(in_frame));
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		void SkinnedAnimation::SetFrameTime(f32 in_timeBetweenFrames)
		{
			m_frameTime = in_timeBetweenFrames;
		}
	}
}

