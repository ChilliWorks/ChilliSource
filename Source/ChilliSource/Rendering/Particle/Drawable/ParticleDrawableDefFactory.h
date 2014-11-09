//
//  ParticleDrawableDefFactory.h
//  Chilli Source
//  Created by Ian Copland on 07/11/2014.
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

#ifndef _ICENGINE_RENDERING_PARTICLES_PARTICLEDRAWABLEDEFFACTORY_H_
#define _ICENGINE_RENDERING_PARTICLES_PARTICLEDRAWABLEDEFFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/GenericFactory.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDef.h>

#include <json/json.h>

#include <functional>
#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
		//--------------------------------------------------------------------------
		/// A factory for creating particle drawable definitions. This allows 
		/// particle drawable definition types to be registered with the class. A
		/// Particle Provider can then create new instances of the registered 
		/// drawable def with a Json descriptor.
		///
		/// @author Ian Copland
		//--------------------------------------------------------------------------
		class ParticleDrawableDefFactory final : public Core::GenericFactory<ParticleDrawableDef>
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleDrawableDefFactory);
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
			/// drawable def factory.
			///
			/// @author Ian Copland
			///
			/// @return The new instance of the system.
			//-----------------------------------------------------------------
			static ParticleDrawableDefFactoryUPtr Create();
			//-----------------------------------------------------------------
			/// Default constructor. Declared private to force the use of the
			/// CreateSystem() method in Application.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			ParticleDrawableDefFactory() = default;
			//-----------------------------------------------------------------
			/// Registers the default particle drawable definitions.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void RegisterDefaults() override;
		};
	}
}

#endif