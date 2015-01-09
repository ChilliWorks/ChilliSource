//
//  DynamicSpriteBatcher.cpp
//  Chilli Source
//  Created by Scott Downie on 06/10/2010.
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

#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Material/Material.h>

namespace ChilliSource
{
	namespace Rendering
	{	
        const u32 kudwMaxSpritesInDynamicBatch = 512;
        
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		DynamicSpriteBatch::DynamicSpriteBatch(RenderSystem* inpRenderSystem) 
        : m_renderSystem(inpRenderSystem), mudwCurrentRenderSpriteBatch(0), mudwSpriteCommandCounter(0)
		{
            for(u32 i=0; i<kudwNumBuffers; ++i)
            {
                mpBatch[i] = new SpriteBatch(kudwMaxSpritesInDynamicBatch, inpRenderSystem, BufferUsage::k_dynamic);
            }
            
            maRenderCommands.reserve(50);
		}
        //-------------------------------------------------------
        /// Render
        ///
        /// Batch the sprite to be rendered later. Track the 
        /// render commands so that the correct subset of the 
        /// mesh buffer can be flushed and the correct material
        /// applied
        ///
        /// @param Sprite data to batch
        //-------------------------------------------------------
		void DynamicSpriteBatch::Render(const SpriteBatch::SpriteData& inpSprite, const Core::Matrix4 * inpTransform)
		{
            //If we exceed the capacity of the buffer then we will be forced to flush it
            if(maSpriteCache.size() >= kudwMaxSpritesInDynamicBatch)
            {
                ForceRender();
            }
            
            //As all the contents of the mesh buffer have the same vertex format we can push sprites into the buffer
            //regardless of the material. However we cannot render the buffer in a single draw call we must render
            //subsets of the buffer based on materials
            if(mpLastMaterial && mpLastMaterial != inpSprite.pMaterial)
            {
                InsertDrawCommand();
            }
            
            maSpriteCache.push_back(inpSprite);
            if(inpTransform)
            {
                for(u32 i = 0; i < k_numSpriteVerts; i++)
					maSpriteCache.back().sVerts[i].vPos = inpSprite.sVerts[i].vPos * *inpTransform;
            }
            mpLastMaterial = inpSprite.pMaterial;
            ++mudwSpriteCommandCounter;
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        void DynamicSpriteBatch::EnableScissoring(const Core::Vector2& in_pos, const Core::Vector2& in_size)
        {
            InsertDrawCommand();
            
            maRenderCommands.resize(maRenderCommands.size() + 1);
            
            RenderCommand &sLastCommand = maRenderCommands.back();
            sLastCommand.m_type = CommandType::k_scissorOn;
            sLastCommand.m_scissorPos = in_pos;
            sLastCommand.m_scissorSize = in_size;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void DynamicSpriteBatch::DisableScissoring()
        {
            InsertDrawCommand();
            
            maRenderCommands.resize(maRenderCommands.size() + 1);
            
            RenderCommand &sLastCommand = maRenderCommands.back();
            sLastCommand.m_type = CommandType::k_scissorOff;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
		void DynamicSpriteBatch::InsertDrawCommand()
		{
            if(!maSpriteCache.empty())
            {
                maRenderCommands.resize(maRenderCommands.size() + 1);
                
                RenderCommand &sLastCommand = maRenderCommands.back();
                
                sLastCommand.m_type = CommandType::k_draw;
                sLastCommand.m_material = mpLastMaterial;
                //The offset of the indices for this batch
                sLastCommand.m_offset = ((maSpriteCache.size() - mudwSpriteCommandCounter) * k_numSpriteIndices) * sizeof(s16);
                //The number of indices in this batch
                sLastCommand.m_stride = mudwSpriteCommandCounter * k_numSpriteIndices;
                
                mudwSpriteCommandCounter = 0;
            }
		}
        //-------------------------------------------------------
        /// Force Render
        ///
        /// Force the currently batched sprites to be rendered
        /// regardless of whether the batch is full
        ///
        /// @param Render system
        //-------------------------------------------------------
        void DynamicSpriteBatch::ForceRender()
        {
            if(!maSpriteCache.empty())
            {
                //Close off the batch
                InsertDrawCommand();
                
                //Copy geometry into the mesh buffer and render
                BuildAndFlushBatch();
            }
        }
        //----------------------------------------------------------
        /// Build and Flush Batch
        ///
        /// Map the batch into the mesh buffer and present the
        /// contents. This will then swap the active buffer
        /// so that it can be filled while the other one is 
        /// rendering
        //----------------------------------------------------------
		void DynamicSpriteBatch::BuildAndFlushBatch()
        {
            if(!maSpriteCache.empty())
            {
                //Build the next buffer
                mpBatch[mudwCurrentRenderSpriteBatch]->Build(maSpriteCache);
                maSpriteCache.clear();
            }   
            
            //Loop round all the render commands and draw the sections of the buffer with the correct material
            for(auto it = maRenderCommands.begin(); it != maRenderCommands.end(); ++it)
            {
                switch(it->m_type)
                {
                case CommandType::k_draw:
                    //Render the last filled buffer
                    mpBatch[mudwCurrentRenderSpriteBatch]->Render(it->m_material, it->m_offset, it->m_stride);
                    break;
                case CommandType::k_scissorOn:
                    m_renderSystem->EnableScissorTesting(true);
                    m_renderSystem->SetScissorRegion(it->m_scissorPos, it->m_scissorSize);
                    break;
                case CommandType::k_scissorOff:
                    m_renderSystem->EnableScissorTesting(false);
                    break;
                }

            }
                
            maRenderCommands.clear();
            mpLastMaterial.reset();
                
            //Swap the buffers
            mudwCurrentRenderSpriteBatch = (mudwCurrentRenderSpriteBatch + 1) % kudwNumBuffers;
		}
        //----------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------
        DynamicSpriteBatch::~DynamicSpriteBatch()
        {
            for(u32 i=0; i<kudwNumBuffers; ++i)
            {
                delete mpBatch[i];
            }
        }
	}
}

