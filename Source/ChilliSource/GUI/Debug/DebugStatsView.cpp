/*
 *  DebugStats.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 03/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifdef DEBUG_STATS

#include <ChilliSource/GUI/Debug/DebugStatsView.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Debugging/Base/DebugStats.h>
#include <ChilliSource/Core/Base/Application.h>

#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Font/Font.h>

namespace ChilliSource
{
	namespace GUI
	{
		DebugStatsViewPtr DebugStatsView::pInstance;
		
		DebugStatsView::DebugStatsView() : mfTextScale(1.0f)
		{
            const f32 kfBestFitTextSize = 10.0f;
            
            const f32 kfCurrentLetterHeight = Core::CApplication::GetDefaultFont()->GetLineHeight();
            
            mfTextScale = kfBestFitTextSize/kfCurrentLetterHeight   ;
            
			DEFINE_DEBUG_STAT(FPS);
			DEFINE_DEBUG_STAT(FrameTime);
			DEFINE_DEBUG_STAT(DrawCalls);
			DEFINE_DEBUG_STAT(Verts);
            DEFINE_DEBUG_STAT(Meshes);
            DEFINE_DEBUG_STAT(Meshes_Trans);
            DEFINE_DEBUG_STAT(Sprites);
            DEFINE_DEBUG_STAT(Sprites_Trans);
            DEFINE_DEBUG_STAT(GUI);
			
			EnableUserInteraction(false);
			EnableTouchConsumption(false);
			EnableAlignmentToParent(true);
			
			SetSize(0.0f, 0.0f, 200.0f, mSubviews.size() * 22.0f);
			SetAlignmentToParent(Core::AlignmentAnchor::k_topLeft);
			SetOffsetFromParentAlignment(0.0f, 0.0f, 10.0f, -10.0f);
			SetLocalAlignment(Core::AlignmentAnchor::k_topLeft);
			
			SetTexture(Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<Rendering::TextureManager>()->GetDefaultTexture());
			SetColour(Core::CColour(0.41f, 0.41f, 0.41f, 0.8f));
			
			u32 i = 0;
			for(Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it, ++i)
			{
				(*it)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, -20.0f * i);
			}
		}
        
        void DebugStatsView::RefreshStats()
        {
            SET_DEBUG_STAT(FPS);
            SET_DEBUG_STAT(FrameTime);
            SET_DEBUG_STAT(DrawCalls);
            SET_DEBUG_STAT(Verts);
            SET_DEBUG_STAT(Meshes);
            SET_DEBUG_STAT(Meshes_Trans);
            SET_DEBUG_STAT(Sprites);
            SET_DEBUG_STAT(Sprites_Trans);
            SET_DEBUG_STAT(GUI);
        }
		
		const DebugStatsViewPtr& DebugStatsView::GetSingletonPtr()
		{
            if(!pInstance)
            {
                pInstance = DebugStatsViewPtr(new DebugStatsView());
            }
            
			return pInstance;
		}
	}
}
#endif

