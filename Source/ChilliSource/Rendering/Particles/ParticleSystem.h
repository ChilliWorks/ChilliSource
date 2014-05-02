//
//  ParticleSystem.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLES_PARTICLESYSTEM_H_
#define _CHILLISOURCE_RENDERING_PARTICLES_PARTICLESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //----------------------------------------------------------
        /// The particle system manages the updating of all
        /// existing particle components
        ///
        /// @author S Downie
        //----------------------------------------------------------
		class ParticleSystem final : public Core::AppSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleSystem);
			
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Comparison Type
            ///
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//-------------------------------------------
			/// @author S Downie
			///
			/// @param Particle Component
			//-------------------------------------------
			void AddParticleComponent(ParticleComponent* in_particle);
			//-------------------------------------------
			/// @author S Downie
			///
			/// @param Particle Component
			//-------------------------------------------
			void RemoveParticleComponent(ParticleComponent* in_particle);
			//-------------------------------------------
			/// Sets a factor to scale update delta time by.
            /// Useful for pausing, slowing down and speeding
            /// up all effects.
            ///
            /// @author S Downie
            ///
            /// @param Time scaler
			//-------------------------------------------
			void SetTimeScaleFactor(f32 in_scale);
            
        private:
            
            friend class Core::Application;
            //----------------------------------------------------------
            /// Factory method to create new particle system
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new particle system
            //----------------------------------------------------------
            static ParticleSystemUPtr Create();
            //----------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //----------------------------------------------------------
			ParticleSystem() = default;
            //-------------------------------------------
			/// Update all the particle components
			///
            /// @author S Downie
            ///
			/// @param Time since last update in seconds
			//-------------------------------------------
			void OnUpdate(f32 in_timeSinceLastUpdate) override;
            //----------------------------------------------------------
			/// @author S Downie
			///
            /// Called when the system is destroyed
			//----------------------------------------------------------
			void OnDestroy() override;
			
		private:
			
			std::vector<ParticleComponent*> m_particleComponents;
            
            f32 m_timeScaleFactor = 1.0f;
		};
	}
}

#endif