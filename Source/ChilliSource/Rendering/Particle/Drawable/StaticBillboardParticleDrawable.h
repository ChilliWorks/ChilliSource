//
//  StaticBillboardParticleDrawable.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_DRAWABLE_BILLBOARDPARTICLEDRAWABLE_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_DRAWABLE_BILLBOARDPARTICLEDRAWABLE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/dynamic_array.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawable.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// A particle drawable for rendering particles as billboards.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		class StaticBillboardParticleDrawable final : public ParticleDrawable
		{
		private:
			friend class StaticBillboardParticleDrawableDef;
			//----------------------------------------------------------------
			/// A container for information on a single billboard, such as
			/// the UVs and the local vertex position data.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			struct BillboardData
			{
				UVs m_uvs;
				Core::Vector2 m_bottomLeft;
				Core::Vector2 m_topRight;
			};
			//----------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The entity the effect is attached to.
			/// @param The particle drawable definition.
			/// @param The concurrent particle data.
			//----------------------------------------------------------------
			StaticBillboardParticleDrawable(const Core::Entity* in_entity, const ParticleDrawableDef* in_drawableDef, ConcurrentParticleData* in_concurrentParticleData);
			//----------------------------------------------------------------
			/// Activates the particle with the given index.
			///
			/// @author Ian Copland
			///
			/// @param The particle draw data.
			/// @param The index of the particle to activate.
			//----------------------------------------------------------------
			void ActivateParticle(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, u32 in_index) override;
			//----------------------------------------------------------------
			/// Renders all active particles in the effect.
			///
			/// @author Ian Copland
			///
			/// @param The particle draw data.
			/// @param The camera component used to render.
			//----------------------------------------------------------------
			void DrawParticles(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, const CameraComponent* in_camera) override;
			//----------------------------------------------------------------
			/// Builds the billboard image data from the provided texture
			/// or texture atlas.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void BuildBillboardImageData();
			//----------------------------------------------------------------
			/// Prepares the particle billboard indices. This will either set
			/// them in order, or randomise them based on the settings for
			/// the drawable.
			///
			/// @author Ian Copland
			//----------------------------------------------------------------
			void PrepareParticleBillboardIndices();
			//----------------------------------------------------------------
			/// Draws the particles taking into account the world space
			/// transform of the owning entity.
			///
			/// @author Ian Copland
			///
			/// @param The particle draw data.
			/// @param The camera component used to render.
			//----------------------------------------------------------------
			void DrawLocalSpace(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, const CameraComponent* in_camera) const;
			//----------------------------------------------------------------
			/// Draws the particles without taking into account the world
			/// space transform of the owning entity as the particles are
			/// already in world space.
			///
			/// @author Ian Copland
			///
			/// @param The particle draw data.
			/// @param The camera component used to render.
			//----------------------------------------------------------------
			void DrawWorldSpace(const Core::dynamic_array<ConcurrentParticleData::Particle>& in_particleData, const CameraComponent* in_camera) const;

			const StaticBillboardParticleDrawableDef* m_billboardDrawableDef;
			std::unique_ptr <Core::dynamic_array<BillboardData>> m_billboards;
			Core::dynamic_array<u32> m_particleBillboardIndices;
			u32 m_nextBillboardIndex = 0;
		};
	}
}

#endif