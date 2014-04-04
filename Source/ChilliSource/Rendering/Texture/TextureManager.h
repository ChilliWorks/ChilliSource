/*
 *  TextureManager.h
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_TEXTUREMANAGER_H_
#define _MOFLO_RENDERING_TEXTUREMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{	
	namespace Rendering
	{
		class TextureManager : public Core::ResourceManager
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(TextureManager);
			
			TextureManager();
			virtual ~TextureManager(){}
			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			Core::InterfaceIDType GetProviderType() const override;
			//----------------------------------------------------------------
			/// Manages Resource Of Type
			///
			/// @param Type
			/// @return Whether this object manages the object of type
			//----------------------------------------------------------------
			bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const override;
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceOldSPtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceOldSPtr AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			//----------------------------------------------------------------
			/// Create Empty Texture
			///
			/// @param Texture width
			/// @param Texture height
			/// @param Out: Texture resource 
			/// @return Success
			//----------------------------------------------------------------
			virtual bool CreateEmptyTexture(u32 inWidth, u32 inHeight, Core::Image::Format ineFormat, TextureSPtr& outpTexture) = 0;
			//-----------------------------------------------------------------
			/// GetDefaultTexture
			///
			/// Returns a 2x2 white texture
			/// @return Pointer to default texture
			//-----------------------------------------------------------------
			const TextureSPtr & GetDefaultTexture();	
			//----------------------------------------------------------------
			/// Create Texture Resource
			///
			/// @return Concrete texture resource based on the render system
			//----------------------------------------------------------------
			virtual TextureSPtr CreateTextureResource() = 0;
            //----------------------------------------------------------------
			/// Create Image From Texture
			///
			/// Rendersystem specific implementations should override this
			/// to return a shared pointer to their Image object
			///
			/// @param mopFlow Texture to create Image from
			//----------------------------------------------------------------
			virtual bool CreateImageFromTexture(Texture* pTexture, Core::ImageSPtr& outpImage) = 0;
            //----------------------------------------------------------------
			/// Create Texture From Image
			///
			/// Rendersystem specific implementations should override this
			/// to return a shared pointer to their Texture object
			///
			/// @param Image to create moFlo texture from
			//----------------------------------------------------------------
			virtual bool CreateTextureFromImage(Core::Image * pImage, TextureSPtr& outpTexture) = 0;
		protected:
			
			struct ImageDesc
			{
				std::string strFilename;
                Core::StorageLocation eStorageLocation;
				Core::ResourceOldSPtr pImageResource;
				TextureSPtr pTextureResource;
			};
			
			//-----------------------------------------------------------------------------------
			/// Image Load Task
			///
			/// Load the source image for the texture
			///
			/// @param Image descriptor
			//-----------------------------------------------------------------------------------
			void ImageLoadTask(ImageDesc& inDesc);
			//-----------------------------------------------------------------------------------
			/// Texture Load Task
			///
			/// Load the texture based on the source image
			///
			/// @param Source image
			/// @param With mipmapping
			/// @param Texture to create
			//-----------------------------------------------------------------------------------
			void TextureLoadTask(const Core::ResourceOldSPtr& inpImage, TextureSPtr& outpTexture);
			//-----------------------------------------------------------------
			/// CreateDefaultTexture
			///
			/// Creates a 2x2 white texture
			/// @return Pointer to default texture
			//-----------------------------------------------------------------
			void CreateDefaultTexture();
			
			TextureSPtr mpDefaultTexture;
		};
	}
	
};

#endif
