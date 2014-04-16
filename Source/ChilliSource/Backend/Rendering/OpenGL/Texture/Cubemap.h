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

#ifndef _MOFLOW_OPENGL_CUBEMAP_H_
#define _MOFLOW_OPENGL_CUBEMAP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		class Cubemap : public ChilliSource::Rendering::Cubemap
		{
		public:

            ~Cubemap();
			//--------------------------------------------------
			/// Init
			///
			/// @param Source images (Must be 6)
			//--------------------------------------------------
			void Init(const std::vector<Core::ResourceOldSPtr>& inapSourceImages);
			//--------------------------------------------------
			/// Is A
			///
			/// @param Interface ID type
			/// @return Whether the object is of this type
			//--------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//--------------------------------------------------
			/// Bind
			///
			/// @param Texture unit to bind texture to (default 0)
			//--------------------------------------------------
			void Bind(u32 inSlot = 0) override;
			//--------------------------------------------------
			/// Unbind
			///
			/// Unbind this texture from a slot if it is bound
			//--------------------------------------------------
			void Unbind() override;
			//--------------------------------------------------
			/// Get Texture ID
			///
			/// @return The GL generated texture handle
			//--------------------------------------------------
			GLuint GetTextureID() const;
            //--------------------------------------------------
            /// Has Mip Maps
            ///
            /// @return Whether or not this was told to use
            ///			mip maps
            //--------------------------------------------------
            bool HasMipMaps() const;
            //--------------------------------------------------
            /// Get Image Format
            ///
            /// @return The format of the image used to create
            ///			the texture.
            //--------------------------------------------------
            Core::ImageFormat GetImageFormat() const;
            //--------------------------------------------------
			/// Set Filter
			///
			/// Set the filtering mode
			/// @param S filter mode
			/// @param T filter mode
			//--------------------------------------------------
			void SetFilter(Rendering::Texture::Filter ineSFilter, Rendering::Texture::Filter ineTFilter) override;
			//--------------------------------------------------
			/// Set Wrap Mode
			///
			/// Set the texture wrap mode
			/// @param S wrap mode
			/// @param T wrap mode
			//--------------------------------------------------
			void SetWrapMode(Rendering::Texture::WrapMode inSWrapMode, Rendering::Texture::WrapMode inTWrapMode) override;
            
        private:
            //--------------------------------------------------
            /// Constructor
            ///
            /// @param Cubemap manager
            //--------------------------------------------------
            Cubemap(CubemapManager* inpManager);
            //---------------------------------------------------
            /// Update Texture Parameters
            ///
            /// Update the texture filter and repeat modes
            //---------------------------------------------------
            void UpdateTextureParameters();
            
            friend class CubemapManager;
            
        private:
            
            GLuint mGLTextureID;
            
            Rendering::Texture::Filter meSFilter;
			Rendering::Texture::Filter meTFilter;
			Rendering::Texture::WrapMode meSWrapMode;
			Rendering::Texture::WrapMode meTWrapMode;
            
            Core::ImageFormat meImageFormat;
            
            bool mbHasTextureFilterModeChanged;
            bool mbHasMipMaps;
            
            CubemapManager* mpCubemapManager;
            Rendering::RenderCapabilities* mpRenderCapabilities;
		};
	}
}

#endif
