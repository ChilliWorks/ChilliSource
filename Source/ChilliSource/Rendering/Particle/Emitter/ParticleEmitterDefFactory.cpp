//
//  ParticleEmitterDefFactory.cpp
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

#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDefFactory.h>

#include <ChilliSource/Rendering/Particle/Emitter/CircleParticleEmitterDef.h>
#include <ChilliSource/Rendering/Particle/Emitter/ConeParticleEmitterDef.h>
#include <ChilliSource/Rendering/Particle/Emitter/PointParticleEmitterDef.h>
#include <ChilliSource/Rendering/Particle/Emitter/SphereParticleEmitterDef.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(ParticleEmitterDefFactory);
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		ParticleEmitterDefFactoryUPtr ParticleEmitterDefFactory::Create()
		{
			return ParticleEmitterDefFactoryUPtr(new ParticleEmitterDefFactory());
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		bool ParticleEmitterDefFactory::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (ParticleEmitterDefFactory::InterfaceID == in_interfaceId);
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		ParticleEmitterDefUPtr ParticleEmitterDefFactory::CreateEmitterDef(const std::string& in_name, const Json::Value& in_jsonParams, const ParticleEmitterDef::LoadedDelegate& in_asyncLoadDelegate) const
		{
			auto delegateIt = m_creatorDelegateMap.find(in_name);
			CS_ASSERT(delegateIt != m_creatorDelegateMap.end(), "Could not create particle emitter def with name: " + in_name);

			return delegateIt->second(in_jsonParams, in_asyncLoadDelegate);
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void ParticleEmitterDefFactory::OnInit()
		{
			RegisterEmitterDef<CircleParticleEmitterDef>("Circle");
			RegisterEmitterDef<ConeParticleEmitterDef>("Cone");
			RegisterEmitterDef<PointParticleEmitterDef>("Point");
			RegisterEmitterDef<SphereParticleEmitterDef>("Sphere");
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void ParticleEmitterDefFactory::OnDestroy()
		{
			m_creatorDelegateMap.clear();
		}
	}
}