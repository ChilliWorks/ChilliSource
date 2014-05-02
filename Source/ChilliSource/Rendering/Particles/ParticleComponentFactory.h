//
//  ParticleComponentFactory.h
//  Chilli Source
//  Created by S Downie on 17/01/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_RENDERING_PARTICLES_PARTICLECOMPONENTFACTORY_H_
#define _CHILLISOURCE_RENDERING_PARTICLES_PARTICLECOMPONENTFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class ParticleComponentFactory final : public Core::AppSystem
		{
		public: 
			
            
			CS_DECLARE_NAMEDTYPE(ParticleComponentFactory);
            
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID
            ///
            /// @return Whether the object is of given type
            //-------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//--------------------------------------------------------
			/// Creates an empty particle component for customisation.
            /// The component will have no emitters, affectors or
            /// render properties.
            ///
            /// @author S Downie
			///
			/// @return New particle component
			//--------------------------------------------------------
			ParticleComponentUPtr CreateParticleComponent();
            //--------------------------------------------------------
            /// Creates a particle component build to the specifications
            /// of the given effect resource. The effect describes
            /// the emitters, affectors and render properties.
            ///
            /// @param Effect resource
            ///
            /// @return New particle component
            //--------------------------------------------------------
            ParticleComponentUPtr CreateParticleComponent(const ParticleEffectCSPtr& in_effect);
            
        private:
            
            friend class Core::Application;
            //--------------------------------------------------------
            /// Factory method for creating new component factory
            ///
            /// @author S Downie
            ///
            /// @param Particle system
            /// @param Emitter factory
            /// @param Affector factory
            ///
            /// @return Ownership of new instance
            //--------------------------------------------------------
            static ParticleComponentFactoryUPtr Create(ParticleSystem* in_particleSystem, ParticleEmitterFactory* in_emitterFactory, ParticleAffectorFactory* in_affectorFactory);
            //--------------------------------------------------------
            /// Private constructor to enforce use of factory method
            ///
            /// @author S Downie
            ///
            /// @param Particle system
            /// @param Emitter factory
            /// @param Affector factory
            //--------------------------------------------------------
			ParticleComponentFactory(ParticleSystem* in_particleSystem, ParticleEmitterFactory* in_emitterFactory, ParticleAffectorFactory* in_affectorFactory);
            
		private:
			
			ParticleSystem* m_particleSystem;
            
            ParticleAffectorFactory* m_affectorFactory;
            ParticleEmitterFactory* m_emitterFactory;
		};
	}
}

#endif