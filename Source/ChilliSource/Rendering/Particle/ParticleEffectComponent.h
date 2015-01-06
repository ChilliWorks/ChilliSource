//
//  ParticleEffectComponent.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PARTICLEEFFECTCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PARTICLEEFFECTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawable.h>

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// A component for rendering a single instance of a particle effect.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class ParticleEffectComponent final : public RenderComponent
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleEffectComponent);
			//----------------------------------------------------------------
			/// The different possible playback types used to play the particle
			/// effect.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			enum class PlaybackType
			{
				k_once,
				k_looping
			};
			//----------------------------------------------------------------
			/// A delegate used for particle related event such as the effect 
			/// completed event.
			///
			/// @author Ian Copland
			///
			/// @param The particle effect component the event related to.
			//----------------------------------------------------------------
			using Delegate = std::function<void(ParticleEffectComponent*)>;
			//----------------------------------------------------------------
			/// Constructor. 
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			ParticleEffectComponent() = default;
			//----------------------------------------------------------------
			/// Constructor. Creates a particle effect component using the 
			/// parameters described in the given particle effect.
			///
			/// @author Ian Copland
			///
			/// @param The particle effect.
			//----------------------------------------------------------------
			ParticleEffectComponent(const ParticleEffectCSPtr& in_particleEffect);
			//----------------------------------------------------------------
			/// Allows querying of whether or not this implements the interface
			/// described by the given Id.
			///
			/// @author Ian Copland
			///
			/// @param The interface Id.
			///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------
			/// Returns the current particle effect. 
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			/// 
			/// @return The particle effect.
			//----------------------------------------------------------------
			ParticleEffectCSPtr GetParticleEffect() const;
			//----------------------------------------------------------------
			/// Returns the current playback type. 
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			///
			/// @return The playback type.
			//----------------------------------------------------------------
			PlaybackType GetPlaybackType() const;
			//----------------------------------------------------------------
			/// Returns whether or not the particle effect is currently playing.
			/// This will return true if the particle effect is no longer 
			/// emitting but particles are still alive.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			///
			/// @return Whether or not the particle effect is currently playing.
			//----------------------------------------------------------------
			bool IsPlaying() const;
			//----------------------------------------------------------------
			/// Returns whether or not the particle effect is currently 
			/// emitting. 
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			///
			/// @return whether or not the particle effect is currently 
			/// emitting.
			//----------------------------------------------------------------
			bool IsEmitting() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The world space AABB of the effect.
			//----------------------------------------------------------------
			const Core::AABB& GetAABB() override;
			//----------------------------------------------------------------
			/// @author Ian Copland
			/// 
			/// @return The world space OBB of the effect.
			//----------------------------------------------------------------
			const Core::OOBB& GetOOBB() override;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The world space bounding sphere of the effect.
			//----------------------------------------------------------------
			const Core::Sphere& GetBoundingSphere() override;
			//----------------------------------------------------------------
			/// Sets a new particle effect for the component to play. If the
			/// component was playing, it will start playing the new effect
			/// from the beginning. Any existing particles will be removed from
			/// the scene.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			/// 
			/// @author Ian Copland
			///
			/// @param The particle effect that this should playback.
			//----------------------------------------------------------------
			void SetParticleEffect(const ParticleEffectCSPtr& in_particleEffect);
			//----------------------------------------------------------------
			/// Sets the playback mode that should be used when playing this 
			/// particle effect.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			///
			/// @param The playback type.
			//----------------------------------------------------------------
			void SetPlaybackType(PlaybackType in_playbackType);
			//----------------------------------------------------------------
			/// Starts the particle effect playing.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void Play();
			//----------------------------------------------------------------
			/// Stops the particle effect playing and removes all existing 
			/// particles.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void Stop();
			//----------------------------------------------------------------
			/// Stops the particle effect emitting. Existing particles will 
			/// continue their life cycle. The stopped event will be sent when 
			/// all remaining particles are inactive.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void StopEmitting();
			//----------------------------------------------------------------
			/// Returns an event that will be fired when a particle effect
			/// fully completes, i.e the effect has finished emitting and 
			/// there are no particles left alive.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			///
			/// @return The event.
			//----------------------------------------------------------------
			Core::IConnectableEvent<Delegate>& GetFinishedEvent();
			//----------------------------------------------------------------
			/// Returns an event that will be fired when the particle effect
			/// finishes emitting. Particles may still be alive when this event
			/// is called.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			///
			/// @return The event.
			//----------------------------------------------------------------
			Core::IConnectableEvent<Delegate>& GetFinishedEmittingEvent();
			//----------------------------------------------------------------
			/// Destructor
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			~ParticleEffectComponent() {};
		private:
			//----------------------------------------------------------------
			/// An enum describing the current playback state of the particle
			/// effect.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			enum class PlaybackState
			{
				k_notPlaying,
				k_starting,
				k_playing,
				k_stopping
			};
			//----------------------------------------------------------------
			/// Sets up the particle effect, ready to be played.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void PrepareParticleEffect();
			//----------------------------------------------------------------
			/// Cleans up the particle effect, deleting all particles.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void CleanupParticleEffect();
			//----------------------------------------------------------------
			/// Takes the calculated local bounds of the particle effect and
			/// stores them for later calculating the world bounds.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void StoreLocalBoundingShapes();
			//----------------------------------------------------------------
			/// Re-calculates the world space bounding shapes if the bounding 
			/// shape cache has been invalidated.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void UpdateWorldBoundingShapes();
			//----------------------------------------------------------------
			/// Called when this is added to an entity.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void OnAddedToEntity() override;
			//----------------------------------------------------------------
			/// Called either when this is added to an entity that is already 
			/// in the scene, or when the entity this is already added to is 
			/// added to the scene. This will start the effect playing.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void OnAddedToScene() override;
			//----------------------------------------------------------------
			/// Called every frame the particle effect is in the scene. This
			/// will update depending on the current playback state of the
			/// effect.
			///
			/// @author Ian Copland
			///
			/// @param The delta time.
			//----------------------------------------------------------------
			void OnUpdate(f32 in_deltaTime) override;
			//----------------------------------------------------------------
			/// Updates while the particle effect is starting. This will wait
			/// until an appropriate time to send a background task to initialise 
			/// the particles and start updating.
			///
			/// @author Ian Copland
			///
			/// @param The delta time.
			//----------------------------------------------------------------
			void UpdateStartingState(f32 in_deltaTime);
			//----------------------------------------------------------------
			/// This will fire off a new particle update background task.
			///
			/// @author Ian Copland
			///
			/// @param The delta time.
			//----------------------------------------------------------------
			void UpdatePlayingState(f32 in_deltaTime);
			//----------------------------------------------------------------
			/// This waits for all activate particles effects to finish then
			/// will end the effect.
			///
			/// @author Ian Copland
			///
			/// @param The delta time.
			//----------------------------------------------------------------
			void UpdateStoppingState(f32 in_deltaTime);
			//----------------------------------------------------------------
			/// Called when the component should render all particles.
			///
			/// @author Ian Copland
			///
			/// @param The render system.
			/// @param The active camera component.
			/// @param The current shader pass.
			//----------------------------------------------------------------
			void Render(RenderSystem* in_renderSystem, CameraComponent* in_camera, ShaderPass in_shaderPass) override;
			//----------------------------------------------------------------
			/// Called when the component should render to the shadow map. 
			/// Particles will never render to the shadow map so this does 
			/// nothing.
			///
			/// @author Ian Copland
			///
			/// @param The render system.
			/// @param The active camera component.
			/// @param The material to render static shadows with.
			/// @param The material to render skinned shadows with.
			//----------------------------------------------------------------
			void RenderShadowMap(RenderSystem* in_renderSystem, CameraComponent* in_camera, const MaterialCSPtr& in_staticShadowMat, const MaterialCSPtr& in_animShadowMat) override {};
			//----------------------------------------------------------------
			/// Called when the entities transform changes. This invalidates
			/// the bounding shape cache.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void OnEntityTransformChanged();
			//----------------------------------------------------------------
			/// Called either when this is removed from an entity that is 
			/// attached to the scene or when an entity this is attached to is 
			/// removed from the scene. This will stop the particle effect 
			/// playing and reset all active particles.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void OnRemovedFromScene() override;
			//----------------------------------------------------------------
			/// Called when this is removed from an entity.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void OnRemovedFromEntity() override;

			ParticleEffectCSPtr m_particleEffect;
			ParticleDrawableUPtr m_drawable;
			ParticleEmitterSPtr m_emitter;
			std::vector<ParticleAffectorSPtr> m_affectors;
			std::shared_ptr<Core::dynamic_array<Particle>> m_particleArray;
			ConcurrentParticleDataSPtr m_concurrentParticleData;

			PlaybackType m_playbackType = PlaybackType::k_once;
			PlaybackState m_playbackState = PlaybackState::k_notPlaying;
			f32 m_playbackTimer = 0.0f;
			f32 m_accumulatedDeltaTime = 0.0f;
			Core::Event<Delegate> m_finishedEvent;
			Core::Event<Delegate> m_finishedEmittingEvent;
			bool m_firstFrame = false;

			Core::AABB m_localAABB;
			Core::Sphere m_localBoundingSphere;
			bool m_invalidateBoundingShapeCache = true;
			Core::EventConnectionUPtr m_entityTransformConnection;
		};
	}
}

#endif