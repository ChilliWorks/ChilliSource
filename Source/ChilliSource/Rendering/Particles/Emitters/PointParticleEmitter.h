//
//  PointParticleEmitter.h
//  Chilli Source
//  Created by Scott Downie on 01/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _MO_FLO_RENDERING_PARTICLES_EMITTERS_POINT_PARTICLE_EMITTER_H_
#define _MO_FLO_RENDERING_PARTICLES_EMITTERS_POINT_PARTICLE_EMITTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace ChilliSource
{
    namespace Rendering
    {
        class PointParticleEmitter : public ParticleEmitter
        {
        public:
            PointParticleEmitter(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent);

            //-----------------------------------------------------
            /// Create
            ///
            /// Create a new emitter from the given parameters
            ///
            /// @param Param dictionary
            /// @param Material
            /// @param Owning system
            /// @return Ownership of point emitter
            //-----------------------------------------------------
            static ParticleEmitterUPtr Create(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent);
            
        private:
            //-----------------------------------------------------
            /// Emit
            ///
            /// Emit some new particles by pulling from the dead
            /// pool. Emission will depend on the number of
            /// particles emitted and the shape of the emitter
            ///
			/// @param Particle
            //-----------------------------------------------------
            void Emit(Particle& in_particle);
        };
    }
}

#endif