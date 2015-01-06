//
//  PointParticleEmitterDef.h
//  Chilli Source
//  Created by Ian Copland on 02/11/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_EMITTER_POINTPARTICLEEMITTERDEF_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_EMITTER_POINTPARTICLEEMITTERDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDef.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// A particle emitter def describing the properties for a Point Particle
		/// Emitter and provides a means to create instances.
		///
		/// As a particle emitter def's contents can potentially be read from 
		/// multiple threads, it is immutable after construction. The exception 
		/// to this is if it was created from a param dictionary with a 
		/// asynchronous delegate, in which case it is immutable after the
		/// delegate is called.
		///
		/// A point emitter def only contains the parameters provided by the base
		/// particle emitter.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class PointParticleEmitterDef final : public ParticleEmitterDef
		{
		public:
			CS_DECLARE_NAMEDTYPE(PointParticleEmitterDef);
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
			/// actually emit each time an emission is attempted. This is used 
			/// to make an effect look more random.
			//----------------------------------------------------------------
			PointParticleEmitterDef(EmissionMode in_emissionMode, ParticlePropertyUPtr<f32> in_emissionRateProperty, ParticlePropertyUPtr<u32> in_particlesPerEmissionProperty,
				ParticlePropertyUPtr<f32> in_emissionChanceProperty);
			//----------------------------------------------------------------
			/// Constructor. Initialises with parameters read from the given 
			/// param dictionary. If the async delegate is not null, then any 
			/// resource loading will occur as a background task. Once complete 
			/// the delegate will be called. The parameters read from json are
            /// described in the base class's class documentation.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters for the particle
			/// emitter def.
			/// @param The loaded delegate. If this is supplied any resources
			/// will be loaded as a background task. Once complete, this
			/// delegate will be called.
			//----------------------------------------------------------------
			PointParticleEmitterDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_loadedDelegate = nullptr);
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
			/// Creates a new instance of a Point Particle Emitter.
			///
			/// @author Ian Copland.
			///
			/// @param The particle array.
			///
			/// @return the instance.
			//----------------------------------------------------------------
			ParticleEmitterUPtr CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const override;
		};
	}
}

#endif