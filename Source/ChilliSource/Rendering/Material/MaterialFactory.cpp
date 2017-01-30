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

#include <ChilliSource/Rendering/Material/MaterialFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Base/CullFace.h>
#include <ChilliSource/Rendering/Base/TestFunc.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(MaterialFactory);
    
    //------------------------------------------------------------------------------
    MaterialFactoryUPtr MaterialFactory::Create() noexcept
    {
        return MaterialFactoryUPtr(new MaterialFactory());
    }
    
    //------------------------------------------------------------------------------
    bool MaterialFactory::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return interfaceId == MaterialFactory::InterfaceID;
    }
    
    //------------------------------------------------------------------------------
    MaterialCSPtr MaterialFactory::CreateUnlit(const std::string& uniqueId, const TextureCSPtr& texture, bool isTransparent, const Colour& colour) const noexcept
    {
        MaterialSPtr material(CreateCustom(uniqueId));
        material->SetShadingType(MaterialShadingType::k_unlit);
        material->AddTexture(texture);
        material->SetTransparencyEnabled(isTransparent);
        material->SetEmissive(colour);
        material->SetLoadState(Resource::LoadState::k_loaded);
        return material;
    }
    
    //------------------------------------------------------------------------------
    MaterialCSPtr MaterialFactory::CreateSkybox(const std::string& uniqueId, const CubemapCSPtr& cubemap) const noexcept
    {
        MaterialSPtr material(CreateCustom(uniqueId));
        material->SetShadingType(MaterialShadingType::k_skybox);
        material->AddCubemap(cubemap);
        material->SetTransparencyEnabled(false);
        material->SetDepthWriteEnabled(false);
        material->SetDepthTestFunc(TestFunc::k_lessEqual);
        material->SetLoadState(Resource::LoadState::k_loaded);
        return material;
    }
    
    //------------------------------------------------------------------------------
    MaterialCSPtr MaterialFactory::CreateBlinn(const std::string& uniqueId, const TextureCSPtr& texture, const Colour& emissiveColour, const Colour& ambientColour, const Colour& diffuseColour,
                              const Colour& specularColour, f32 shininess) const noexcept
    {
        MaterialSPtr material(CreateCustom(uniqueId));
        material->SetShadingType(MaterialShadingType::k_blinn);
        material->AddTexture(texture);
        material->SetEmissive(emissiveColour);
        material->SetAmbient(ambientColour);
        material->SetDiffuse(diffuseColour);
        material->SetSpecular(Colour(specularColour.r, specularColour.g, specularColour.b, shininess));
        material->SetLoadState(Resource::LoadState::k_loaded);
        return material;
    }
    
    //------------------------------------------------------------------------------
    MaterialSPtr MaterialFactory::CreateCustom(const std::string& uniqueId) const noexcept
    {
        ResourcePool* resourcePool = Application::Get()->GetResourcePool();
        return resourcePool->CreateResource<Material>(uniqueId);
    }
}
