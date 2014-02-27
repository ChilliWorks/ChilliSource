//
//  ParticleEmitterFactory.cpp
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>

#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //------------------------------------------------------------------
        /// Register 
        ///
        /// Resgiters a new emitter type with the given key
        ///
        /// @param Emitter instance static create delegate
        /// @param Key string
        //------------------------------------------------------------------
        void ParticleEmitterFactory::Register(const EmitterCreateDelegate& inDelegate, const std::string& instrKey)
        {
            mmapDelegateToType.insert(std::make_pair(instrKey, inDelegate));
        }
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
        ParticleEmitter* ParticleEmitterFactory::CreateParticleEmitter(const std::string& instrType, const Core::ParamDictionary& insParams, 
                                                                         const MaterialSPtr& inpMaterial, ParticleComponent* inpOwner)
        {
            MapDelegateToString::iterator it = mmapDelegateToType.find(instrType);
            
            if(it != mmapDelegateToType.end())
            {
                return (it->second)(insParams, inpMaterial, inpOwner);
            }
            else
            {
                CS_ERROR_LOG("Cannot create particle emitter of type: " + instrType);
                return nullptr;
            }
        }
    }
}
