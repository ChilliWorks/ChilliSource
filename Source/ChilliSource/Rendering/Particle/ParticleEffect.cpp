//
//  ParticleEffect.cpp
//  Chilli Source
//  Created by Ian Copland on 06/10/2014.
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

#include <ChilliSource/Rendering/Particle/ParticleEffect.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(ParticleEffect);
		//-------------------------------------------------------
		//-------------------------------------------------------
		ParticleEffectUPtr ParticleEffect::Create()
		{
			return ParticleEffectUPtr(new ParticleEffect());
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ParticleEffect::ParticleEffect()
		{
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool ParticleEffect::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (ParticleEffect::InterfaceID == in_interfaceId);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		f32 ParticleEffect::GetDuration() const
		{
			return m_duration;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		u32 ParticleEffect::GetMaxParticles() const
		{
			return m_maxParticles;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ParticleEffect::SimulationSpace ParticleEffect::GetSimulationSpace() const
		{
			return m_simulationSpace;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const ParticleProperty<f32>* ParticleEffect::GetLifetimeProperty() const
		{
			return m_lifetimeProperty.get();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const ParticleProperty<Core::Vector2>* ParticleEffect::GetInitialScaleProperty() const
		{
			return m_initialScaleProperty.get();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const ParticleProperty<f32>* ParticleEffect::GetInitialRotationProperty() const
		{
			return m_initialRotationProperty.get();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const ParticleProperty<Core::Colour>* ParticleEffect::GetInitialColourProperty() const
		{
			return m_initialColourProperty.get();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const ParticleProperty<f32>* ParticleEffect::GetInitialSpeedProperty() const
		{
			return m_initialSpeedProperty.get();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const ParticleProperty<f32>* ParticleEffect::GetInitialAngularVelocityProperty() const
		{
			return m_initialAngularVelocityProperty.get();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const ParticleDrawableDef* ParticleEffect::GetDrawableDef() const
		{
			return m_drawableDef.get();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const ParticleEmitterDef* ParticleEffect::GetEmitterDef() const
		{
			return m_emitterDef.get();
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		std::vector<const ParticleAffectorDef*> ParticleEffect::GetAffectorDefs() const
		{
			std::vector<const ParticleAffectorDef*> output;

			for (const auto& affectorDef : m_affectorDefs)
			{
				output.push_back(affectorDef.get());
			}

			return output;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetDuration(f32 in_duration)
		{
			m_duration = in_duration;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetMaxParticles(u32 in_maxParticles)
		{
			m_maxParticles = in_maxParticles;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetSimulationSpace(SimulationSpace in_simulationSpace)
		{
			m_simulationSpace = in_simulationSpace;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetLifetimeProperty(ParticlePropertyUPtr<f32> in_lifetimeProperty)
		{
			m_lifetimeProperty = std::move(in_lifetimeProperty);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetInitialScaleProperty(ParticlePropertyUPtr<Core::Vector2> in_initialScaleProperty)
		{
			m_initialScaleProperty = std::move(in_initialScaleProperty);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetInitialRotationProperty(ParticlePropertyUPtr<f32> in_initialRotationProperty)
		{
			m_initialRotationProperty = std::move(in_initialRotationProperty);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetInitialColourProperty(ParticlePropertyUPtr<Core::Colour> in_initialColourProperty)
		{
			m_initialColourProperty = std::move(in_initialColourProperty);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetInitialSpeedProperty(ParticlePropertyUPtr<f32> in_initialSpeedProperty)
		{
			m_initialSpeedProperty = std::move(in_initialSpeedProperty);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetInitialAngularVelocityProperty(ParticlePropertyUPtr<f32> in_initialAngularVelocityProperty)
		{
			m_initialAngularVelocityProperty = std::move(in_initialAngularVelocityProperty);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetDrawableDef(ParticleDrawableDefUPtr in_drawableDef)
		{
			CS_ASSERT(m_drawableDef == nullptr, "Cannot change the drawable definition in a Particle Effect.");

			m_drawableDef = std::move(in_drawableDef);
			m_drawableDef->SetParticleEffect(this);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetEmitterDef(ParticleEmitterDefUPtr in_emitterDef)
		{
			CS_ASSERT(m_emitterDef == nullptr, "Cannot change the drawable definition in a Particle Effect.");

			m_emitterDef = std::move(in_emitterDef);
			m_emitterDef->SetParticleEffect(this);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void ParticleEffect::SetAffectorDefs(std::vector<ParticleAffectorDefUPtr> in_affectorDefs)
		{
			CS_ASSERT(m_affectorDefs.empty() == true, "Cannot change the affector definitions in a Particle Effect.");

			m_affectorDefs = std::move(in_affectorDefs);
			for (auto& affectorDef : m_affectorDefs)
			{
				affectorDef->SetParticleEffect(this);
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ParticleEffect::~ParticleEffect()
		{
			m_lifetimeProperty.reset();
			m_initialScaleProperty.reset();
			m_initialRotationProperty.reset();
			m_initialColourProperty.reset();
			m_initialSpeedProperty.reset();
			m_initialAngularVelocityProperty.reset();

			m_drawableDef.reset();
			m_emitterDef.reset();
			m_affectorDefs.clear();
		}
	}
}