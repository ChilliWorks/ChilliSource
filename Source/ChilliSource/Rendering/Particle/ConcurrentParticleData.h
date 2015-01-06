//
//  ConcurrentParticleData.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_CONCURRENTPARTICLEDATA_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_CONCURRENTPARTICLEDATA_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Container/dynamic_array.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

#include <mutex>
#include <vector>

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------------------------------------------
		/// A container for particle effect data that needs to be shared across
		/// the main thread and the particle update thread. This includes the
		/// draw information for each particle, the list of newly updated particles
		/// and the total bounds of the particle effect.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------
		class ConcurrentParticleData final
		{
		public:
			//-----------------------------------------------------------------
			/// A struct containing just the information required for drawing a
			/// particle.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			struct Particle final
			{
				bool m_isActive = false;
				Core::Vector3 m_position;
                Core::Vector2 m_scale = Core::Vector2::k_zero;
				f32 m_rotation = 0.0f;
				Core::Colour m_colour = Core::Colour::k_white;
			};
			//-----------------------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param The number of particles.
			//-----------------------------------------------------------------
			ConcurrentParticleData(u32 in_particleCount);
			//-----------------------------------------------------------------
			/// This will return false if no particle data has been commited
			/// since the last time this was called. If false is returned a 
			/// new update should not be started.
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @param Whether or not the update can start.
			//-----------------------------------------------------------------
			bool StartUpdate();
			//-----------------------------------------------------------------
			/// This is thread-safe, lock doesn't need to be called first.
			///
			/// @author Ian Copland
			///
			/// @param Whether or not there are any active particles.
			//-----------------------------------------------------------------
			bool HasActiveParticles() const;
			//-----------------------------------------------------------------
			/// This is thread-safe, lock doesn't need to be called first.
			///
			/// @author Ian Copland
			///
			/// @author The AABB for the particle effect. Whether or not this
			/// is in world or local space is determined by the simulation space
			/// of a particle.
			//-----------------------------------------------------------------
			Core::AABB GetAABB() const;
			//-----------------------------------------------------------------
			/// This is thread-safe, lock doesn't need to be called first.
			///
			/// @author Ian Copland
			///
			/// @author The bounding sphere for the particle effect. Whether or 
			/// not this is in world or local space is determined by the 
			/// simulation space of a particle.
			//-----------------------------------------------------------------
			Core::Sphere GetBoundingSphere() const;
			//-----------------------------------------------------------------
			/// Locks the container so other threads cannot update it while
			/// accessing the new indices and particle list.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void Lock() const;
			//-----------------------------------------------------------------
			/// Returns the list of indices that have changed since the last
			/// time this was called. The list will be cleared when called.
			/// Before this is called the container must be locked to ensure
			/// that new particles are not activated prior to being rendered.
			/// If the container is not locked the app is considered to be
			/// in an irrecoverable state and will terminate.
			/// 
			/// @author Ian Copland
			///
			/// @author A vector of particle indices.
			//-----------------------------------------------------------------
			std::vector<u32> TakeNewIndices();
			//-----------------------------------------------------------------
			/// Before this is called the container must be locked to ensure
			/// that any iteration over the particle data is safe. If not the
			/// app is considered to be in an irrecoverable state and will
			/// terminate.
			///
			/// @author Ian Copland
			///
			/// @param The particle list.
			//-----------------------------------------------------------------
			const Core::dynamic_array<ConcurrentParticleData::Particle>& GetParticles() const;
			//-----------------------------------------------------------------
			/// Unlocks the container. This should be called as soon as possible
			/// after dealing with data that needs to be locked.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void Unlock() const;
			//-----------------------------------------------------------------
			/// Updates the particle data.
			///
			/// This is thread-safe, lock doesn't need to be called first.
			///
			/// @author Ian Copland
			///
			/// @param The list of particles.
			/// @param The new indices.
			/// @param The aabb.
			/// @param The obb.
			/// @param The bounding sphere.
			//-----------------------------------------------------------------
			void CommitParticleData(const Core::dynamic_array<Rendering::Particle>* in_particles, const std::vector<u32>& in_newIndices, const Core::AABB& in_aabb, const Core::Sphere& in_boundingSphere);
		private:

			Core::dynamic_array<ConcurrentParticleData::Particle> m_particles;
			std::vector<u32> m_newParticleIndices;
			Core::AABB m_aabb;
			Core::Sphere m_boundingSphere;
			bool m_updating = false;
			bool m_activeParticles = false;
			
			mutable std::recursive_mutex m_mutex;
			mutable std::unique_lock<std::recursive_mutex> m_lock;
		};
	}
}

#endif