/*
 *  Subtitles.h
 *  moFlow
 *
 *  Created by Ian Copland 21/02/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_VIDEO_SUBTITLES_H_
#define _MOFLOW_VIDEO_SUBTITLES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>

namespace ChilliSource
{
	namespace Video
	{
        //=======================================================
		/// Subtitles
		///
		/// A subtitles resource. This is used for displaying
        /// subtitles over videos played using the video player
        /// activity.
		//=======================================================
		class Subtitles : public Core::IResource
		{
		public:
			DECLARE_NAMED_INTERFACE(Subtitles);
            //----------------------------------------------------------
			/// Style
			//----------------------------------------------------------
            struct Style
            {
                std::string strName;
                std::string strFontName;
                u32 udwFontSize;
                Core::CColour Colour;
                Core::Rectangle Bounds;
                Rendering::AlignmentAnchor eAlignment;
                TimeIntervalMs FadeTimeMS;
            };
            typedef std::shared_ptr<Style> StylePtr;
            //----------------------------------------------------------
			/// Subtitle
			//----------------------------------------------------------
            struct Subtitle
            {
                std::string strStyleName;
                TimeIntervalMs StartTimeMS;
                TimeIntervalMs EndTimeMS;
                std::string strTextID;
            };
            typedef std::shared_ptr<Subtitle> SubtitlePtr;
            //----------------------------------------------------------
			/// Constructor
			//----------------------------------------------------------
            Subtitles();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //----------------------------------------------------------
			/// Add Style
            ///
			/// @param the Style to add.
			//----------------------------------------------------------
            void AddStyle(const StylePtr& inpStyle);
            //----------------------------------------------------------
			/// Add Subtitle
            ///
			/// @param The subtitle to add.
			//----------------------------------------------------------
            void AddSubtitle(const SubtitlePtr& inpSubtitle);
            //----------------------------------------------------------
			/// Get Subtitles At Time
			///
			/// returns all the active subtitles at the given time.
            ///
			/// @param the time.
			/// @return a vector of subtitles.
			//----------------------------------------------------------
            std::vector<SubtitlePtr> GetSubtitlesAtTime(TimeIntervalMs inTimeMS) const;
            //----------------------------------------------------------
			/// Get Style With Name
			///
			/// @param the style name
			/// @return The style with the given name.
			//----------------------------------------------------------
            StylePtr GetStyleWithName(const std::string& instrName) const;
        private:
            std::unordered_map<std::string, StylePtr> mStyleMap;
            std::vector<SubtitlePtr> mSubtitles;
		};
	}
}

#endif
