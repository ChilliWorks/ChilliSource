//
//  ParticleEmitterFactory.h
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_PARTICLES_PARTICLE_EMITTER_FACTORY_H_
#define _MO_FLO_RENDERING_PARTICLES_PARTICLE_EMITTER_FACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

#include <unordered_map>

namespace ChilliSource
{
    namespace Rendering
    {
        class CParticleEmitterFactory
        {
        public:
            typedef fastdelegate::FastDelegate3<const Core::ParamDictionary&, const MaterialPtr&, CParticleComponent*, CParticleEmitter*> EmitterCreateDelegate; 

            //------------------------------------------------------------------
            /// Register 
            ///
            /// Resgiters a new emitter type with the given key
            ///
            /// @param Emitter instance static create delegate
            /// @param Key string
            //------------------------------------------------------------------
            void Register(const EmitterCreateDelegate& inDelegate, const std::string& instrKey);
            //------------------------------------------------------------------
            /// Create Particle Emitter
            ///
            /// Construct a particle emitter from the given shape identifier
            ///
            /// @param Shape type i.e. "Point", "Ring"
            /// @param Param dictionary
            /// @param Material
            /// @param Owning particle system
            /// @return Ownership of particle emitter of that type
            //-------------------------------------------------------------------
            CParticleEmitter* CreateParticleEmitter(const std::string& instrType, const Core::ParamDictionary& insParams, 
                                                    const MaterialPtr& inpMaterial, CParticleComponent* inpOwner);
            
        private:
            
            typedef std::unordered_map<std::string, EmitterCreateDelegate> MapDelegateToString;
            MapDelegateToString mmapDelegateToType;
        };
    }
}

#endif