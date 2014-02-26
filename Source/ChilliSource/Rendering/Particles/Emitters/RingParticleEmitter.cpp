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
        CRingParticleEmitter::CRingParticleEmitter(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent) 
        :CParticleEmitter(inParams, inpMaterial, inpComponent), mfRadius(1.0f)
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
        CParticleEmitter* CRingParticleEmitter::Create(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent)
        {
            return new CRingParticleEmitter(inParams, inpMaterial, inpComponent);
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
		void CRingParticleEmitter::Emit(Particle* inpParticles, u32 udwParticleIndex)
        {
            
    
            Core::CVector3 vPos;
            vPos.x = Core::CMathUtils::RandomInRange(-1.0f, 1.0f);
            vPos.z = Core::CMathUtils::RandomInRange(-1.0f, 1.0f);
            
            vPos.Normalise() *=  mfRadius;
            
            inpParticles->vTranslation[udwParticleIndex] += vPos;
            inpParticles->vVelocity[udwParticleIndex] = Core::CVector3::Y_UNIT_POSITIVE * mfInitialVelocity;
            
        }
    }
}
