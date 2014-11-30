//
//  ParticleDrawable.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_DRAWABLE_PARTICLEDRAWABLE_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_DRAWABLE_PARTICLEDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particle/ConcurrentParticleData.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// The base class for all particle drawables. Particle drawables are 
		/// reponsible for the actual rendering  of the particles. 
		///
		/// Particle drawables are drawn from the main thread, however particles 
		/// are updated from a background task. Information is passed between 
		/// threads using the concurrent particle draw data array. This contains 
		/// just the information needed to render a particle and can be locked 
		/// before use.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class ParticleDrawable
		{
		public:
			CS_DECLARE_NOCOPY(ParticleDrawable);
			//----------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The entity the effect is attached to.
			/// @param The particle drawable definition.
			/// @param The concurrent particle data.
			//----------------------------------------------------------------
			ParticleDrawable(const Core::Entity* in_entity, const ParticleDrawableDef* in_drawableDef, ConcurrentParticleData* in_concurrentParticleData);
			//----------------------------------------------------------------
			/// Updates the particle drawable and renders all active particles
			/// in the effect. 
			///
			/// This must be called on the main thread.
			///
			/// @author Ian Copland
			///
			/// @param The camera component used to render.
			//----------------------------------------------------------------
			void Draw(const CameraComponent* in_camera);
			//----------------------------------------------------------------
			/// Destructor
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			virtual ~ParticleDrawable() {};
		protected:
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The entity the owning particle component is attached 
			/// to. This should never be null.
			//----------------------------------------------------------------
			const Core::Entity* GetEntity() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The particle drawable definition
			//----------------------------------------------------------------
			const ParticleDrawableDef* GetDrawableDef() const;
			//----------------------------------------------------------------
			/// Activates the particle with the given index.
			///
			/// This is called on the main thread, but the particle data can 
			/// be modified on other threads so make sure to lock it prior to 
			/// use.
			///
			/// @author Ian Copland
			///
			/// @param The particle draw data.
			/// @param The index of the particle to activate.
			//----------------------------------------------------------------
			virtual void ActivateParticle(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, u32 in_index) = 0;
			//----------------------------------------------------------------
			/// Renders all active particles in the effect. 
			///
			/// This is always called on the main thread.
			///
			/// @author Ian Copland
			///
			/// @param The particle draw data.
			/// @param The camera component used to render.
			//----------------------------------------------------------------
			virtual void DrawParticles(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, const CameraComponent* in_camera) = 0;
		private:
			const Core::Entity* m_entity = nullptr;
			const ParticleDrawableDef* m_drawableDef = nullptr;
			ConcurrentParticleData* m_concurrentParticleData = nullptr;
		};
	}
}

#endif