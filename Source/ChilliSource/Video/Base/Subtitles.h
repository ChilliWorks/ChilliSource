//
//  Subtitles.h
//  Chilli Source
//
//  Created by Ian Copland 21/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_VIDEO_BASE_SUBTITLES_H_
#define _CHILLISOURCE_VIDEO_BASE_SUBTITLES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>

namespace ChilliSource
{
	namespace Video
	{
        //----------------------------------------------------------
		/// A subtitles resource. This is used for displaying
        /// subtitles over videos played using the video player
        /// activity.
        ///
        /// @author I Copland
		//----------------------------------------------------------
		class Subtitles final : public Core::Resource
		{
		public:
			CS_DECLARE_NAMEDTYPE(Subtitles);
            //----------------------------------------------------------
			/// Holds the style of the subtitles such as colour,
            /// font, etc.
            ///
            /// @author I Copland
			//----------------------------------------------------------
            struct Style
            {
                std::string m_name;
                std::string m_fontName;
                u32 m_fontSize;
                Core::Colour m_colour;
                Core::Rectangle m_bounds;
                Rendering::AlignmentAnchor m_alignment;
                TimeIntervalMs m_fadeTimeMS;
            };
            typedef std::unique_ptr<const Style> StyleCUPtr;
            typedef std::unique_ptr<Style> StyleUPtr;
            //----------------------------------------------------------
			/// Holds the information about a single subtitle instance
            ///
            /// @author I Copland
			//----------------------------------------------------------
            struct Subtitle
            {
                std::string m_styleName;
                TimeIntervalMs m_startTimeMS;
                TimeIntervalMs m_endTimeMS;
                std::string m_textId;
            };
            typedef std::unique_ptr<const Subtitle> SubtitleCUPtr;
            typedef std::unique_ptr<Subtitle> SubtitleUPtr;
			//----------------------------------------------------------
			/// @author I Copland
			///
			/// @param Comparison Type
            ///
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------------
            /// @author I Copland
            ///
			/// @param The Style to add to the available styles
			//----------------------------------------------------------
            void AddStyle(StyleCUPtr in_style);
            //----------------------------------------------------------
			/// @author I Copland
            ///
			/// @param The subtitle to add to the current list of subtitles
			//----------------------------------------------------------
            void AddSubtitle(SubtitleCUPtr in_subtitle);
            //----------------------------------------------------------
			/// Returns all the active subtitles at the given time.
            ///
            /// @author I Copland
            ///
			/// @param Time in MS from the start if the subtitle sequence.
            ///
			/// @return List subtitles.
			//----------------------------------------------------------
            std::vector<const Subtitles::Subtitle*> GetSubtitlesAtTime(TimeIntervalMs in_timeMS) const;
            //----------------------------------------------------------
            /// @author I Copland
            ///
			/// @param Style name
            ///
			/// @return The style with the given name or null
			//----------------------------------------------------------
            const Style* GetStyleWithName(const std::string& in_name) const;
            
        private:
            
            friend class Core::ResourcePool;
            //----------------------------------------------------------
            /// Factory create method that creates a new empty subtitle
            /// resource. Only accessed by the resource pool
            ///
            /// @author S Downie
            //----------------------------------------------------------
            static SubtitlesCUPtr Create();
            //----------------------------------------------------------
			/// Private constructor to force the use of the factory
            /// create method
            ///
            /// @author S Downie
			//----------------------------------------------------------
            Subtitles() = default;
            
        private:
            
            std::unordered_map<std::string, StyleCUPtr> m_styles;
            std::vector<SubtitleCUPtr> m_subtitles;
		};
	}
}

#endif
