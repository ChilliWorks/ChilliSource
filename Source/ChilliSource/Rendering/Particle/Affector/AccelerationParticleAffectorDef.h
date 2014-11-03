//
//  AccelerationParticleAffectorDef.h
//  Chilli Source
//  Created by Ian Copland on 03/11/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_ACCELERATIONPARTICLEAFFECTORDEF_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_ACCELERATIONPARTICLEAFFECTORDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDef.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// The definition for an acceleration particle affector. This describes 
		/// a particle affector which will alter the the speed of a particle 
		/// over time.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class AccelerationParticleAffectorDef final : public ParticleAffectorDef
		{
		public:
			CS_DECLARE_NAMEDTYPE(AccelerationParticleAffectorDef);
			//----------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The property which describes the acceleration of a
			/// particle.
			//----------------------------------------------------------------
			AccelerationParticleAffectorDef(ParticlePropertyUPtr<Core::Vector3> in_accelerationProperty);
			//----------------------------------------------------------------
			/// Constructor. Loads the params for the affector def from the 
			/// given param dictionary. If the async delegate is not null, then
			/// any resource loading will occur as a background task. Once 
			/// complete the delegate delegate will be called.
			///
			/// The values read from param dictionary are:
			///
			/// "Acceleration": The property describing the acceleration of a
			/// particle.
			///
			/// @author Ian Copland
			///
			/// @param The param dictionary.
			/// @param The async delegate.
			//----------------------------------------------------------------
			AccelerationParticleAffectorDef(const Core::ParamDictionary& in_params, const LoadedDelegate& in_asyncDelegate = nullptr);
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
			/// Creates an instance of the particle affector described by this.
			///
			/// @author Ian Copland.
			///
			/// @param The particle array.
			///
			/// @return the instance.
			//----------------------------------------------------------------
			ParticleAffectorUPtr CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const override;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return A property describing the acceleration.
			//----------------------------------------------------------------
			const ParticleProperty<Core::Vector3>* GetAccelerationProperty() const;
			//----------------------------------------------------------------
			/// Destructor
			///
			/// @author Ian Copland.
			//----------------------------------------------------------------
			virtual ~AccelerationParticleAffectorDef() {}
		private:

			ParticlePropertyUPtr<Core::Vector3> m_accelerationProperty;
		};
	}
}

#endif