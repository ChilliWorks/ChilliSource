//
//  DebugStats.h
//  Chilli Source
//  Created by Scott Downie on 04/08/2011.
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

#ifndef _CHILLISOURCE_DEBUGGING_BASE_DEBUGSTATS_H_
#define _CHILLISOURCE_DEBUGGING_BASE_DEBUGSTATS_H_

#ifdef CS_ENABLE_DEBUGSTATS

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <unordered_map>

namespace ChilliSource
{
    namespace Debugging
    {
        //---------------------------------------------------------------------
        /// An application system used for displaying debug statistics in a
        /// pane over the game as it is running.
        ///
        /// @author S Downie
        //---------------------------------------------------------------------
        class DebugStats final : public Core::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(DebugStats);
            
            //------------------------------------------------------------
            /// Allows querying of whether or not this system implements
            /// a given interface.
            ///
			/// @author I Copland
			///
			/// @param Interface Id
			///
			/// @return Whether this object is of the given type.
			//------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Whether tracking is enabled
            //------------------------------------------------------------
            bool IsEnabled() const;
            //------------------------------------------------------------
            ///  @author S Downie
            ///
            /// @param Whether tracking is enabled
            //------------------------------------------------------------
            void SetEnabled(bool in_enabled);
            //------------------------------------------------------------
            /// Record a debug stat event with a name and value. This will
            /// overwrite any previous values for the same type.
            ///
            /// @author S Downie
            ///
            /// @param The name of the event.
            /// @param The statistic value.
            //------------------------------------------------------------
            template<typename TType> void RecordEvent(const std::string& in_eventName, const TType& in_value);
            //------------------------------------------------------------
            /// Add the given value to the value already stored for the
            /// given event name. This total will be cleared at the end
            /// of the frame.
            ///
            /// @author S Downie
            ///
            /// @param The name of the debug statistic.
            /// @param The value to add.
            //------------------------------------------------------------
            void AddToEvent(const std::string& in_eventName, u32 in_value);
            //------------------------------------------------------------
            /// Add the given value to the value already stored for the
            /// given event name. This total will be cleared at the end
            /// of the frame.
            ///
            /// @author S Downie
            ///
            /// @param The name of the debug statistic.
            /// @param The value to add.
            //------------------------------------------------------------
            void AddToEvent(const std::string& in_eventName, s32 in_value);
            //------------------------------------------------------------
            /// Add the given value to the value already stored for the
            /// given event name. This total will be cleared at the end
            /// of the frame.
            ///
            /// @author S Downie
            ///
            /// @param The name of the debug statistic.
            /// @param The value to add.
            //------------------------------------------------------------
            void AddToEvent(const std::string& in_eventName, f32 in_value);
            //------------------------------------------------------------
            /// Draws the current stored stats to the screen.
            ///
            /// @author I Copland
            ///
            /// @param The canvas renderer.
            /// @param The window.
            //------------------------------------------------------------
            void DrawStats(Rendering::CanvasRenderer* in_canvas, GUI::Window* in_window);
            //------------------------------------------------------------
            /// Clear all the previously recorded stats.
            ///
            /// @author S Downie
            //------------------------------------------------------------
            void Clear();
        private:
            friend class Core::Application;
			//------------------------------------------------------------
			/// Factory create method called by application to create a new
            /// instance of the system.
            ///
            /// @author I Copland
			///
			/// @return Creates a new instance of the system.
			//------------------------------------------------------------
			static DebugStatsUPtr Create();
			//------------------------------------------------------------
			/// Private constructor to enforce use of the factory method.
			///
			/// @author I Copland
			//------------------------------------------------------------
			DebugStats();
            //------------------------------------------------------------
            /// Initialisation method called at a time when all App Systems
            /// have been created. System initialisation occurs in the
            /// order they were created.
            ///
            /// @author I Copland
            //------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------
            /// Called when the application is being destroyed. This should
            /// be used to cleanup memory and references to other systems.
            /// System destruction occurs in the reverse order to which
            /// they were created.
            ///
            /// @author I Copland
            //------------------------------------------------------------
            void OnDestroy() override;

            std::unordered_map<std::string, std::string> m_events;
            bool m_enabled;
            DebugStatsViewSPtr m_view;
            TimeIntervalMs m_lastRefresh;
        };
        //------------------------------------------------------------
        //------------------------------------------------------------
        template<typename TType> void DebugStats::RecordEvent(const std::string& in_eventName, const TType& in_value)
        {
            if(m_enabled == true)
            {
                m_events[in_eventName] = Core::ToString(in_value);
            }
        }
    }
}

#endif

#endif