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
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDefFactory.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDefFactory.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDefFactory.h>
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
			/// A delegate called when one of the asynchronous loading stages 
			/// has completed.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			using AsyncCompleteDelegate = std::function<void()>;
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
			/// Reads the base properties in the particle effect such as the
			/// effect duration, the number of particles and the initial
			/// particle values.
			///
			/// This is thread-safe.
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
				if (jsonValue.isNull() == false)
				{
					CS_ASSERT(jsonValue.isString(), "Duration value must be a string.");
					out_particleEffect->SetDuration(Core::ParseF32(jsonValue.asString()));
				}

				//Max Particles
				jsonValue = in_jsonRoot.get("MaxParticles", Json::nullValue);
				if (jsonValue.isNull() == false)
				{
					CS_ASSERT(jsonValue.isString(), "Max particles value must be a string.");
					out_particleEffect->SetMaxParticles(Core::ParseU32(jsonValue.asString()));
				}

				//Simulation Space
				jsonValue = in_jsonRoot.get("SimulationSpace", Json::nullValue);
				if (jsonValue.isNull() == false)
				{
					CS_ASSERT(jsonValue.isString(), "SimulationSpace value must be a string.");
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
			/// Reads the drawable def from the csparticle json.
			///
			/// This is not thread safe and must be run on the main thread.
			/// ReadDrawableDefAsync() should be used for background loading.
			///
			/// @author Ian Copland
			///
			/// @param The root json object
			/// @param The particle drawable def factory.
			/// @param [Out] The particle effect that should be populated.
			//-----------------------------------------------------------------
			void ReadDrawableDef(const Json::Value& in_jsonRoot, const ParticleDrawableDefFactory* in_drawableDefFactory, const ParticleEffectSPtr& out_particleEffect)
			{
				Json::Value drawableJson = in_jsonRoot.get("Drawable", Json::nullValue);
				CS_ASSERT(drawableJson.isNull() == false && drawableJson.isObject() == true, "CSParticle file '" + out_particleEffect->GetName() + "' does not contain a drawable.");

				Json::Value typeJson = drawableJson.get("Type", Json::nullValue);
				CS_ASSERT(typeJson.isNull() == false && typeJson.isString() == true, "CSParticle file '" + out_particleEffect->GetName() + "' has a drawable with an invalid type.");

				out_particleEffect->SetDrawableDef(in_drawableDefFactory->CreateInstance(typeJson.asString(), drawableJson));
			}
			//-----------------------------------------------------------------
			/// Reads the emitter def from the csparticle json.
			///			
			/// This is not thread safe and must be run on the main thread.
			/// ReadEmitterDefAsync() should be used for background loading.
			///
			/// @author Ian Copland
			///
			/// @param The root json object
			/// @param The particle emitter def factory.
			/// @param [Out] The particle effect that should be populated.
			//-----------------------------------------------------------------
			void ReadEmitterDef(const Json::Value& in_jsonRoot, const ParticleEmitterDefFactory* in_emitterDefFactory, const ParticleEffectSPtr& out_particleEffect)
			{
				Json::Value emitterJson = in_jsonRoot.get("Emitter", Json::nullValue);
				CS_ASSERT(emitterJson.isNull() == false && emitterJson.isObject() == true, "CSParticle file '" + out_particleEffect->GetName() + "' does not contain an emitter.");

				Json::Value typeJson = emitterJson.get("Type", Json::nullValue);
				CS_ASSERT(typeJson.isNull() == false && typeJson.isString() == true, "CSParticle file '" + out_particleEffect->GetName() + "' has an emitter with an invalid type.");

				out_particleEffect->SetEmitterDef(in_emitterDefFactory->CreateInstance(typeJson.asString(), emitterJson));
			}
			//-----------------------------------------------------------------
			/// Reads the affector defs from the csparticle json.
			///			
			/// This is not thread safe and must be run on the main thread.
			/// ReadAffectorDefsAsync() should be used for background loading.
			///
			/// @author Ian Copland
			///
			/// @param The root json object
			/// @param The particle affector def factory.
			/// @param [Out] The particle effect that should be populated.
			//-----------------------------------------------------------------
			void ReadAffectorDefs(const Json::Value& in_jsonRoot, const ParticleAffectorDefFactory* in_affectorDefFactory, const ParticleEffectSPtr& out_particleEffect)
			{
				Json::Value affectorsJson = in_jsonRoot.get("Affectors", Json::nullValue);
				if (affectorsJson.isNull() == false)
				{
					CS_ASSERT(affectorsJson.isArray() == true, "CSParticle file '" + out_particleEffect->GetName() + "' contains an 'Affectors' object that isn't an array.");

					std::vector<ParticleAffectorDefUPtr> affectorDefs;
					for (const Json::Value& affectorJson : affectorsJson)
					{
						CS_ASSERT(affectorJson.isObject() == true, "CSParticle file '" + out_particleEffect->GetName() + "' contains affector json that isn't an object.");

						Json::Value typeJson = affectorJson.get("Type", Json::nullValue);
						CS_ASSERT(typeJson.isNull() == false && typeJson.isString() == true, "CSParticle file '" + out_particleEffect->GetName() + "' has an affector with an invalid type.");

						affectorDefs.push_back(in_affectorDefFactory->CreateInstance(typeJson.asString(), affectorJson));
					}

					out_particleEffect->SetAffectorDefs(std::move(affectorDefs));
				}
			}
			//-----------------------------------------------------------------
			/// Reads the CSParticle json file and populates the particle effect.
			///
			/// This is not thread safe and must be run on the main thread.
			/// LoadCSParticleAsync() should be used for background loading.
			///
			/// @author Ian Copland
			///
			/// @param The storage location of the csparticle json file.
			/// @param The file path to the csparticle json file.
			/// @param The particle drawable def factory.
			/// @param The particle emitter def factory.
			/// @param The particle affector def factory.
			/// @param [Out] The particle effect that should be populated.
			//-----------------------------------------------------------------
			void LoadCSParticle(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const ParticleDrawableDefFactory* in_drawableDefFactory,
				const ParticleEmitterDefFactory* in_emitterDefFactory, const ParticleAffectorDefFactory* in_affectorDefFactory, const ParticleEffectSPtr& out_particleEffect)
			{
				Json::Value jsonRoot;
                if (Core::JsonUtils::ReadJson(in_storageLocation, in_filePath, jsonRoot) == false)
				{
					out_particleEffect->SetLoadState(Core::Resource::LoadState::k_failed);
					return;
				}

				ReadBaseValues(jsonRoot, out_particleEffect);
				ReadDrawableDef(jsonRoot, in_drawableDefFactory, out_particleEffect);
				ReadEmitterDef(jsonRoot, in_emitterDefFactory, out_particleEffect);
				ReadAffectorDefs(jsonRoot, in_affectorDefFactory, out_particleEffect);

				out_particleEffect->SetLoadState(Core::Resource::LoadState::k_loaded);
			}
			//-----------------------------------------------------------------
			/// Reads the drawable def from the csparticle json asynchronously.
			///
			/// @author Ian Copland
			///
			/// @param The root json object
			/// @param The particle drawable def factory.
			/// @param [Out] The particle effect that should be populated.
			/// @param The completion delegate.
			//-----------------------------------------------------------------
			void ReadDrawableDefAsync(const Json::Value& in_jsonRoot, const ParticleDrawableDefFactory* in_drawableDefFactory, const ParticleEffectSPtr& out_particleEffect, 
				const AsyncCompleteDelegate& in_completionDelegate)
			{
				Json::Value drawableJson = in_jsonRoot.get("Drawable", Json::nullValue);
				CS_ASSERT(drawableJson.isNull() == false && drawableJson.isObject() == true, "CSParticle file '" + out_particleEffect->GetName() + "' does not contain a drawable.");

				Json::Value typeJson = drawableJson.get("Type", Json::nullValue);
				CS_ASSERT(typeJson.isNull() == false && typeJson.isString() == true, "CSParticle file '" + out_particleEffect->GetName() + "' has a drawable with an invalid type.");

				in_drawableDefFactory->CreateInstanceAsync(typeJson.asString(), drawableJson, [=](ParticleDrawableDefUPtr in_drawableDef)
				{
					out_particleEffect->SetDrawableDef(std::move(in_drawableDef));
					in_completionDelegate();
				});
			}
			//-----------------------------------------------------------------
			/// Reads the emitter def from the csparticle json asynchronously.
			///
			/// @author Ian Copland
			///
			/// @param The root json object
			/// @param The particle emitter def factory.
			/// @param [Out] The particle effect that should be populated.
			/// @param The completion delegate.
			//-----------------------------------------------------------------
			void ReadEmitterDefAsync(const Json::Value& in_jsonRoot, const ParticleEmitterDefFactory* in_emitterDefFactory, const ParticleEffectSPtr& out_particleEffect, 
				const AsyncCompleteDelegate& in_completionDelegate)
			{
				Json::Value emitterJson = in_jsonRoot.get("Emitter", Json::nullValue);
				CS_ASSERT(emitterJson.isNull() == false && emitterJson.isObject() == true, "CSParticle file '" + out_particleEffect->GetName() + "' does not contain an emitter.");

				Json::Value typeJson = emitterJson.get("Type", Json::nullValue);
				CS_ASSERT(typeJson.isNull() == false && typeJson.isString() == true, "CSParticle file '" + out_particleEffect->GetName() + "' has an emitter with an invalid type.");

				in_emitterDefFactory->CreateInstanceAsync(typeJson.asString(), emitterJson, [=](ParticleEmitterDefUPtr in_emitterDef)
				{
					out_particleEffect->SetEmitterDef(std::move(in_emitterDef));
					in_completionDelegate();
				});
			}
			//-----------------------------------------------------------------
			/// Reads a single affector def from the csparticle json 
			/// asynchronously.
			///
			/// @author Ian Copland
			///
			/// @param The vetor of affector def json.
			/// @param The current index into the vector.
			/// @param The particle affector def factory.
			/// @param [Out] The particle effect that should be populated.
			/// @param The completion delegate.
			//-----------------------------------------------------------------
			void ReadAffectorDefAsync(const std::vector<Json::Value>& in_affectorDefJsons, u32 in_affectorIndex, const ParticleAffectorDefFactory* in_affectorDefFactory, 
				std::shared_ptr<std::vector<ParticleAffectorDefUPtr>> in_particleAffectorDefs, const AsyncCompleteDelegate& in_completionDelegate)
			{
				Json::Value affectorJson = in_affectorDefJsons[in_affectorIndex];
				Json::Value typeJson = affectorJson.get("Type", Json::nullValue);
				CS_ASSERT(typeJson.isNull() == false && typeJson.isString() == true, "Invalid affector type.");

				in_affectorDefFactory->CreateInstanceAsync(typeJson.asString(), affectorJson, [=](ParticleAffectorDefUPtr in_drawableDef)
				{
					in_particleAffectorDefs->push_back(std::move(in_drawableDef));

					u32 nextIndex = in_affectorIndex + 1;
					if (nextIndex < in_affectorDefJsons.size())
					{
						ReadAffectorDefAsync(in_affectorDefJsons, nextIndex, in_affectorDefFactory, in_particleAffectorDefs, in_completionDelegate);
					}
					else
					{
						in_completionDelegate();
					}
				});
			}
			//-----------------------------------------------------------------
			/// Reads the affector defs from the csparticle json asynchronously.
			///
			/// @author Ian Copland
			///
			/// @param The root json object
			/// @param The particle affector def factory.
			/// @param [Out] The particle effect that should be populated.
			/// @param The completion delegate.
			//-----------------------------------------------------------------
			void ReadAffectorDefsAsync(const Json::Value& in_jsonRoot, const ParticleAffectorDefFactory* in_affectorDefFactory, const ParticleEffectSPtr& out_particleEffect, 
				const AsyncCompleteDelegate& in_completionDelegate)
			{
				Json::Value affectorsJson = in_jsonRoot.get("Affectors", Json::nullValue);
				if (affectorsJson.isNull() == false)
				{
					CS_ASSERT(affectorsJson.isArray() == true, "CSParticle file '" + out_particleEffect->GetName() + "' contains an 'Affectors' object that isn't an array.");

					std::vector<Json::Value> affectorJsons;
					for (const Json::Value& affectorJson : affectorsJson)
					{
						CS_ASSERT(affectorJson.isObject() == true, "CSParticle file '" + out_particleEffect->GetName() + "' contains affector json that isn't an object.");
						affectorJsons.push_back(affectorJson);
					}
					CS_ASSERT(affectorJsons.empty() == false, "CSParticle file '" + out_particleEffect->GetName() + "' has Affectors key with no affectors.");

					std::shared_ptr<std::vector<ParticleAffectorDefUPtr>> particleAffectorDefs(new std::vector<ParticleAffectorDefUPtr>());
					ReadAffectorDefAsync(affectorJsons, 0, in_affectorDefFactory, particleAffectorDefs, [=]()
					{
						out_particleEffect->SetAffectorDefs(std::move(*particleAffectorDefs));
						in_completionDelegate();
					});
				}
				else
				{
					in_completionDelegate();
				}
			}
			//-----------------------------------------------------------------
			/// Asynchonously Reads the CSParticle json file and populates the 
			/// particle effect.
			///
			/// @author Ian Copland
			///
			/// @param The storage location of the csparticle json file.
			/// @param The file path to the csparticle json file.
			/// @param The particle drawable def factory.
			/// @param The particle emitter def factory.
			/// @param The particle affector def factory.
			/// @param [Out] The particle effect that should be populated.
			//-----------------------------------------------------------------
			void LoadCSParticleAsync(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const ParticleDrawableDefFactory* in_drawableDefFactory,
				const ParticleEmitterDefFactory* in_emitterDefFactory, const ParticleAffectorDefFactory* in_affectorDefFactory, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate,
				const ParticleEffectSPtr& out_particleEffect)
			{
				Json::Value jsonRoot;
				if (Core::JsonUtils::ReadJson(in_storageLocation, in_filePath, jsonRoot) == false)
				{
					out_particleEffect->SetLoadState(Core::Resource::LoadState::k_failed);
					Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_particleEffect));
					return;
				}

				ReadBaseValues(jsonRoot, out_particleEffect);

				ReadDrawableDefAsync(jsonRoot, in_drawableDefFactory, out_particleEffect, [=]()
				{
					ReadEmitterDefAsync(jsonRoot, in_emitterDefFactory, out_particleEffect, [=]()
					{
						ReadAffectorDefsAsync(jsonRoot, in_affectorDefFactory, out_particleEffect, [=]()
						{
							out_particleEffect->SetLoadState(Core::Resource::LoadState::k_loaded);
							Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_particleEffect));
						});
					});
				});
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
			LoadCSParticle(in_location, in_filePath, m_drawableDefFactory, m_emitterDefFactory, m_affectorDefFactory, particleEffect);
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
			Core::Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(&LoadCSParticleAsync, in_location, in_filePath, m_drawableDefFactory, m_emitterDefFactory, m_affectorDefFactory, in_delegate, particleEffect));
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void CSParticleProvider::OnInit()
		{
			m_drawableDefFactory = Core::Application::Get()->GetSystem<ParticleDrawableDefFactory>();
			CS_ASSERT(m_drawableDefFactory != nullptr, "CSParticle Provider is missing required system: ParticleDrawableDefFactory.");

			m_emitterDefFactory = Core::Application::Get()->GetSystem<ParticleEmitterDefFactory>();
			CS_ASSERT(m_emitterDefFactory != nullptr, "CSParticle Provider is missing required system: ParticleEmitterDefFactory.");

			m_affectorDefFactory = Core::Application::Get()->GetSystem<ParticleAffectorDefFactory>();
			CS_ASSERT(m_affectorDefFactory != nullptr, "CSParticle Provider is missing required system: ParticleAffectorDefFactory.");
		}
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		void CSParticleProvider::OnDestroy()
		{
			m_drawableDefFactory = nullptr;
			m_emitterDefFactory = nullptr;
			m_affectorDefFactory = nullptr;
		}
	}
}