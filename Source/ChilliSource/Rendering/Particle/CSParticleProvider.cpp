//
//  CSParticleProvider.cpp
//  Chilli Source
//  Created by Ian Copland on 04/11/2014.
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

#include <ChilliSource/Rendering/Particle/CSParticleProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyFactory.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace
		{
			const std::string k_fileExtension = "csparticle";
			//-----------------------------------------------------------------
			/// Parses a simulation space string value.
			///
			/// @author Ian Copland
			///
			/// @param The string value.
			///
			/// @return The simulation space described by the string.
			//-----------------------------------------------------------------
			ParticleEffect::SimulationSpace ParseSimulationSpace(const std::string& in_string)
			{
				std::string simulationSpaceString = in_string;
				Core::StringUtils::ToLowerCase(simulationSpaceString);

				if (simulationSpaceString == "world")
				{
					return ParticleEffect::SimulationSpace::k_world;
				}
				else if (simulationSpaceString == "local")
				{
					return ParticleEffect::SimulationSpace::k_local;
				}

				CS_LOG_FATAL("Invalid simulation space in particle effect: " + in_string);
				return ParticleEffect::SimulationSpace::k_world;
			}
			//-----------------------------------------------------------------
			/// Opens and reads the CSParticle file into a json object.
			///
			/// @author Ian Copland
			///
			/// @param The storage location of the csparticle json file.
			/// @param The file path to the csparticle json file.
			/// @param [Out] The json value which should be populated
			//-----------------------------------------------------------------
			bool OpenCSParticleFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Json::Value& out_jsonRoot)
			{
				if (Core::Utils::ReadJson(in_storageLocation, in_filePath, &out_jsonRoot) == false)
				{
					CS_LOG_ERROR("Failed to read csparticle file: " + in_filePath);
					return false;
				}

				if (out_jsonRoot.isNull())
				{
					CS_LOG_ERROR("Failed to read csparticle file: " + in_filePath);
					return false;
				}

				return true;
			}
			//-----------------------------------------------------------------
			/// Reads the base properties in the particle effect such as the
			/// effect duration, the number of particles and the initial
			/// particle values.
			///
			/// @author Ian Copland
			///
			/// @param The root json object
			/// @param [Out] The particle effect that should be populated.
			//-----------------------------------------------------------------
			void ReadBaseValues(const Json::Value& in_jsonRoot, const ParticleEffectSPtr& out_particleEffect)
			{
				//Duration
				Json::Value jsonValue = in_jsonRoot.get("Duration", Json::nullValue);
				if (jsonValue.isNull() == false && jsonValue.isString() == true)
				{
					out_particleEffect->SetDuration(Core::ParseF32(jsonValue.asString()));
				}

				//Max Particles
				jsonValue = in_jsonRoot.get("MaxParticles", Json::nullValue);
				if (jsonValue.isNull() == false && jsonValue.isString() == true)
				{
					out_particleEffect->SetMaxParticles(Core::ParseU32(jsonValue.asString()));
				}

				//Simulation Space
				jsonValue = in_jsonRoot.get("SimulationSpace", Json::nullValue);
				if (jsonValue.isNull() == false && jsonValue.isString() == true)
				{
					out_particleEffect->SetSimulationSpace(ParseSimulationSpace(jsonValue.asString()));
				}

				//Lifetime Property
				jsonValue = in_jsonRoot.get("LifetimeProperty", Json::nullValue);
				if (jsonValue.isNull() == false)
				{
					auto prop = ParticlePropertyFactory::CreateProperty<f32>(jsonValue);
					out_particleEffect->SetLifetimeProperty(std::move(prop));
				}

				//Initial Scale Property
				jsonValue = in_jsonRoot.get("InitialScaleProperty", Json::nullValue);
				if (jsonValue.isNull() == false)
				{
					auto prop = ParticlePropertyFactory::CreateProperty<Core::Vector2>(jsonValue);
					out_particleEffect->SetInitialScaleProperty(std::move(prop));
				}

				//Initial Rotation Property
				jsonValue = in_jsonRoot.get("InitialRotationProperty", Json::nullValue);
				if (jsonValue.isNull() == false)
				{
					auto prop = ParticlePropertyFactory::CreateProperty<f32>(jsonValue);
					out_particleEffect->SetInitialRotationProperty(std::move(prop));
				}

				//Initial Colour Property
				jsonValue = in_jsonRoot.get("InitialColourProperty", Json::nullValue);
				if (jsonValue.isNull() == false)
				{
					auto prop = ParticlePropertyFactory::CreateProperty<Core::Colour>(jsonValue);
					out_particleEffect->SetInitialColourProperty(std::move(prop));
				}

				//Initial Speed Property
				jsonValue = in_jsonRoot.get("InitialSpeedProperty", Json::nullValue);
				if (jsonValue.isNull() == false)
				{
					auto prop = ParticlePropertyFactory::CreateProperty<f32>(jsonValue);
					out_particleEffect->SetInitialSpeedProperty(std::move(prop));
				}

				//Initial Angular Velocity Property
				jsonValue = in_jsonRoot.get("InitialAngularVelocityProperty", Json::nullValue);
				if (jsonValue.isNull() == false)
				{
					auto prop = ParticlePropertyFactory::CreateProperty<f32>(jsonValue);
					out_particleEffect->SetInitialAngularVelocityProperty(std::move(prop));
				}
			}
			//-----------------------------------------------------------------
			/// Reads the CSParticle json file and populates the particle effect.
			/// If an async load delete is provided any resource loading will
			/// be executed on a background thread. Once complete the async
			/// load delegate will be called on the main thread.
			///
			/// @author Ian Copland
			///
			/// @param The storage location of the csparticle json file.
			/// @param The file path to the csparticle json file.
			/// @param The async load delegate. This can be null.
			/// @param [Out] The particle effect that should be populated.
			//-----------------------------------------------------------------
			void LoadCSParticle(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate,
				const ParticleEffectSPtr& out_particleEffect)
			{
				Json::Value jsonRoot;
				if (OpenCSParticleFile(in_storageLocation, in_filePath, jsonRoot) == false)
				{
					out_particleEffect->SetLoadState(Core::Resource::LoadState::k_failed);
					if (in_delegate != nullptr)
					{
						Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_particleEffect));
					}
					return;
				}

				ReadBaseValues(jsonRoot, out_particleEffect);


				//TODO: continue here...
			}
		}

		CS_DEFINE_NAMEDTYPE(CSParticleProvider);
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		CSParticleProviderUPtr CSParticleProvider::Create()
		{
			return CSParticleProviderUPtr(new CSParticleProvider());
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		bool CSParticleProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (Core::ResourceProvider::InterfaceID == in_interfaceId || CSParticleProvider::InterfaceID == in_interfaceId);
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		Core::InterfaceIDType CSParticleProvider::GetResourceType() const
		{
			return ParticleEffect::InterfaceID;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		bool CSParticleProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			std::string lowerExtension = in_extension;
			Core::StringUtils::ToLowerCase(lowerExtension);
			return (lowerExtension == k_fileExtension);
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void CSParticleProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, 
			const Core::ResourceSPtr& out_resource)
		{
			CS_ASSERT(out_resource != nullptr, "resource cannot be null.");
			CS_ASSERT(out_resource->IsA<ParticleEffect>() == true, "resource must be a particle effect.");

			ParticleEffectSPtr particleEffect = std::static_pointer_cast<ParticleEffect>(out_resource);
			LoadCSParticle(in_location, in_filePath, nullptr, particleEffect);
		}
		//-----------------------------------------------------------------
		//------------------------------------------------------------------
		void CSParticleProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, 
			const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
		{
			CS_ASSERT(out_resource != nullptr, "resource cannot be null.");
			CS_ASSERT(out_resource->IsA<ParticleEffect>() == true, "resource must be a particle effect."); 
			CS_ASSERT(in_delegate != nullptr, "Async load delegate cannot be null.");

			ParticleEffectSPtr particleEffect = std::static_pointer_cast<ParticleEffect>(out_resource);
			LoadCSParticle(in_location, in_filePath, in_delegate, particleEffect);
		}
	}
}