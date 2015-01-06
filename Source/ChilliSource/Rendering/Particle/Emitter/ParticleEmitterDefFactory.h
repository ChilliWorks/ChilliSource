//
//  ParticleEmitterDefFactory.h
//  Chilli Source
//  Created by Ian Copland on 08/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _ICENGINE_RENDERING_PARTICLES_PARTICLEEMITTERDEFFACTORY_H_
#define _ICENGINE_RENDERING_PARTICLES_PARTICLEEMITTERDEFFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/GenericFactory.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDef.h>

#include <json/json.h>

#include <functional>
#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
		//--------------------------------------------------------------------------
		/// A factory for creating particle emitter definitions. This allows 
		/// particle emitter definition types to be registered with the class. A
		/// Particle Provider can then create new instances of the registered 
		/// emitter def with a Json descriptor.
		///
		/// @author Ian Copland
		//--------------------------------------------------------------------------
		class ParticleEmitterDefFactory final : public Core::GenericFactory<ParticleEmitterDef>
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleEmitterDefFactory);
			//-----------------------------------------------------------------
			/// Allows querying of whether or not this implements the interface
			/// described by the given interface Id.
			/// 
			/// @author Ian Copland
			///
			/// @param The interface Id.
			///
			/// @return Whether this implements the interface.
			//-----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
		private:
			friend class Core::Application;
			//-----------------------------------------------------------------
			/// A factory method for creating new instances of a particle 
			/// emitter def factory.
			///
			/// @author Ian Copland
			///
			/// @return The new instance of the system.
			//-----------------------------------------------------------------
			static ParticleEmitterDefFactoryUPtr Create();
			//-----------------------------------------------------------------
			/// Default constructor. Declared private to force the use of the
			/// CreateSystem() method in Application.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			ParticleEmitterDefFactory() = default;
			//-----------------------------------------------------------------
			/// Registers the default particle emitter definitions.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void RegisterDefaults() override;
		};
	}
}

#endif