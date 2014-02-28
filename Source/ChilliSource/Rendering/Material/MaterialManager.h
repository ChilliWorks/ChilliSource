/*
 * File: MaterialManager.h
 * Date: 22/11/2010 2010 
 * Description: Loads and caches material files
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_RENDERING_MATERIAL_MANAGER_H_
#define _MO_FLO_RENDERING_MATERIAL_MANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class MaterialManager : public Core::ResourceManager
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(MaterialManager);

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
			/// Get Material From File
			///
			/// Loads the Material from resource and returns a handle to it.
			/// Alternately if the Material already exists it will return the
			/// handle without loading
			///
            /// @param The storage location to load from
			/// @param File name
			/// @return A handle to the Material
			//----------------------------------------------------------------
			MaterialSPtr GetMaterialFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePathMaterial);
			//----------------------------------------------------------------
			/// Async Get Material From File
			///
			/// starts loading Material from resource and returns a handle to it.
			/// Alternately if the Material already exists it will return the
			/// handle without loading
			///
            /// @param The storage location to load from
			/// @param File name
			/// @return A handle to the Material
			//----------------------------------------------------------------
			MaterialSPtr AsyncGetMaterialFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePathMaterial);
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
		};
	}
}

#endif