//
//  RenderTarget.h
//  Chilli Source
//  Created by Scott Downie on 05/10/2010.
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

#ifndef _CHILLISOURCE_RENDERING_RENDER_TARGET_H_
#define _CHILLISOURCE_RENDERING_RENDER_TARGET_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//=====================================================
		/// Description:
		///
		/// Abstract render target. Contains data about a 
		/// render target.
		//=====================================================
		class RenderTarget
		{
		public:
			RenderTarget(){}
			virtual ~RenderTarget(){}
			
            //------------------------------------------------------
            /// Set Target Textures
            ///
            /// Sets the textures which will be rendered into. If the
            /// depth texture is nullptr a depth buffer will be
            /// created. If the colour texture is nullptr only the
            /// depth texture will be attached.
            ///
            /// @param The colour texture.
            /// @param The depth texture.
            //------------------------------------------------------
			virtual void SetTargetTextures(const TextureSPtr& inpColourTexture, const TextureSPtr& inpDepthTexture) = 0;
            //------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The colour texture.
            //------------------------------------------------------
			virtual const CSRendering::TextureSPtr& GetColourTexture() const = 0;
            //------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The depth texture.
            //------------------------------------------------------
			virtual const CSRendering::TextureSPtr& GetDepthTexture() const = 0;
            //------------------------------------------------------
            /// Bind
            ///
            /// Bind the frame buffer to the context
            //------------------------------------------------------
			virtual void Bind() = 0;
            //------------------------------------------------------
            /// Discard
            ///
            /// Discard the frame buffer contents
            //------------------------------------------------------
			virtual void Discard() = 0;
            //------------------------------------------------------
            /// Get Width
            ///
            /// @return Width of render target
            //------------------------------------------------------
			inline u32 GetWidth() const {return mudwWidth;}
            //------------------------------------------------------
            /// Get Height
            ///
            /// @return Height of render target
            //------------------------------------------------------
			inline u32 GetHeight() const {return mudwHeight;}
            //------------------------------------------------------
            /// Destroy
            ///
            /// Release the buffers
            //------------------------------------------------------
			virtual void Destroy() = 0;
			
		protected:
			u32 mudwWidth;
			u32 mudwHeight;
		};
	}
}

#endif
