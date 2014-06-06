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
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return The resource type this provider can create
            //----------------------------------------------------
            Core::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;

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
			/// @param File path
            /// @param Options to customise the creation
			/// @param [Out] the output resource pointer
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// Create mesh resource from model file on a background thread
			///
            /// @author I Copland
            ///
            /// @param The storage location to load from
			/// @param File path
            /// @param Options to customise the creation
            /// @param Delegate to callback on completion either success or failure
			/// @param [Out] the output resource pointer
			//----------------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// @author I Copland
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Delegate to callback on completion either success or failure
			/// @param the output resource pointer
			//----------------------------------------------------------------------------
			void LoadMeshDataTask(Core::StorageLocation in_location, const std::string& in_filePath, const AsyncLoadDelegate& in_delegate, const MeshSPtr& out_resource);
			//----------------------------------------------------------------------------
			/// Constructs the mesh buffer from the mesh description
			///
			/// @author I Copland
			///
            /// @param Delegate to callback on completion either success or failure
			/// @param [Out] The MeshDescriptor used to build the mesh
			/// @param [Out] The mesh resource
			//----------------------------------------------------------------------------
			void BuildMesh(const AsyncLoadDelegate& in_delegate, MeshDescriptor& out_meshDesc, const MeshSPtr& out_resource);
		};
	}
}

#endif
