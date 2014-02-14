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

namespace moFlo
{
    namespace Rendering
    {
        //-------------------------------------------------------------
        /// Constructor
        ///
        /// @param Param Dictionary
        //-------------------------------------------------------------
        CPointParticleEmitter::CPointParticleEmitter(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent) 
        :CParticleEmitter(inParams, inpMaterial, inpComponent)
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
        CParticleEmitter* CPointParticleEmitter::Create(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent)
        {
            return new CPointParticleEmitter(inParams, inpMaterial, inpComponent);
        }
		//-----------------------------------------------------
		/// Emit
		///
		/// Emit some new particles by pulling from the dead
		/// pool. Emission will depend on the number of
		/// particles emitted and the shape of the emitter
		///
		/// @param Particles
		/// @param Index of current particle
		//-----------------------------------------------------
		void CPointParticleEmitter::Emit(Particle* inpParticles, u32 udwParticleIndex)
        {
			Core::CVector3 vDirection(Core::CMathUtils::RandomInRange(-1.0f, 1.0f),
									  Core::CMathUtils::RandomInRange(-1.0f, 1.0f),
									  Core::CMathUtils::RandomInRange(-1.0f, 1.0f));
			
			inpParticles->vVelocity[udwParticleIndex] = vDirection;
			inpParticles->vVelocity[udwParticleIndex].Normalise();
            inpParticles->vVelocity[udwParticleIndex] *= Core::CMathUtils::RandomInRange(mfMinInitialVelocity, mfInitialVelocity);
        }
    }
}