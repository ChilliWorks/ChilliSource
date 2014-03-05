/*
 *  GLTextureManager.h
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_OPENGL_TEXTUREMANAGER_H_
#define _MOFLOW_OPENGL_TEXTUREMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		class TextureManager : public ChilliSource::Rendering::TextureManager
		{
		public:

			//----------------------------------------------------------------
			/// Create Texture Resource
			///
			/// @return Concrete texture resource based on the render system
			//----------------------------------------------------------------
			ChilliSource::Rendering::TextureSPtr CreateTextureResource();
			//----------------------------------------------------------------
			/// Create Empty Texture
			///
			/// @param Texture width
			/// @param Texture height
			/// @param Out: Texture resource
			/// @return Success
			//----------------------------------------------------------------
			bool CreateEmptyTexture(u32 inudwWidth, u32 inudwHeight, Core::Image::Format ineFormat, ChilliSource::Rendering::TextureSPtr& outpTexture);
            //----------------------------------------------------------------
			/// Create Image From Texture
			///
			/// Rendersystem specific implementations should override this
			/// to return a shared pointer to their Image object
			///
			/// @param mopFlow Texture to create Image from
			//----------------------------------------------------------------
			bool CreateImageFromTexture(Rendering::Texture* inpTexture, Core::ImageSPtr& outpImage);
			//----------------------------------------------------------------
			/// Create Texture From Image
			///
			/// @param Image
			/// @param Generate mip maps
			/// @param Out: Texture resource
			/// @return Success
			//----------------------------------------------------------------
			bool CreateTextureFromImage(Core::Image * inpImage, bool inbWithMipsMaps, ChilliSource::Rendering::TextureSPtr& outpTexture);
			//----------------------------------------------------------------
			/// Backup
			///
			/// Backs up all textures that cannot be restored from file in
			/// memory.
			//----------------------------------------------------------------
			void Backup();
			//----------------------------------------------------------------
			/// Restore
			///
			/// Restore all textures either from file or from cached images.
			//----------------------------------------------------------------
			void Restore();
			//----------------------------------------------------------------
			/// Add Restorable Texture
			///
			/// Adds the texture to the list of all created textures. This
			/// list is used to restore the textures when the context is lost.
			///
			/// @param The texture pointer.
			//----------------------------------------------------------------
			void AddRestorableTexture(const Rendering::TextureSPtr& inpTexture);
			//----------------------------------------------------------------
			/// Remove Restorable Texture
			///
			/// Removes the texture from the list of all created textures. This
			/// list is used to restore the textures when the context is lost.
			///
			/// @param The texture pointer.
			//----------------------------------------------------------------
			void RemoveRestorableTexture(Texture* inpTexture);
			//----------------------------------------------------------------
			/// Destructor
			//----------------------------------------------------------------
			~TextureManager();
		private:
#ifdef CS_TARGETPLATFORM_ANDROID
			std::vector<Rendering::TextureWPtr> mapTextureCache;
			std::unordered_map<Texture*, Core::ImageSPtr> mapBackedUpImages;
#endif
		};
	}
}

#endif
