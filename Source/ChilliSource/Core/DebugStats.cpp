/*
 *  DebugStats.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 04/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/DebugStats.h>
#include <ChilliSource/GUI/Defaults/DebugStatsView.h>

#ifdef DEBUG_STATS

namespace moFlo
{
    CDebugStats::MapStringToString CDebugStats::mmapEvents;
    bool CDebugStats::mbEnabled = true;
    
    //----------------------------------------------------------------------------------
    /// Get Value For Event
    ///
    /// Get the current value for the given event type
    ///
    /// @param Type ID
    /// @return Value as string
    //----------------------------------------------------------------------------------
    const std::string& CDebugStats::GetValueForEvent(const std::string& instrType)
    {
        return mmapEvents[instrType];
    }
    //----------------------------------------------------------------------------------
    /// Add To Event
    ///
    /// Add the given value to the value already stored for the given event
    ///
    /// @param Type ID
    /// @param Value to add
    //----------------------------------------------------------------------------------
    void CDebugStats::AddToEvent(const std::string& instrType, u32 inudwValue)
    {
        if(!mbEnabled) return;
        
        MapStringToString::iterator it = mmapEvents.find(instrType);
        
        u32 udwNewValue = inudwValue;
        
        if(it != mmapEvents.end())
        {
            udwNewValue = Core::CStringConverter::ParseUnsignedInt(mmapEvents[instrType]) + inudwValue;
            it->second = STRING_CAST(udwNewValue);
        }
        else
        {
            mmapEvents[instrType] = STRING_CAST(udwNewValue);
        }
    }
    //----------------------------------------------------------------------------------
    /// Add To Event
    ///
    /// Add the given value to the value already stored for the given event
    ///
    /// @param Type ID
    /// @param Value to add
    //----------------------------------------------------------------------------------
    void CDebugStats::AddToEvent(const std::string& instrType, s32 indwValue)
    {
        if(!mbEnabled) return;
        
        MapStringToString::iterator it = mmapEvents.find(instrType);
        
        s32 dwNewValue = indwValue;
        
        if(it != mmapEvents.end())
        {
            dwNewValue = Core::CStringConverter::ParseInt(mmapEvents[instrType]) + indwValue;
            it->second = STRING_CAST(dwNewValue);
        }
        else
        {
            mmapEvents[instrType] = STRING_CAST(dwNewValue);
        }
    }
    //----------------------------------------------------------------------------------
    /// Add To Event
    ///
    /// Add the given value to the value already stored for the given event
    ///
    /// @param Type ID
    /// @param Value to add
    //----------------------------------------------------------------------------------
    void CDebugStats::AddToEvent(const std::string& instrType, f32 infValue)
    {
        if(!mbEnabled) return;
        
        MapStringToString::iterator it = mmapEvents.find(instrType);
        
        f32 fNewValue = infValue;
        
        if(it != mmapEvents.end())
        {
            fNewValue = Core::CStringConverter::ParseFloat(mmapEvents[instrType]) + infValue;
            it->second = STRING_CAST(fNewValue);
        }
        else
        {
            mmapEvents[instrType] = STRING_CAST(fNewValue);
        }
    }
    //----------------------------------------------------------------------------------
    /// Set Enabled
    ///
    /// @param Whether tracking is enabled
    //----------------------------------------------------------------------------------
    void CDebugStats::SetEnabled(bool inbEnabled)
    {
        mbEnabled = inbEnabled;
    }
    //----------------------------------------------------------------------------------
    /// Is Enabled
    ///
    /// @return Whether tracking is enabled
    //----------------------------------------------------------------------------------
    bool CDebugStats::IsEnabled()
    {
        return mbEnabled;
    }
    //----------------------------------------------------------------------------------
    /// Clear
    ///
    /// Clear all the previously recorded stats
    //----------------------------------------------------------------------------------
    void CDebugStats::Clear()
    {
        mmapEvents.clear();
    }
}

#endif
