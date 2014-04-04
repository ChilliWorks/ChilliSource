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
			virtual bool CreateCubemapFromImages(const std::vector<Core::ResourceOldSPtr>& inaImages, CubemapSPtr& outpCubemap) = 0;
		protected:
			
			struct ImageDesc
			{
				std::string strFilenames[6];
                Core::StorageLocation eStorageLocation;
				std::vector<Core::ResourceOldSPtr> pImageResources;
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
			void CubemapLoadTask(const std::vector<Core::ResourceOldSPtr>& inaImages, CubemapSPtr& outpCubemap);
		};
	}
	
};

#endif
