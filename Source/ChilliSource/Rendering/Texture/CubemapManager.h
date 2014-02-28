/*
 *  CubemapManager.h
 *  moFlo
 *
 *  Created by Tag Games on 01/08/2013.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_CUBEMAPMANAGER_H_
#define _MOFLO_RENDERING_CUBEMAPMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{	
	namespace Rendering
	{
		class CubemapManager: public Core::ResourceManager
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(CubemapManager);
			
			CubemapManager();
			virtual ~CubemapManager(){}
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
			/// Get Cubemap From File
			///
			/// Loads the Cubemap from resource and returns a handle to it.
			/// Alternately if the Cubemap already exists it will return the
			/// handle without loading
			///
            /// @param The storage location to load from
			/// @param File name
			/// @param Generate mip-maps. Default = false
			/// @return A handle to the Cubemap
			//----------------------------------------------------------------
			CubemapSPtr GetCubemapFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat = Core::Image::Format::k_default, bool inbWithMipsMaps = false);
			//-----------------------------------------------------------------
			/// Async Get Cubemap From File
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
			CubemapSPtr AsyncGetCubemapFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat = Core::Image::Format::k_default, bool inbWithMipsMaps = false);
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
			//----------------------------------------------------------------
			/// Create Cubemap Resource
			///
			/// @return Concrete Cubemap resource based on the render system
			//----------------------------------------------------------------
			virtual CubemapSPtr CreateCubemapResource() = 0;
            //----------------------------------------------------------------
			/// Create Cubemap From Image
			///
			/// Rendersystem specific implementations should override this
			/// to return a shared pointer to their Cubemap object
			///
			/// @param Image to create moFlo Cubemap from
			//----------------------------------------------------------------
			virtual bool CreateCubemapFromImages(const std::vector<Core::ResourceSPtr>& inaImages, bool inbWithMipsMaps, CubemapSPtr& outpCubemap) = 0;
		protected:
			
			struct ImageDesc
			{
				std::string strFilenames[6];
				Core::Image::Format eImageFormat;
				bool bUseMipmaps;
                Core::StorageLocation eStorageLocation;
				std::vector<Core::ResourceSPtr> pImageResources;
				CubemapSPtr pCubemapResource;
			};
			
			//-----------------------------------------------------------------------------------
			/// Image Load Task
			///
			/// Load the source image for the Cubemap
			///
			/// @param Image descriptor
			//-----------------------------------------------------------------------------------
			void ImageLoadTask(ImageDesc& inDesc);
			//-----------------------------------------------------------------------------------
			/// Cubemap Load Task
			///
			/// Load the Cubemap based on the source image
			///
			/// @param Source images
			/// @param With mipmapping
			/// @param Cubemap to create
			//-----------------------------------------------------------------------------------
			void CubemapLoadTask(const std::vector<Core::ResourceSPtr>& inaImages, bool inbWithMipsMaps, CubemapSPtr& outpCubemap);
		};
	}
	
};

#endif
