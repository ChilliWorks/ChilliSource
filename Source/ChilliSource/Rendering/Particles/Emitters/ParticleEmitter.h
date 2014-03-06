//
//  ParticleEmitter.h
//  moFloTest
//
//  Created by Scott Downie on 07/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_PARTICLES_PARTICLE_EMITTER_H_
#define _MO_FLO_RENDERING_PARTICLES_PARTICLE_EMITTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
    namespace Rendering
    {
        struct Particle
        {
            Core::Vector3* vScale;
            Core::Vector3* vTranslation;
			Core::Colour* Col;				//The colour of the particle
            Core::Vector3* vVelocity;		//The speed of the particle in 3D space
            f32* fEnergy;					//Measures the full lifetime of the particle from birth at 1 to death at 0
            f32* fAngularRotation;
        };
        
        class ParticleEmitter
        {
        public:
            
            ParticleEmitter(const Core::ParamDictionary& inParams, const MaterialSPtr &inpMaterial, ParticleComponent* inpComponent);
            virtual ~ParticleEmitter();
            //-----------------------------------------------------
            /// Update
            ///
            /// Update all the active particles with respect to
            /// time
            ///
            /// @param Time between frames
            //-----------------------------------------------------
            void Update(f32 infDT);
            //-----------------------------------------------------
            /// Render
            ///
            /// Render the active particles based on their
            /// renderable type
            ///
            /// @param Render system
            //-----------------------------------------------------
            void Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam);
            //---------------------------------------------------
            /// Start Emitting
            ///
            /// Particle emitters will resume emitting
            //---------------------------------------------------
            void StartEmitting();
            //---------------------------------------------------
            /// Emit Burst
            ///
            /// Particle emitters will fire particles once
            //---------------------------------------------------
            void EmitBurst();
            //---------------------------------------------------
            /// Stop Emitting
            ///
            /// No new particles will be emitted once the current
            /// ones die
            //---------------------------------------------------
            void StopEmitting();
            //-----------------------------------------------------
            /// Add Effector
            ///
            /// Add a new effector to apply to the particles
            ///
            /// @param Particle effector
            //-----------------------------------------------------
            void AddEffector(ParticleEffector* inpEffector);
            //-----------------------------------------------------
            /// Remove Effector
            ///
            /// Remove the effector so it no longer applies to
            /// the system
            ///
            /// @param Particle effector
            //-----------------------------------------------------
            void RemoveEffector(ParticleEffector* inpEffector);
            //-----------------------------------------------------
            /// Set Velocity
            ///
            /// Set the initial velocity of the particles leaving
            /// the emitter
            ///
            /// @param Velocity
            //-----------------------------------------------------
            void SetVelocity(f32 infVelocity);
            //-----------------------------------------------------
            /// Set Colour
            ///
            /// Set the initial colour of the particles
            ///
            /// @param Colour
            //-----------------------------------------------------
            void SetColour(const Core::Colour inColour);
            //-----------------------------------------------------
            /// Set Number of Particles Per Emission
            ///
            /// Sets the number of particles that are emitted
            /// at any given time. This is a maximum if there are
            /// no particles available no new ones will be created
            ///
            /// @param Number of particles
            //-----------------------------------------------------
            void SetNumParticlesPerEmission(u32 inudwNumParticles);
            //-----------------------------------------------------
            /// Set Material
            ///
            /// The material that each particle will use
            ///
            /// @param Material ptr
            //-----------------------------------------------------
            void SetMaterial(const MaterialSPtr& inpMaterial);
            //-----------------------------------------------------
            /// Set Emission Frequency
            ///
            /// After what period in seconds should particles be
            /// emitted
            ///
            /// @param Frequency in seconds
            //-----------------------------------------------------
            void SetEmissionFrequency(f32 infFreq);
            //-----------------------------------------------------
            /// Set Lifetime
            ///
            /// Set the lifetime of the particles in second
            ///
            /// @param Lifetime in seconds
            //-----------------------------------------------------
            void SetLifetime(f32 infLifetime);
            //-----------------------------------------------------
            /// Set Looping
            ///
            /// Set whether the emitter is continuous
            ///
            /// @param Toggle on/off
            //-----------------------------------------------------
            void SetLooping(bool inbLooping);
			//-------------------------------------------------
            /// Sets the UVs used for a particle
            //-------------------------------------------------
            void SetParticleUVs(const Core::Rectangle & insUVs);

			//-------------------------------------------------
            /// Has this emitter finished emitting its particles? (EmitOnce only!)
            //-------------------------------------------------
            bool GetIsEmittingFinished() { return mbIsEmittingFinished; }
            
        private:
            //-----------------------------------------------------
            /// Update Particle
            ///
            /// Update an indivdual particle with the effectors
            ///
            /// @param Particle array index
            /// @param DT
            //-----------------------------------------------------
            void UpdateParticle(u32 inudwParticleIndex, f32 infDT);
            //-----------------------------------------------------
            /// Emit
            ///
            /// Emit some new particles by pulling from the dead
            /// pool. Emission will depend on the number of
            /// particles emitted and the shape of the emitter
            ///
			/// @param Particles
			/// @param Index of current particle
            //-----------------------------------------------------
            virtual void Emit(Particle* inpParticles, u32 udwParticleIndex) = 0;
			
		protected:
			
			//-----------------------------------------------------
			/// Update Sprite Data
			///
			/// Rebuild the sprite data
			//-----------------------------------------------------
			void UpdateSpriteData(const Core::Vector3& invPos, const Core::Colour & insTintColour, SpriteComponent::SpriteData& outsData,
                                  const Core::Vector3& invRight, const Core::Vector3& invUp, const Core::Vector3& invScale);
            
        protected:
            
            Particle mParticles;
            std::vector<ParticleEffector*> mEffectors;
            
            f32 mfInitialVelocity;
            f32 mfMinInitialVelocity;
            
            Core::Colour mInitialColour;
            Core::Vector3 mvLastEmissionPos;
            
			Core::Rectangle msParticleUVs;
			
            MaterialSPtr mpMaterial;
            
            u32 mudwMaxNumParticles;
            u32 mudwMaxNumParticlesPerEmission;
            u32 mudwNumUsed;
            
            f32 mfCurrentTime;
            f32 mfLastEmissionTime;
            f32 mfEmissionFreq;
            f32 mfTimeToLive;
            f32 mfEnergyLoss;
            Core::Vector2 mvInitialScale;
            
            bool mbShouldLoop;
            bool mbIsEmitting;
            u32 mudwBurstCounter;
            bool mbIsEmittingFinished;
            bool mbIsGlobalSpace;
            
            ParticleComponent* mpOwningComponent;
        };
    }
}

#endif