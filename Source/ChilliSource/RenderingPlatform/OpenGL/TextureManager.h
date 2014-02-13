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

#include <moFlo/RenderingPlatform/OpenGL/ForwardDeclarations.h>
#include <moFlo/Rendering/ForwardDeclarations.h>
#include <moFlo/Rendering/TextureManager.h>

namespace moFlo
{
	namespace OpenGL
	{
		class CTextureManager : public moFlo::Rendering::ITextureManager
		{
		public:
			//----------------------------------------------------------------
			/// Create Texture Resource
			///
			/// @return Concrete texture resource based on the render system
			//----------------------------------------------------------------
			moFlo::Rendering::TexturePtr CreateTextureResource();
			//----------------------------------------------------------------
			/// Create Empty Texture
			///
			/// @param Texture width
			/// @param Texture height
			/// @param Out: Texture resource
			/// @return Success
			//----------------------------------------------------------------
			bool CreateEmptyTexture(u32 inudwWidth, u32 inudwHeight, Core::CImage::Format ineFormat, moFlo::Rendering::TexturePtr& outpTexture);
            //----------------------------------------------------------------
			/// Create Image From Texture
			///
			/// Rendersystem specific implementations should override this
			/// to return a shared pointer to their CImage object
			///
			/// @param mopFlow Texture to create Image from
			//----------------------------------------------------------------
			bool CreateImageFromTexture(Rendering::ITexture* inpTexture, Core::ImagePtr& outpImage);
			//----------------------------------------------------------------
			/// Create Texture From Image
			///
			/// @param Image
			/// @param Generate mip maps
			/// @param Out: Texture resource
			/// @return Success
			//----------------------------------------------------------------
			bool CreateTextureFromImage(Core::CImage * inpImage, bool inbWithMipsMaps, moFlo::Rendering::TexturePtr& outpTexture);
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
			void AddRestorableTexture(const Rendering::TexturePtr& inpTexture);
			//----------------------------------------------------------------
			/// Remove Restorable Texture
			///
			/// Removes the texture from the list of all created textures. This
			/// list is used to restore the textures when the context is lost.
			///
			/// @param The texture pointer.
			//----------------------------------------------------------------
			void RemoveRestorableTexture(CTexture* inpTexture);
		private:
#ifdef TARGET_ANDROID
			DYNAMIC_ARRAY<Rendering::TextureWeakPtr> mapTextureCache;
			HASH_MAP<CTexture*, Core::ImagePtr> mapBackedUpImages;
#endif
		};
	}
}

#endif
