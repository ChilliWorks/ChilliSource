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

#include <ChilliSource/Core/ResourceManager.h>
#include <ChilliSource/Core/Image.h>

#include <ChilliSource/Rendering/ForwardDeclarations.h>

namespace moFlo
{	
	namespace Rendering
	{
		class ICubemapManager: public Core::IResourceManager
		{
		public:
			
			DECLARE_NAMED_INTERFACE(ICubemapManager);
			
			ICubemapManager();
			virtual ~ICubemapManager(){}
			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			Core::InterfaceIDType GetProviderType() const; 
			//----------------------------------------------------------------
			/// Manages Resource Of Type
			///
			/// @param Type
			/// @return Whether this object manages the object of type
			//----------------------------------------------------------------
			bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const;
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
			CubemapPtr GetCubemapFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat = Core::CImage::FORMAT_DEFAULT, bool inbWithMipsMaps = false);
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
			CubemapPtr AsyncGetCubemapFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat = Core::CImage::FORMAT_DEFAULT, bool inbWithMipsMaps = false);
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr GetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath);
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr AsyncGetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath);
			//----------------------------------------------------------------
			/// Create Cubemap Resource
			///
			/// @return Concrete Cubemap resource based on the render system
			//----------------------------------------------------------------
			virtual CubemapPtr CreateCubemapResource() = 0;
            //----------------------------------------------------------------
			/// Create Cubemap From Image
			///
			/// Rendersystem specific implementations should override this
			/// to return a shared pointer to their ICubemap object
			///
			/// @param Image to create moFlo Cubemap from
			//----------------------------------------------------------------
			virtual bool CreateCubemapFromImages(const DYNAMIC_ARRAY<Core::ResourcePtr>& inaImages, bool inbWithMipsMaps, CubemapPtr& outpCubemap) = 0;
		protected:
			
			struct ImageDesc
			{
				std::string strFilenames[6];
				Core::CImage::Format eImageFormat;
				bool bUseMipmaps;
                Core::STORAGE_LOCATION eStorageLocation;
				DYNAMIC_ARRAY<Core::ResourcePtr> pImageResources;
				CubemapPtr pCubemapResource;
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
			void CubemapLoadTask(const DYNAMIC_ARRAY<Core::ResourcePtr>& inaImages, bool inbWithMipsMaps, CubemapPtr& outpCubemap);
		};
	}
	
};

#endif
