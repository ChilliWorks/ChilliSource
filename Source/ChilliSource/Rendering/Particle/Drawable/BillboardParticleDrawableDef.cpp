//
//  BillboardParticleDrawableDef.cpp
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

#include <ChilliSource/Rendering/Particle/Drawable/BillboardParticleDrawableDef.h>

#include <ChilliSource/Rendering/Particle/Drawable/BillboardParticleDrawable.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(BillboardParticleDrawableDef);
		//--------------------------------------------------
		//--------------------------------------------------
		BillboardParticleDrawableDef::BillboardParticleDrawableDef(const CSRendering::MaterialCSPtr& in_material, const CSCore::Vector2& in_particleSize, SizePolicy in_sizePolicy)
			: m_material(in_material), m_particleSize(in_particleSize), m_sizePolicy(in_sizePolicy)
		{
			CS_ASSERT(m_material != nullptr, "Cannot create a Billboard Particle Drawable Def with a null material.");
		}
		//--------------------------------------------------
		//--------------------------------------------------
		BillboardParticleDrawableDef::BillboardParticleDrawableDef(const CSRendering::MaterialCSPtr& in_material, const CSRendering::TextureAtlasCSPtr& in_textureAtlas, const std::string& in_atlasId, const CSCore::Vector2& in_particleSize, SizePolicy in_sizePolicy)
			: m_material(in_material), m_textureAtlas(in_textureAtlas), m_particleSize(in_particleSize), m_sizePolicy(in_sizePolicy)
		{
			CS_ASSERT(m_material != nullptr, "Cannot create a Billboard Particle Drawable Def with a null material.");
			CS_ASSERT(m_textureAtlas != nullptr, "Cannot create a Billboard Particle Drawable Def with a null texture atlas.");

			m_atlasIds.push_back(in_atlasId);
		}
		//--------------------------------------------------
		//--------------------------------------------------
		BillboardParticleDrawableDef::BillboardParticleDrawableDef(const CSRendering::MaterialCSPtr& in_material, const CSRendering::TextureAtlasCSPtr& in_textureAtlas, const std::vector<std::string>& in_atlasIds, ImageSelectionType in_imageSelectionType, const CSCore::Vector2& in_particleSize, SizePolicy in_sizePolicy)
			: m_material(in_material), m_textureAtlas(in_textureAtlas), m_atlasIds(in_atlasIds), m_imageSelectionType(in_imageSelectionType), m_particleSize(in_particleSize), m_sizePolicy(in_sizePolicy)
		{
			CS_ASSERT(m_material != nullptr, "Cannot create a Billboard Particle Drawable Def with a null material.");
			CS_ASSERT(m_textureAtlas != nullptr, "Cannot create a Billboard Particle Drawable Def with a null texture atlas.");
		}
		//--------------------------------------------------
		//--------------------------------------------------
		BillboardParticleDrawableDef::BillboardParticleDrawableDef(const CSCore::ParamDictionary& in_params, const LoadedDelegate& in_asyncDelegate)
		{
			//TODO: !?
			CS_LOG_FATAL("Unimplemented: BillboardParticleDrawableDef::BillboardParticleDrawableDef(const CSCore::ParamDictionary& in_params, const LoadedDelegate& in_asyncDelegate)");
		}
		//--------------------------------------------------
		//-------------------------------------------------
		bool BillboardParticleDrawableDef::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (BillboardParticleDrawableDef::InterfaceID == in_interfaceId);
		}
		//--------------------------------------------------
		//--------------------------------------------------
		ParticleDrawableUPtr BillboardParticleDrawableDef::CreateInstance(const CSCore::Entity* in_entity, const concurrent_dynamic_array<ParticleDrawData>* in_particleDrawDataArray) const
		{
			return ParticleDrawableUPtr(new BillboardParticleDrawable(in_entity, this, in_particleDrawDataArray));
		}
		//--------------------------------------------------
		//--------------------------------------------------
		const CSRendering::MaterialCSPtr& BillboardParticleDrawableDef::GetMaterial() const
		{
			return m_material;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		const CSRendering::TextureAtlasCSPtr& BillboardParticleDrawableDef::GetTextureAltas() const
		{
			return m_textureAtlas;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		const std::vector<std::string>& BillboardParticleDrawableDef::GetAtlasIds() const
		{
			return m_atlasIds;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		BillboardParticleDrawableDef::ImageSelectionType BillboardParticleDrawableDef::GetImageSelectionType() const
		{
			return m_imageSelectionType;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		const CSCore::Vector2& BillboardParticleDrawableDef::GetParticleSize() const
		{
			return m_particleSize;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		BillboardParticleDrawableDef::SizePolicy BillboardParticleDrawableDef::GetSizePolicy() const
		{
			return m_sizePolicy;
		}
	}
}