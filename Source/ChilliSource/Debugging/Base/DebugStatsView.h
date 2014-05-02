//
//  DebugStatsView.h
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

#ifndef _CHILLISOURCE_DEBUGGING_BASE_DEBUGSTATSVIEW_H_
#define _CHILLISOURCE_DEBUGGING_BASE_DEBUGSTATSVIEW_H_

#ifdef CS_ENABLE_DEBUGSTATS

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Image/ImageView.h>
#include <ChilliSource/GUI/Label/Label.h>

namespace ChilliSource
{
	namespace Debugging
	{
        //----------------------------------------------------------
        /// A GUI view for displaying the contents of the Debug
        /// Stats system.
        ///
        /// @author S Downie
        //----------------------------------------------------------
		class DebugStatsView final : public GUI::ImageView
		{
		public:
            //--------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //--------------------------------------------------
            DebugStatsView();
            //--------------------------------------------------
            /// Refreshes the debug stats from the debug stats
            /// system. If the view heirachy has changed then it
            /// will be rebuilt.
            ///
            /// @author I Copland
            ///
            /// @param The map of stats.
            //--------------------------------------------------
            void Refresh(const std::unordered_map<std::string, std::string>& in_statsMap);
        private:
            //--------------------------------------------------
            /// A container for the group of UI and information
            /// needed for displaying a single stat.
            ///
            /// @author I Copland
            //--------------------------------------------------
            struct GUIStat
            {
                GUI::LabelSPtr m_nameLabel;
                GUI::LabelSPtr m_valueLabel;
            };
            //--------------------------------------------------
            /// Tries to built the GUI heirarchy if the
            /// given stats map contains different stats.
            ///
            /// @author I Copland
            ///
            /// @param The stat map.
            //--------------------------------------------------
            void TryBuild(const std::unordered_map<std::string, std::string>& in_statsMap);
            //--------------------------------------------------
            /// Checked whether or not the list of GUI statistics
            /// and the map of statistics are refering to the
            /// same set of stats.
            ///
            /// @author I Copland
            ///
            /// @param The stat map.
            ///
            /// @return Whether or not the stats are the same.
            //--------------------------------------------------
            bool ContainsSameStats(const std::unordered_map<std::string, std::string>& in_statsMap) const;
            //--------------------------------------------------
            /// Removes and destroys all GUI Stats.
            ///
            /// @author I Copland
            //--------------------------------------------------
            void CleanupGUIStats();
            
            std::unordered_map<std::string, GUIStat> m_guiStats;
		};
	}
}

#endif

#endif