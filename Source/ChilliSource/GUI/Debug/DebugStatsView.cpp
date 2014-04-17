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

#include <ChilliSource/Debugging/Base/DebugStats.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Font/Font.h>

namespace ChilliSource
{
	namespace GUI
	{
		DebugStatsViewSPtr DebugStatsView::pInstance;
		
		DebugStatsView::DebugStatsView() : mfTextScale(1.0f)
		{
            const f32 kfBestFitTextSize = 10.0f;
            
            const f32 kfCurrentLetterHeight = Core::Application::Get()->GetDefaultFont()->GetLineHeight();
            
            mfTextScale = kfBestFitTextSize/kfCurrentLetterHeight;
            
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
			
            Rendering::TextureCSPtr bgTex = Core::Application::Get()->GetResourcePool()->GetResource<Rendering::Texture>("_GUIBackgroundTex");
            if(bgTex == nullptr)
            {
                const u32 k_numPixels = 4;
                const u32 k_numBytesPerPixel = 4;
                Rendering::Texture::Descriptor desc;
                desc.m_width = 2;
                desc.m_height = 2;
                desc.m_format = Core::ImageFormat::k_RGBA8888;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_dataSize = k_numPixels * k_numBytesPerPixel;
                u8* data = new u8[desc.m_dataSize];
                memset(data, 255, desc.m_dataSize);
                
                Rendering::TextureSPtr texture = Core::Application::Get()->GetResourcePool()->CreateResource<Rendering::Texture>("_GUIBackgroundTex");
                texture->Build(desc, Rendering::Texture::TextureDataUPtr(data));
                bgTex = texture;
            }
            
			SetTexture(bgTex);
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
		
		const DebugStatsViewSPtr& DebugStatsView::GetSingletonPtr()
		{
            if(!pInstance)
            {
                pInstance = DebugStatsViewSPtr(new DebugStatsView());
            }
            
			return pInstance;
		}
	}
}
#endif

