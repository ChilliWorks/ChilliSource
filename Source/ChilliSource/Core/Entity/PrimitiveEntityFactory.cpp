//
//  PrimitiveEntityFactory.cpp
//  ChilliSource
//  Created by Ian Copland on 04/08/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <ChilliSource/Core/Entity/PrimitiveEntityFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Model/Model.h>
#include <ChilliSource/Rendering/Model/StaticModelComponent.h>
#include <ChilliSource/Rendering/Model/PrimitiveModelFactory.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(PrimitiveEntityFactory);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    PrimitiveEntityFactoryUPtr PrimitiveEntityFactory::Create()
    {
        return PrimitiveEntityFactoryUPtr(new PrimitiveEntityFactory());
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool PrimitiveEntityFactory::IsA(InterfaceIDType in_interfaceId) const
    {
        return (PrimitiveEntityFactory::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    EntityUPtr PrimitiveEntityFactory::CreatePlane(const Colour& colour, const Vector2& size)
    {
        CS_RELEASE_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
        
        MaterialCSPtr material = CreateStaticBlinnColourMaterial(colour);
        return CreatePlane(CreateStaticBlinnColourMaterial(colour), size);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    EntityUPtr PrimitiveEntityFactory::CreatePlane(const MaterialCSPtr& material, const Vector2& size)
    {
        CS_RELEASE_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
        
        ModelCSPtr mesh = m_primitiveModelFactory->CreatePlane(size);
        
        StaticModelComponentSPtr meshComponent = std::make_shared<StaticModelComponent>(mesh, material);
        meshComponent->SetShadowCastingEnabled(true);
        
        auto entity = Entity::Create();
        entity->SetName(ToString(m_entityCount++) + "-Plane");
        entity->AddComponent(meshComponent);
        return entity;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    EntityUPtr PrimitiveEntityFactory::CreateBox(const Colour& colour, const Vector3& size)
    {
        CS_RELEASE_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
        
        MaterialCSPtr material = CreateStaticBlinnColourMaterial(colour);
        return CreateBox(CreateStaticBlinnColourMaterial(colour), size);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    EntityUPtr PrimitiveEntityFactory::CreateBox(const MaterialCSPtr& material, const Vector3& size)
    {
        CS_RELEASE_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
        
        ModelCSPtr mesh = m_primitiveModelFactory->CreateBox(size);
        
        StaticModelComponentSPtr meshComponent = std::make_shared<StaticModelComponent>(mesh, material);
        meshComponent->SetShadowCastingEnabled(true);
        
        auto entity = Entity::Create();
        entity->SetName(ToString(m_entityCount++) + "-Box");
        entity->AddComponent(meshComponent);
        return entity;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    MaterialCSPtr PrimitiveEntityFactory::CreateStaticBlinnColourMaterial(const Colour& in_colour) const
    {
        auto materialName = "_PrimitiveStaticBlinnColour(" + ToString(in_colour) + ")";
        auto material = m_resourcePool->GetResource<Material>(materialName);
        
        if (material == nullptr)
        {
            const Colour specularColour(0.5f, 0.5f, 0.5f, 1.0f);
            const f32 shininess = 10.0f;
            
            auto texture = m_resourcePool->LoadResource<Texture>(StorageLocation::k_chilliSource, "Textures/Blank.csimage");
            material = m_materialFactory->CreateBlinn(materialName, texture, Colour::k_black, in_colour, in_colour, specularColour, shininess);
        }
        
        return material;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void PrimitiveEntityFactory::OnInit()
    {
        m_resourcePool = Application::Get()->GetResourcePool();
        
        m_primitiveModelFactory = Application::Get()->GetSystem<PrimitiveModelFactory>();
        CS_ASSERT(m_primitiveModelFactory, "PrimitiveEntityFactory could not find required app system: PrimitiveModelFactory");
        
        m_materialFactory = Application::Get()->GetSystem<MaterialFactory>();
        CS_ASSERT(m_materialFactory, "PrimitiveEntityFactory could not find required app system: MaterialFactory");
        
        m_entityCount = 0;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void PrimitiveEntityFactory::OnDestroy()
    {
        m_resourcePool = nullptr;
        m_primitiveModelFactory = nullptr;
        m_entityCount = 0;
    }
}
