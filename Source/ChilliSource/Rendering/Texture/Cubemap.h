//
//  CubeMap.h
//  Chilli Source
//  Created by Scott Downie on 15/07/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_CUBEMAP_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_CUBEMAP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <array>

namespace ChilliSource
{
	namespace Rendering
	{
        //--------------------------------------------------------------
        /// Interface class to backend rendering cubemap resource. A
        /// cubemap resource is created from 6 images into a format
        /// that can be used by the render system.
        ///
        /// @author S Downie
        //--------------------------------------------------------------
		class Cubemap : public Core::Resource
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(Cubemap);
            //--------------------------------------------------
            /// Construct the texture from the given image data.
            /// The texture will take ownership of the image data
            ///
            /// Order is as follows:
            /// - Pos X
            /// - Neg X
            /// - Pos Y
            /// - Neg Y
            /// - Pos Z
            /// - Neg Z
            ///
            /// @author S Downie
            ///
            /// @param Texture descriptors (6)
            /// @param Image datas (6)
            //--------------------------------------------------
            virtual void Build(const std::array<Texture::Descriptor, 6>& in_descs, const std::array<Texture::TextureDataUPtr, 6>& in_datas) = 0;
			//--------------------------------------------------------------
            /// Binds this cubemap to the given texture unit allowing it to
            /// be accessed by the shaders and operations to be performed on it
            ///
            /// @author S Downie
            ///
            /// @param Texture unit
            //--------------------------------------------------------------
			virtual void Bind(u32 inSlot = 0) = 0;
            //--------------------------------------------------------------
            /// Unbind this cubemap from its current texture unit. This
            /// means it can no longer be used or changed until rebound.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			virtual void Unbind() = 0;
			//--------------------------------------------------------------
            /// Future sampling of the cubemap will use the given filter function
            ///
            /// @author S Downie
            ///
            /// @param Filter mode
            //--------------------------------------------------------------
			virtual void SetFilterMode(Texture::FilterMode in_mode) = 0;
            //--------------------------------------------------------------
            /// Future sampling of the cubemap will use the given wrap mode
            ///
            /// @author S Downie
            ///
            /// @param Horizontal wrapping
            /// @param Vertical wrapping
            //--------------------------------------------------------------
			virtual void SetWrapMode(Texture::WrapMode in_sMode, Texture::WrapMode in_tMode) = 0;
            //--------------------------------------------------------------
            /// Generate mip map levels for the cubemap to reduce
            /// aliasing
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			virtual void GenerateMipMaps() = 0;
            //--------------------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			virtual ~Cubemap(){}
            
        protected:
            friend class Core::ResourcePool;
            //--------------------------------------------------------------
            /// Factory method for creating an empty cubemap resource.
            /// Only called by the resource pool
            ///
            /// @author S Downie
            ///
            /// @return Concrete cubemap resource
            //--------------------------------------------------------------
            static CubemapUPtr Create();
		};
	}
}

#endif