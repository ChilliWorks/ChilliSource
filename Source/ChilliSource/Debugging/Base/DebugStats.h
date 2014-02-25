/*
 *  DebugStats.h
 *  iOSTemplate
 *
 *  Created by Scott Downie on 04/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_DEBUG_STATS_H_
#define _MO_FLO_CORE_DEBUG_STATS_H_

#ifdef DEBUG_STATS

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Debugging
    {
        class CDebugStats
        {
        public:
            
            //----------------------------------------------------------------------------------
            /// Record Event
            ///
            /// Record a debug stat event with a type and value. This will overwrite
            /// any previous values for the same type
            ///
            /// @param Type ID
            /// @param Value as T
            //----------------------------------------------------------------------------------
            template<typename T> static void RecordEvent(const std::string& instrType, const T& inValue)
            {
                if(!mbEnabled) return;
                
                mmapEvents[instrType] = ToString(inValue);
            }
            //----------------------------------------------------------------------------------
            /// Record Event (String Value)
            ///
            /// Record a debug stat event with a type and value. This will overwrite
            /// any previous values for the same type
            ///
            /// @param Type ID
            /// @param Value as string
            //----------------------------------------------------------------------------------
            static void RecordEvent(const std::string& instrType, const std::string& instrValue) 
            {
                if(!mbEnabled) return;
                
                mmapEvents[instrType] = instrValue;
            }
            //----------------------------------------------------------------------------------
            /// Add To Event
            ///
            /// Add the given value to the value already stored for the given event
            ///
            /// @param Type ID
            /// @param Value to add
            //----------------------------------------------------------------------------------
            static void AddToEvent(const std::string& instrType, u32 inudwValue);
            //----------------------------------------------------------------------------------
            /// Add To Event
            ///
            /// Add the given value to the value already stored for the given event
            ///
            /// @param Type ID
            /// @param Value to add
            //----------------------------------------------------------------------------------
            static void AddToEvent(const std::string& instrType, s32 indwValue);
            //----------------------------------------------------------------------------------
            /// Add To Event
            ///
            /// Add the given value to the value already stored for the given event
            ///
            /// @param Type ID
            /// @param Value to add
            //----------------------------------------------------------------------------------
            static void AddToEvent(const std::string& instrType, f32 infValue);
            //----------------------------------------------------------------------------------
            /// Get Value For Event
            ///
            /// Get the current value for the given event type
            ///
            /// @param Type ID
            /// @return Value as string
            //----------------------------------------------------------------------------------
            static const std::string& GetValueForEvent(const std::string& instrType);
            //----------------------------------------------------------------------------------
            /// Clear
            ///
            /// Clear all the previously recorded stats
            //----------------------------------------------------------------------------------
            static void Clear();
            //----------------------------------------------------------------------------------
            /// Set Enabled
            ///
            /// @param Whether tracking is enabled
            //----------------------------------------------------------------------------------
            static void SetEnabled(bool inbEnabled);
            //----------------------------------------------------------------------------------
            /// Is Enabled
            ///
            /// @return Whether tracking is enabled
            //----------------------------------------------------------------------------------
            static bool IsEnabled();
            
        private:
            
            typedef HASH_MAP<std::string, std::string> MapStringToString;
            static MapStringToString mmapEvents;
            
            static bool mbEnabled;
        };
    }
}

#endif

#endif