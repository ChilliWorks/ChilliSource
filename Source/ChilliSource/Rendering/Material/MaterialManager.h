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

#include <ChilliSource/Core/ResourceManager.h>
#include <ChilliSource/Rendering/ForwardDeclarations.h>

namespace moFlo
{
	namespace Rendering
	{
		class IMaterialManager : public Core::IResourceManager
		{
		public:
			
			DECLARE_NAMED_INTERFACE(IMaterialManager);

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
			MaterialPtr GetMaterialFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePathMaterial);
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
			MaterialPtr AsyncGetMaterialFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePathMaterial);
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
		};
	}
}

#endif