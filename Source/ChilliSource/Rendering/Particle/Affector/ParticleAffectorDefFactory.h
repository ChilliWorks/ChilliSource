//
//  ParticleAffectorDefFactory.h
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

#ifndef _ICENGINE_RENDERING_PARTICLES_PARTICLEAFFECTORDEFFACTORY_H_
#define _ICENGINE_RENDERING_PARTICLES_PARTICLEAFFECTORDEFFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDef.h>

#include <json/json.h>

#include <functional>
#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
		//--------------------------------------------------------------------------
		/// A factory for creating particle affector definitions. This allows 
		/// particle affector definition types to be registered with the class. A
		/// Particle Provider can then create new instances of the registered 
		/// affector def with a Json descriptor.
		///
		/// @author Ian Copland
		//--------------------------------------------------------------------------
		class ParticleAffectorDefFactory final : public Core::AppSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(ParticleAffectorDefFactory);
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
			/// Registers a new particle affector definition type with the 
			/// factory. Future calls to CreateAffectorDef() with the given name 
			/// will instantiate a particle affector definition of this type. 
			/// The given name must be unique, this will try to assert if it is
			/// not.
			///
			/// @author Ian Copland
			///
			/// @param The name of the affector def.
			//-----------------------------------------------------------------
			template <typename TAffectorDefType> void RegisterAffectorDef(const std::string& in_typeName);
			//-----------------------------------------------------------------
			/// Creates an instance of the particle affector def registered 
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
			/// @param The name of the affector def.
			/// @param A json object describing the parameters of the affector
			/// def.
			/// @param The async load delegate. Can be null.
			///
			/// @return The new particle affector def.
			//-----------------------------------------------------------------
			ParticleAffectorDefUPtr CreateAffectorDef(const std::string& in_name, const Json::Value& in_jsonParams, const ParticleAffectorDef::LoadedDelegate& in_asyncLoadDelegate) const;
		private:
			friend class Core::Application;
			//-----------------------------------------------------------------
			/// A delegate which is used to instantiate the registered particle
			/// affector def types.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters to create the
			/// particle affector def with.
			/// @param The async load delegate. Can be null.
			//-----------------------------------------------------------------
			using CreatorDelegate = std::function<ParticleAffectorDefUPtr(const Json::Value& in_jsonParams, const ParticleAffectorDef::LoadedDelegate& in_asyncLoadDelegate)>;
			//-----------------------------------------------------------------
			/// A factory method for creating new instances of a particle 
			/// affector def factory.
			///
			/// @author Ian Copland
			///
			/// @return The new instance of the system.
			//-----------------------------------------------------------------
			static ParticleAffectorDefFactoryUPtr Create();
			//-----------------------------------------------------------------
			/// Default constructor. Declared private to force the use of the
			/// CreateSystem() method in Application.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			ParticleAffectorDefFactory() = default;
			//-----------------------------------------------------------------
			/// Creates a new instance of the affector def of the given type.
			/// This is the method refered to by the creator delegates.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters for the particle
			/// affector def.
			/// @param The async load delegate. Can be null.
			//-----------------------------------------------------------------
			template <typename TAffectorDefType> std::unique_ptr<TAffectorDefType> CreateAffectorDef(const Json::Value& in_jsonParams, const ParticleAffectorDef::LoadedDelegate& in_asyncLoadDelegate);
			//-----------------------------------------------------------------
			/// Initialised the factory, registering all default affector def
			/// types.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void OnInit() override;
			//-----------------------------------------------------------------
			/// Deregisters all affector def types.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void OnDestroy() override;

			std::unordered_map<std::string, CreatorDelegate> m_creatorDelegateMap;
		};
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAffectorDefType> void ParticleAffectorDefFactory::RegisterAffectorDef(const std::string& in_typeName)
		{
			m_creatorDelegateMap.insert(std::make_pair(in_typeName, Core::MakeDelegate(this, &ParticleAffectorDefFactory::CreateAffectorDef<TAffectorDefType>)));
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAffectorDefType> std::unique_ptr<TAffectorDefType> ParticleAffectorDefFactory::CreateAffectorDef(const Json::Value& in_jsonParams, const ParticleAffectorDef::LoadedDelegate& in_asyncLoadDelegate)
		{
			return std::unique_ptr<TAffectorDefType>(new TAffectorDefType(in_jsonParams, in_asyncLoadDelegate));
		}
	}
}

#endif