//
//  ConeParticleEmitter.cpp
//  MoshiMonsters
//
//  Created by Andrew Glass on 02/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Emitters/ConeParticleEmitter.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //-------------------------------------------------------------
        /// Constructor
        ///
        /// @param Param Dictionary
        //-------------------------------------------------------------
        ConeParticleEmitter::ConeParticleEmitter(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent)
        :ParticleEmitter(inParams, inpMaterial, inpComponent)
        {
            std::string strTemp;
            
            f32 fRadius = 45.0f;
            //Radius
            if(inParams.TryGetValue("Radius", strTemp))
            {
                fRadius =Core::MathUtils::Clamp( Core::ParseF32(strTemp), 1.0f, 89.0f);
            }
            
            mfBoxArea = sinf(Core::MathUtils::DegToRad(fRadius));
            mfBoxHeight = cosf(Core::MathUtils::DegToRad(fRadius));
            
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
        ParticleEmitterUPtr ConeParticleEmitter::Create(const Core::ParamDictionary& inParams, const MaterialCSPtr &inpMaterial, ParticleComponent* inpComponent)
        {
            return ParticleEmitterUPtr(new ConeParticleEmitter(inParams, inpMaterial, inpComponent));
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		void ConeParticleEmitter::Emit(Particle& in_particle)
        {
            f32 fBoxAreaSq = mfBoxArea * mfBoxArea;
            Core::Vector3 vDir;
            do
            {
                vDir.x = Core::MathUtils::RandomInRange(-mfBoxArea, mfBoxArea);
                vDir.z = Core::MathUtils::RandomInRange(-mfBoxArea, mfBoxArea);
            }while(fBoxAreaSq < (vDir.x *vDir.x + vDir.z *vDir.z));
            
            vDir.y = mfBoxHeight;
            in_particle.m_velocity = vDir;
			in_particle.m_velocity.Normalise();
            in_particle.m_velocity *= mfInitialVelocity;
        }
        
    }
}