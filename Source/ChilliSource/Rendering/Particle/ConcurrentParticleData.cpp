//
//  ConcurrentParticleData.cpp
//  Chilli Source
//  Created by Ian Copland on 02/11/2014.
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

#include <ChilliSource/Rendering/Particle/ConcurrentParticleData.h>

#include <ChilliSource/Rendering/Particle/Particle.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		ConcurrentParticleData::ConcurrentParticleData(u32 in_particleCount)
			: m_particles(in_particleCount), m_lock(m_mutex, std::defer_lock)
		{
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		bool ConcurrentParticleData::StartUpdate()
		{
			std::unique_lock<std::recursive_mutex> lock(m_mutex);
			if (m_updating == false)
			{
				m_updating = true;
				return true;
			}

			return false;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		bool ConcurrentParticleData::HasActiveParticles() const
		{
			std::unique_lock<std::recursive_mutex> lock(m_mutex);
			return m_activeParticles;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		Core::AABB ConcurrentParticleData::GetAABB() const
		{
			std::unique_lock<std::recursive_mutex> lock(m_mutex);
			return m_aabb;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		Core::Sphere ConcurrentParticleData::GetBoundingSphere() const
		{
			std::unique_lock<std::recursive_mutex> lock(m_mutex);
			return m_boundingSphere;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void ConcurrentParticleData::Lock() const
		{
			m_lock.lock();
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		std::vector<u32> ConcurrentParticleData::TakeNewIndices()
		{
			CS_ASSERT(m_lock.owns_lock() == true, "Must be locked when taking new indices!")

			std::vector<u32> output = m_newParticleIndices;
			m_newParticleIndices.clear();
			return output;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		const Core::dynamic_array<ConcurrentParticleData::Particle>& ConcurrentParticleData::GetParticles() const
		{
			CS_ASSERT(m_lock.owns_lock() == true, "Must be locked when getting particles!");

			return m_particles;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void ConcurrentParticleData::Unlock() const
		{
			m_lock.unlock();
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void ConcurrentParticleData::CommitParticleData(const Core::dynamic_array<Rendering::Particle>* in_particles, const std::vector<u32>& in_newIndices, const Core::AABB& in_aabb, const Core::Sphere& in_boundingSphere)
		{
			std::unique_lock<std::recursive_mutex> lock(m_mutex);

			CS_ASSERT(in_particles->size() == m_particles.size(), "Particle data lists must be the same size.");

			m_activeParticles = false;
			for (u32 i = 0; i < m_particles.size(); ++i)
			{
				Particle& concurrentParticle = m_particles[i];
				const Rendering::Particle& particle = (*in_particles)[i];

				concurrentParticle.m_isActive = particle.m_isActive;
				concurrentParticle.m_position = particle.m_position;
				concurrentParticle.m_rotation = particle.m_rotation;
				concurrentParticle.m_scale = particle.m_scale;
				concurrentParticle.m_colour = particle.m_colour;

				if (concurrentParticle.m_isActive == true)
				{
					m_activeParticles = true;
				}
			}

			m_newParticleIndices.insert(m_newParticleIndices.end(), in_newIndices.begin(), in_newIndices.end());
			m_aabb = in_aabb;
			m_boundingSphere = in_boundingSphere;
			m_updating = false;
		}
	}
}