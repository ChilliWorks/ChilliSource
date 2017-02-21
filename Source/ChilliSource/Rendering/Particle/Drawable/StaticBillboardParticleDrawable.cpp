//
//  StaticBillboardParticleDrawable.cpp
//  ChilliSource
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
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Particle/ConcurrentParticleData.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Drawable/StaticBillboardParticleDrawableDef.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ColourUtils.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Sprite/SpriteMeshBuilder.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
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
        Vector2 CalcBillboardSize(const Vector2& in_particleSize, const Vector2& in_imageSize, SizePolicy in_sizePolicy)
        {
            switch (in_sizePolicy)
            {
            case SizePolicy::k_none:
                return in_particleSize;
            case SizePolicy::k_usePreferredSize:
                return in_imageSize;
            case SizePolicy::k_useWidthMaintainingAspect:
                return AspectRatioUtils::KeepOriginalWidthAdaptHeight(in_particleSize, in_imageSize.x / in_imageSize.y);
            case SizePolicy::k_useHeightMaintainingAspect:
                return AspectRatioUtils::KeepOriginalHeightAdaptWidth(in_particleSize, in_imageSize.x / in_imageSize.y);
            case SizePolicy::k_fitMaintainingAspect:
                return AspectRatioUtils::FitOriginal(in_particleSize, in_imageSize.x / in_imageSize.y);
            case SizePolicy::k_fillMaintainingAspect:
                return AspectRatioUtils::FillOriginal(in_particleSize, in_imageSize.x / in_imageSize.y);
            default:
                CS_LOG_FATAL("Invalid size policy.");
                return Vector2::k_zero;
            }
        }
    }

    //----------------------------------------------
    //----------------------------------------------
    StaticBillboardParticleDrawable::StaticBillboardParticleDrawable(const Entity* in_entity, const ParticleDrawableDef* in_drawableDef, ConcurrentParticleData* in_concurrentParticleData)
        : ParticleDrawable(in_entity, in_drawableDef, in_concurrentParticleData), m_billboardDrawableDef(static_cast<const StaticBillboardParticleDrawableDef*>(in_drawableDef)),
        m_particleBillboardIndices(in_drawableDef->GetParticleEffect()->GetMaxParticles())
    {
        BuildBillboardImageData();
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void StaticBillboardParticleDrawable::ActivateParticle(const dynamic_array<ConcurrentParticleData::Particle>& in_particleData, u32 in_index)
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
            m_particleBillboardIndices[in_index] = Random::Generate<u32>(0, static_cast<s32>(m_billboards->size()) - 1);
            break;
        default:
            CS_LOG_FATAL("Invalid image selection type.");
            break;
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void StaticBillboardParticleDrawable::DrawParticles(const dynamic_array<ConcurrentParticleData::Particle>& particleData, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator)
    {
        switch (GetDrawableDef()->GetParticleEffect()->GetSimulationSpace())
        {
        case ParticleEffect::SimulationSpace::k_local:
            DrawLocalSpace(particleData, renderSnapshot, frameAllocator);
            break;
        case ParticleEffect::SimulationSpace::k_world:
            DrawWorldSpace(particleData, renderSnapshot, frameAllocator);
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
            m_billboards = std::unique_ptr<dynamic_array<BillboardData>>(new dynamic_array<BillboardData>(m_billboardDrawableDef->GetAtlasIds().size()));
            for (u32 i = 0; i < m_billboards->size(); ++i)
            {
                const auto& frame = textureAtlas->GetFrame(m_billboardDrawableDef->GetAtlasIds()[i]);

                //calculate the normalised bounds.
                f32 left = (-0.5f * frame.m_originalSize.x + frame.m_offset.x) / frame.m_originalSize.x;
                f32 right = (0.5f * frame.m_originalSize.x - (frame.m_originalSize.x - frame.m_offset.x - frame.m_croppedSize.x)) / frame.m_originalSize.x;
                f32 top = (0.5f * frame.m_originalSize.y - frame.m_offset.y) / frame.m_originalSize.y;
                f32 bottom = (-0.5f * frame.m_originalSize.y + (frame.m_originalSize.y - frame.m_offset.y - frame.m_croppedSize.y)) / frame.m_originalSize.y;

                //Get the billboard size. This is determined by the size policy.
                Vector2 billboardSize = CalcBillboardSize(m_billboardDrawableDef->GetParticleSize(), frame.m_originalSize, m_billboardDrawableDef->GetSizePolicy());

                //build the billboard data.
                BillboardData& billboardData = m_billboards->at(i);
                billboardData.m_localSize = Vector2(right - left, top - bottom) * billboardSize;
                billboardData.m_localCentre = Vector2(left, bottom) * billboardSize + 0.5f * billboardData.m_localSize;
                billboardData.m_uvs = frame.m_uvs;
            }
        }
        else
        {
            auto texture = m_billboardDrawableDef->GetMaterial()->GetTexture();
            CS_ASSERT(texture != nullptr, "Particle effect material cannot have no texture.");

            m_billboards = std::unique_ptr<dynamic_array<BillboardData>>(new dynamic_array<BillboardData>(1));

            Vector2 billboardSize = CalcBillboardSize(m_billboardDrawableDef->GetParticleSize(), Vector2(f32(texture->GetDimensions().x), f32(texture->GetDimensions().y)),
                m_billboardDrawableDef->GetSizePolicy());

            BillboardData& billboardData = m_billboards->at(0);
            billboardData.m_localSize = billboardSize;
            billboardData.m_localCentre = Vector2(0.0f, 0.0f);
            billboardData.m_uvs = UVs(0.0f, 0.0f, 1.0f, 1.0f);
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void StaticBillboardParticleDrawable::DrawLocalSpace(const dynamic_array<ConcurrentParticleData::Particle>& particleData, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) const
    {
        auto renderMaterialGroup = m_billboardDrawableDef->GetMaterial()->GetRenderMaterialGroup();
        auto entityWorldTransform = GetEntity()->GetTransform().GetWorldTransform();

        //we can't directly apply the parent entities scale to the particles as this would look strange as
        //the camera moved around an emitting entity with a non-uniform scale, so this works out a uniform
        //scale from the average of the components.
        auto entityScale = GetEntity()->GetTransform().GetWorldScale();
        f32 particleScaleFactor = (entityScale.x + entityScale.y + entityScale.z) / 3.0f;

        //billboard by applying the inverse of the view orientation. The view orientation is the inverse of the camera entity orientation.
        auto inverseView = renderSnapshot.GetRenderCamera().GetOrientation();

        for (u32 i = 0; i < particleData.size(); ++i)
        {
            const auto& particle = particleData[i];

            if (particle.m_isActive == true && particle.m_colour != Colour::k_transparent)
            {
                const auto& billboardData = m_billboards->at(m_particleBillboardIndices[i]);
                
                auto worldPosition = particle.m_position * entityWorldTransform;
                auto worldScale = Vector3(particle.m_scale * particleScaleFactor, 1.0f);
                auto worldOrientation = Quaternion(Vector3::k_unitPositiveZ, particle.m_rotation) * inverseView; //rotate locally in the XY plane before rotating to face the camera.
                auto worldMatrix = Matrix4::CreateTransform(worldPosition, worldScale, worldOrientation);
                
                auto renderDynamicMesh = SpriteMeshBuilder::Build(frameAllocator, Vector3(billboardData.m_localCentre, 0.0f), billboardData.m_localSize, billboardData.m_uvs,
                                                                  particle.m_colour, AlignmentAnchor::k_middleCentre);
                auto worldBoundingSphere = Sphere::Transform(renderDynamicMesh->GetBoundingSphere(), worldPosition, worldOrientation, worldScale);
                
                renderSnapshot.AddRenderObject(RenderObject(renderMaterialGroup, renderDynamicMesh.get(), worldMatrix, worldBoundingSphere, false, RenderLayer::k_standard));
                renderSnapshot.AddRenderDynamicMesh(std::move(renderDynamicMesh));
            }
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void StaticBillboardParticleDrawable::DrawWorldSpace(const dynamic_array<ConcurrentParticleData::Particle>& particleData, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) const
    {
        auto renderMaterialGroup = m_billboardDrawableDef->GetMaterial()->GetRenderMaterialGroup();

        //billboard by applying the inverse of the view orientation. The view orientation is the inverse of the camera entity orientation.
        auto inverseView = renderSnapshot.GetRenderCamera().GetOrientation();

        for (u32 i = 0; i < particleData.size(); ++i)
        {
            const auto& particle = particleData[i];

            if (particle.m_isActive == true && particle.m_colour != Colour::k_transparent)
            {
                const auto& billboardData = m_billboards->at(m_particleBillboardIndices[i]);
                
                auto worldPosition = particle.m_position;
                auto worldScale = Vector3(particle.m_scale, 1.0f);
                auto worldOrientation = Quaternion(Vector3::k_unitPositiveZ, particle.m_rotation) * inverseView;  //rotate locally in the XY plane before rotating to face the camera.
                auto worldMatrix = Matrix4::CreateTransform(worldPosition, worldScale, worldOrientation);

                auto renderDynamicMesh = SpriteMeshBuilder::Build(frameAllocator, Vector3(billboardData.m_localCentre, 0.0f), billboardData.m_localSize, billboardData.m_uvs,
                                                                  particle.m_colour, AlignmentAnchor::k_middleCentre);
                auto worldBoundingSphere = Sphere::Transform(renderDynamicMesh->GetBoundingSphere(), worldPosition, worldOrientation, worldScale);
                
                renderSnapshot.AddRenderObject(RenderObject(renderMaterialGroup, renderDynamicMesh.get(), worldMatrix, worldBoundingSphere, false, RenderLayer::k_standard));
                renderSnapshot.AddRenderDynamicMesh(std::move(renderDynamicMesh));
            }
        }
    }
}
