/*
 *  DebugStats.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 03/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifdef DEBUG_STATS

#include <moFlo/GUI/Defaults/DebugStatsView.h>

#include <moFlo/Core/ResourceManagerDispenser.h>
#include <moFlo/Core/DebugStats.h>
#include <moFlo/Core/Application.h>

#include <moFlo/Rendering/TextureManager.h>
#include <moFlo/Rendering/Font.h>

namespace moFlo
{
	namespace GUI
	{
		DebugStatsViewPtr CDebugStatsView::pInstance;
		
		CDebugStatsView::CDebugStatsView() : mfTextScale(1.0f)
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
			SetAlignmentToParent(Core::ALIGN_TOP_LEFT);
			SetOffsetFromParentAlignment(0.0f, 0.0f, 10.0f, -10.0f);
			SetLocalAlignment(Core::ALIGN_TOP_LEFT);
			
			SetTexture(Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<Rendering::ITextureManager>()->GetDefaultTexture());
			SetColour(Core::CColour(0.41f, 0.41f, 0.41f, 0.8f));
			
			u32 i = 0;
			for(Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it, ++i)
			{
				(*it)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, -20.0f * i);
			}
		}
        
        void CDebugStatsView::RefreshStats()
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
		
		const DebugStatsViewPtr& CDebugStatsView::GetSingletonPtr()
		{
            if(!pInstance)
            {
                pInstance = DebugStatsViewPtr(new CDebugStatsView());
            }
            
			return pInstance;
		}
	}
}
#endif

