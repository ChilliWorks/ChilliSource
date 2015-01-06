//
//  ParticleEmitterDef.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_EMITTER_PARTICLEEMITTERDEF_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_EMITTER_PARTICLEEMITTERDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/Rendering/Particle/Property/ConstantParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>

#include <json/json.h>

#include <functional>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// A particle emitter def describes the properties that should be used 
		/// to create a particle emitter and creates the emitter instances.
		///
		/// As a particle emitter def's contents can potentially be read from 
		/// multiple threads, it is immutable after construction. The exception 
		/// to this is if it was created from a param dictionary with a 
		/// asynchronous delegate, in which case it is immutable after the
		/// delegate is called. Classes inheriting from this should also follow 
		/// these rules.
		///
		/// The following are the parameters that all particle emitter definitions
		/// contain. Specific particle emitter types may contain additional
		/// parameters.
		///
		/// "EmissionMode": Describes the emission mode. Possible values are 
		/// "Stream" or "Burst". Defaults to "Stream".
		///
		/// "EmissionRateProperty": A property describing the rate of emission
		/// when in stream emission mode. Ignored in burst emission mode.
		/// 
		/// "ParticlesPerEmissionProperty": A property describing the number of 
		/// particles in each emission.
		///
		/// "EmissionChanceProperty": A property describing the chance that a 
		/// particle will actually be emitted during each attempt.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class ParticleEmitterDef : public Core::QueryableInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleEmitterDef);
			CS_DECLARE_NOCOPY(ParticleEmitterDef);
			//----------------------------------------------------------------
			/// The emitter def loaded delegate. This is used when background 
			/// loading the particle emitter def, once it has finished loading 
			/// this delegate should be called.
			///
			/// @author Ian Copland
			///
			/// @param The particle emitter def.
			//----------------------------------------------------------------
			using LoadedDelegate = std::function<void(ParticleEmitterDef* in_emitterdef)>;
			//----------------------------------------------------------------
			/// An enum describing the possible emission modes for an emitter.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			enum class EmissionMode
			{
				k_stream,
				k_burst
			};
			//----------------------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param The emission mode.
			/// @param The emission rate property. This is ignored for when in 
			/// burst emission mode.
			/// @param The property describing the number of particles in each 
			/// emission.
			/// @param The property describing the chance that a particle will 
			/// actually emit each time a  emission is attempted. This is used 
			/// to make an effect look more random.
			//----------------------------------------------------------------
			ParticleEmitterDef(EmissionMode in_emissionMode, ParticlePropertyUPtr<f32> in_emissionRateProperty, ParticlePropertyUPtr<u32> in_particlesPerEmissionProperty,
				ParticlePropertyUPtr<f32> in_emissionChanceProperty);
			//----------------------------------------------------------------
			/// Constructor. Initialises with parameters read from the given 
			/// param dictionary. Inheriting classes should also take a delegate 
			/// as a parameter for their equivelent constructor. If the delegate 
			/// is not null, resources should be loaded asynchronously and the 
			/// delegate should be called once finished. The parameters takem
			/// by a particle emitter def are described in the class documentation.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters for the
			/// particle emitter def.
			//----------------------------------------------------------------
			ParticleEmitterDef(const Json::Value& in_paramsJson);
			//----------------------------------------------------------------
			/// Creates an instance of the particle emitter described by this.
			///
			/// @author Ian Copland.
			///
			/// @param The particle effect.
			/// @param The particle array.
			///
			/// @return the instance.
			//----------------------------------------------------------------
			virtual ParticleEmitterUPtr CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const = 0;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The particle effect that owns this particle emitter
			/// definition.
			//----------------------------------------------------------------
			const ParticleEffect* GetParticleEffect() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The emission mode.
			//----------------------------------------------------------------
			EmissionMode GetEmissionMode() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			/// 
			/// @return The number of particles per second that this will emit. 
			/// This is always null when in burst emission mode.
			//----------------------------------------------------------------
			const ParticleProperty<f32>* GetEmissionRateProperty() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The number of particles in each emission. In a burst 
			/// mode particle this is the number of paricles in the burst.
			//----------------------------------------------------------------
			const ParticleProperty<u32>* GetParticlesPerEmissionProperty() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The chance of actually emitting a particle during each
			/// attempt.
			//----------------------------------------------------------------
			const ParticleProperty<f32>* GetEmissionChanceProperty() const;
			//----------------------------------------------------------------
			/// Destructor
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			virtual ~ParticleEmitterDef() {};
		private:
			friend class ParticleEffect;
			//----------------------------------------------------------------
			/// Sets the owning particle effect. This can only be called by the 
			/// particle effect itself when this is added to it.
			///
			/// @author Ian Copland
			///
			/// @param The particle effect.
			//----------------------------------------------------------------
			void SetParticleEffect(const ParticleEffect* in_particleEffect);

			const ParticleEffect* m_particleEffect = nullptr;
			EmissionMode m_emissionMode = EmissionMode::k_stream;
			ParticlePropertyUPtr<f32> m_emissionRateProperty = ParticlePropertyUPtr<f32>(new ConstantParticleProperty<f32>(10.0f));
			ParticlePropertyUPtr<u32> m_particlesPerEmissionProperty = ParticlePropertyUPtr<u32>(new ConstantParticleProperty<u32>(1));
			ParticlePropertyUPtr<f32> m_emissionChanceProperty = ParticlePropertyUPtr<f32>(new ConstantParticleProperty<f32>(1.0f));
		};
	}
}

#endif