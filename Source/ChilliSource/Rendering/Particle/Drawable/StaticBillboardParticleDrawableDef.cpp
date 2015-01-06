//
//  StaticBillboardParticleDrawableDef.cpp
//  Chilli Source
//  Created by Ian Copland on 13/10/2014.
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

#include <ChilliSource/Rendering/Particle/Drawable/StaticBillboardParticleDrawableDef.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Particle/Drawable/StaticBillboardParticleDrawable.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			//-----------------------------------------------------------------
			/// Parse an image selection type from the given string. This is
			/// case insensitive. If the string is not a valid image selection
			/// type this will error.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return the parsed image selection type.
			//-----------------------------------------------------------------
			StaticBillboardParticleDrawableDef::ImageSelectionType ParseImageSelectionType(const std::string& in_imageSelectionTypeString)
			{
				std::string imageSelectionTypeString = in_imageSelectionTypeString;
				Core::StringUtils::ToLowerCase(imageSelectionTypeString);

				if (imageSelectionTypeString == "cycle")
				{
					return StaticBillboardParticleDrawableDef::ImageSelectionType::k_cycle;
				}
				else if (imageSelectionTypeString == "random")
				{
					return StaticBillboardParticleDrawableDef::ImageSelectionType::k_random;
				}

				CS_LOG_FATAL("Invalid image selection type: " + in_imageSelectionTypeString);
				return StaticBillboardParticleDrawableDef::ImageSelectionType::k_random;
			}
			//-----------------------------------------------------------------
			/// Parse a size policy from the given string. This is case 
			/// insensitive. If the string is not a valid size policy this will 
			/// error.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return the parsed size policy.
			//-----------------------------------------------------------------
			StaticBillboardParticleDrawableDef::SizePolicy ParseSizePolicy(const std::string& in_sizePolicyString)
			{
				std::string sizePolicyString = in_sizePolicyString;
				Core::StringUtils::ToLowerCase(sizePolicyString);

				if (sizePolicyString == "fillmaintainingaspect")
				{
					return StaticBillboardParticleDrawableDef::SizePolicy::k_fillMaintainingAspect;
				}
				else if (sizePolicyString == "fitmaintainingaspect")
				{
					return StaticBillboardParticleDrawableDef::SizePolicy::k_fitMaintainingAspect;
				}
				else if (sizePolicyString == "none")
				{
					return StaticBillboardParticleDrawableDef::SizePolicy::k_none;
				}
				else if (sizePolicyString == "useheightmaintainingaspect")
				{
					return StaticBillboardParticleDrawableDef::SizePolicy::k_useHeightMaintainingAspect;
				}
				else if (sizePolicyString == "usepreferredsize")
				{
					return StaticBillboardParticleDrawableDef::SizePolicy::k_usePreferredSize;
				}
				else if (sizePolicyString == "usewidthmaintainingaspect")
				{
					return StaticBillboardParticleDrawableDef::SizePolicy::k_useWidthMaintainingAspect;
				}

				CS_LOG_FATAL("Invalid size policy: " + sizePolicyString);
				return StaticBillboardParticleDrawableDef::SizePolicy::k_none;
			}
			//-----------------------------------------------------------------
			/// Parse a list of space separated strings.
			///
			/// @author Ian Copland
			///
			/// @param The string to parse.
			///
			/// @return the parsed list of strings.
			//-----------------------------------------------------------------
			std::vector<std::string> ParseStringList(const std::string& in_stringList)
			{
				return Core::StringUtils::Split(in_stringList, " ");
			}
		}

		CS_DEFINE_NAMEDTYPE(StaticBillboardParticleDrawableDef);
		//--------------------------------------------------
		//--------------------------------------------------
		StaticBillboardParticleDrawableDef::StaticBillboardParticleDrawableDef(const MaterialCSPtr& in_material, const Core::Vector2& in_particleSize, SizePolicy in_sizePolicy)
			: m_material(in_material), m_particleSize(in_particleSize), m_sizePolicy(in_sizePolicy)
		{
			CS_ASSERT(m_material != nullptr, "Cannot create a Billboard Particle Drawable Def with a null material.");
		}
		//--------------------------------------------------
		//--------------------------------------------------
		StaticBillboardParticleDrawableDef::StaticBillboardParticleDrawableDef(const MaterialCSPtr& in_material, const TextureAtlasCSPtr& in_textureAtlas, const std::string& in_atlasId, const Core::Vector2& in_particleSize, SizePolicy in_sizePolicy)
			: m_material(in_material), m_textureAtlas(in_textureAtlas), m_particleSize(in_particleSize), m_sizePolicy(in_sizePolicy)
		{
			CS_ASSERT(m_material != nullptr, "Cannot create a Billboard Particle Drawable Def with a null material.");
			CS_ASSERT(m_textureAtlas != nullptr, "Cannot create a Billboard Particle Drawable Def with a null texture atlas.");

			m_atlasIds.push_back(in_atlasId);
		}
		//--------------------------------------------------
		//--------------------------------------------------
		StaticBillboardParticleDrawableDef::StaticBillboardParticleDrawableDef(const MaterialCSPtr& in_material, const TextureAtlasCSPtr& in_textureAtlas, const std::vector<std::string>& in_atlasIds, ImageSelectionType in_imageSelectionType, const Core::Vector2& in_particleSize, SizePolicy in_sizePolicy)
			: m_material(in_material), m_textureAtlas(in_textureAtlas), m_atlasIds(in_atlasIds), m_imageSelectionType(in_imageSelectionType), m_particleSize(in_particleSize), m_sizePolicy(in_sizePolicy)
		{
			CS_ASSERT(m_material != nullptr, "Cannot create a Billboard Particle Drawable Def with a null material.");
			CS_ASSERT(m_textureAtlas != nullptr, "Cannot create a Billboard Particle Drawable Def with a null texture atlas.");
		}
		//--------------------------------------------------
		//--------------------------------------------------
		StaticBillboardParticleDrawableDef::StaticBillboardParticleDrawableDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate)
		{
			//Image selection type
			Json::Value jsonValue = in_paramsJson.get("ImageSelectionType", Json::nullValue);
			if (jsonValue.isNull() == false)
			{
				CS_ASSERT(jsonValue.isString(), "Image selection type must be a string.");
				m_imageSelectionType = ParseImageSelectionType(jsonValue.asString());
			}

			//Particle size
			jsonValue = in_paramsJson.get("ParticleSize", Json::nullValue);
			if (jsonValue.isNull() == false)
			{
				CS_ASSERT(jsonValue.isString(), "particle size must be a string.");
				m_particleSize = Core::ParseVector2(jsonValue.asString());
			}

			//Size Policy
			jsonValue = in_paramsJson.get("SizePolicy", Json::nullValue);
			if (jsonValue.isNull() == false)
			{
				CS_ASSERT(jsonValue.isString(), "size policy must be a string.");
				m_sizePolicy = ParseSizePolicy(jsonValue.asString());
			}

			//load the resources.
			if (in_asyncDelegate == nullptr)
			{
				LoadResources(in_paramsJson);
			}
			else
			{
				LoadResourcesAsync(in_paramsJson, in_asyncDelegate);
			}
		}
		//--------------------------------------------------
		//-------------------------------------------------
		bool StaticBillboardParticleDrawableDef::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (ParticleDrawableDef::InterfaceID == in_interfaceId || StaticBillboardParticleDrawableDef::InterfaceID == in_interfaceId);
		}
		//--------------------------------------------------
		//--------------------------------------------------
		ParticleDrawableUPtr StaticBillboardParticleDrawableDef::CreateInstance(const Core::Entity* in_entity, ConcurrentParticleData* in_concurrentParticleData) const
		{
			return ParticleDrawableUPtr(new StaticBillboardParticleDrawable(in_entity, this, in_concurrentParticleData));
		}
		//--------------------------------------------------
		//--------------------------------------------------
		const MaterialCSPtr& StaticBillboardParticleDrawableDef::GetMaterial() const
		{
			return m_material;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		const TextureAtlasCSPtr& StaticBillboardParticleDrawableDef::GetTextureAltas() const
		{
			return m_textureAtlas;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		const std::vector<std::string>& StaticBillboardParticleDrawableDef::GetAtlasIds() const
		{
			return m_atlasIds;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		StaticBillboardParticleDrawableDef::ImageSelectionType StaticBillboardParticleDrawableDef::GetImageSelectionType() const
		{
			return m_imageSelectionType;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		const Core::Vector2& StaticBillboardParticleDrawableDef::GetParticleSize() const
		{
			return m_particleSize;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		StaticBillboardParticleDrawableDef::SizePolicy StaticBillboardParticleDrawableDef::GetSizePolicy() const
		{
			return m_sizePolicy;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void StaticBillboardParticleDrawableDef::LoadResources(const Json::Value& in_paramsJson)
		{
			auto resourcePool = Core::Application::Get()->GetResourcePool();

			//material
			Json::Value materialLocationJson = in_paramsJson.get("MaterialLocation", "Package");
			Json::Value materialPathJson = in_paramsJson.get("MaterialPath", Json::nullValue);
			CS_ASSERT(materialLocationJson.isNull() == false && materialLocationJson.isString() == true && materialPathJson.isNull() == false &&
				materialPathJson.isString() == true, "Must provide a valid material for a billboard particle drawable.");

			m_material = resourcePool->LoadResource<Rendering::Material>(Core::ParseStorageLocation(materialLocationJson.asString()), materialPathJson.asString());
			CS_ASSERT((m_material != nullptr && m_material->GetLoadState() == Core::Resource::LoadState::k_loaded), "Could not load material: " + materialPathJson.asString());

			//texture atlas
			Json::Value atlasPathJson = in_paramsJson.get("AtlasPath", Json::nullValue);
			if (atlasPathJson.isNull() == false)
			{
				CS_ASSERT(atlasPathJson.isString() == true, "Atlas path must be a string.");

				Json::Value imageIdsJson = in_paramsJson.get("ImageIds", Json::nullValue);
				CS_ASSERT(imageIdsJson.isNull() == false && imageIdsJson.isString() == true, "Must provide valid Image Ids if using a texture atlas.");

				m_atlasIds = ParseStringList(imageIdsJson.asString());
				CS_ASSERT(m_atlasIds.empty() == false, "Atlas Ids empty.");

				Json::Value atlasLocationJson = in_paramsJson.get("AtlasLocation", "Package");
				CS_ASSERT(atlasLocationJson.isNull() == false && atlasLocationJson.isString() == true, "Invalid atlas path and location.");

				m_textureAtlas = resourcePool->LoadResource<Rendering::TextureAtlas>(Core::ParseStorageLocation(atlasLocationJson.asString()), atlasPathJson.asString());
				CS_ASSERT((m_textureAtlas != nullptr && m_textureAtlas->GetLoadState() == Core::Resource::LoadState::k_loaded), "Could not load texture atlas: " + atlasPathJson.asString());
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void StaticBillboardParticleDrawableDef::LoadResourcesAsync(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate)
		{
			auto resourcePool = Core::Application::Get()->GetResourcePool();

			//material
			Json::Value materialLocationJson = in_paramsJson.get("MaterialLocation", "Package");
			Json::Value materialPathJson = in_paramsJson.get("MaterialPath", Json::nullValue);
			CS_ASSERT(materialLocationJson.isNull() == false && materialLocationJson.isString() == true && materialPathJson.isNull() == false &&
				materialPathJson.isString() == true, "Must provide a valid material for a billboard particle drawable.");

			resourcePool->LoadResourceAsync<Rendering::Material>(Core::ParseStorageLocation(materialLocationJson.asString()), materialPathJson.asString(), [=](const Rendering::MaterialCSPtr& in_material)
			{
				m_material = in_material;
				CS_ASSERT((m_material != nullptr && m_material->GetLoadState() == Core::Resource::LoadState::k_loaded), "Could not load material: " + materialPathJson.asString());

				//texture atlas
				Json::Value atlasPathJson = in_paramsJson.get("AtlasPath", Json::nullValue);
				if (atlasPathJson.isNull() == false)
				{
					CS_ASSERT(atlasPathJson.isString() == true, "Atlas path must be a string.");

					Json::Value imageIdsJson = in_paramsJson.get("ImageIds", Json::nullValue);
					CS_ASSERT(imageIdsJson.isNull() == false && imageIdsJson.isString() == true, "Must provide valid Image Ids if using a texture atlas.");

					m_atlasIds = ParseStringList(imageIdsJson.asString());
					CS_ASSERT(m_atlasIds.empty() == false, "Atlas Ids empty.");

					Json::Value atlasLocationJson = in_paramsJson.get("AtlasLocation", "Package");
					CS_ASSERT(atlasLocationJson.isNull() == false && atlasLocationJson.isString() == true, "Invalid atlas path and location.");

					resourcePool->LoadResourceAsync<Rendering::TextureAtlas>(Core::ParseStorageLocation(atlasLocationJson.asString()), atlasPathJson.asString(), [=](const Rendering::TextureAtlasCSPtr& in_textureAtlas)
					{
						m_textureAtlas = in_textureAtlas;
						CS_ASSERT((m_textureAtlas != nullptr && m_textureAtlas->GetLoadState() == Core::Resource::LoadState::k_loaded), "Could not load texture atlas: " + atlasPathJson.asString());

						in_asyncDelegate(this);
					});
				}
			});
		}
	}
}