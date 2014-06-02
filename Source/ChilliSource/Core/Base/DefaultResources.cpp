//
//  DefaultResources.cpp
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

#include <ChilliSource/Core/Base/DefaultResources.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/AppConfig.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/Mesh.h>


namespace ChilliSource
{
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(DefaultResources);
        //---------------------------------------------------------
        //---------------------------------------------------------
        DefaultResourcesUPtr DefaultResources::Create()
        {
            return DefaultResourcesUPtr(new DefaultResources());
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        bool DefaultResources::IsA(InterfaceIDType in_interfaceId) const
        {
            return (DefaultResources::InterfaceID == in_interfaceId);
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const LocalisedTextCSPtr& DefaultResources::GetLocalisedText() const
        {
            return m_localisedText;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const Rendering::FontCSPtr& DefaultResources::GetFont() const
        {
            return m_font;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const Rendering::MaterialCSPtr& DefaultResources::GetMaterial() const
        {
            return m_material;
        }
        //---------------------------------------------------------
        //--------------------------------------------------------
        const Rendering::MeshCSPtr& DefaultResources::GetMesh() const
        {
            return m_mesh;
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        void DefaultResources::Load()
        {
            AppConfig* appConfig = Application::Get()->GetAppConfig();
            ResourcePool* resourcePool = Application::Get()->GetResourcePool();
            CS_ASSERT(appConfig != nullptr, "DefaultResources is missing required system: AppConfig");
            CS_ASSERT(resourcePool != nullptr, "DefaultResources is missing required system: ResourcePool");
            
            if (appConfig->GetDefaultLocalisedTextFilePath().empty() != true)
            {
                m_localisedText = resourcePool->LoadResource<LocalisedText>(appConfig->GetDefaultLocalisedTextStorageLocation(), appConfig->GetDefaultLocalisedTextFilePath());
            }
            
            if (appConfig->GetDefaultFontFilePath().empty() != true)
            {
                m_font = resourcePool->LoadResource<Rendering::Font>(appConfig->GetDefaultFontStorageLocation(), appConfig->GetDefaultFontFilePath());
            }
            
            if (appConfig->GetDefaultMaterialFilePath().empty() != true)
            {
                m_material = resourcePool->LoadResource<Rendering::Material>(appConfig->GetDefaultMaterialStorageLocation(), appConfig->GetDefaultMaterialFilePath());
            }
            
            if (appConfig->GetDefaultMeshFilePath().empty() != true)
            {
                m_mesh = resourcePool->LoadResource<Rendering::Mesh>(appConfig->GetDefaultMeshStorageLocation(), appConfig->GetDefaultMeshFilePath());
            }
        }
    }
}
