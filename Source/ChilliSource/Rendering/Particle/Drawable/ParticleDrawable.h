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
			/// @param The concurrent particle draw data array.
			//----------------------------------------------------------------
			ParticleDrawable(const CSCore::Entity* in_entity, const ParticleDrawableDef* in_drawableDef, const Core::concurrent_dynamic_array<ParticleDrawData>* in_particleDrawDataArray);
			//----------------------------------------------------------------
			/// Renders all active particles in the effect. 
			///
			/// This is called on the main thread, but the particle draw data
			/// array can be modified on other threads so make sure to lock it
			/// prior to use.
			///
			/// @author Ian Copland
			///
			/// @param The camera component used to render.
			//----------------------------------------------------------------
			virtual void Draw(const CSRendering::CameraComponent* in_camera) = 0;
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
			const CSCore::Entity* GetEntity() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The particle drawable definition
			//----------------------------------------------------------------
			const ParticleDrawableDef* GetDrawableDef() const;
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The concurrent particle draw data array.
			//----------------------------------------------------------------
			const Core::concurrent_dynamic_array<ParticleDrawData>& GetParticleDrawDataArray() const;
		private:
			const CSCore::Entity* m_entity = nullptr;
			const ParticleDrawableDef* m_drawableDef = nullptr;
			const Core::concurrent_dynamic_array<ParticleDrawData>* m_particleDrawDataArray = nullptr;
		};
	}
}

#endif