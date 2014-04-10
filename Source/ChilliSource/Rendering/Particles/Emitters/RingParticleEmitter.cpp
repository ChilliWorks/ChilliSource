//
//  RingParticleEmitter.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Emitters/RingParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffector.h>
#include <ChilliSource/Rendering/Particles/ParticleComponent.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //-------------------------------------------------------------
        /// Constructor
        ///
        /// @param Param Dictionary
        //-------------------------------------------------------------
        RingParticleEmitter::RingParticleEmitter(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent)
        :ParticleEmitter(inParams, inpMaterial, inpComponent), mfRadius(1.0f)
        {
            std::string strTemp;
            
            //Frequency
            if(inParams.TryGetValue("Radius", strTemp))
            {
                mfRadius = Core::ParseF32(strTemp);
            }
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
        ParticleEmitterUPtr RingParticleEmitter::Create(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent)
        {
            return ParticleEmitterUPtr(new RingParticleEmitter(inParams, inpMaterial, inpComponent));
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		void RingParticleEmitter::Emit(Particle& in_particle)
        {
            Core::Vector3 vPos;
            vPos.x = Core::MathUtils::RandomInRange(-1.0f, 1.0f);
            vPos.z = Core::MathUtils::RandomInRange(-1.0f, 1.0f);
            
            vPos.Normalise() *=  mfRadius;
            
            in_particle.m_translation += vPos;
            in_particle.m_velocity = Core::Vector3::Y_UNIT_POSITIVE * mfInitialVelocity;
            
        }
    }
}
