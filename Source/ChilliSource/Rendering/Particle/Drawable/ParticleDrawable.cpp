//
//  ParticleDrawable.cpp
//  Chilli Source
//  Created by Ian Copland on 13/10/2014.
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

#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawable.h>

#include <ChilliSource/Rendering/Particle/ConcurrentParticleData.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//----------------------------------------------
		//----------------------------------------------
		ParticleDrawable::ParticleDrawable(const Core::Entity* in_entity, const ParticleDrawableDef* in_drawableDef, ConcurrentParticleData* in_concurrentParticleData)
			: m_entity(in_entity), m_drawableDef(in_drawableDef), m_concurrentParticleData(in_concurrentParticleData)
		{
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		void ParticleDrawable::Draw(const CameraComponent* in_camera)
		{
			m_concurrentParticleData->Lock();

			auto newIndices = m_concurrentParticleData->TakeNewIndices();
			for (const auto& index : newIndices)
			{
				ActivateParticle(m_concurrentParticleData->GetParticles(), index);
			}

			DrawParticles(m_concurrentParticleData->GetParticles(), in_camera);

			m_concurrentParticleData->Unlock();
		}
		//----------------------------------------------
		//----------------------------------------------
		const Core::Entity* ParticleDrawable::GetEntity() const
		{
			return m_entity;
		}
		//----------------------------------------------
		//----------------------------------------------
		const ParticleDrawableDef* ParticleDrawable::GetDrawableDef() const
		{
			return m_drawableDef;
		}
	}
}