//
//  Texture.h
//  Chilli Source
//  Created by Scott Downie on 01/10/2010.
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

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_TEXTURE_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_TEXTURE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //--------------------------------------------------------------
        /// Interface class to backend rendering texture resource. A
        /// texture resource is created from an image into a format
        /// that can be used by the render system.
        ///
        /// @author S Downie
        //--------------------------------------------------------------
		class Texture : public Core::Resource
		{
		public:
			
            CS_DECLARE_NAMEDTYPE(Texture);
            
            using TextureDataUPtr = std::unique_ptr<u8[]>;
            
            //--------------------------------------------------------------
            /// Holds the description for building a texture from image data
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            struct Descriptor
            {
                u32 m_width;
                u32 m_height;
                Core::ImageFormat m_format;
                Core::ImageCompression m_compression;
                u32 m_dataSize;
            };
			//--------------------------------------------------------------
            /// The filter mode for determining how to sample texels based
            /// on nearby texels in order to reduce texture aliasing.
            ///
            /// * Nearest neighbour - Fastest method. Chooses the closest texel
            /// * Bilinear - Samples from 4 texels and uses weighted average
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			enum class FilterMode
			{
                k_nearestNeighbour,
                k_bilinear
			};
            //--------------------------------------------------------------
            /// The wrap mode for determining how a texel should be sampled
            /// if the texture coordinates exceed the bounds
            ///
            /// * Clamp - Clamps the texcoord between 0 and 1
            /// * Wrap - Tex coord > 1 will wrap to be (x - 1) and vice-versa
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			enum class WrapMode
			{
                k_clamp,
                k_repeat
			};
            //--------------------------------------------------------------
            /// Construct the texture from the given image data. The texture
            /// will take ownership of the image data.
            ///
            /// @author S Downie
            ///
            /// @param Texture descriptor
            /// @param Image data
            /// @param Whether the texture should have mip maps generated
            /// @param Whether or not the texture data should be
            /// restored after a context loss. This involves maintaining
            /// a copy of the texture data in memory which is costly
            /// so this should be disabled for any textures that can
            /// easily be recreated, i.e any texture that is rendered
            /// into every frame. This has no effect on textures that
            /// are loaded from file as they are always restored from
            /// disk. This will only work for RGBA8888, RGB888, RGBA4444
            /// and RGB565 textures.
            //--------------------------------------------------------------
            virtual void Build(const Descriptor& in_desc, TextureDataUPtr in_data, bool in_mipMap, bool in_restoreTextureDataEnabled) = 0;
			//--------------------------------------------------------------
            /// Binds this texture to the given texture unit allowing it to
            /// be accessed by the shaders and operations to be performed on it
            ///
            /// @author S Downie
            ///
            /// @param Texture unit
            //--------------------------------------------------------------
			virtual void Bind(u32 in_texUnit = 0) = 0;
            //--------------------------------------------------------------
            /// Unbind this texture from its current texture unit. This
            /// means it can no longer be used or changed until rebound.
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			virtual void Unbind() = 0;
			//--------------------------------------------------------------
            /// Future sampling of the texture will use the given filter function
            ///
            /// @author S Downie
            ///
            /// @param Filter mode
            //--------------------------------------------------------------
			virtual void SetFilterMode(FilterMode in_mode) = 0;
            //--------------------------------------------------------------
            /// Future sampling of the texture will use the given wrap mode
            ///
            /// @author S Downie
            ///
            /// @param Horizontal wrapping
            /// @param Vertical wrapping
            //--------------------------------------------------------------
			virtual void SetWrapMode(WrapMode in_sMode, WrapMode in_tMode) = 0;
			//--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The width of the texture in texels
            //--------------------------------------------------------------
			virtual u32 GetWidth() const = 0;
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The height of the texture in texels
            //--------------------------------------------------------------
			virtual u32 GetHeight() const = 0;
            //--------------------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //--------------------------------------------------------------
			virtual ~Texture(){}
            
        protected:
            friend class Core::ResourcePool;
            //--------------------------------------------------------------
            /// Factory method for creating an empty texture resource.
            /// Only called by the resource pool
            ///
            /// @author S Downie
            ///
            /// @return Concrete texture resource
            //--------------------------------------------------------------
            static TextureUPtr Create();
		};
	}
}

#endif
