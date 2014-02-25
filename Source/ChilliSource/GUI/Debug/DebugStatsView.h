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

#ifdef DEBUG_STATS

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Image/ImageView.h>
#include <ChilliSource/GUI/Label/Label.h>

#define DECLARE_DEBUG_STAT(NAME) \
LabelPtr mpLabel##NAME; \
LabelPtr mpValue##NAME 

#define DEFINE_DEBUG_STAT(NAME) \
mpLabel##NAME = LabelPtr(new CLabel()); \
mpLabel##NAME->SetText(#NAME); \
mpLabel##NAME->SetTextScale(mfTextScale); \
mpLabel##NAME->SetAlignmentToParent(Core::AlignmentAnchor::k_topLeft); \
mpLabel##NAME->SetOffsetFromParentAlignment(0.0f, 0.0f, 5.0f, 0.0f); \
mpLabel##NAME->EnableAlignmentToParent(true); \
mpLabel##NAME->SetSize(0.0f, 0.0f, 90.0f, 20.0f); \
mpLabel##NAME->SetHorizontalJustification(JUSTIFY_LEFT); \
mpLabel##NAME->SetLocalAlignment(Core::AlignmentAnchor::k_topLeft); \
mpLabel##NAME->EnableUserInteraction(false); \
mpLabel##NAME->EnableTouchConsumption(false); \
mpLabel##NAME->EnableBackground(false); \
AddSubview(mpLabel##NAME); \
mpValue##NAME = LabelPtr(new CLabel()); \
mpValue##NAME->SetTextScale(mfTextScale); \
mpValue##NAME->SetLocalAlignment(Core::AlignmentAnchor::k_middleLeft); \
mpValue##NAME->SetAlignmentToParent(Core::AlignmentAnchor::k_middleRight); \
mpValue##NAME->SetOffsetFromParentAlignment(0.0f, 0.0f, 10.0f, 0.0f); \
mpValue##NAME->EnableAlignmentToParent(true); \
mpValue##NAME->SetSize(0.0f, 0.0f, 90.0f, 20.0f); \
mpValue##NAME->SetHorizontalJustification(JUSTIFY_LEFT); \
mpValue##NAME->EnableUserInteraction(false); \
mpValue##NAME->EnableTouchConsumption(false); \
mpValue##NAME->EnableBackground(false); \
mpLabel##NAME->AddSubview(mpValue##NAME) 

#define SET_DEBUG_STAT(NAME) \
{\
    std::string strValue = CDebugStats::GetValueForEvent(#NAME);\
    if(strValue.empty())\
        strValue = "0";\
    mpValue##NAME->SetText(strValue);\
}

namespace ChilliSource
{
    class ISurface;
    
	namespace GUI
	{
		class CDebugStatsView : public CImageView
		{
		public:
			
			static const DebugStatsViewPtr& GetSingletonPtr();
            
            void RefreshStats();
			
		public:
			
			DECLARE_DEBUG_STAT(FPS);
			DECLARE_DEBUG_STAT(FrameTime);
			DECLARE_DEBUG_STAT(DrawCalls);
			DECLARE_DEBUG_STAT(Verts);
            DECLARE_DEBUG_STAT(Meshes);
            DECLARE_DEBUG_STAT(Meshes_Trans);
            DECLARE_DEBUG_STAT(Sprites);
            DECLARE_DEBUG_STAT(Sprites_Trans);
            DECLARE_DEBUG_STAT(GUI);
			
		private:
			
            CDebugStatsView();
            
			static DebugStatsViewPtr pInstance;
            
            f32 mfTextScale;
		};
	}
}

#endif

#endif