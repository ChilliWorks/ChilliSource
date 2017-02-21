//
//  PointParticleEmitter.h
//  ChilliSource
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLES_EMITTER_POINTPARTICLEEMITTER_H_
#define _CHILLISOURCE_RENDERING_PARTICLES_EMITTER_POINTPARTICLEEMITTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitter.h>

namespace ChilliSource
{
    //-----------------------------------------------------------------------
    /// A point particle emitter. The spawns particles at the entity's
    /// position with a random direction.
    ///
    /// Particle emitters will be updated as a background task and should not
    /// be accessed from other threads.
    ///
    /// @author Ian Copland
    //-----------------------------------------------------------------------
    class PointParticleEmitter final : public ParticleEmitter
    {
    public:
        //----------------------------------------------------------------
        /// Generates the position and direction of a new emission. These 
        /// values are in local space. This will be called as part of a 
        /// background task.
        ///
        /// @author Ian Copland
        ///
        /// @param The normalised emission playback time.
        /// @param [Out] The generated position in local space.
        /// @param [Out] The generate direction in local space.
        //----------------------------------------------------------------
        void GenerateEmission(f32 in_normalisedEmissionTime, Vector3& out_position, Vector3& out_direction) override;
    private:
        friend class PointParticleEmitterDef;
        //----------------------------------------------------------------
        /// Constructor.
        ///
        /// @author Ian Copland
        ///
        /// @param The particle emitter definition.
        /// @param The particle array.
        //----------------------------------------------------------------
        PointParticleEmitter(const ParticleEmitterDef* in_particleEmitter, dynamic_array<Particle>* in_particleArray);
    };
}

#endif
