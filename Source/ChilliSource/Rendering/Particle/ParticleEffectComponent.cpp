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
	namespace Rendering
	{
		namespace
		{
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
			/// @param 
			//----------------------------------------------------------------
			std::tuple<Core::AABB, Core::OOBB, Core::Sphere> CalculateBoundingShapes(const ParticleEffect* in_particleEffect, const Core::dynamic_array<Particle>* in_particleArray, 
				const Core::Vector3& in_entityPosition, const Core::Vector3& in_entityScale, const Core::Quaternion& in_entityOrientation)
			{
				//TODO: !? do properly.
				return std::make_tuple(Core::AABB(Core::Vector3::k_zero, Core::Vector3::k_one), Core::OOBB(Core::Vector3::k_zero, Core::Vector3::k_one), Core::Sphere(Core::Vector3::k_zero, 1.0f));
			}
			//----------------------------------------------------------------
			/// Updates the particles on a background thread. This will emit
			/// new particles, update existing particles and apply particle
			/// affectors. These changes will then be committed to the 
			/// draw data array to update the next render.
			///
			/// @author Ian Copland
			///
			/// @param Whether or not this needs to initialise the particles.
			/// @param The particle effect.
			/// @param The particle emitter. If null, the effect is no longer
			/// emitting.
			/// @param The list of particle affectors.
			/// @param The array of particles.
			/// @param The particle draw data array.
			/// @param The playback time.
			/// @param The delta time.
			/// @param The entity's world position.
			/// @param The entity's world scale.
			/// @param The entity's world orientation.
			//----------------------------------------------------------------
			void ParticleUpdateTask(ParticleEffectCSPtr in_particleEffect, ParticleEmitterSPtr in_particleEmitter, std::vector<ParticleAffectorSPtr> in_particleAffectors, 
				std::shared_ptr<Core::dynamic_array<Particle>> in_particleArray, ConcurrentParticleDataSPtr in_concurrentParticleData, f32 in_playbackTime, 
				f32 in_deltaTime, Core::Vector3 in_entityPosition, Core::Vector3 in_entityScale, Core::Quaternion in_entityOrientation)
			{
				CS_ASSERT(in_particleEffect != nullptr, "Cannot update particles with null particle effect.");
				CS_ASSERT(in_particleArray != nullptr, "Cannot update particles with null particle array.");
				CS_ASSERT(in_concurrentParticleData != nullptr, "Cannot update particles with null concurrent particle data.");

				//update the particles
				for (u32 i = 0; i < in_particleArray->size(); ++i)
				{
					Particle& particle = (*in_particleArray)[i];
					if (particle.m_isActive == true)
					{
						particle.m_energy -= in_deltaTime;
						if (particle.m_energy > 0.0f)
						{
							particle.m_position += particle.m_velocity * in_deltaTime;
							particle.m_rotation += particle.m_angularVelocity * in_deltaTime;
						}
						else
						{
							particle.m_energy = 0.0f;
							particle.m_isActive = false;
						}
					}
				}

				//apply affectors
				for (auto& affector : in_particleAffectors)
				{
					affector->AffectParticles(in_playbackTime, in_deltaTime);
				}

				//try to emit
				std::vector<u32> newIndices;
				if (in_particleEmitter != nullptr)
				{
					newIndices = in_particleEmitter->TryEmit(in_playbackTime, in_entityPosition, in_entityScale, in_entityOrientation);
				}

				//Initialise any new particles in each affector.
				for (u32 newIndex : newIndices)
				{
					for (auto& affector : in_particleAffectors)
					{
						affector->ActivateParticle(in_playbackTime, newIndex);
					}
				}

				auto boundingShapes = CalculateBoundingShapes(in_particleEffect.get(), in_particleArray.get(), in_entityPosition, in_entityScale, in_entityOrientation);
				in_concurrentParticleData->CommitParticleData(in_particleArray.get(), newIndices, std::get<0>(boundingShapes), std::get<1>(boundingShapes), std::get<2>(boundingShapes));
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
		bool ParticleEffectComponent::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (RenderComponent::InterfaceID == in_interfaceId || ParticleEffectComponent::InterfaceID == in_interfaceId);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ParticleEffectCSPtr ParticleEffectComponent::GetParticleEffect() const
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only get the Particle Effect on the main thread.");

			return m_particleEffect;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ParticleEffectComponent::PlaybackType ParticleEffectComponent::GetPlaybackType() const
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only get the playback type of a particle effect on the main thread.");

			return m_playbackType;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool ParticleEffectComponent::IsPlaying() const
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only query whether a particle effect is playing on the main thread.");

			return (m_playbackState == PlaybackState::k_starting || m_playbackState == PlaybackState::k_playing || m_playbackState == PlaybackState::k_stopping);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool ParticleEffectComponent::IsEmitting() const
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only query whether a particle effect is emitting on the main thread.");

			return (m_playbackState == PlaybackState::k_starting || m_playbackState == PlaybackState::k_playing);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffectComponent::SetParticleEffect(const ParticleEffectCSPtr& in_particleEffect)
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Particle Effect type must be set on the main thread.");

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
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Play back type must be set on the main thread.");

			m_playbackType = in_playbackType;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffectComponent::Play()
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Play must be called on the main thread.");
			CS_ASSERT((m_playbackState == PlaybackState::k_notPlaying || m_playbackState == PlaybackState::k_stopping), "Cannot play a particle effect when it is already playing.");

			if (m_playbackState == PlaybackState::k_notPlaying)
			{
				m_playbackTimer = 0.0f;
				m_accumulatedDeltaTime = 0.0f;
				m_playbackState = PlaybackState::k_starting;
			}
			else if (m_playbackState == PlaybackState::k_stopping)
			{
				m_playbackTimer = 0.0f;
				m_accumulatedDeltaTime = 0.0f;
				m_playbackState = PlaybackState::k_playing;
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffectComponent::Stop()
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Stop must be called on the main thread.");
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
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Stop Emitting must be called on the main thread.");
			CS_ASSERT((m_playbackState == PlaybackState::k_playing || m_playbackState == PlaybackState::k_starting), "Cannot stop a particle effect emitting when it is not emitting.");

			m_playbackTimer = m_particleEffect->GetDuration();
			m_playbackState = PlaybackState::k_stopping;
			m_finishedEmittingEvent.NotifyConnections(this);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::IConnectableEvent<ParticleEffectComponent::Delegate>& ParticleEffectComponent::GetFinishedEvent()
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only get the Finished Event on the main thread.");

			return m_finishedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::IConnectableEvent<ParticleEffectComponent::Delegate>& ParticleEffectComponent::GetFinishedEmittingEvent()
		{
			CS_ASSERT(Core::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Can only get the Finished Emitting Event on the main thread.");

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

				m_particleArray = std::make_shared<Core::dynamic_array<Particle>>(m_particleEffect->GetMaxParticles());
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

				//TODO: Handle more properly
				mBoundingBox = Core::AABB(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(1.0f, 1.0f, 1.0f));
				mOBBoundingBox = Core::OOBB(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(1.0f, 1.0f, 1.0f));
				mOBBoundingBox.SetTransform(Core::Matrix4::k_identity);
				mBoundingSphere = Core::Sphere(Core::Vector3(0.0f, 0.0f, 0.0f), 1.0f);
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
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffectComponent::OnAddedToEntity()
		{
			PrepareParticleEffect();
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
			m_playbackTimer += in_deltaTime;
			m_accumulatedDeltaTime += in_deltaTime;
			if (m_concurrentParticleData->StartUpdate() == true)
			{
				//intialise the particles by disabling them all.
				for (u32 i = 0; i < m_particleArray->size(); ++i)
				{
					m_particleArray->at(i).m_isActive = false;
				}
				m_concurrentParticleData->CommitParticleData(m_particleArray.get(), std::vector<u32>(), mBoundingBox, mOBBoundingBox, mBoundingSphere);

				Core::Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(ParticleUpdateTask, m_particleEffect, m_emitter, m_affectors, m_particleArray, m_concurrentParticleData,
					m_playbackTimer, m_accumulatedDeltaTime, GetEntity()->GetTransform().GetWorldPosition(), GetEntity()->GetTransform().GetWorldScale(), GetEntity()->GetTransform().GetWorldOrientation()));

				m_accumulatedDeltaTime = 0.0f;

				m_playbackState = PlaybackState::k_playing;
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ParticleEffectComponent::UpdatePlayingState(f32 in_deltaTime)
		{
			m_playbackTimer += in_deltaTime;

			//Update the playback timer. If we're not looping and times up, then stop emitting.
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
				Core::Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(ParticleUpdateTask, m_particleEffect, m_emitter, m_affectors, m_particleArray, m_concurrentParticleData,
					m_playbackTimer, m_accumulatedDeltaTime, GetEntity()->GetTransform().GetWorldPosition(), GetEntity()->GetTransform().GetWorldScale(), GetEntity()->GetTransform().GetWorldOrientation()));

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
					Core::Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(ParticleUpdateTask, m_particleEffect, nullptr, m_affectors, m_particleArray, m_concurrentParticleData,
						m_playbackTimer, m_accumulatedDeltaTime, GetEntity()->GetTransform().GetWorldPosition(), GetEntity()->GetTransform().GetWorldScale(), GetEntity()->GetTransform().GetWorldOrientation()));

					m_accumulatedDeltaTime = 0.0f;
				}
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffectComponent::Render(RenderSystem* in_renderSystem, CameraComponent* in_camera, ShaderPass in_shaderPass)
		{
			if (m_particleEffect != nullptr && (m_playbackState == PlaybackState::k_playing || m_playbackState == PlaybackState::k_stopping))
			{
				CS_ASSERT(m_drawable != nullptr, "Cannot render without a drawable.");

				m_drawable->Draw(in_camera);
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffectComponent::OnRemovedFromScene()
		{
			Stop();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffectComponent::OnRemovedFromEntity()
		{
			CleanupParticleEffect();
		}
	}
}