//
//  StaticBillboardParticleDrawable.cpp
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

#include <ChilliSource/Rendering/Particle/Drawable/StaticBillboardParticleDrawable.h>

#include <ChilliSource/Core/Container/dynamic_array.h>
#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/ConcurrentParticleData.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Drawable/StaticBillboardParticleDrawableDef.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ColourUtils.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			//-----------------------------------------------------------------------------
			/// Returns the billboard size for the given size of image with the given 
			/// size policy
			///
			/// @author Ian Copland
			///
			/// @param The particle size.
			/// @param The image size.
			/// @param The size policy.
			///
			/// @param The billboard size.
			//-----------------------------------------------------------------------------
			Core::Vector2 CalcBillboardSize(const Core::Vector2& in_particleSize, const Core::Vector2& in_imageSize, StaticBillboardParticleDrawableDef::SizePolicy in_sizePolicy)
			{
				switch (in_sizePolicy)
				{
				case StaticBillboardParticleDrawableDef::SizePolicy::k_none:
					return in_particleSize;
				case StaticBillboardParticleDrawableDef::SizePolicy::k_usePreferredSize:
					return in_imageSize;
				case StaticBillboardParticleDrawableDef::SizePolicy::k_useWidthMaintainingAspect:
					return AspectRatioUtils::KeepOriginalWidthAdaptHeight(in_particleSize, in_imageSize.x / in_imageSize.y);
				case StaticBillboardParticleDrawableDef::SizePolicy::k_useHeightMaintainingAspect:
					return AspectRatioUtils::KeepOriginalHeightAdaptWidth(in_particleSize, in_imageSize.x / in_imageSize.y);
				case StaticBillboardParticleDrawableDef::SizePolicy::k_fitMaintainingAspect:
					return AspectRatioUtils::FitOriginal(in_particleSize, in_imageSize.x / in_imageSize.y);
				case StaticBillboardParticleDrawableDef::SizePolicy::k_fillMaintainingAspect:
					return AspectRatioUtils::FillOriginal(in_particleSize, in_imageSize.x / in_imageSize.y);
				default:
					CS_LOG_FATAL("Invalid size policy.");
					return Core::Vector2::k_zero;
				}
			}
			//-----------------------------------------------------------------------------
			/// Builds world space sprite data for a particle from the given parameters.
			///
			/// @author Ian Copland
			///
			/// @param The material.
			/// @param The uvs.
			/// @param The local bottom left vertex position of the sprite.
			/// @param The local top right vertex position of the sprite.
			/// @param The world position of the sprite.
			/// @param The world scale of the sprite.
			/// @param The world orientation of the sprite.
			/// @param The colour of the sprite.
			///
			/// @param The sprite data.
			//-----------------------------------------------------------------------------
			SpriteBatch::SpriteData BuildSpriteData(const MaterialCSPtr& in_material, const UVs& in_uvs, const Core::Vector2& in_localBL,
				const Core::Vector2& in_localTR, const Core::Vector3& in_worldPosition, const Core::Vector2& in_worldScale, const Core::Quaternion& in_worldOrientation,
				const Core::Colour& in_colour)
			{
				SpriteBatch::SpriteData spriteData;
				spriteData.pMaterial = in_material;

				//set the sprite colour
				Core::ByteColour colour = Core::ColourUtils::ColourToByteColour(in_colour);
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].Col = colour;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].Col = colour;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].Col = colour;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].Col = colour;

				//set the UVs.
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vTex.x = in_uvs.m_u;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vTex.y = in_uvs.m_v;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vTex.x = in_uvs.m_u;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vTex.y = in_uvs.m_v + in_uvs.m_t;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vTex.x = in_uvs.m_u + in_uvs.m_s;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vTex.y = in_uvs.m_v;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vTex.x = in_uvs.m_u + in_uvs.m_s;
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vTex.y = in_uvs.m_v + in_uvs.m_t;

				//Build the vertex data.
				Core::Vector3 localTopLeft(in_localBL.x * in_worldScale.x, in_localTR.y * in_worldScale.y, 0.0f);
				Core::Vector3 worldTopLeft = in_worldPosition + Core::Vector3::Rotate(localTopLeft, in_worldOrientation);
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vPos = Core::Vector4(worldTopLeft, 1.0f);

				Core::Vector3 localTopRight(in_localTR.x * in_worldScale.x, in_localTR.y * in_worldScale.y, 0.0f);
				Core::Vector3 worldTopRight = in_worldPosition + Core::Vector3::Rotate(localTopRight, in_worldOrientation);
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vPos = Core::Vector4(worldTopRight, 1.0f);

				Core::Vector3 localBottomLeft(in_localBL.x * in_worldScale.x, in_localBL.y * in_worldScale.y, 0.0f);
				Core::Vector3 worldBottomLeft = in_worldPosition + Core::Vector3::Rotate(localBottomLeft, in_worldOrientation);
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vPos = Core::Vector4(worldBottomLeft, 1.0f);

				Core::Vector3 localBottomRight(in_localTR.x * in_worldScale.x, in_localBL.y * in_worldScale.y, 0.0f);
				Core::Vector3 worldBottomRight = in_worldPosition + Core::Vector3::Rotate(localBottomRight, in_worldOrientation);
				spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vPos = Core::Vector4(worldBottomRight, 1.0f);

				return spriteData;
			}
		}

		//----------------------------------------------
		//----------------------------------------------
		StaticBillboardParticleDrawable::StaticBillboardParticleDrawable(const Core::Entity* in_entity, const ParticleDrawableDef* in_drawableDef, ConcurrentParticleData* in_concurrentParticleData)
			: ParticleDrawable(in_entity, in_drawableDef, in_concurrentParticleData), m_billboardDrawableDef(static_cast<const StaticBillboardParticleDrawableDef*>(in_drawableDef)),
			m_particleBillboardIndices(in_drawableDef->GetParticleEffect()->GetMaxParticles())
		{
			BuildBillboardImageData();
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void StaticBillboardParticleDrawable::ActivateParticle(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, u32 in_index)
		{
			CS_ASSERT(in_index >= 0 && in_index < m_particleBillboardIndices.size(), "Index out of bounds!");

			switch (m_billboardDrawableDef->GetImageSelectionType())
			{
			case StaticBillboardParticleDrawableDef::ImageSelectionType::k_cycle:
				m_particleBillboardIndices[in_index] = m_nextBillboardIndex++;
				if (m_nextBillboardIndex >= m_billboards->size())
				{
					m_nextBillboardIndex = 0;
				}
				break;
			case StaticBillboardParticleDrawableDef::ImageSelectionType::k_random:
				m_particleBillboardIndices[in_index] = Core::Random::Generate<u32>(0, m_billboards->size() - 1);
				break;
			default:
				CS_LOG_FATAL("Invalid image selection type.");
				break;
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void StaticBillboardParticleDrawable::DrawParticles(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, const CameraComponent* in_camera)
		{
			switch (GetDrawableDef()->GetParticleEffect()->GetSimulationSpace())
			{
			case ParticleEffect::SimulationSpace::k_local:
				DrawLocalSpace(in_particleData, in_camera);
				break;
			case ParticleEffect::SimulationSpace::k_world:
				DrawWorldSpace(in_particleData, in_camera);
				break;
			default:
				CS_LOG_FATAL("Invalid simulation space.");
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void StaticBillboardParticleDrawable::BuildBillboardImageData()
		{
			const auto& textureAtlas = m_billboardDrawableDef->GetTextureAltas();
			const auto& atlasIds = m_billboardDrawableDef->GetAtlasIds();

			if (textureAtlas != nullptr && atlasIds.empty() == false)
			{
				m_billboards = std::unique_ptr<Core::dynamic_array<BillboardData>>(new Core::dynamic_array<BillboardData>(m_billboardDrawableDef->GetAtlasIds().size()));
				for (u32 i = 0; i < m_billboards->size(); ++i)
				{
					const auto& frame = textureAtlas->GetFrame(m_billboardDrawableDef->GetAtlasIds()[i]);

					//calculate the normalised bounds.
					f32 left = (-0.5f * frame.m_originalSize.x + frame.m_offset.x) / frame.m_originalSize.x;
					f32 right = (0.5f * frame.m_originalSize.x - (frame.m_originalSize.x - frame.m_offset.x - frame.m_croppedSize.x)) / frame.m_originalSize.x;
					f32 top = (0.5f * frame.m_originalSize.y - frame.m_offset.y) / frame.m_originalSize.y;
					f32 bottom = (-0.5f * frame.m_originalSize.y + (frame.m_originalSize.y - frame.m_offset.y - frame.m_croppedSize.y)) / frame.m_originalSize.y;

					//Get the billboard size. This is determined by the size policy.
					Core::Vector2 billboardSize = CalcBillboardSize(m_billboardDrawableDef->GetParticleSize(), frame.m_originalSize, m_billboardDrawableDef->GetSizePolicy());

					//scale the normalised bounds up to the billboard size.
					BillboardData& billboardData = m_billboards->at(i);
					billboardData.m_bottomLeft.x = left * billboardSize.x;
					billboardData.m_bottomLeft.y = bottom * billboardSize.y;
					billboardData.m_topRight.x = right * billboardSize.y;
					billboardData.m_topRight.y = top * billboardSize.x;

					billboardData.m_uvs = frame.m_uvs;
				}
			}
			else
			{
				auto texture = m_billboardDrawableDef->GetMaterial()->GetTexture();
				CS_ASSERT(texture != nullptr, "Particle effect material cannot have no texture.");

				m_billboards = std::unique_ptr<Core::dynamic_array<BillboardData>>(new Core::dynamic_array<BillboardData>(1));

				Core::Vector2 billboardSize = CalcBillboardSize(m_billboardDrawableDef->GetParticleSize(), Core::Vector2(f32(texture->GetWidth()), f32(texture->GetHeight())),
					m_billboardDrawableDef->GetSizePolicy());

				BillboardData& billboardData = m_billboards->at(0);
				billboardData.m_bottomLeft.x = -0.5f * billboardSize.x;
				billboardData.m_bottomLeft.y = -0.5f * billboardSize.y;
				billboardData.m_topRight.x = 0.5f * billboardSize.y;
				billboardData.m_topRight.y = 0.5f * billboardSize.x;

				billboardData.m_uvs.m_u = 0.0f;
				billboardData.m_uvs.m_v = 0.0f;
				billboardData.m_uvs.m_s = 1.0f;
				billboardData.m_uvs.m_t = 1.0f;
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void StaticBillboardParticleDrawable::DrawLocalSpace(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, const CameraComponent* in_camera) const
		{
			const auto& material = m_billboardDrawableDef->GetMaterial();
			auto entityWorldTransform = GetEntity()->GetTransform().GetWorldTransform();

			//we can't directly apply the parent entities scale to the particles as this would look strange as
			//the camera moved around an emitting entity with a non-uniform scale, so this works out a uniform
			//scale from the average of the components.
			auto entityScale = GetEntity()->GetTransform().GetWorldScale();
			f32 particleScaleFactor = (entityScale.x + entityScale.y + entityScale.z) / 3.0f;

			//billboard by applying the inverse of the view orientation. The view orientation is the inverse of the camera entity orientation.
			auto inverseView = in_camera->GetEntity()->GetTransform().GetWorldOrientation();

			for (u32 i = 0; i < in_particleData.size(); ++i)
			{
				const auto& particle = in_particleData[i];

				if (particle.m_isActive == true && particle.m_colour != Core::Colour::k_transparent)
				{
					auto worldPosition = particle.m_position * entityWorldTransform;
					auto worldScale = particle.m_scale * particleScaleFactor;

					//rotate locally in the XY plane before rotating to face the camera.
					auto worldOrientation = Core::Quaternion(Core::Vector3::k_unitPositiveZ, particle.m_rotation) * inverseView;

					const auto& billboardData = m_billboards->at(m_particleBillboardIndices[i]);
					auto spriteData = BuildSpriteData(material, billboardData.m_uvs, billboardData.m_bottomLeft, billboardData.m_topRight, worldPosition, worldScale, worldOrientation,
						particle.m_colour);

					Core::Application::Get()->GetRenderSystem()->GetDynamicSpriteBatchPtr()->Render(spriteData, nullptr);
				}
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void StaticBillboardParticleDrawable::DrawWorldSpace(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, const CameraComponent* in_camera) const
		{
			const auto& material = m_billboardDrawableDef->GetMaterial();

			//billboard by applying the inverse of the view orientation. The view orientation is the inverse of the camera entity orientation.
			auto inverseView = in_camera->GetEntity()->GetTransform().GetWorldOrientation();

			for (u32 i = 0; i < in_particleData.size(); ++i)
			{
				const auto& particle = in_particleData[i];

				if (particle.m_isActive == true && particle.m_colour != Core::Colour::k_transparent)
				{
					//rotate locally in the XY plane before rotating to face the camera.
					auto worldOrientation = Core::Quaternion(Core::Vector3::k_unitPositiveZ, particle.m_rotation) * inverseView;

					const auto& billboardData = m_billboards->at(m_particleBillboardIndices[i]);
					auto spriteData = BuildSpriteData(material, billboardData.m_uvs, billboardData.m_bottomLeft, billboardData.m_topRight, particle.m_position, particle.m_scale,
						worldOrientation, particle.m_colour);

					Core::Application::Get()->GetRenderSystem()->GetDynamicSpriteBatchPtr()->Render(spriteData, nullptr);
				}
			}
		}
	}
}
