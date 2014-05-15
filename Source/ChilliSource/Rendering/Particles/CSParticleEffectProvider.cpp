//
//  CSParticleEffectProvider.cpp
//  Chilli Source
//  Created by S Downie on 30/04/2014.
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

#include <ChilliSource/Rendering/Particles/CSParticleEffectProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Container/ParamDictionarySerialiser.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Particles/ParticleEffect.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const std::string k_fileExtension("csparticle");
            
            //-------------------------------------------------------
            /// Load the texture atlas and id from the particle
            /// effect root
            ///
            /// @author S Downie
            ///
            /// @param Root element
            ///
            /// @return Pair <Atlas, Id>
            //-------------------------------------------------------
            std::pair<TextureAtlasCSPtr, std::string> LoadTextureAtlas(TiXmlElement* in_root)
            {
                std::pair<TextureAtlasCSPtr, std::string> result;
                
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                
                TiXmlElement* atlasEl = Core::XMLUtils::FirstChildElementWithName(in_root, "atlas");
                if(atlasEl != nullptr)
                {
                    result.first = resourcePool->LoadResource<TextureAtlas>(Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(atlasEl, "location", "Package")), Core::XMLUtils::GetAttributeValueOrDefault<std::string>(atlasEl, "filename", ""));
                    result.second = Core::XMLUtils::GetAttributeValueOrDefault<std::string>(atlasEl, "id", "");
                }
                
                return result;
            }
            //-------------------------------------------------------
            /// Load the descriptions of the emitters from the
            /// effect root
            ///
            /// @author S Downie
            ///
            /// @param Root element
            ///
            /// @return List of emitter descs
            //-------------------------------------------------------
            std::vector<Core::ParamDictionary> LoadEmitterDescs(TiXmlElement* in_root)
            {
                std::vector<Core::ParamDictionary> emitterDescs;
                
                TiXmlElement* emittersEl = Core::XMLUtils::FirstChildElementWithName(in_root, "emitters");
                if(emittersEl == nullptr)
                {
                    return emitterDescs;
                }
                
                TiXmlElement* emitterEl = Core::XMLUtils::FirstChildElementWithName(emittersEl, "emitter");
                while(emitterEl)
                {
                    //Get the param dictionary config values
                    Core::ParamDictionary params = Core::ParamDictionarySerialiser::FromString(emitterEl->GetText());
                    
                    emitterDescs.push_back(std::move(params));
                    
                    emitterEl = Core::XMLUtils::NextSiblingElementWithName(emitterEl);
                }
                
                return emitterDescs;
            }
            //-------------------------------------------------------
            /// Load the descriptions of the affectors from the
            /// effect root
            ///
            /// @author S Downie
            ///
            /// @param Root element
            ///
            /// @return List of affector descs
            //-------------------------------------------------------
            std::vector<Core::ParamDictionary> LoadAffectorDescs(TiXmlElement* in_root)
            {
                std::vector<Core::ParamDictionary> affectorDescs;
                TiXmlElement* affectorsEl = Core::XMLUtils::FirstChildElementWithName(in_root, "affectors");
                if(affectorsEl)
                {
                    TiXmlElement* affectorEl = Core::XMLUtils::FirstChildElementWithName(affectorsEl, "affector");
                    while(affectorEl)
                    {
                        //Get the param dictionary config values
                        Core::ParamDictionary params = Core::ParamDictionarySerialiser::FromString(affectorEl->GetText());
                        
                        affectorDescs.push_back(params);
                        
                        affectorEl = Core::XMLUtils::NextSiblingElementWithName(affectorEl);
                    }
                }
                
                return affectorDescs;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(CSParticleEffectProvider);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        CSParticleEffectProviderUPtr CSParticleEffectProvider::Create()
        {
            return CSParticleEffectProviderUPtr(new CSParticleEffectProvider());
        }
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		bool CSParticleEffectProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CSParticleEffectProvider::InterfaceID;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        Core::InterfaceIDType CSParticleEffectProvider::GetResourceType() const
        {
            return ParticleEffect::InterfaceID;
        }
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool CSParticleEffectProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_fileExtension;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void CSParticleEffectProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceSPtr& out_resource)
		{
            TiXmlDocument doc(in_filePath);
			doc.LoadFile(in_location);
			
			TiXmlElement* root = doc.RootElement();
			
            if(root == nullptr)
            {
                CS_LOG_ERROR("Failed to open particle effect file " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            else if(root->ValueStr() != "effect")
            {
                CS_LOG_ERROR("Failed to load particle effect due to incorrect file format " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            //---Load the material
            TiXmlElement* materialEl = Core::XMLUtils::FirstChildElementWithName(root, "material");
            if(materialEl == nullptr)
            {
                CS_LOG_ERROR("Failed to load particle effect due to missing material " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            MaterialCSPtr material = resourcePool->LoadResource<Material>(Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(materialEl, "location", "Package")),
                                                                          Core::XMLUtils::GetAttributeValueOrDefault<std::string>(materialEl, "filename", ""));

            if(material == nullptr || material->GetLoadState() == Core::Resource::LoadState::k_failed)
            {
                CS_LOG_ERROR("Failed to load particle effect due to missing material");
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
  
            auto emitterDescs = LoadEmitterDescs(root);
            if(emitterDescs.empty() == true)
            {
                CS_LOG_ERROR("Failed to load particle effect as it requires at least one emitter");
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            auto atlasDesc = LoadTextureAtlas(root);
            auto affectorDescs = LoadAffectorDescs(root);
            
            ParticleEffect* effectResource = (ParticleEffect*)out_resource.get();
            effectResource->Build(std::move(emitterDescs), std::move(affectorDescs), material, atlasDesc.first, std::move(atlasDesc.second));
            effectResource->SetLoadState(Core::Resource::LoadState::k_loaded);
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void CSParticleEffectProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
			auto task = std::bind(&CSParticleEffectProvider::LoadResourceAsync, this, in_location, in_filePath, in_delegate, out_resource);
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void CSParticleEffectProvider::LoadResourceAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
            auto LoadingError = [&in_filePath, &in_delegate, &out_resource](const std::string& in_msg)
            {
                CS_LOG_ERROR(in_msg + " " + in_filePath);
                
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
            };
            
            //We need to create the document on the heap as it is required by other async functions
            std::shared_ptr<TiXmlDocument> doc(new TiXmlDocument(in_filePath));
			doc->LoadFile(in_location);
			
			TiXmlElement* root = doc->RootElement();
			
            if(root == nullptr)
            {
                LoadingError("Failed to open particle effect file");
                return;
            }
            else if(root->ValueStr() != "effect")
            {
                LoadingError("Failed to load particle effect due to incorrect file format");
                return;
            }
            
            //---Load the material
            TiXmlElement* materialEl = Core::XMLUtils::FirstChildElementWithName(root, "material");
            if(materialEl == nullptr)
            {
                LoadingError("Failed to load particle effect due to missing material");
                return;
            }
            
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            resourcePool->LoadResourceAsync<Material>(Core::ParseStorageLocation(Core::XMLUtils::GetAttributeValueOrDefault<std::string>(materialEl, "location", "Package")),
                                                      Core::XMLUtils::GetAttributeValueOrDefault<std::string>(materialEl, "filename", ""),
            [doc, root, in_delegate, out_resource](const MaterialCSPtr& in_material)
            {
                //This lambda is executed on the main thread so we don't need to explicitly kick off a main thread task
                if(in_material == nullptr || in_material->GetLoadState() == Core::Resource::LoadState::k_failed)
                {
                    CS_LOG_ERROR("Failed to load particle effect due to missing material");
                    
                    out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                    in_delegate(out_resource);
                    return;
                }
                
                //We now need to kick off another async task to load the rest of the effect
                Core::Application::Get()->GetTaskScheduler()->ScheduleTask([doc, root, in_delegate, in_material, out_resource]()
                {
                    auto emitterDescs = LoadEmitterDescs(root);
                    if(emitterDescs.empty() == true)
                    {
                        CS_LOG_ERROR("Failed to load particle effect as it requires at least one emitter");
                        
                        out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                        Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                        return;
                    }
                    auto atlasDesc = LoadTextureAtlas(root);
                    auto affectorDescs = LoadAffectorDescs(root);
                    
                    ParticleEffect* effectResource = (ParticleEffect*)out_resource.get();
                    effectResource->Build(std::move(emitterDescs), std::move(affectorDescs), in_material, atlasDesc.first, std::move(atlasDesc.second));
                    effectResource->SetLoadState(Core::Resource::LoadState::k_loaded);
                    Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                });
            });
        }
	}
}

