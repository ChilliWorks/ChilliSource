//
//  AppConfig.cpp
//  Chilli Source
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
    namespace Core
    {
        namespace
        {
            const std::string k_configFilePath = "App.config";
            const std::string k_defaultDisplayableName = "Chilli Source App";
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
                
                const Json::Value& fileTags = root["FileTags"];
                
                if(fileTags.isNull() == false)
                {
                    Application::Get()->GetTaggedFilePathResolver()->SetFromJson(fileTags);
                }
            }
        }
    }
}
