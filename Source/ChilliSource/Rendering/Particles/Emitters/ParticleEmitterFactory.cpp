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
        //------------------------------------------------------------------
        void ParticleEmitterFactory::AddCreator(const EmitterCreateDelegate& inDelegate, const std::string& instrKey)
        {
            mmapDelegateToType.insert(std::make_pair(instrKey, inDelegate));
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        void ParticleEmitterFactory::RemoveAllCreators()
        {
            mmapDelegateToType.clear();
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
        ParticleEmitterUPtr ParticleEmitterFactory::CreateParticleEmitter(const std::string& instrType, const Core::ParamDictionary& insParams,
                                                                          const MaterialCSPtr& inpMaterial, ParticleComponent* inpOwner) const
        {
            MapDelegateToString::const_iterator it = mmapDelegateToType.find(instrType);
            
            if(it != mmapDelegateToType.end())
            {
                return (it->second)(insParams, inpMaterial, inpOwner);
            }
            else
            {
                CS_LOG_ERROR("Cannot create particle emitter of type: " + instrType);
                return nullptr;
            }
        }
    }
}
