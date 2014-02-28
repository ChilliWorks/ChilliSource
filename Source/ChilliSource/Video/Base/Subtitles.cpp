/*
 *  Subtitles.cpp
 *  moFlow
 *
 *  Created by Ian Copland 21/02/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Video/Base/Subtitles.h>

namespace ChilliSource
{
	namespace Video
	{
		DEFINE_NAMED_INTERFACE(Subtitles);
		//------------------------------------------------
		/// Constructor
		//------------------------------------------------
		Subtitles::Subtitles()
		{
		}
        //----------------------------------------------------------
        /// Is A
        //----------------------------------------------------------
        bool Subtitles::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (inInterfaceID == Subtitles::InterfaceID);
        }
        //----------------------------------------------------------
        /// Add Style
        //----------------------------------------------------------
        void Subtitles::AddStyle(const StylePtr& inpStyle)
        {
            mStyleMap.insert(std::pair<std::string, StylePtr>(inpStyle->strName, inpStyle));
        }
        //----------------------------------------------------------
        /// Add Subtitle
        //----------------------------------------------------------
        void Subtitles::AddSubtitle(const SubtitlePtr& inpSubtitle)
        {
            mSubtitles.push_back(inpSubtitle);
        }
        //----------------------------------------------------------
        /// Get Subtitles At Time
        //----------------------------------------------------------
        std::vector<Subtitles::SubtitlePtr> Subtitles::GetSubtitlesAtTime(TimeIntervalMs inTimeMS) const
        {
            std::vector<SubtitlePtr> subtitles;
            
            for (std::vector<SubtitlePtr>::const_iterator it = mSubtitles.begin(); it != mSubtitles.end(); ++it)
            {
                if (inTimeMS >= (*it)->StartTimeMS && inTimeMS <= (*it)->EndTimeMS)
                {
                    subtitles.push_back((*it));
                }
            }
            
            return subtitles;
        }
        //----------------------------------------------------------
        /// Get Style With Name
        //----------------------------------------------------------
        Subtitles::StylePtr Subtitles::GetStyleWithName(const std::string& instrName) const
        {
            std::unordered_map<std::string, StylePtr>::const_iterator found = mStyleMap.find(instrName);
            if (found != mStyleMap.end())
            {
                return found->second;
            }
            
            CS_LOG_WARNING("Could not find style '" + instrName + "' in Subtitles '" + GetName() + "'");
            return StylePtr();
        }
	}
}

