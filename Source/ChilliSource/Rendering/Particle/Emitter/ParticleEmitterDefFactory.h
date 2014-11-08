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
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/System/AppSystem.h>
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
		class ParticleEmitterDefFactory final : public Core::AppSystem
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
			//-----------------------------------------------------------------
			/// Registers a new particle emitter definition type with the 
			/// factory. Future calls to CreateEmitterDef() with the given name 
			/// will instantiate a particle emitter definition of this type. 
			/// The given name must be unique, this will try to assert if it is
			/// not.
			///
			/// @author Ian Copland
			///
			/// @param The name of the emitter def.
			//-----------------------------------------------------------------
			template <typename TEmitterDefType> void RegisterEmitterDef(const std::string& in_typeName);
			//-----------------------------------------------------------------
			/// Creates an instance of the particle emitter def registered 
			/// under the given name with the given json description. If a 
			/// async load delegate is provided then any resources will be
			/// asynchronously loaded and the async load delegate called on
			/// completion. Other-wise resources are loaded on the the current
			/// thread, which must be the main thread.
			///
			/// This is thread-safe when a async load delegate is supplied.
			///
			/// @author Ian Copland
			///
			/// @param The name of the emitter def.
			/// @param A json object describing the parameters of the emitter
			/// def.
			/// @param The async load delegate. Can be null.
			///
			/// @return The new particle emitter def.
			//-----------------------------------------------------------------
			ParticleEmitterDefUPtr CreateEmitterDef(const std::string& in_name, const Json::Value& in_jsonParams, const ParticleEmitterDef::LoadedDelegate& in_asyncLoadDelegate) const;
		private:
			friend class Core::Application;
			//-----------------------------------------------------------------
			/// A delegate which is used to instantiate the registered particle
			/// emitter def types.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters to create the
			/// particle emitter def with.
			/// @param The async load delegate. Can be null.
			//-----------------------------------------------------------------
			using CreatorDelegate = std::function<ParticleEmitterDefUPtr(const Json::Value& in_jsonParams, const ParticleEmitterDef::LoadedDelegate& in_asyncLoadDelegate)>;
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
			/// Creates a new instance of the emitter def of the given type.
			/// This is the method refered to by the creator delegates.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters for the particle
			/// emitter def.
			/// @param The async load delegate. Can be null.
			//-----------------------------------------------------------------
			template <typename TEmitterDefType> std::unique_ptr<TEmitterDefType> CreateEmitterDef(const Json::Value& in_jsonParams, const ParticleEmitterDef::LoadedDelegate& in_asyncLoadDelegate);
			//-----------------------------------------------------------------
			/// Initialised the factory, registering all default emitter def
			/// types.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void OnInit() override;
			//-----------------------------------------------------------------
			/// Deregisters all emitter def types.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void OnDestroy() override;

			std::unordered_map<std::string, CreatorDelegate> m_creatorDelegateMap;
		};
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TEmitterDefType> void ParticleEmitterDefFactory::RegisterEmitterDef(const std::string& in_typeName)
		{
			m_creatorDelegateMap.insert(std::make_pair(in_typeName, Core::MakeDelegate(this, &ParticleEmitterDefFactory::CreateEmitterDef<TEmitterDefType>)));
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TEmitterDefType> std::unique_ptr<TEmitterDefType> ParticleEmitterDefFactory::CreateEmitterDef(const Json::Value& in_jsonParams, const ParticleEmitterDef::LoadedDelegate& in_asyncLoadDelegate)
		{
			return std::unique_ptr<TEmitterDefType>(new TEmitterDefType(in_jsonParams, in_asyncLoadDelegate));
		}
	}
}

#endif