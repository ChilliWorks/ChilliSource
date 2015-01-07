//
//  ParticleEffect.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PARTICLEEFFECT_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PARTICLEEFFECT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDef.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDef.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDef.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/ConstantParticleProperty.h>

#include <vector>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// A Particle Effect contains all the properties and definitions for the
		/// drawable, emitter and effectors of a particle effect. This can be used
		/// in conjunction with a Particle Effect Component to display an instance
		/// of the effect in the scene.
		///
		/// A particle effects properties and definitions should not be changed 
		/// while it is in use. Particle Effect Components update in a background
		/// task and rely on Particle Effect being immutable for lockless thread
		/// safety.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class ParticleEffect final : public Core::Resource
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleEffect);
			//----------------------------------------------------------------
			/// An enum describing the possible simulation space values, i.e 
			/// local or world space.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			enum class SimulationSpace
			{
				k_local,
				k_world
			};
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
			/// @author Ian Copland
			///
			/// @return The duration of the particle effect.
			//----------------------------------------------------------------
			f32 GetDuration() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The max number of particles in the effect.
			//----------------------------------------------------------------
			u32 GetMaxParticles() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The space in which the particle effect is simulated.
			//----------------------------------------------------------------
			SimulationSpace GetSimulationSpace() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The property used to generate the lifetime of a new 
			/// particle.
			//----------------------------------------------------------------
			const ParticleProperty<f32>* GetLifetimeProperty() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The property used to generate the initial scale of a 
			/// new particle.
			//----------------------------------------------------------------
			const ParticleProperty<Core::Vector2>* GetInitialScaleProperty() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The property used to generate the initial rotation of 
			/// a new particle.
			//----------------------------------------------------------------
			const ParticleProperty<f32>* GetInitialRotationProperty() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The property used to generate the initial colour of 
			/// a new particle.
			//----------------------------------------------------------------
			const ParticleProperty<Core::Colour>* GetInitialColourProperty() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The property used to generate the initial speed of a 
			/// new particle. The initial direction of motion is described by 
			/// the emitter.
			//----------------------------------------------------------------
			const ParticleProperty<f32>* GetInitialSpeedProperty() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The property used to generate the initial angular 
			/// velocity of a new particle.
			//----------------------------------------------------------------
			const ParticleProperty<f32>* GetInitialAngularVelocityProperty() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return This particle effect's drawable definition.
			//----------------------------------------------------------------
			const ParticleDrawableDef* GetDrawableDef() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The particle emitter definition for this effect.
			//----------------------------------------------------------------
			const ParticleEmitterDef* GetEmitterDef() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The list of affector definitions for this effect.
			//----------------------------------------------------------------
			std::vector<const ParticleAffectorDef*> GetAffectorDefs() const;
			//----------------------------------------------------------------
			/// Sets the duration of the particle effect. 
			///
			/// @author Ian Copland
			///
			/// @param The duration.
			//----------------------------------------------------------------
			void SetDuration(f32 in_duration);
			//----------------------------------------------------------------
			/// Sets the maximum number of particles in the effect.
			///
			/// @author Ian Copland
			///
			/// @param The max number of particles.
			//----------------------------------------------------------------
			void SetMaxParticles(u32 in_maxParticles);
			//----------------------------------------------------------------
			/// Sets the simulation space of the particle effect.
			///
			/// @author Ian Copland
			///
			/// @param The simulation space.
			//----------------------------------------------------------------
			void SetSimulationSpace(SimulationSpace in_simulationSpace);
			//----------------------------------------------------------------
			/// Sets the property used to generate the lifetime of a new 
			/// particle.
			///
			/// @author Ian Copland
			///
			/// @param The propery. 
			//----------------------------------------------------------------
			void SetLifetimeProperty(ParticlePropertyUPtr<f32> in_lifetimeProperty);
			//----------------------------------------------------------------
			/// Sets the property used to generate the initial scale of a new 
			/// particle.
			///
			/// @author Ian Copland
			///
			/// @param The property
			//----------------------------------------------------------------
			void SetInitialScaleProperty(ParticlePropertyUPtr<Core::Vector2> in_initialScaleProperty);
			//----------------------------------------------------------------
			/// Sets the property used to generate the initial rotation of a 
			/// new particle.
			///
			/// @author Ian Copland
			///
			/// @param The property
			//----------------------------------------------------------------
			void SetInitialRotationProperty(ParticlePropertyUPtr<f32> in_initialRotationProperty);
			//----------------------------------------------------------------
			/// Sets the property used to generate the initial colour of a new 
			/// particle.
			///
			/// @author Ian Copland
			///
			/// @param The property
			//----------------------------------------------------------------
			void SetInitialColourProperty(ParticlePropertyUPtr<Core::Colour> in_initialColourProperty);
			//----------------------------------------------------------------
			/// Sets the property used to generate the initial speed of a new 
			/// particle. The initial direction of motion is described by the 
			/// emitter.
			///
			/// @author Ian Copland
			///
			/// @param The property
			//----------------------------------------------------------------
			void SetInitialSpeedProperty(ParticlePropertyUPtr<f32> in_initialSpeedProperty);
			//----------------------------------------------------------------
			/// Sets the property used to generate the initial angular 
			/// velocity of a new particle.
			///
			/// @author Ian Copland
			///
			/// @param The property
			//----------------------------------------------------------------
			void SetInitialAngularVelocityProperty(ParticlePropertyUPtr<f32> in_initialAngularVelocityProperty);
			//----------------------------------------------------------------
			/// Sets the particle drawable for this particle effect.
			///
			/// @author Ian Copland
			///
			/// @param The particle graphic definition.
			//----------------------------------------------------------------
			void SetDrawableDef(ParticleDrawableDefUPtr in_drawableDef);
			//----------------------------------------------------------------
			/// Sets the particle emitter definition for this particle effect.
			///
			/// @author Ian Copland
			///
			/// @param The particle emitter definition.
			//----------------------------------------------------------------
			void SetEmitterDef(ParticleEmitterDefUPtr in_emitterDef);
			//----------------------------------------------------------------
			/// Sets the particle affector definitions for this particle effect.
			///
			/// @author Ian Copland
			///
			/// @param The particle Affector definition.
			//----------------------------------------------------------------
			void SetAffectorDefs(std::vector<ParticleAffectorDefUPtr> in_affectorDefs);
			//----------------------------------------------------------------
			/// Destructor
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			virtual ~ParticleEffect();
		private:
			friend class Core::ResourcePool;
			//----------------------------------------------------------------
			/// Factory method to create an new instance of an empty material 
			/// resource. Only called by the resource pool.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			static ParticleEffectUPtr Create();
			//----------------------------------------------------------------
			/// Private constructor to ensure that the factory method is used
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			ParticleEffect();

			f32 m_duration = 1.0f;
			u32 m_maxParticles = 100;
			SimulationSpace m_simulationSpace = SimulationSpace::k_local;

			ParticlePropertyUPtr<f32> m_lifetimeProperty;
			ParticlePropertyUPtr<Core::Vector2> m_initialScaleProperty = ParticlePropertyUPtr<Core::Vector2>(new ConstantParticleProperty<Core::Vector2>(Core::Vector2::k_one));
			ParticlePropertyUPtr<f32> m_initialRotationProperty = ParticlePropertyUPtr<f32>(new ConstantParticleProperty<f32>(0.0f));
			ParticlePropertyUPtr<Core::Colour> m_initialColourProperty = ParticlePropertyUPtr<Core::Colour>(new ConstantParticleProperty<Core::Colour>(Core::Colour::k_white));
			ParticlePropertyUPtr<f32> m_initialSpeedProperty = ParticlePropertyUPtr<f32>(new ConstantParticleProperty<f32>(0.0f));
			ParticlePropertyUPtr<f32> m_initialAngularVelocityProperty = ParticlePropertyUPtr<f32>(new ConstantParticleProperty<f32>(0.0f));

			ParticleDrawableDefUPtr m_drawableDef;
			ParticleEmitterDefUPtr m_emitterDef;
			std::vector<ParticleAffectorDefUPtr> m_affectorDefs;
		};
	}
}

#endif