//
//  ParticleAffectorDef.h
//  Chilli Source
//  Created by Ian Copland on 21/10/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_PARTICLEAFFECTORDEF_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_PARTICLEAFFECTORDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

#include <functional>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// A particle affector def describes the properties that should be used 
		/// to create a particle affector and creates the affector instances.
		///
		/// As a particle affector def's contents can potentially be read from 
		/// multiple threads, it is immutable after construction. The exception 
		/// to this is if it was created from a param dictionary with a 
		/// asynchronous delegate, in which case it is immutable after the
		/// delegate is called. Classes inheriting from this should also follow 
		/// these rules.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class ParticleAffectorDef : public Core::QueryableInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleAffectorDef);
			CS_DECLARE_NOCOPY(ParticleAffectorDef);
			//----------------------------------------------------------------
			/// The affector def loaded delegate. This is used when background 
			/// loading the particle affector def, once it has finished loading 
			/// this delegate should be called.
			///
			/// @author Ian Copland
			///
			/// @param The particle affector def.
			//----------------------------------------------------------------
			using LoadedDelegate = std::function<void(ParticleAffectorDef* in_affectorDef)>;
			//----------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			ParticleAffectorDef() = default;
			//----------------------------------------------------------------
			/// Creates an instance of the particle affector described by this.
			///
			/// @author Ian Copland.
			///
			/// @param The particle array.
			///
			/// @return the instance.
			//----------------------------------------------------------------
			virtual ParticleAffectorUPtr CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const = 0;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The particle effect that owns this particle drawable
			/// definition.
			//----------------------------------------------------------------
			const ParticleEffect* GetParticleEffect() const;
			//----------------------------------------------------------------
			/// Destructor
			///
			/// @author Ian Copland.
			//----------------------------------------------------------------
			virtual ~ParticleAffectorDef() {}
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
		};
	}
}

#endif