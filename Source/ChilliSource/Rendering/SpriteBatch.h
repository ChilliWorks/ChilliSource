/*
 *  SpriteBatch.h
 *  moFloTest
 *
 *  Created by Scott Downie on 10/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_SPRITE_BATCH_H_
#define _MO_FLO_RENDERING_SPRITE_BATCH_H_

#include <moFlo/Rendering/ForwardDeclarations.h>
#include <moFlo/Rendering/MeshBuffer.h>
#include <moFlo/Rendering/Material.h>
#include <moFlo/Rendering/Components/SpriteComponent.h>

namespace moFlo
{
	namespace Rendering
	{
		class CSpriteBatch
		{
		public:
			CSpriteBatch(u32 inudwCapacity, IRenderSystem * inpRenderSystem, BufferUsage ineUsage);
			~CSpriteBatch();
			//------------------------------------------------------
			/// Build
			///
			/// Finalise the batch. Nothing can be changed once
			/// the batch is built
            ///
            /// @param Sprite array
			//------------------------------------------------------
			void Build(DYNAMIC_ARRAY<CSpriteComponent::SpriteData>* inpSprites);
			//------------------------------------------------------
			/// Render
			///
			/// Draw the contents of the mesh buffer
			///
			/// @param Active render system
            /// @param Offset into mesh buffer
            /// @param Stride within mesh buffer
			//------------------------------------------------------
			void Render(IRenderSystem* inpRenderSystem, const CMaterial& inMaterial, u32 inudwOffset, u32 inudwStride) const;
            //------------------------------------------------------
            /// Render
            ///
            /// Draw the contents of the mesh buffer
            ///
            /// @param Active render system
            //------------------------------------------------------
            void Render(IRenderSystem* inpRenderSystem, const CMaterial& inMaterial) const;
			//------------------------------------------------------
			/// Get Tag
			///
			/// Associated data usually used to determine layer
			//------------------------------------------------------
			s32 GetTag() const;
			//------------------------------------------------------
			/// Set Tag
			///
			/// Associated data usually used to determine layer
			//------------------------------------------------------
			void SetTag(s32 indwValue);
			//------------------------------------------------------
			/// Remap Sprite
			///
			/// Replaces the contents of a sprite in the batch buffer with the given one
			///
			/// @param Index of which sprite to replace
			/// @param New sprite to map over the contents
			//------------------------------------------------------
			void RemapSprite(u32 inudwIndex, const CSpriteComponent::SpriteData &inpSprite);
			
		private:
            //-------------------------------------------------------
            /// Map Sprite Into Buffer
            ///
            /// Load the given sprite data into the memory held by
            /// the vertex buffer
            ///
            /// @param Pointer to sprite offset in buffer
            /// @param Sprite to map
            //-------------------------------------------------------
			void MapSpriteIntoBuffer(CSpriteComponent::SpriteVertex* inpBuffer, const CSpriteComponent::SpriteData& inpSprite);
			//-------------------------------------------------------
			/// Build Indices For Number Sprites
			///
			/// If required sets indices for the given number of sprites.
			/// Skips over if the indices have already been filled in enough
			//------------------------------------------------------
			void BuildIndicesForNumberSprites(u32 inudwNumSprites);
            
		private:
			
			u32 mudwNumSpritesBuiltIndicesFor;
			
			s32 mdwTag;
			
			//---Render Buffer
			IMeshBuffer* mpSpriteBuffer;
		};
	}
}

#endif