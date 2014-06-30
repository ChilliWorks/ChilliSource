//
//  SkinnedAnimation.cpp
//  Chilli Source
//  Created by Ian Copland on 17/10/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
		const SkinnedAnimation::Frame* SkinnedAnimation::GetFrameAtIndex(u32 in_index) const
		{
            CS_ASSERT(in_index < m_frames.size(), "Skinned animation frame out of bounds");
            return m_frames[in_index].get();
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

