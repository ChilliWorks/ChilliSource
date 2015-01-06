//
//  ScaleOverLifetimeParticleAffector.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_SCALEOVERLIFETIMEPARTICLEAFFECTOR_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_SCALEOVERLIFETIMEPARTICLEAFFECTOR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Container/dynamic_array.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffector.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//---------------------------------------------------------------------
		/// A particle affector which will change the size of particles over
		/// its lifetime by the calculated scale factor.
		///
		/// @author Ian Copland
		//---------------------------------------------------------------------
		class ScaleOverLifetimeParticleAffector final : public ParticleAffector
		{
		public:
			//----------------------------------------------------------------
			/// Stores the initial size and generates the scale factor for the
			/// activated particle so it can be used to generate the 
			/// interpolated size during updates.
			///
			/// @author Ian Copland
            ///
            /// @param The index of the particle to activate.
            /// @param The current normalised (0.0 to 1.0) progress through
            /// playback of the particle effect.
			//----------------------------------------------------------------
			void ActivateParticle(u32 in_index, f32 in_effectProgress) override;
			//----------------------------------------------------------------
			/// Updates the size of each particle.
			///
			/// @author Ian Copland
            ///
            /// @param The delta time.
            /// @param The current normalised (0.0 to 1.0) progress through
            /// playback of the particle effect.
			//----------------------------------------------------------------
			void AffectParticles(f32 in_deltaTime, f32 in_effectProgress) override;
			//----------------------------------------------------------------
			/// Destructor
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			virtual ~ScaleOverLifetimeParticleAffector() {};
		private:
			friend class ScaleOverLifetimeParticleAffectorDef;
			//----------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The particle affector definition.
			/// @param The particle array.
			//----------------------------------------------------------------
			ScaleOverLifetimeParticleAffector(const ParticleAffectorDef* in_affectorDef, Core::dynamic_array<Particle>* in_particleArray);
			//----------------------------------------------------------------
			/// A container for the initial scale and scale factor of a single 
			/// particle.
			///
			/// @author Ian Copland.
			//----------------------------------------------------------------
			struct ScaleData
			{
				Core::Vector2 m_initialScale;
				Core::Vector2 m_targetScale;
			};

			const ScaleOverLifetimeParticleAffectorDef* m_scaleOverLifetimeAffectorDef = nullptr;
			Core::dynamic_array<ScaleData> m_particleScaleData;
		};
	}
}

#endif