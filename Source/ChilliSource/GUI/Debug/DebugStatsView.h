/*
 *  DebugStats.h
 *  iOSTemplate
 *
 *  Created by Scott Downie on 03/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_GUI_DEFAULTS_DEBUG_STATS_H_
#define _MO_FLO_GUI_DEFAULTS_DEBUG_STATS_H_

#ifdef CS_ENABLE_DEBUGSTATS

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Image/ImageView.h>
#include <ChilliSource/GUI/Label/Label.h>

#define CS_DECLARE_DEBUGSTAT(NAME) \
LabelSPtr mpLabel##NAME; \
LabelSPtr mpValue##NAME 

#define CS_DEFINE_DEBUGSTAT(NAME) \
mpLabel##NAME = LabelSPtr(new Label()); \
mpLabel##NAME->SetText(#NAME); \
mpLabel##NAME->SetTextScale(mfTextScale); \
mpLabel##NAME->SetAlignmentToParent(Rendering::AlignmentAnchor::k_topLeft); \
mpLabel##NAME->SetOffsetFromParentAlignment(0.0f, 0.0f, 5.0f, 0.0f); \
mpLabel##NAME->EnableAlignmentToParent(true); \
mpLabel##NAME->SetSize(0.0f, 0.0f, 90.0f, 20.0f); \
mpLabel##NAME->SetHorizontalJustification(TextJustification::k_left); \
mpLabel##NAME->SetLocalAlignment(Rendering::AlignmentAnchor::k_topLeft); \
mpLabel##NAME->EnableUserInteraction(false); \
mpLabel##NAME->EnableTouchConsumption(false); \
mpLabel##NAME->EnableBackground(false); \
mpLabel##NAME->SetTextColour(Core::Colour::k_white); \
AddSubview(mpLabel##NAME); \
mpValue##NAME = LabelSPtr(new Label()); \
mpValue##NAME->SetTextScale(mfTextScale); \
mpValue##NAME->SetLocalAlignment(Rendering::AlignmentAnchor::k_middleLeft); \
mpValue##NAME->SetAlignmentToParent(Rendering::AlignmentAnchor::k_middleRight); \
mpValue##NAME->SetOffsetFromParentAlignment(0.0f, 0.0f, 10.0f, 0.0f); \
mpValue##NAME->EnableAlignmentToParent(true); \
mpValue##NAME->SetSize(0.0f, 0.0f, 90.0f, 20.0f); \
mpValue##NAME->SetHorizontalJustification(TextJustification::k_left); \
mpValue##NAME->EnableUserInteraction(false); \
mpValue##NAME->EnableTouchConsumption(false); \
mpValue##NAME->EnableBackground(false); \
mpValue##NAME->SetTextColour(Core::Colour::k_white); \
mpLabel##NAME->AddSubview(mpValue##NAME) 

#define CS_SET_DEBUGSTAT(NAME) \
{\
    std::string strValue = Debugging::DebugStats::GetValueForEvent(#NAME);\
    if(strValue.empty())\
        strValue = "0";\
    mpValue##NAME->SetText(strValue);\
}

namespace ChilliSource
{
    class ISurface;
    
	namespace GUI
	{
		class DebugStatsView : public ImageView
		{
		public:
			
			static const DebugStatsViewSPtr& GetSingletonPtr();
            
            void RefreshStats();
			
		public:
			
			CS_DECLARE_DEBUGSTAT(FPS);
			CS_DECLARE_DEBUGSTAT(FrameTime);
			CS_DECLARE_DEBUGSTAT(DrawCalls);
			CS_DECLARE_DEBUGSTAT(Verts);
            CS_DECLARE_DEBUGSTAT(Meshes);
            CS_DECLARE_DEBUGSTAT(Meshes_Trans);
            CS_DECLARE_DEBUGSTAT(Sprites);
            CS_DECLARE_DEBUGSTAT(Sprites_Trans);
            CS_DECLARE_DEBUGSTAT(GUI);
			
		private:
			
            DebugStatsView();
            
			static DebugStatsViewSPtr pInstance;
            
            f32 mfTextScale;
		};
	}
}

#else

#define CS_DECLARE_DEBUGSTAT(NAME)
#define CS_DEFINE_DEBUGSTAT(NAME) 
#define CS_SET_DEBUGSTAT(NAME)

#endif

#endif