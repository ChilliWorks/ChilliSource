/*
 *  SpriteBatch.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 06/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>

#include <ChilliSource/Core/Math/MathUtils.h>

#ifdef CS_ENABLE_DEBUGSTATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

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
		void DynamicSpriteBatch::Render(const SpriteComponent::SpriteData& inpSprite, const Core::Matrix4x4 * inpTransform)
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
                for(u32 i = 0; i < kudwVertsPerSprite; i++)
                    Core::Matrix4x4::Multiply(&inpSprite.sVerts[i].vPos, inpTransform, &maSpriteCache.back().sVerts[i].vPos);
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
                sLastCommand.m_offset = ((maSpriteCache.size() - mudwSpriteCommandCounter) * kudwIndicesPerSprite) * sizeof(s16);
                //The number of indices in this batch
                sLastCommand.m_stride = mudwSpriteCommandCounter * kudwIndicesPerSprite;
                
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
#ifdef CS_ENABLE_DEBUGSTATS
                if(!maRenderCommands.empty())
                {
                    RenderCommand &sLastCommand = maRenderCommands.back();
                    if(sLastCommand.m_material != nullptr)
                    {
                        if(sLastCommand.m_material->IsTransparencyEnabled())
                        {
                            Debugging::DebugStats::AddToEvent("Sprites_Trans", (u32)maSpriteCache.size());
                        }
                        else
                        {
                            Debugging::DebugStats::AddToEvent("Sprites", (u32)maSpriteCache.size());
                        }
                    }
                }
#endif
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
                mpBatch[mudwCurrentRenderSpriteBatch]->Build(&maSpriteCache);
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
            mudwCurrentRenderSpriteBatch = Core::MathUtils::Wrap(++mudwCurrentRenderSpriteBatch, 0u, kudwBufferArrayBounds);
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

