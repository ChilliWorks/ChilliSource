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
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/System/AppSystem.h>
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
		class ParticleDrawableDefFactory final : public Core::AppSystem
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
			//-----------------------------------------------------------------
			/// Registers a new particle drawable definition type with the 
			/// factory. Future calls to CreateDrawableDef() with the given name 
			/// will instantiate a particle drawable definition of this type. 
			/// The given name must be unique, this will try to assert if it is
			/// not.
			///
			/// @author Ian Copland
			///
			/// @param The name of the drawable def.
			//-----------------------------------------------------------------
			template <typename TDrawableDefType> void RegisterDrawableDef(const std::string& in_typeName);
			//-----------------------------------------------------------------
			/// Creates an instance of the particle drawable def registered 
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
			/// @param The name of the drawable def.
			/// @param A json object describing the parameters of the drawable
			/// def.
			/// @param The async load delegate. Can be null.
			///
			/// @return The new particle drawable def.
			//-----------------------------------------------------------------
			ParticleDrawableDefUPtr CreateDrawableDef(const std::string& in_name, const Json::Value& in_jsonParams, const ParticleDrawableDef::LoadedDelegate& in_asyncLoadDelegate) const;
		private:
			friend class Core::Application;
			//-----------------------------------------------------------------
			/// A delegate which is used to instantiate the registered particle
			/// drawable def types.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters to create the
			/// particle drawable def with.
			/// @param The async load delegate. Can be null.
			//-----------------------------------------------------------------
			using CreatorDelegate = std::function<ParticleDrawableDefUPtr(const Json::Value& in_jsonParams, const ParticleDrawableDef::LoadedDelegate& in_asyncLoadDelegate)>;
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
			/// Creates a new instance of the drawable def of the given type.
			/// This is the method refered to by the creator delegates.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters for the particle
			/// drawable def.
			/// @param The async load delegate. Can be null.
			//-----------------------------------------------------------------
			template <typename TDrawableDefType> std::unique_ptr<TDrawableDefType> CreateDrawableDef(const Json::Value& in_jsonParams, const ParticleDrawableDef::LoadedDelegate& in_asyncLoadDelegate);
			//-----------------------------------------------------------------
			/// Initialised the factory, registering all default drawable def
			/// types.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void OnInit() override;
			//-----------------------------------------------------------------
			/// Deregisters all drawable def types.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void OnDestroy() override;

			std::unordered_map<std::string, CreatorDelegate> m_creatorDelegateMap;
		};
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TDrawableDefType> void ParticleDrawableDefFactory::RegisterDrawableDef(const std::string& in_typeName)
		{
			m_creatorDelegateMap.insert(std::make_pair(in_typeName, Core::MakeDelegate(this, &ParticleDrawableDefFactory::CreateDrawableDef<TDrawableDefType>)));
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TDrawableDefType> std::unique_ptr<TDrawableDefType> ParticleDrawableDefFactory::CreateDrawableDef(const Json::Value& in_jsonParams, const ParticleDrawableDef::LoadedDelegate& in_asyncLoadDelegate)
		{
			return std::unique_ptr<TDrawableDefType>(new TDrawableDefType(in_jsonParams, in_asyncLoadDelegate));
		}
	}
}

#endif