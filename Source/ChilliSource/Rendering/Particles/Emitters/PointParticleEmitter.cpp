//
//  PointParticleEmitter.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Emitters/PointParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffector.h>
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
        PointParticleEmitter::PointParticleEmitter(const Core::ParamDictionary& inParams, const MaterialSPtr &inpMaterial, ParticleComponent* inpComponent) 
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
        ParticleEmitterUPtr PointParticleEmitter::Create(const Core::ParamDictionary& inParams, const MaterialSPtr &inpMaterial, ParticleComponent* inpComponent)
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