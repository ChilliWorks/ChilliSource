//
//  PointParticleEmitter.cpp
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

#include <ChilliSource/Rendering/Particles/Emitters/PointParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Affectors/ParticleAffector.h>
#include <ChilliSource/Rendering/Particles/ParticleComponent.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //-------------------------------------------------------------
        /// Constructor
        ///
        /// @param Param Dictionary
        //-------------------------------------------------------------
        PointParticleEmitter::PointParticleEmitter(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent)
        :ParticleEmitter(inParams, inpMaterial, inpComponent)
        {
            
        }
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
        ParticleEmitterUPtr PointParticleEmitter::Create(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent)
        {
            return ParticleEmitterUPtr(new PointParticleEmitter(inParams, inpMaterial, inpComponent));
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		void PointParticleEmitter::Emit(Particle& in_particle)
        {
			Core::Vector3 vDirection(Core::MathUtils::RandomInRange(-1.0f, 1.0f),
									  Core::MathUtils::RandomInRange(-1.0f, 1.0f),
									  Core::MathUtils::RandomInRange(-1.0f, 1.0f));
			
			in_particle.m_velocity = vDirection;
			in_particle.m_velocity.Normalise();
            in_particle.m_velocity *= Core::MathUtils::RandomInRange(mfMinInitialVelocity, mfInitialVelocity);
        }
    }
}