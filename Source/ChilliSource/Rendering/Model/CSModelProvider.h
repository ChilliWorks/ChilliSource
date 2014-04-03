//
//  CSModelProvider.h
//  Chilli Source
//
//  Created by Ian Copland on 25/08/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_RENDERING_MODEL_CSMODELPROVIDER_H_
#define _CHILLISOURCE_RENDERING_MODEL_CSMODELPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-------------------------------------------------------------------------
		/// Loads model files into a Mesh resource.
		//-------------------------------------------------------------------------
		class CSModelProvider final : public Core::ResourceProvider
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(CSModelProvider);
            
            //-------------------------------------------------------------------------
            /// Factory method for creating provider
            ///
            /// @author I Copland
            //-------------------------------------------------------------------------
            static CSModelProviderUPtr Create();
			//-------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Interface to compare
            ///
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Type to compare
            ///
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const override;

		private:
            
            //----------------------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------
            CSModelProvider() = default;
			//----------------------------------------------------------------------------
			/// Create mesh resource from model file
            ///
            /// @author I Copland
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
            ///
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// Create mesh resource from model file on a background thread
			///
            /// @author I Copland
            ///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
            ///
			/// @return whether or not this was successfully scheduled
			//----------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// @author I Copland
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			//----------------------------------------------------------------------------
			void LoadMeshDataTask(Core::StorageLocation in_location, const std::string& in_filePath, const MeshSPtr& out_resource);
			//----------------------------------------------------------------------------
			/// Constructs the mesh buffer from the mesh description
			///
			/// @author I Copland
			///
			/// @param The MeshDescriptor used to build the mesh
			/// @param the mesh resource
			//----------------------------------------------------------------------------
			void BuildMesh(MeshDescriptor& inMeshDescriptor, const MeshSPtr& out_resource);

		private:	
			
			friend class MeshManager;
		};
	}
}

#endif
