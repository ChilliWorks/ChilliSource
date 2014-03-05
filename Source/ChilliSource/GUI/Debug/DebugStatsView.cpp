/*
 *  DebugStats.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 03/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifdef CS_ENABLE_DEBUGSTATS

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
            
            const f32 kfCurrentLetterHeight = Core::Application::Get()->GetDefaultFont()->GetLineHeight();
            
            mfTextScale = kfBestFitTextSize/kfCurrentLetterHeight   ;
            
			CS_DEFINE_DEBUGSTAT(FPS);
			CS_DEFINE_DEBUGSTAT(FrameTime);
			CS_DEFINE_DEBUGSTAT(DrawCalls);
			CS_DEFINE_DEBUGSTAT(Verts);
            CS_DEFINE_DEBUGSTAT(Meshes);
            CS_DEFINE_DEBUGSTAT(Meshes_Trans);
            CS_DEFINE_DEBUGSTAT(Sprites);
            CS_DEFINE_DEBUGSTAT(Sprites_Trans);
            CS_DEFINE_DEBUGSTAT(GUI);
			
			EnableUserInteraction(false);
			EnableTouchConsumption(false);
			EnableAlignmentToParent(true);
			
			SetSize(0.0f, 0.0f, 200.0f, mSubviews.size() * 22.0f);
			SetAlignmentToParent(Rendering::AlignmentAnchor::k_topLeft);
			SetOffsetFromParentAlignment(0.0f, 0.0f, 10.0f, -10.0f);
			SetLocalAlignment(Rendering::AlignmentAnchor::k_topLeft);
			
			SetTexture(Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<Rendering::TextureManager>()->GetDefaultTexture());
			SetColour(Core::Colour(0.41f, 0.41f, 0.41f, 0.8f));
			
			u32 i = 0;
			for(Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it, ++i)
			{
				(*it)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, -20.0f * i);
			}
		}
        
        void DebugStatsView::RefreshStats()
        {
            CS_SET_DEBUGSTAT(FPS);
            CS_SET_DEBUGSTAT(FrameTime);
            CS_SET_DEBUGSTAT(DrawCalls);
            CS_SET_DEBUGSTAT(Verts);
            CS_SET_DEBUGSTAT(Meshes);
            CS_SET_DEBUGSTAT(Meshes_Trans);
            CS_SET_DEBUGSTAT(Sprites);
            CS_SET_DEBUGSTAT(Sprites_Trans);
            CS_SET_DEBUGSTAT(GUI);
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

