//
//  ParticleComponentFactory.cpp
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

#include <ChilliSource/Rendering/Particles/ParticleComponentFactory.h>

#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Rendering/Particles/ParticleComponent.h>
#include <ChilliSource/Rendering/Particles/ParticleEffect.h>
#include <ChilliSource/Rendering/Particles/ParticleSystem.h>
#include <ChilliSource/Rendering/Particles/Affectors/ParticleAffector.h>
#include <ChilliSource/Rendering/Particles/Affectors/ParticleAffectorFactory.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(ParticleComponentFactory);
		
        //--------------------------------------------------------
        //--------------------------------------------------------
        ParticleComponentFactoryUPtr ParticleComponentFactory::Create(ParticleSystem* in_particleSystem, ParticleEmitterFactory* in_emitterFactory, ParticleAffectorFactory* in_affectorFactory)
        {
            return ParticleComponentFactoryUPtr(new ParticleComponentFactory(in_particleSystem, in_emitterFactory, in_affectorFactory));
        }
		//--------------------------------------------------------
		//--------------------------------------------------------
		ParticleComponentFactory::ParticleComponentFactory(ParticleSystem* in_particleSystem, ParticleEmitterFactory* in_emitterFactory, ParticleAffectorFactory* in_affectorFactory)
        : m_particleSystem(in_particleSystem), m_emitterFactory(in_emitterFactory), m_affectorFactory(in_affectorFactory)
		{
            CS_ASSERT(m_particleSystem != nullptr, "Particle factory cannot have null system");
            CS_ASSERT(m_emitterFactory != nullptr, "Particle factory cannot have null emitter factory");
            CS_ASSERT(m_affectorFactory != nullptr, "Particle factory cannot have null affector factory");
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
		bool ParticleComponentFactory::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ParticleComponentFactory::InterfaceID;
		}
        //--------------------------------------------------------
        //--------------------------------------------------------
        ParticleComponentUPtr ParticleComponentFactory::CreateParticleComponent()
		{
			ParticleComponentUPtr particleComp(new ParticleComponent());
			m_particleSystem->AddParticleComponent(particleComp.get());
			return particleComp;
		}
        //--------------------------------------------------------
        //--------------------------------------------------------
        ParticleComponentUPtr ParticleComponentFactory::CreateParticleComponent(const ParticleEffectCSPtr& in_effect)
		{
			ParticleComponent* particleComp(new ParticleComponent());
            
            const MaterialCSPtr& material = in_effect->GetMaterial();
            particleComp->SetMaterial(material);
            
            u32 numEmitters = in_effect->GetNumEmitters();
            for(u32 i=0; i<numEmitters; ++i)
            {
                const auto& properties = in_effect->GetEmitterDesc(i);
                const std::string& type = properties.ValueForKey("Type");
                auto emitter = m_emitterFactory->CreateParticleEmitter(type, properties, material, particleComp);
                CS_ASSERT(emitter != nullptr, "Cannot create particle emitter of type " + type);
                emitter->SetTextureAtlas(in_effect->GetAtlas());
                emitter->SetTextureAtlasId(in_effect->GetAtlasId());
                particleComp->AddEmitter(std::move(emitter));
            }
            
            u32 numAffectors = in_effect->GetNumAffectors();
            for(u32 i=0; i<numAffectors; ++i)
            {
                const auto& properties = in_effect->GetAffectorDesc(i);
                const std::string& type = properties.ValueForKey("Type");
                auto affector = m_affectorFactory->CreateParticleAffector(type, properties);
                CS_ASSERT(affector != nullptr, "Cannot create particle affector of type " + type);
                particleComp->AddAffector(std::move(affector));
            }
            
            m_particleSystem->AddParticleComponent(particleComp);
			return ParticleComponentUPtr(particleComp);
		}
	}
}