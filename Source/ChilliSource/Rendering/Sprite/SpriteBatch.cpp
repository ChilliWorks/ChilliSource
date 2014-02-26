/*
 *  SpriteBatch.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 10/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Rendering/Sprite/SpriteBatch.h>
#include <ChilliSource/Rendering/Base/VertexLayouts.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>

#ifdef DEBUG_STATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------------------------
		/// Constructor
		///
		/// Default
		//------------------------------------------------------
		CSpriteBatch::CSpriteBatch(u32 inudwCapacity, IRenderSystem * inpRenderSystem, BufferUsage ineUsage) 
        : mpSpriteBuffer(nullptr), mdwTag(0), mudwNumSpritesBuiltIndicesFor(0)
		{
			BufferDescription desc;
			desc.eUsageFlag = ineUsage;
			desc.VertexDataCapacity = (inudwCapacity + 1) * kudwVertsPerSprite * sizeof(CSpriteComponent::SpriteVertex); 
			desc.IndexDataCapacity  = (inudwCapacity + 1) * sizeof(u16) * kudwIndicesPerSprite;
			desc.ePrimitiveType = PrimitiveType::k_tri;
			desc.eAccessFlag = BufferAccess::k_read;
			desc.VertexLayout = VertexLayout::kSprite;
			
			mpSpriteBuffer = inpRenderSystem->CreateBuffer(desc);
		}
		//------------------------------------------------------
		/// Build
		///
		/// Finalise the batch. Nothing can be changed once
		/// the batch is built
        ///
        /// @param Sprite array
		//------------------------------------------------------
		void CSpriteBatch::Build(std::vector<CSpriteComponent::SpriteData>* inpSprites)
		{
			//Sanity check
			if(!mpSpriteBuffer) return;
			
			const u32 udwNumSprites = inpSprites->size();
			const u32 udwNumIndices = kudwIndicesPerSprite * udwNumSprites; 
			
			mpSpriteBuffer->Bind();
			mpSpriteBuffer->SetVertexCount(kudwVertsPerSprite *  udwNumSprites);
			mpSpriteBuffer->SetIndexCount(udwNumIndices);
			
			//Get the buffer locations
			CSpriteComponent::SpriteVertex* pVBuffer = nullptr;
			mpSpriteBuffer->LockVertex((f32**)&pVBuffer, 0, 0);

			u32 VertIdx = 0;
									
			//The vertex data depends on the sprite vertex layout.
			for(std::vector<CSpriteComponent::SpriteData>::iterator pSpriteItr = inpSprites->begin(); pSpriteItr != inpSprites->end(); ++pSpriteItr)
			{
				MapSpriteIntoBuffer(&pVBuffer[VertIdx], (*pSpriteItr));
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
		void CSpriteBatch::MapSpriteIntoBuffer(CSpriteComponent::SpriteVertex* inpBuffer, const CSpriteComponent::SpriteData &inpSprite)
        {
			//---Map the vertex data into the buffer	
			inpBuffer[(u32)CSpriteComponent::Verts::k_topLeft] = inpSprite.sVerts[(u32)CSpriteComponent::Verts::k_topLeft];
			inpBuffer[(u32)CSpriteComponent::Verts::k_bottomLeft] = inpSprite.sVerts[(u32)CSpriteComponent::Verts::k_bottomLeft];
			inpBuffer[(u32)CSpriteComponent::Verts::k_topRight] = inpSprite.sVerts[(u32)CSpriteComponent::Verts::k_topRight];
			inpBuffer[(u32)CSpriteComponent::Verts::k_bottomRight] = inpSprite.sVerts[(u32)CSpriteComponent::Verts::k_bottomRight];
		}
        //------------------------------------------------------
        /// Remap Sprite
        ///
        /// Replaces the contents of a sprite in the batch buffer with the given one
        ///
        /// @param Index of which sprite to replace
        /// @param New sprite to map over the contents
        //------------------------------------------------------
		void CSpriteBatch::RemapSprite(u32 inudwIndex, const CSpriteComponent::SpriteData& inpSprite)
        {
			//No validation, cause it to crash as you want
			mpSpriteBuffer->Bind();
			
            CSpriteComponent::SpriteVertex* pVBuffer = nullptr;
			mpSpriteBuffer->LockVertex((f32**)&pVBuffer, 0, 0);
			
			pVBuffer += kudwVertsPerSprite * inudwIndex;
			MapSpriteIntoBuffer(pVBuffer, inpSprite);
			
			mpSpriteBuffer->UnlockVertex();
		}
        //-------------------------------------------------------
        /// Build Indices For Number Sprites
        ///
        /// If required sets indices for the given number of sprites.
        /// Skips over if the indices have already been filled in enough
        //------------------------------------------------------
		void CSpriteBatch::BuildIndicesForNumberSprites(u32 inudwNumSprites)
        {
			if(inudwNumSprites > mudwNumSpritesBuiltIndicesFor) 
            {
				mudwNumSpritesBuiltIndicesFor = inudwNumSprites;
				
				u16* pIBuffer = nullptr;
				mpSpriteBuffer->LockIndex(&pIBuffer, 0, 0);
				
				for	(u16 nSprite = 0; nSprite < mudwNumSpritesBuiltIndicesFor; nSprite++)
                {
					for(u32 i=0; i<kudwIndicesPerSprite; ++i)
					{
						pIBuffer[nSprite * kudwIndicesPerSprite + i] = kauwLocalIndices[i] + (nSprite * kudwVertsPerSprite);
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
		/// @param Active render system
        /// @param Offset into mesh buffer
        /// @param Stride within mesh buffer
		//------------------------------------------------------
		void CSpriteBatch::Render(IRenderSystem* inpRenderSystem, const CMaterial& inMaterial, u32 inudwOffset, u32 inudwStride) const
		{
            if(inudwStride > 0)
            {
                MOFLOW_ASSERT(inMaterial.GetActiveShaderProgram(), "Cannot render a sprite batch with no active shader.");
                
                mpSpriteBuffer->Bind();
			
                //Tell the render system to draw the contents of the buffer
                inpRenderSystem->ApplyMaterial(inMaterial);
#ifdef DEBUG_STATS
                CDebugStats::AddToEvent("Verts", (inudwStride*2)/3);
#endif
                inpRenderSystem->RenderBuffer(mpSpriteBuffer, inudwOffset, inudwStride, Core::CMatrix4x4::IDENTITY);
            }
		}
        //------------------------------------------------------
		/// Render
		///
		/// Draw the contents of the mesh buffer
		///
		/// @param Active render system
		//------------------------------------------------------
		void CSpriteBatch::Render(IRenderSystem* inpRenderSystem, const CMaterial& inMaterial) const
		{
            if(mpSpriteBuffer->GetIndexCount() > 0)
            {
                MOFLOW_ASSERT(inMaterial.GetActiveShaderProgram(), "Cannot render a sprite batch with no active shader.");
                
                mpSpriteBuffer->Bind();
                
                //Tell the render system to draw the contents of the buffer
                inpRenderSystem->ApplyMaterial(inMaterial);
#ifdef DEBUG_STATS
                CDebugStats::AddToEvent("Verts", (mpSpriteBuffer->GetIndexCount()*2)/3);
#endif
                inpRenderSystem->RenderBuffer(mpSpriteBuffer, 0, mpSpriteBuffer->GetIndexCount(), Core::CMatrix4x4::IDENTITY);
            }
		}
		//------------------------------------------------------
		/// Get Tag
		///
		/// Associated data usually used to determine layer
		//------------------------------------------------------
		s32 CSpriteBatch::GetTag() const
		{
			return mdwTag;
		}
		//------------------------------------------------------
		/// Set Tag
		///
		/// Associated data usually used to determine layer
		//------------------------------------------------------
		void CSpriteBatch::SetTag(s32 indwValue)
		{
			mdwTag = indwValue;
		}
		//------------------------------------------------------
		/// Destructor
		///
		//------------------------------------------------------
		CSpriteBatch::~CSpriteBatch() 
		{
			SAFE_DELETE(mpSpriteBuffer);
		}
	}
}