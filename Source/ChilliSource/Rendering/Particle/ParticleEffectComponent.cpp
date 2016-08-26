//
//  ParticleEffectComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 06/10/2014.
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

#include <ChilliSource/Rendering/Particle/ParticleEffectComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/dynamic_array.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/State/State.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Camera/PerspectiveCameraComponent.h>
#include <ChilliSource/Rendering/Particle/ConcurrentParticleData.h>
#include <ChilliSource/Rendering/Particle/Particle.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffector.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDef.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawable.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDef.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitter.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDef.h>

#include <limits>
#include <tuple>

namespace ChilliSource
{
    namespace
    {
        //----------------------------------------------------------------
        /// A container for all information required by the background
        /// particle update.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------------
        struct ParticleUpdateDesc final
        {
            ParticleEffectCSPtr m_particleEffect;
            ParticleEmitterSPtr m_particleEmitter;
            std::vector<ParticleAffectorSPtr> m_particleAffectors;
            std::shared_ptr<dynamic_array<Particle>> m_particleArray;
            ConcurrentParticleDataSPtr m_concurrentParticleData;
            f32 m_playbackTime = 0.0f;
            f32 m_deltaTime = 0.0f; 
            Vector3 m_entityPosition;
            Vector3 m_entityScale;
            Quaternion m_entityOrientation;
            bool m_interpolateEmission = false;
        };
        //----------------------------------------------------------------
        /// Performs a series of assertions to ensure the given particle 
        /// effect is ready for use.
        ///
        /// @author Ian Copland
        ///
        /// @param The particle effect.
        //----------------------------------------------------------------
        void ValidateParticleEffect(const ParticleEffectCSPtr& in_particleEffect)
        {
            CS_ASSERT(in_particleEffect->GetDrawableDef() != nullptr, "Trying to use incomplete particle effect: Drawable missing.");
            CS_ASSERT(in_particleEffect->GetEmitterDef() != nullptr, "Trying to use incomplete particle effect: Emitter missing.");
            CS_ASSERT(in_particleEffect->GetLifetimeProperty() != nullptr, "Trying to use incomplete particle effect: Lifetime property missing.");
            CS_ASSERT(in_particleEffect->GetInitialScaleProperty() != nullptr, "Trying to use incomplete particle effect: Initial scale property missing.");
            CS_ASSERT(in_particleEffect->GetInitialRotationProperty() != nullptr, "Trying to use incomplete particle effect: Initial rotation property missing.");
            CS_ASSERT(in_particleEffect->GetInitialColourProperty() != nullptr, "Trying to use incomplete particle effect: Initial colour property missing.");
            CS_ASSERT(in_particleEffect->GetInitialSpeedProperty() != nullptr, "Trying to use incomplete particle effect: Initial speed property missing.");
            CS_ASSERT(in_particleEffect->GetInitialAngularVelocityProperty() != nullptr, "Trying to use incomplete particle effect: Initial angular velocity property missing.");
        }
        //----------------------------------------------------------------
        /// Calculates the bounding shapes for the given set of particles.
        ///
        /// @author Ian Copland
        ///
        /// @param The particle effect.
        /// @param The array of particles.
        /// 
        /// @return a pair containing the AABB and the Bounding Sphere.
        //----------------------------------------------------------------
        std::pair<AABB, Sphere> CalculateBoundingShapes(const ParticleEffect* in_particleEffect, const dynamic_array<Particle>* in_particleArray)
        {
            Vector3 min = Vector3(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
            Vector3 max = Vector3(-std::numeric_limits<f32>::max(), -std::numeric_limits<f32>::max(), -std::numeric_limits<f32>::max());

            bool anyActive = false;
            for (const auto& particle : *in_particleArray)
            {
                if (particle.m_isActive == true)
                {
                    anyActive = true;

                    if (particle.m_position.x < min.x)
                        min.x = particle.m_position.x;
                    if (particle.m_position.y < min.y)
                        min.y = particle.m_position.y;
                    if (particle.m_position.z < min.z)
                        min.z = particle.m_position.z;

                    if (particle.m_position.x > max.x)
                        max.x = particle.m_position.x;
                    if (particle.m_position.y > max.y)
                        max.y = particle.m_position.y;
                    if (particle.m_position.z > max.z)
                        max.z = particle.m_position.z;
                }
            }

            if (anyActive == false)
            {
                min = Vector3::k_zero;
                max = Vector3::k_zero;
            }

            Vector3 size = max - min;
            Vector3 centre = min + 0.5f * size;

            return std::make_pair(AABB(centre, size), Sphere(centre, size.Length() * 0.5f));
        }
        //----------------------------------------------------------------
        /// Updates the particles on a background thread. This will emit
        /// new particles, update existing particles and apply particle
        /// affectors. These changes will then be committed to the 
        /// draw data array to update the next render.
        ///
        /// @author Ian Copland
        ///
        /// @param in_desc - The particle update description. This contains
        /// a snapshot of all data required to update the particle effect.
        //----------------------------------------------------------------
        void ParticleUpdateTask(const ParticleUpdateDesc& in_desc)
        {
            CS_ASSERT(in_desc.m_particleEffect != nullptr, "Cannot update particles with null particle effect.");
            CS_ASSERT(in_desc.m_particleArray != nullptr, "Cannot update particles with null particle array.");
            CS_ASSERT(in_desc.m_concurrentParticleData != nullptr, "Cannot update particles with null concurrent particle data.");

            //update the particles
            for (auto& particle : *in_desc.m_particleArray)
            {
                if (particle.m_isActive == true)
                {
                    particle.m_energy -= in_desc.m_deltaTime;
                    if (particle.m_energy > 0.0f)
                    {
                        particle.m_position += particle.m_velocity * in_desc.m_deltaTime;
                        particle.m_rotation += particle.m_angularVelocity * in_desc.m_deltaTime;
                    }
                    else
                    {
                        particle.m_energy = 0.0f;
                        particle.m_isActive = false;
                    }
                }
            }

            //calculate the normalised playback progress.
            const f32 effectProgress = in_desc.m_playbackTime / in_desc.m_particleEffect->GetDuration();
            
            //apply affectors
            for (auto& affector : in_desc.m_particleAffectors)
            {
                affector->AffectParticles(in_desc.m_deltaTime, effectProgress);
            }

            //try to emit
            std::vector<u32> newIndices;
            if (in_desc.m_particleEmitter != nullptr)
            {
                newIndices = in_desc.m_particleEmitter->TryEmit(in_desc.m_playbackTime, in_desc.m_entityPosition, in_desc.m_entityScale, in_desc.m_entityOrientation, in_desc.m_interpolateEmission);
            }

            //Initialise any new particles in each affector.
            for (u32 newIndex : newIndices)
            {
                for (auto& affector : in_desc.m_particleAffectors)
                {
                    affector->ActivateParticle(newIndex, effectProgress);
                }
            }

            auto boundingShapes = CalculateBoundingShapes(in_desc.m_particleEffect.get(), in_desc.m_particleArray.get());
            in_desc.m_concurrentParticleData->CommitParticleData(in_desc.m_particleArray.get(), newIndices, boundingShapes.first, boundingShapes.second);
        }
    }
    CS_DEFINE_NAMEDTYPE(ParticleEffectComponent);
    //-------------------------------------------------------
    //-------------------------------------------------------
    ParticleEffectComponent::ParticleEffectComponent(const ParticleEffectCSPtr& in_particleEffect)
    {
        SetParticleEffect(in_particleEffect);
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    bool ParticleEffectComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (VolumeComponent::InterfaceID == in_interfaceId || ParticleEffectComponent::InterfaceID == in_interfaceId);
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    ParticleEffectCSPtr ParticleEffectComponent::GetParticleEffect() const
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only get the Particle Effect on the main thread.");

        return m_particleEffect;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    ParticleEffectComponent::PlaybackType ParticleEffectComponent::GetPlaybackType() const
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only get the playback type of a particle effect on the main thread.");

        return m_playbackType;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    bool ParticleEffectComponent::IsPlaying() const
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only query whether a particle effect is playing on the main thread.");

        return (m_playbackState == PlaybackState::k_starting || m_playbackState == PlaybackState::k_playing || m_playbackState == PlaybackState::k_stopping);
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    bool ParticleEffectComponent::IsEmitting() const
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only query whether a particle effect is emitting on the main thread.");

        return (m_playbackState == PlaybackState::k_starting || m_playbackState == PlaybackState::k_playing);
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    const AABB& ParticleEffectComponent::GetAABB()
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only query for particle effect bounding shapes on the main thread.");

        UpdateWorldBoundingShapes();
        return mBoundingBox;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    const OOBB& ParticleEffectComponent::GetOOBB()
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only query for particle effect bounding shapes on the main thread.");

        UpdateWorldBoundingShapes();
        return mOBBoundingBox;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    const Sphere& ParticleEffectComponent::GetBoundingSphere()
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only query for particle effect bounding shapes on the main thread.");

        UpdateWorldBoundingShapes();
        return mBoundingSphere;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::SetParticleEffect(const ParticleEffectCSPtr& in_particleEffect)
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Particle Effect type must be set on the main thread.");

        m_particleEffect = in_particleEffect;

        if (GetEntity() != nullptr)
        {
            PrepareParticleEffect();
        }

        m_playbackTimer = 0.0f;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::SetPlaybackType(PlaybackType in_playbackType)
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Play back type must be set on the main thread.");

        m_playbackType = in_playbackType;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::Play()
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Play must be called on the main thread.");
        CS_ASSERT((m_playbackState == PlaybackState::k_notPlaying || m_playbackState == PlaybackState::k_stopping), "Cannot play a particle effect when it is already playing.");

        if (m_playbackState == PlaybackState::k_notPlaying)
        {

            m_playbackState = PlaybackState::k_starting;
        }
        else if (m_playbackState == PlaybackState::k_stopping)
        {
            m_playbackState = PlaybackState::k_playing;
        }

        m_playbackTimer = 0.0f;
        m_accumulatedDeltaTime = 0.0f;
        m_firstFrame = true;

        //reset the bounding shapes.
        m_localAABB = AABB();
        m_localBoundingSphere = Sphere();
        m_invalidateBoundingShapeCache = true;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::Stop()
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Stop must be called on the main thread.");
        CS_ASSERT((m_playbackState == PlaybackState::k_playing || m_playbackState == PlaybackState::k_starting || m_playbackState == PlaybackState::k_stopping), "Cannot stop a particle effect when it is already stopped.");

        if (m_playbackState == PlaybackState::k_starting || m_playbackState == PlaybackState::k_playing)
        {
            StopEmitting();
        }
        m_playbackState = PlaybackState::k_notPlaying;
        m_finishedEvent.NotifyConnections(this);
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::StopEmitting()
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Stop Emitting must be called on the main thread.");
        CS_ASSERT((m_playbackState == PlaybackState::k_playing || m_playbackState == PlaybackState::k_starting), "Cannot stop a particle effect emitting when it is not emitting.");

        m_playbackTimer = m_particleEffect->GetDuration();
        m_playbackState = PlaybackState::k_stopping;
        m_finishedEmittingEvent.NotifyConnections(this);
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    IConnectableEvent<ParticleEffectComponent::Delegate>& ParticleEffectComponent::GetFinishedEvent()
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only get the Finished Event on the main thread.");

        return m_finishedEvent;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    IConnectableEvent<ParticleEffectComponent::Delegate>& ParticleEffectComponent::GetFinishedEmittingEvent()
    {
        CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only get the Finished Emitting Event on the main thread.");

        return m_finishedEmittingEvent;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::PrepareParticleEffect()
    {
        CleanupParticleEffect();

        if (m_particleEffect != nullptr)
        {
            ValidateParticleEffect(m_particleEffect);

            m_particleArray = std::make_shared<dynamic_array<Particle>>(m_particleEffect->GetMaxParticles());
            m_concurrentParticleData = std::make_shared<ConcurrentParticleData>(m_particleEffect->GetMaxParticles());

            m_drawable = m_particleEffect->GetDrawableDef()->CreateInstance(GetEntity(), m_concurrentParticleData.get());
            CS_ASSERT(m_drawable != nullptr, "Failed to create particle drawable.");

            m_emitter = m_particleEffect->GetEmitterDef()->CreateInstance(m_particleArray.get());
            CS_ASSERT(m_emitter != nullptr, "Failed to create particle emitter.");

            const std::vector<const ParticleAffectorDef*> affectorDefs = m_particleEffect->GetAffectorDefs();
            for (const auto& affectorDef : affectorDefs)
            {
                ParticleAffectorSPtr affector = affectorDef->CreateInstance(m_particleArray.get());
                CS_ASSERT(affector != nullptr, "Failed to create particle emitter.");

                m_affectors.push_back(affector);
            }

            mpMaterial = m_particleEffect->GetDrawableDef()->GetMaterial();

            //reset the bounding shapes.
            m_localAABB = AABB();
            m_localBoundingSphere = Sphere();
            m_invalidateBoundingShapeCache = true;
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::CleanupParticleEffect()
    {
        m_particleArray.reset();
        m_concurrentParticleData.reset();
        m_drawable.reset();
        m_emitter.reset();
        m_affectors.clear();
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ParticleEffectComponent::StoreLocalBoundingShapes()
    {
        CS_ASSERT((m_playbackState == PlaybackState::k_playing || m_playbackState == PlaybackState::k_stopping), "Can only store local bounding shapes when playing or stopping.");

        m_localAABB = m_concurrentParticleData->GetAABB();
        m_localBoundingSphere = m_concurrentParticleData->GetBoundingSphere();
        m_invalidateBoundingShapeCache = true;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ParticleEffectComponent::UpdateWorldBoundingShapes()
    {
        CS_ASSERT(GetEntity() != nullptr, "Cannot get world bounding shapes without being attached to an entity.");

        if (m_invalidateBoundingShapeCache == true)
        {
            if (m_particleEffect != nullptr && m_particleEffect->GetSimulationSpace() == ParticleEffect::SimulationSpace::k_world)
            {
                mBoundingBox = m_localAABB;
                mOBBoundingBox = OOBB(m_localAABB.GetOrigin(), m_localAABB.GetSize());
                mOBBoundingBox.SetTransform(Matrix4::k_identity);
                mBoundingSphere = m_localBoundingSphere;
            }
            else
            {
                const Matrix4& worldMatrix = GetEntity()->GetTransform().GetWorldTransform();

                //OBB is the AABB with the entity transform.
                mOBBoundingBox = OOBB(m_localAABB.GetOrigin(), m_localAABB.GetSize());
                mOBBoundingBox.SetTransform(worldMatrix);

                //transform the 8 points of the AABB into world space and recalculate.
                std::vector<Vector3> points;
                points.push_back(m_localAABB.BackBottomLeft() * worldMatrix);
                points.push_back(m_localAABB.BackBottomRight() * worldMatrix);
                points.push_back(m_localAABB.BackTopLeft() * worldMatrix);
                points.push_back(m_localAABB.BackTopRight() * worldMatrix);
                points.push_back(m_localAABB.FrontBottomLeft() * worldMatrix);
                points.push_back(m_localAABB.FrontBottomRight() * worldMatrix);
                points.push_back(m_localAABB.FrontTopLeft() * worldMatrix);
                points.push_back(m_localAABB.FrontTopRight() * worldMatrix);

                Vector3 min = Vector3(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
                Vector3 max = Vector3(-std::numeric_limits<f32>::max(), -std::numeric_limits<f32>::max(), -std::numeric_limits<f32>::max());
                for (auto& point : points)
                {
                    if (point.x < min.x)
                        min.x = point.x;
                    if (point.y < min.y)
                        min.y = point.y;
                    if (point.z < min.z)
                        min.z = point.z;

                    if (point.x > max.x)
                        max.x = point.x;
                    if (point.y > max.y)
                        max.y = point.y;
                    if (point.z > max.z)
                        max.z = point.z;
                }

                Vector3 size = max - min;
                Vector3 centre = min + 0.5f * size;

                mBoundingBox = AABB(centre, size);

                //bounding sphere encapsulates the AABB.
                mBoundingSphere = Sphere(centre, size.Length() * 0.5f);
            }

            m_invalidateBoundingShapeCache = false;
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::OnAddedToEntity()
    {
        PrepareParticleEffect();

        m_entityTransformConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(MakeDelegate(this, &ParticleEffectComponent::OnEntityTransformChanged));
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::OnAddedToScene()
    {
        Play();
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::OnUpdate(f32 in_deltaTime)
    {
        if (m_particleEffect != nullptr)
        {
            switch (m_playbackState)
            {
            case PlaybackState::k_notPlaying:
                //Do nothing.
                break;
            case PlaybackState::k_starting:
                UpdateStartingState(in_deltaTime);
                break;
            case PlaybackState::k_playing:
                UpdatePlayingState(in_deltaTime);
                break;
            case PlaybackState::k_stopping:
                UpdateStoppingState(in_deltaTime);
                break;
            default:
                CS_LOG_FATAL("Invalid playback state.");
                break;
            }
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ParticleEffectComponent::UpdateStartingState(f32 in_deltaTime)
    {
        if (m_concurrentParticleData->StartUpdate() == true)
        {
            //intialise the particles by disabling them all.
            for (auto& particle : *m_particleArray)
            {
                particle.m_isActive = false;
            }
            m_concurrentParticleData->CommitParticleData(m_particleArray.get(), std::vector<u32>(), AABB(), Sphere());

            m_playbackState = PlaybackState::k_playing;
            UpdatePlayingState(in_deltaTime);
        }
        else
        {
            m_playbackTimer += in_deltaTime;
            m_accumulatedDeltaTime += in_deltaTime;
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ParticleEffectComponent::UpdatePlayingState(f32 in_deltaTime)
    {
        m_playbackTimer += in_deltaTime;

        //Update the playback timer. If we're not looping and time's up, then stop emitting.
        if (m_playbackType == PlaybackType::k_once && m_playbackTimer >= m_particleEffect->GetDuration())
        {
            StopEmitting();
            UpdateStoppingState(in_deltaTime);
            return;
        }
        else
        {
            while (m_playbackTimer >= m_particleEffect->GetDuration())
            {
                m_playbackTimer -= m_particleEffect->GetDuration();
            }
        }

        m_accumulatedDeltaTime += in_deltaTime;
        if (m_concurrentParticleData->StartUpdate() == true)
        {
            StoreLocalBoundingShapes();

            ParticleUpdateDesc desc;
            desc.m_particleEffect = m_particleEffect;
            desc.m_particleEmitter = m_emitter;
            desc.m_particleAffectors = m_affectors;
            desc.m_particleArray = m_particleArray;
            desc.m_concurrentParticleData = m_concurrentParticleData;
            desc.m_playbackTime = m_playbackTimer;
            desc.m_deltaTime = m_accumulatedDeltaTime;
            desc.m_entityPosition = GetEntity()->GetTransform().GetWorldPosition();
            desc.m_entityScale = GetEntity()->GetTransform().GetWorldScale();
            desc.m_entityOrientation = GetEntity()->GetTransform().GetWorldOrientation();
            desc.m_interpolateEmission = (m_firstFrame == false);
            Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_small, [=](const TaskContext&) noexcept
            {
                ParticleUpdateTask(desc);
            });

            m_firstFrame = false;
            m_accumulatedDeltaTime = 0.0f;
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ParticleEffectComponent::UpdateStoppingState(f32 in_deltaTime)
    {
        if (m_concurrentParticleData->HasActiveParticles() == false)
        {
            Stop();
        }
        else
        {
            m_accumulatedDeltaTime += in_deltaTime;
            if (m_concurrentParticleData->StartUpdate() == true)
            {
                StoreLocalBoundingShapes();

                ParticleUpdateDesc desc;
                desc.m_particleEffect = m_particleEffect;
                desc.m_particleEmitter = nullptr;
                desc.m_particleAffectors = m_affectors;
                desc.m_particleArray = m_particleArray;
                desc.m_concurrentParticleData = m_concurrentParticleData;
                desc.m_playbackTime = m_playbackTimer;
                desc.m_deltaTime = m_accumulatedDeltaTime;
                desc.m_entityPosition = GetEntity()->GetTransform().GetWorldPosition();
                desc.m_entityScale = GetEntity()->GetTransform().GetWorldScale();
                desc.m_entityOrientation = GetEntity()->GetTransform().GetWorldOrientation();
                desc.m_interpolateEmission = (m_firstFrame == false);
                Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_small, [=](const TaskContext&) noexcept
                {
                    ParticleUpdateTask(desc);
                });

                m_firstFrame = false;
                m_accumulatedDeltaTime = 0.0f;
            }
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ParticleEffectComponent::OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        if (m_particleEffect != nullptr && (m_playbackState == PlaybackState::k_playing || m_playbackState == PlaybackState::k_stopping))
        {
            CS_ASSERT(m_drawable != nullptr, "Cannot render without a drawable.");
            
            m_drawable->Draw(renderSnapshot, frameAllocator);
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    void ParticleEffectComponent::OnEntityTransformChanged()
    {
        m_invalidateBoundingShapeCache = true;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::OnRemovedFromScene()
    {
        if (IsPlaying() == true)
        {
            Stop();
        }
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void ParticleEffectComponent::OnRemovedFromEntity()
    {
        m_entityTransformConnection.reset();

        CleanupParticleEffect();
    }
}