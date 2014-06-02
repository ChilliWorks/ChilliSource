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

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            const std::string k_defaultDisplayableName = "Chilli Source App";
            const u32 k_defaultPreferredFPS = 30;
            const std::string k_defaultLocation = "Package";
            
            const std::string k_configFilePath = "App.config";
            const std::string k_displayableName = "DisplayableName";
            const std::string k_preferredFPS = "PreferredFPS";
            const std::string k_defaultLocalisedTextTag = "DefaultLocalisedText";
            const std::string k_defaultFontTag = "DefaultFont";
            const std::string k_defaultMaterialTag = "DefaultMaterial";
            const std::string k_defaultMeshTag = "DefaultMesh";
            const std::string k_locationTag = "Location";
            const std::string k_pathTag = "Path";
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
        : m_preferredFPS(k_defaultPreferredFPS), m_displayableName(k_defaultDisplayableName), m_defaultLocalisedTextStorageLocation(StorageLocation::k_package), m_defaultFontStorageLocation(StorageLocation::k_package),
            m_defaultMaterialStorageLocation(StorageLocation::k_package), m_defaultMeshStorageLocation(StorageLocation::k_package)
        {
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        bool AppConfig::IsA(InterfaceIDType in_interfaceId) const
        {
            return (AppConfig::InterfaceID == in_interfaceId);
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const std::string& AppConfig::GetDisplayableName() const
        {
            return m_displayableName;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        u32 AppConfig::GetPreferredFPS() const
        {
            return m_preferredFPS;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const std::string& AppConfig::GetDefaultLocalisedTextFilePath() const
        {
            return m_defaultLocalisedTextFilePath;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        StorageLocation AppConfig::GetDefaultLocalisedTextStorageLocation() const
        {
            return m_defaultLocalisedTextStorageLocation;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const std::string& AppConfig::GetDefaultFontFilePath() const
        {
            return m_defaultFontFilePath;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        StorageLocation AppConfig::GetDefaultFontStorageLocation() const
        {
            return m_defaultFontStorageLocation;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const std::string& AppConfig::GetDefaultMaterialFilePath() const
        {
            return m_defaultMaterialFilePath;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        StorageLocation AppConfig::GetDefaultMaterialStorageLocation() const
        {
            return m_defaultMaterialStorageLocation;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const std::string& AppConfig::GetDefaultMeshFilePath() const
        {
            return m_defaultMeshFilePath;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        StorageLocation AppConfig::GetDefaultMeshStorageLocation() const
        {
            return m_defaultMeshStorageLocation;
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void AppConfig::Load()
        {
            Json::Value root;
            if(Utils::ReadJson(StorageLocation::k_package, k_configFilePath, &root) == true)
            {
                if(root.isMember(k_displayableName))
                {
                    m_displayableName = root[k_displayableName].asString();
                }
                
                if(root.isMember(k_preferredFPS))
                {
                    m_preferredFPS = root[k_preferredFPS].asUInt();
                }
                
                if(root.isMember(k_defaultLocalisedTextTag))
                {
                    m_defaultLocalisedTextStorageLocation = ParseStorageLocation(root[k_defaultLocalisedTextTag].get(k_locationTag, k_defaultLocation).asString());
                    m_defaultLocalisedTextFilePath = root[k_defaultLocalisedTextTag].get(k_pathTag, "").asString();
                }
                
                if(root.isMember(k_defaultFontTag))
                {
                    m_defaultFontStorageLocation = ParseStorageLocation(root[k_defaultFontTag].get(k_locationTag, k_defaultLocation).asString());
                    m_defaultFontFilePath = root[k_defaultFontTag].get(k_pathTag, "").asString();
                }
                
                if(root.isMember(k_defaultMaterialTag))
                {
                    m_defaultMaterialStorageLocation = ParseStorageLocation(root[k_defaultMaterialTag].get(k_locationTag, k_defaultLocation).asString());
                    m_defaultMaterialFilePath = root[k_defaultMaterialTag].get(k_pathTag, "").asString();
                }
                
                if(root.isMember(k_defaultMeshTag))
                {
                    m_defaultMeshStorageLocation = ParseStorageLocation(root[k_defaultMeshTag].get(k_locationTag, k_defaultLocation).asString());
                    m_defaultMeshFilePath = root[k_defaultMeshTag].get(k_pathTag, "").asString();
                }
            }
        }
    }
}
