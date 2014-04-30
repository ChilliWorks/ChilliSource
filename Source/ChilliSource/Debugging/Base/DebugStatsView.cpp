/*
 *  DebugStats.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 03/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifdef CS_ENABLE_DEBUGSTATS

#include <ChilliSource/Debugging/Base/DebugStatsView.h>

#include <ChilliSource/Debugging/Base/DebugStats.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Font/Font.h>

namespace ChilliSource
{
	namespace Debugging
	{
        namespace
        {
            //--------------------------------------------------
            /// Creates the GUI background texture if it does
            /// not already exist.
            ///
            /// @author S Downie.
            //--------------------------------------------------
            Rendering::TextureCSPtr CreateBackgroundTexture()
            {
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
                
                return bgTex;
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        DebugStatsView::DebugStatsView()
        {
            EnableUserInteraction(false);
            EnableTouchConsumption(false);
            EnableAlignmentToParent(true);
            
            SetSize(0.0f, 0.0f, 200.0f, mSubviews.size() * 22.0f);
            SetAlignmentToParent(Rendering::AlignmentAnchor::k_topLeft);
            SetOffsetFromParentAlignment(0.0f, 0.0f, 10.0f, -10.0f);
            SetLocalAlignment(Rendering::AlignmentAnchor::k_topLeft);

            SetTexture(CreateBackgroundTexture());
            SetColour(Core::Colour(0.41f, 0.41f, 0.41f, 0.8f));
            
            u32 i = 0;
            for(Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it, ++i)
            {
                (*it)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, -20.0f * i);
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void DebugStatsView::Refresh(const std::unordered_map<std::string, std::string>& in_statsMap)
        {
            TryBuild(in_statsMap);
            
            for (auto& guiStatPair : m_guiStats)
            {
                auto statIt = in_statsMap.find(guiStatPair.first);
                CS_ASSERT(statIt != in_statsMap.end(), "Debug Stats View: Logic error. The list of stats and GUI stats should be the same by now.");
                guiStatPair.second.m_valueLabel->SetText(statIt->second);
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void DebugStatsView::TryBuild(const std::unordered_map<std::string, std::string>& in_statsMap)
        {
            if (ContainsSameStats(in_statsMap) == false)
            {
                CleanupGUIStats();
                
                const f32 k_bestFitTextSize = 20.0f;
                const f32 currentLetterHeight = Core::Application::Get()->GetDefaultFont()->GetLineHeight();
                const f32 textScale = k_bestFitTextSize / currentLetterHeight;
                
                u32 count = 0;
                for (const auto& stat : in_statsMap)
                {
                    GUIStat guiStat;
                    
                    guiStat.m_nameLabel = GUI::LabelSPtr(new GUI::Label());
                    guiStat.m_nameLabel->SetText(stat.first);
                    guiStat.m_nameLabel->SetTextScale(textScale);
                    guiStat.m_nameLabel->SetAlignmentToParent(Rendering::AlignmentAnchor::k_topLeft);
                    guiStat.m_nameLabel->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, -20.0f * count);
                    guiStat.m_nameLabel->EnableAlignmentToParent(true);
                    guiStat.m_nameLabel->SetSize(0.0f, 0.0f, 90.0f, 20.0f);
                    guiStat.m_nameLabel->SetHorizontalJustification(GUI::TextJustification::k_left);
                    guiStat.m_nameLabel->SetLocalAlignment(Rendering::AlignmentAnchor::k_topLeft);
                    guiStat.m_nameLabel->EnableUserInteraction(false);
                    guiStat.m_nameLabel->EnableTouchConsumption(false);
                    guiStat.m_nameLabel->EnableBackground(false);
                    guiStat.m_nameLabel->SetTextColour(Core::Colour::k_white);
                    AddSubview(guiStat.m_nameLabel);
                    
                    guiStat.m_valueLabel = GUI::LabelSPtr(new GUI::Label());
                    guiStat.m_valueLabel->SetTextScale(textScale);
                    guiStat.m_valueLabel->SetLocalAlignment(Rendering::AlignmentAnchor::k_middleLeft);
                    guiStat.m_valueLabel->SetAlignmentToParent(Rendering::AlignmentAnchor::k_middleRight);
                    guiStat.m_valueLabel->SetOffsetFromParentAlignment(0.0f, 0.0f, 10.0f, 0.0f);
                    guiStat.m_valueLabel->EnableAlignmentToParent(true);
                    guiStat.m_valueLabel->SetSize(0.0f, 0.0f, 90.0f, 20.0f);
                    guiStat.m_valueLabel->SetHorizontalJustification(GUI::TextJustification::k_left);
                    guiStat.m_valueLabel->EnableUserInteraction(false);
                    guiStat.m_valueLabel->EnableTouchConsumption(false);
                    guiStat.m_valueLabel->EnableBackground(false);
                    guiStat.m_valueLabel->SetTextColour(Core::Colour::k_white);
                    guiStat.m_nameLabel->AddSubview(guiStat.m_valueLabel);
                    
                    m_guiStats.emplace(stat.first, guiStat);
                    count++;
                }
                
                SetSize(0.0f, 0.0f, 200.0f, m_guiStats.size() * 22.0f);
            }
        }
        //--------------------------------------------------
        //--------------------------------------------------
        bool DebugStatsView::ContainsSameStats(const std::unordered_map<std::string, std::string>& in_statsMap) const
        {
            if (in_statsMap.size() != m_guiStats.size())
            {
                return false;
            }
            
            for (const auto& guiStat : m_guiStats)
            {
                if (in_statsMap.find(guiStat.first) == in_statsMap.end())
                {
                    return false;
                }
            }
            
            return true;
        }
        //--------------------------------------------------
        //--------------------------------------------------
        void DebugStatsView::CleanupGUIStats()
        {
            for (auto& guiStat : m_guiStats)
            {
                guiStat.second.m_nameLabel->RemoveFromParentView();
            }
            m_guiStats.clear();
        }
	}
}

#endif

/*


 
 
 
 
 
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

 
 
*/