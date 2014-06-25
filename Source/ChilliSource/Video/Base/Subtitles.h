//
//  Subtitles.h
//  Chilli Source
//  Created by Ian Copland 21/02/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#ifndef _CHILLISOURCE_VIDEO_BASE_SUBTITLES_H_
#define _CHILLISOURCE_VIDEO_BASE_SUBTITLES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Video
	{
        //----------------------------------------------------------
		/// A subtitles resource. This is used for displaying
        /// subtitles over videos played using the video player
        /// activity.
        ///
        /// @author Ian Copland
		//----------------------------------------------------------
		class Subtitles final : public Core::Resource
		{
		public:
			CS_DECLARE_NAMEDTYPE(Subtitles);
            //----------------------------------------------------------
			/// Holds the style of the subtitles such as colour,
            /// font, etc.
            ///
            /// @author Ian Copland
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
            /// @author Ian Copland
			//----------------------------------------------------------
            struct Subtitle
            {
                std::string m_styleName;
                TimeIntervalMs m_startTimeMS;
                TimeIntervalMs m_endTimeMS;
                std::string m_localisedTextId;
            };
            typedef std::unique_ptr<const Subtitle> SubtitleCUPtr;
            typedef std::unique_ptr<Subtitle> SubtitleUPtr;
			//----------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Comparison Type
            ///
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------------
            /// @author Ian Copland
            ///
			/// @param The Style to add to the available styles
			//----------------------------------------------------------
            void AddStyle(StyleCUPtr in_style);
            //----------------------------------------------------------
			/// @author Ian Copland
            ///
			/// @param The subtitle to add to the current list of subtitles
			//----------------------------------------------------------
            void AddSubtitle(SubtitleCUPtr in_subtitle);
            //----------------------------------------------------------
			/// Returns all the active subtitles at the given time.
            ///
            /// @author Ian Copland
            ///
			/// @param Time in MS from the start if the subtitle sequence.
            ///
			/// @return List subtitles.
			//----------------------------------------------------------
            std::vector<const Subtitles::Subtitle*> GetSubtitlesAtTime(TimeIntervalMs in_timeMS) const;
            //----------------------------------------------------------
            /// @author Ian Copland
            ///
			/// @param Style name
            ///
			/// @return The style with the given name or null
			//----------------------------------------------------------
            const Style* GetStyleWithName(const std::string& in_name) const;
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Localised text resource used by the internal
            /// subtitles
            //----------------------------------------------------------
            void SetLocalisedText(const Core::LocalisedTextCSPtr& in_text);
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Localised text resource used by the internal
            /// subtitles
            //----------------------------------------------------------
            const Core::LocalisedTextCSPtr& GetLocalisedText() const;
            
        private:
            
            friend class Core::ResourcePool;
            //----------------------------------------------------------
            /// Factory create method that creates a new empty subtitle
            /// resource. Only accessed by the resource pool
            ///
            /// @author S Downie
            //----------------------------------------------------------
            static SubtitlesUPtr Create();
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
            
            Core::LocalisedTextCSPtr m_localisedText;
		};
	}
}

#endif
