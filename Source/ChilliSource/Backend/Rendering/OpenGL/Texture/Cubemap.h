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
            Core::Image::Format GetImageFormat() const;
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
            
            Core::Image::Format meImageFormat;
            
            bool mbHasTextureFilterModeChanged;
            bool mbHasMipMaps;
            
            CubemapManager* mpCubemapManager;
            Rendering::RenderCapabilities* mpRenderCapabilities;
		};
	}
}

#endif
