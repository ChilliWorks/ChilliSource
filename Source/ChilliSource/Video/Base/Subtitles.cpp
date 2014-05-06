//
//  Subtitles.cpp
//  Chilli Source
//
//  Created by Ian Copland 21/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Video/Base/Subtitles.h>

namespace ChilliSource
{
	namespace Video
	{
		CS_DEFINE_NAMEDTYPE(Subtitles);
        
        //----------------------------------------------------------
        //----------------------------------------------------------
        SubtitlesUPtr Subtitles::Create()
        {
            return SubtitlesUPtr(new Subtitles());
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        bool Subtitles::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == Subtitles::InterfaceID;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Subtitles::AddStyle(StyleCUPtr in_style)
        {
            CS_ASSERT(in_style != nullptr, "Cannot add null style to subtitles");
            m_styles.insert(std::make_pair(in_style->m_name, std::move(in_style)));
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Subtitles::AddSubtitle(SubtitleCUPtr in_subtitle)
        {
            CS_ASSERT(in_subtitle != nullptr, "Cannot add null subtitle to subtitles");
            m_subtitles.push_back(std::move(in_subtitle));
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        void Subtitles::SetLocalisedText(const Core::LocalisedTextCSPtr& in_text)
        {
            m_localisedText = in_text;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        const Core::LocalisedTextCSPtr& Subtitles::GetLocalisedText() const
        {
            return m_localisedText;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        std::vector<const Subtitles::Subtitle*> Subtitles::GetSubtitlesAtTime(TimeIntervalMs in_timeMS) const
        {
            std::vector<const Subtitles::Subtitle*> subtitles;
            
            for (auto it = m_subtitles.begin(); it != m_subtitles.end(); ++it)
            {
                if (in_timeMS >= (*it)->m_startTimeMS && in_timeMS <= (*it)->m_endTimeMS)
                {
                    subtitles.push_back(it->get());
                }
            }
            
            return subtitles;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        const Subtitles::Style* Subtitles::GetStyleWithName(const std::string& in_name) const
        {
            auto found = m_styles.find(in_name);
            if (found != m_styles.end())
            {
                return found->second.get();
            }
            
            CS_LOG_WARNING("Could not find style '" + in_name + "' in Subtitles '" + GetFilePath() + "'");
            return nullptr;
        }
	}
}

