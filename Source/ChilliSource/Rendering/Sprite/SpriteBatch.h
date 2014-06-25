//
//  SpriteBatch.h
//  Chilli Source
//  Created by Scott Downie on 10/12/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_SPRITE_BATCH_H_
#define _CHILLISOURCE_RENDERING_SPRITE_BATCH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class SpriteBatch
		{
		public:
			SpriteBatch(u32 inudwCapacity, RenderSystem * inpRenderSystem, BufferUsage ineUsage);
			~SpriteBatch();
			//------------------------------------------------------
			/// Build
			///
			/// Finalise the batch. Nothing can be changed once
			/// the batch is built
            ///
            /// @param Sprite array
			//------------------------------------------------------
			void Build(std::vector<SpriteComponent::SpriteData>* inpSprites);
			//------------------------------------------------------
			/// Render
			///
			/// Draw the contents of the mesh buffer
			///
            /// @param Offset into mesh buffer
            /// @param Stride within mesh buffer
			//------------------------------------------------------
			void Render(const MaterialCSPtr& inMaterial, u32 inudwOffset, u32 inudwStride) const;
            //------------------------------------------------------
            /// Render
            ///
            /// Draw the contents of the mesh buffer
            ///
            /// @param Active render system
            //------------------------------------------------------
            void Render(const MaterialCSPtr& inMaterial) const;
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
			void RemapSprite(u32 inudwIndex, const SpriteComponent::SpriteData &inpSprite);
			
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
			void MapSpriteIntoBuffer(SpriteComponent::SpriteVertex* inpBuffer, const SpriteComponent::SpriteData& inpSprite);
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
			MeshBuffer* mpSpriteBuffer;
            
            RenderSystem* m_renderSystem;
		};
	}
}

#endif