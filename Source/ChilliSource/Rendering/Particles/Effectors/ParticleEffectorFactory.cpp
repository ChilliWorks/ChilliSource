//
//  ParticleEffectorFactory.cpp
//  moFloTest
//
//  Created by Scott Downie on 07/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffectorFactory.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffector.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //------------------------------------------------------------------
        /// Register 
        ///
        /// Resgiters a new effector type with the given key
        ///
        /// @param Effector static create delegate
        /// @param Key string
        //------------------------------------------------------------------
        void ParticleEffectorFactory::Register(const EffectorCreateDelegate& inDelegate, const std::string& instrKey)
        {
            mmapDelegateToType.insert(std::make_pair(instrKey, inDelegate));
        }
        //------------------------------------------------------------------
        /// Create Particle Effector
        ///
        /// Construct a particle effector from the given shape identifier
        ///
        /// @param Shape type i.e. "colour fader", "linear force"
        /// @param Param dictionary
        /// @return Ownership of particle effector of that type
        //-------------------------------------------------------------------
        ParticleEffectorUPtr ParticleEffectorFactory::CreateParticleEffector(const std::string& instrType, const Core::ParamDictionary& insParams) const
        {
            MapDelegateToString::const_iterator it = mmapDelegateToType.find(instrType);
            
            if(it != mmapDelegateToType.end())
            {
                return (it->second)(insParams);
            }
            else
            {
                CS_LOG_ERROR("Cannot create particle effector of type: " + instrType);
                return nullptr;
            }
        }
    }
}