//
//  DebugStatsView.cpp
//  Chilli Source
//  Created by Scott Downie on 03/08/2011.
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
            const std::string k_defaultFont = "Fonts/CarlitoMed.csfont";
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

            SetTexture(Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Texture>(Core::StorageLocation::k_chilliSource, "Textures/Blank.csimage"));
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
                
                Rendering::FontCSPtr font = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Font>(Core::StorageLocation::k_chilliSource, k_defaultFont);
                
                const f32 k_bestFitTextSize = 20.0f;
                const f32 currentLetterHeight = font->GetLineHeight();
                const f32 textScale = k_bestFitTextSize / currentLetterHeight;
                
                u32 count = 0;
                for (const auto& stat : in_statsMap)
                {
                    GUIStat guiStat;
                    
                    guiStat.m_nameLabel = GUI::LabelSPtr(new GUI::Label());
                    guiStat.m_nameLabel->SetFont(font);
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
                    guiStat.m_nameLabel->SetColour(Core::Colour::k_white);
                    guiStat.m_nameLabel->EnableInheritedColour(false);
                    guiStat.m_nameLabel->EnableInheritedOpacity(false);
                    AddSubview(guiStat.m_nameLabel);
                    
                    guiStat.m_valueLabel = GUI::LabelSPtr(new GUI::Label());
                    guiStat.m_valueLabel->SetFont(font);
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
                    guiStat.m_valueLabel->SetColour(Core::Colour::k_white);
                    guiStat.m_valueLabel->EnableInheritedColour(false);
                    guiStat.m_valueLabel->EnableInheritedOpacity(false);
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