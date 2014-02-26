/*
 *  Cubemap.h
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_OPENGL_CUBEMAP_H_
#define _MOFLOW_OPENGL_CUBEMAP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		class CCubemap : public ChilliSource::Rendering::ICubemap
		{
		public:

            ~CCubemap();
			//--------------------------------------------------
			/// Init
			///
			/// @param Source images (Must be 6)
			/// @param Whether to create mip maps
			//--------------------------------------------------
			void Init(const std::vector<Core::ResourcePtr>& inapSourceImages, bool inbWithMipsMaps);
			//--------------------------------------------------
			/// Is A
			///
			/// @param Interface ID type
			/// @return Whether the object is of this type
			//--------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------
			/// Bind
			///
			/// @param Texture unit to bind texture to (default 0)
			//--------------------------------------------------
			void Bind(u32 inSlot = 0);
			//--------------------------------------------------
			/// Unbind
			///
			/// Unbind this texture from a slot if it is bound
			//--------------------------------------------------
			void Unbind();
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
            Core::CImage::Format GetImageFormat() const;
            //--------------------------------------------------
			/// Set Filter
			///
			/// Set the filtering mode
			/// @param S filter mode
			/// @param T filter mode
			//--------------------------------------------------
			void SetFilter(Rendering::ITexture::Filter ineSFilter, Rendering::ITexture::Filter ineTFilter);
			//--------------------------------------------------
			/// Set Wrap Mode
			///
			/// Set the texture wrap mode
			/// @param S wrap mode
			/// @param T wrap mode
			//--------------------------------------------------
			void SetWrapMode(Rendering::ITexture::WrapMode inSWrapMode, Rendering::ITexture::WrapMode inTWrapMode);
            
        private:
            //--------------------------------------------------
            /// Constructor
            ///
            /// @param Cubemap manager
            //--------------------------------------------------
            CCubemap(CCubemapManager* inpManager);
            //---------------------------------------------------
            /// Update Texture Parameters
            ///
            /// Update the texture filter and repeat modes
            //---------------------------------------------------
            void UpdateTextureParameters();
            
            friend class CCubemapManager;
            
        private:
            
            GLuint mGLTextureID;
            
            Rendering::ITexture::Filter meSFilter;
			Rendering::ITexture::Filter meTFilter;
			Rendering::ITexture::WrapMode meSWrapMode;
			Rendering::ITexture::WrapMode meTWrapMode;
            
            Core::CImage::Format meImageFormat;
            
            bool mbHasTextureFilterModeChanged;
            bool mbHasMipMaps;
            
            CCubemapManager* mpCubemapManager;
            Rendering::RenderCapabilities* mpRenderCapabilities;
		};
	}
}

#endif
