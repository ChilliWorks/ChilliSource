//
//  ParticleEffectorFactory.h
//  moFloTest
//
//  Created by Scott Downie on 07/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_RENDERING_PARTICLES_EFFECTORS_PARTICLE_EFFECTOR_FACTORY_H_
#define _MO_FLO_RENDERING_PARTICLES_EFFECTORS_PARTICLE_EFFECTOR_FACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

#include <functional>
#include <unordered_map>

namespace ChilliSource
{
    namespace Rendering
    {
        class ParticleEffectorFactory
        {
        public:
            typedef std::function<ParticleEffectorUPtr(const Core::ParamDictionary&)> EffectorCreateDelegate;
            
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
            ParticleEffectorUPtr CreateParticleEffector(const std::string& instrType, const Core::ParamDictionary& insParams) const;
            
        private:
            
            typedef std::unordered_map<std::string, EffectorCreateDelegate> MapDelegateToString;
            MapDelegateToString mmapDelegateToType;
        };
    }
}

#endif
