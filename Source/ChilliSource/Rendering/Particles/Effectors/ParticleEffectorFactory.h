//
//  ParticleEffectorFactory.h
//  moFloTest
//
//  Created by Scott Downie on 07/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_RENDERING_PARTICLES_EFFECTORS_PARTICLE_EFFECTOR_FACTORY_H_
#define _MO_FLO_RENDERING_PARTICLES_EFFECTORS_PARTICLE_EFFECTOR_FACTORY_H_

#include <ChilliSource/Rendering/ForwardDeclarations.h>

#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
    namespace Rendering
    {
        class CParticleEffectorFactory
        {
        public:
            typedef fastdelegate::FastDelegate1<const Core::ParamDictionary&, IParticleEffector*> EffectorCreateDelegate; 
            
            //------------------------------------------------------------------
            /// Register 
            ///
            /// Resgiters a new effector type with the given key
            ///
            /// @param Effector static create delegate
            /// @param Key string
            //------------------------------------------------------------------
            void Register(const EffectorCreateDelegate& inDelegate, const std::string& instrKey);
            //------------------------------------------------------------------
            /// Create Particle Effector
            ///
            /// Construct a particle effector from the given shape identifier
            ///
            /// @param Shape type i.e. "colour fader", "linear force"
            /// @param Param dictionary
            /// @return Ownership of particle effector of that type
            //-------------------------------------------------------------------
            IParticleEffector* CreateParticleEffector(const std::string& instrType, const Core::ParamDictionary& insParams);
            
        private:
            
            typedef HASH_MAP<std::string, EffectorCreateDelegate> MapDelegateToString;
            MapDelegateToString mmapDelegateToType;
        };
    }
}

#endif