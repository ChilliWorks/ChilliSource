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
			//----------------------------------------------------------------
			/// Get Texture From File
			///
			/// Loads the texture from resource and returns a handle to it.
			/// Alternately if the texture already exists it will return the
			/// handle without loading
			///
            /// @param The storage location to load from
			/// @param File name
			/// @param Generate mip-maps. Default = false
			/// @return A handle to the texture
			//----------------------------------------------------------------
			TextureSPtr GetTextureFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat = Core::Image::Format::k_default, bool inbWithMipsMaps = false);
			//-----------------------------------------------------------------
			/// Async Get Texture From File
			///
			/// Generic call to get the managers resource. This will
			/// load the resource on a seperate thread but will return the
			/// resource pointer synchronously. Before using the resource
			/// pointer the IsLoaded flag should be checked
			///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @param Image format
			/// @param Enable mip-mapping
			/// @return Generic pointer to resource type
			//-----------------------------------------------------------------
			TextureSPtr AsyncGetTextureFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat = Core::Image::Format::k_default, bool inbWithMipsMaps = false);
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
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceSPtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceSPtr AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
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
			virtual bool CreateTextureFromImage(Core::Image * pImage, bool inbWithMipsMaps, TextureSPtr& outpTexture) = 0;
		protected:
			
			struct ImageDesc
			{
				std::string strFilename;
				Core::Image::Format eImageFormat;
				bool bUseMipmaps;
                Core::StorageLocation eStorageLocation;
				Core::ResourceSPtr pImageResource;
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
			void TextureLoadTask(const Core::ResourceSPtr& inpImage, bool inbWithMipsMaps, TextureSPtr& outpTexture);
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
