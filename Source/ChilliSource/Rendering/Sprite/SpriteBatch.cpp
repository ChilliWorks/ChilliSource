//
//  SpriteBatch.hcpp
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

#include <ChilliSource/Rendering/Sprite/SpriteBatch.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/ShaderPass.h>
#include <ChilliSource/Rendering/Base/VertexLayouts.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            //CCW winding order (back face cull)
            const u16 k_localIndices[] = {0,1,2,1,3,2};
        }
		//------------------------------------------------------
		/// Constructor
		///
		/// Default
		//------------------------------------------------------
		SpriteBatch::SpriteBatch(u32 inudwCapacity, RenderSystem * inpRenderSystem, BufferUsage ineUsage) 
        : m_renderSystem(inpRenderSystem), mpSpriteBuffer(nullptr), mdwTag(0), mudwNumSpritesBuiltIndicesFor(0)
		{
			BufferDescription desc;
			desc.eUsageFlag = ineUsage;
			desc.VertexDataCapacity = (inudwCapacity + 1) * k_numSpriteVerts * sizeof(SpriteVertex); 
			desc.IndexDataCapacity  = (inudwCapacity + 1) * sizeof(u16) * k_numSpriteIndices;
			desc.ePrimitiveType = PrimitiveType::k_tri;
			desc.eAccessFlag = BufferAccess::k_read;
			desc.VertexLayout = VertexLayout::kSprite;
			
			mpSpriteBuffer = m_renderSystem->CreateBuffer(desc);
		}
		//------------------------------------------------------
		/// Build
		///
		/// Finalise the batch. Nothing can be changed once
		/// the batch is built
        ///
        /// @param Sprite array
		//------------------------------------------------------
		void SpriteBatch::Build(const std::vector<SpriteData>& in_sprites)
		{
			const u32 udwNumSprites = in_sprites.size();
			const u32 udwNumIndices = k_numSpriteIndices * udwNumSprites; 
			
			mpSpriteBuffer->Bind();
			mpSpriteBuffer->SetVertexCount(k_numSpriteVerts *  udwNumSprites);
			mpSpriteBuffer->SetIndexCount(udwNumIndices);
			
			//Get the buffer locations
			SpriteVertex* pVBuffer = nullptr;
			mpSpriteBuffer->LockVertex((f32**)&pVBuffer, 0, 0);

			u32 VertIdx = 0;
									
			//The vertex data depends on the sprite vertex layout.
			for(const auto& sprite : in_sprites)
			{
				MapSpriteIntoBuffer(&pVBuffer[VertIdx], sprite);
				VertIdx+=4;
			}
			
			//---End mapping - Vertex
			mpSpriteBuffer->UnlockVertex();

			BuildIndicesForNumberSprites(udwNumSprites);
		}
        //-------------------------------------------------------
        /// Map Sprite Into Buffer
        ///
        /// Load the given sprite data into the memory held by
        /// the vertex buffer
        ///
        /// @param Pointer to sprite offset in buffer
        /// @param Sprite to map
        //-------------------------------------------------------
		void SpriteBatch::MapSpriteIntoBuffer(SpriteVertex* inpBuffer, const SpriteData &inpSprite)
        {
			//---Map the vertex data into the buffer	
			inpBuffer[(u32)Verts::k_topLeft] = inpSprite.sVerts[(u32)Verts::k_topLeft];
			inpBuffer[(u32)Verts::k_bottomLeft] = inpSprite.sVerts[(u32)Verts::k_bottomLeft];
			inpBuffer[(u32)Verts::k_topRight] = inpSprite.sVerts[(u32)Verts::k_topRight];
			inpBuffer[(u32)Verts::k_bottomRight] = inpSprite.sVerts[(u32)Verts::k_bottomRight];
		}
        //------------------------------------------------------
        /// Remap Sprite
        ///
        /// Replaces the contents of a sprite in the batch buffer with the given one
        ///
        /// @param Index of which sprite to replace
        /// @param New sprite to map over the contents
        //------------------------------------------------------
		void SpriteBatch::RemapSprite(u32 inudwIndex, const SpriteData& inpSprite)
        {
			//No validation, cause it to crash as you want
			mpSpriteBuffer->Bind();
			
            SpriteVertex* pVBuffer = nullptr;
			mpSpriteBuffer->LockVertex((f32**)&pVBuffer, 0, 0);
			
			pVBuffer += k_numSpriteVerts * inudwIndex;
			MapSpriteIntoBuffer(pVBuffer, inpSprite);
			
			mpSpriteBuffer->UnlockVertex();
		}
        //-------------------------------------------------------
        /// Build Indices For Number Sprites
        ///
        /// If required sets indices for the given number of sprites.
        /// Skips over if the indices have already been filled in enough
        //------------------------------------------------------
		void SpriteBatch::BuildIndicesForNumberSprites(u32 inudwNumSprites)
        {
			if(inudwNumSprites > mudwNumSpritesBuiltIndicesFor) 
            {
				mudwNumSpritesBuiltIndicesFor = inudwNumSprites;
				
				u16* pIBuffer = nullptr;
				mpSpriteBuffer->LockIndex(&pIBuffer, 0, 0);
				
				for	(u32 nSprite = 0; nSprite < mudwNumSpritesBuiltIndicesFor; nSprite++)
                {
					for(u32 i=0; i<k_numSpriteIndices; ++i)
					{
						pIBuffer[nSprite * k_numSpriteIndices + i] = k_localIndices[i] + (nSprite * k_numSpriteVerts);
					}	
				}
				
				mpSpriteBuffer->UnlockIndex();
			}
		}
		//------------------------------------------------------
		/// Render
		///
		/// Draw the contents of the mesh buffer
		///
        /// @param Offset into mesh buffer
        /// @param Stride within mesh buffer
		//------------------------------------------------------
		void SpriteBatch::Render(const MaterialCSPtr& inMaterial, u32 inudwOffset, u32 inudwStride) const
		{
            if(inudwStride > 0)
            {
                mpSpriteBuffer->Bind();
			
                //Tell the render system to draw the contents of the buffer
                m_renderSystem->ApplyMaterial(inMaterial, ShaderPass::k_ambient);

                m_renderSystem->RenderBuffer(mpSpriteBuffer, inudwOffset, inudwStride, Core::Matrix4::k_identity);
            }
		}
        //------------------------------------------------------
		/// Render
		///
		/// Draw the contents of the mesh buffer
		///
		/// @param Active render system
		//------------------------------------------------------
		void SpriteBatch::Render(const MaterialCSPtr& inMaterial) const
		{
            if(mpSpriteBuffer->GetIndexCount() > 0)
            {
                mpSpriteBuffer->Bind();
                
                //Tell the render system to draw the contents of the buffer
                m_renderSystem->ApplyMaterial(inMaterial, ShaderPass::k_ambient);

                m_renderSystem->RenderBuffer(mpSpriteBuffer, 0, mpSpriteBuffer->GetIndexCount(), Core::Matrix4::k_identity);
            }
		}
		//------------------------------------------------------
		/// Get Tag
		///
		/// Associated data usually used to determine layer
		//------------------------------------------------------
		s32 SpriteBatch::GetTag() const
		{
			return mdwTag;
		}
		//------------------------------------------------------
		/// Set Tag
		///
		/// Associated data usually used to determine layer
		//------------------------------------------------------
		void SpriteBatch::SetTag(s32 indwValue)
		{
			mdwTag = indwValue;
		}
		//------------------------------------------------------
		/// Destructor
		///
		//------------------------------------------------------
		SpriteBatch::~SpriteBatch() 
		{
			CS_SAFEDELETE(mpSpriteBuffer);
		}
	}
}
