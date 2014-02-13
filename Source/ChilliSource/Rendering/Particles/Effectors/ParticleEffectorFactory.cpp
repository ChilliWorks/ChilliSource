//
//  ParticleEffectorFactory.cpp
//  moFloTest
//
//  Created by Scott Downie on 07/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <moFlo/Rendering/Particles/Effectors/ParticleEffectorFactory.h>
#include <moFlo/Rendering/Particles/Effectors/ParticleEffector.h>

namespace moFlo
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
        void CParticleEffectorFactory::Register(const EffectorCreateDelegate& inDelegate, const std::string& instrKey)
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
        IParticleEffector* CParticleEffectorFactory::CreateParticleEffector(const std::string& instrType, const Core::ParamDictionary& insParams)
        {
            MapDelegateToString::iterator it = mmapDelegateToType.find(instrType);
            
            if(it != mmapDelegateToType.end())
            {
                return (it->second)(insParams);
            }
            else
            {
                ERROR_LOG("Cannot create particle effector of type: " + instrType);
                return NULL;
            }
        }
    }
}