//
//  AppConfig.cpp
//  ChilliSource
//  Created by Ian Copland on 02/06/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Core/Base/AppConfig.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/File/TaggedFilePathResolver.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Rendering/Base/SurfaceFormat.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace
    {
        const std::string k_configFilePath = "App.config";
        const std::string k_defaultDisplayableName = "ChilliSource App";
        const u32 k_defaultPreferredFPS = 30;
    }
    
    CS_DEFINE_NAMEDTYPE(AppConfig);

    //---------------------------------------------------------
    //---------------------------------------------------------
    AppConfigUPtr AppConfig::Create()
    {
        return AppConfigUPtr(new AppConfig());
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    AppConfig::AppConfig()
    : m_preferredFPS(k_defaultPreferredFPS), m_displayableName(k_defaultDisplayableName)
    {
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    bool AppConfig::IsA(InterfaceIDType in_interfaceId) const
    {
        return (AppConfig::InterfaceID == in_interfaceId);
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    const std::string& AppConfig::GetDisplayableName() const
    {
        return m_displayableName;
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    u32 AppConfig::GetPreferredFPS() const
    {
        return m_preferredFPS;
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    bool AppConfig::IsVSyncEnabled() const
    {
        return m_isVSyncEnabled;
    }
    //---------------------------------------------------------
    //---------------------------------------------------------
    void AppConfig::Load()
    {
        Json::Value root;
        if(JsonUtils::ReadJson(StorageLocation::k_package, k_configFilePath, root) == true)
        {
            m_displayableName = root.get("DisplayableName", k_defaultDisplayableName).asString();
            m_preferredFPS = root.get("PreferredFPS", k_defaultPreferredFPS).asUInt();
            m_isVSyncEnabled = root.get("VSync", false).asBool();
            std::string cursorType = root.get("CursorType", "System").asString();
            m_cursorType = ParseCursorType(cursorType);
            m_defaultCursorUIPath = root.get("DefaultCursorPath", "Widgets/DefaultCursor.csui").asString();
            std::string cursorLocation = root.get("DefaultCursorLocation", "ChilliSource").asString();
            m_defaultCursorUILocation = ParseStorageLocation(cursorLocation);
            
#if defined CS_TARGETPLATFORM_IOS
            std::string platform = "iOS";
#elif defined CS_TARGETPLATFORM_ANDROID
            std::string platform = "Android";
#elif defined CS_TARGETPLATFORM_WINDOWS
            std::string platform = "Windows";
#elif defined CS_TARGETPLATFORM_RPI
            std::string platform = "RPi";
#endif
            
            //Configurations can be overloaded per platform
            const Json::Value& platformRoot = root[platform];
            if(platformRoot.isNull() == false)
            {
                m_cursorType = ParseCursorType(platformRoot.get("CursorType", cursorType).asString());
                m_isVSyncEnabled = platformRoot.get("VSync", m_isVSyncEnabled).asBool();
                m_preferredFPS = platformRoot.get("PreferredFPS", m_preferredFPS).asUInt();
                m_defaultCursorUIPath = platformRoot.get("DefaultCursorPath", m_defaultCursorUIPath).asString();
                m_defaultCursorUILocation = ParseStorageLocation(platformRoot.get("DefaultCursorLocation", cursorLocation.c_str()).asString());
            }
            
            const Json::Value& fileTags = root["FileTags"];
            if(fileTags.isNull() == false)
            {
                Application::Get()->GetTaggedFilePathResolver()->SetFromJson(fileTags);
            }
        }
    }
}
