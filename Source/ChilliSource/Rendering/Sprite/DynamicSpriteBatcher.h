//
//  DynamicSpriteBatcher.h
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

#ifndef _CHILLISOURCE_RENDERING_DYNAMICSPRITEBATCH_H_
#define _CHILLISOURCE_RENDERING_DYNAMICSPRITEBATCH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Sprite/SpriteBatch.h>

namespace ChilliSource
{
	namespace Rendering
	{
        const u32 kudwNumBuffers = 2;
        
        enum class CommandType
        {
            k_draw,
            k_scissorOn,
            k_scissorOff
        };
        
        struct RenderCommand
        {
            MaterialCSPtr m_material;
            
            Core::Vector2 m_scissorPos;
            Core::Vector2 m_scissorSize;
            
            u32 m_offset;
            u32 m_stride;
            
            CommandType m_type;
        };
        
		class DynamicSpriteBatch
		{
		public:
			DynamicSpriteBatch(RenderSystem* inpRenderSystem);
            ~DynamicSpriteBatch();
            
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
			void Render(const SpriteBatch::SpriteData& inpSprite, const Core::Matrix4 * inpTransform = nullptr);
            //-------------------------------------------------------
			/// Enable scissoring with the given region. Any
            /// subsequent renders to the batcher will be clipped
            /// based on the region
            ///
            /// @author S Downie
            ///
            /// @param Bottom left pos of the scissor rect
            /// @param Size of the scissor rect
			//-------------------------------------------------------
			void EnableScissoring(const Core::Vector2& in_pos, const Core::Vector2& in_size);
            //-------------------------------------------------------
			/// Disable scissoring. Any
            /// subsequent renders to the batcher will not be clipped/
            ///
            /// @author S Downie
			//-------------------------------------------------------
			void DisableScissoring();
            //-------------------------------------------------------
			/// Force Render
			///
            /// Force the currently batched sprites to be rendered
            /// regardless of whether the batch is full
			//-------------------------------------------------------
			void ForceRender();
			
		private:
            
            //-------------------------------------------------------
            /// Take the contents of the sprite cache as it stands
            /// and generate a draw call commmand
            ///
            /// @author S Downie
            //-------------------------------------------------------
            void InsertDrawCommand();
            //----------------------------------------------------------
            /// Build and Flush Batch
            ///
            /// Map the batch into the mesh buffer and present the
            /// contents. This will then swap the active buffer
            /// so that it can be filled while the other one is 
            /// rendering
            //----------------------------------------------------------
            void BuildAndFlushBatch();
			
		private:
        
			SpriteBatch* mpBatch[kudwNumBuffers];
			std::vector<SpriteBatch::SpriteData> maSpriteCache;
            std::vector<RenderCommand> maRenderCommands;
            
            MaterialCSPtr mpLastMaterial;
            
            u32 mudwCurrentRenderSpriteBatch;
            u32 mudwSpriteCommandCounter;
            
            RenderSystem* m_renderSystem;
		};
	}
}

#endif
