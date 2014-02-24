/*
 * File: ShaderManager.h
 * Date: 22/11/2010 2010 
 * Description: Interface class for loading and compiling shader programs. Wraps the concrete implementations such as GLShaderManager; but handles the resource caching
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_RENDERING_SHADER_MANAGER_H_
#define _MO_FLO_RENDERING_SHADER_MANAGER_H_

#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Rendering/ForwardDeclarations.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class IShaderManager : public Core::IResourceManager
		{
		public:
		
			DECLARE_NAMED_INTERFACE(IShaderManager);
			
			IShaderManager();
			virtual ~IShaderManager();
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
			/// Get Shader From File
			///
			/// Loads the Shader from resource and returns a handle to it.
			/// Alternately if the Shader already exists it will return the
			/// handle without loading
			///
            /// @param The storage location to load from
			/// @param File name
			/// @return A handle to the Shader
			//----------------------------------------------------------------
			ShaderPtr GetShaderFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath);
			//----------------------------------------------------------------
			/// Async Get Shader From File
			///
			/// Starts loading the Shader from resource and returns a handle to it.
			/// Alternately if the Shader already exists it will return the
			/// handle without loading
			///
            /// @param The storage location to load from
			/// @param File name
			/// @return A handle to the Shader
			//----------------------------------------------------------------
			ShaderPtr AsyncGetShaderFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath);
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath);
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource, loaded in the background
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath);
            //-----------------------------------------------------------------
            /// Set Render System
            ///
            /// @param Render system 
            //-----------------------------------------------------------------
            void SetRenderSystem(IRenderSystem* inpRenderSystem);
			
		protected:
			
			//----------------------------------------------------------------
			/// Create Shader Resource
			///
			/// @return Concrete shader resource object based on the render
			/// system
			//----------------------------------------------------------------
			virtual ShaderPtr CreateShaderResource() const = 0;
			//---------------------------------------------------------
			/// Create Shader Program From File
			///
			/// Load and compile a shader program from the given
			/// file. 
			///
            /// @param The storage location to load from
			/// @param Shader file path
			/// @param Out: Shader resource
			/// @return Success
			//---------------------------------------------------------
			virtual bool CreateShaderProgramFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath, ShaderPtr& outpShader) = 0;
			//---------------------------------------------------------
			/// Async Create Shader Program From File
			///
			/// Load and compile a shader program from the given
			/// file, in the background
			///
            /// @param The storage location to load from
			/// @param Shader file path
			/// @param Out: Shader resource
			/// @return Success
			//---------------------------------------------------------
			virtual bool AsyncCreateShaderProgramFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath, ShaderPtr& outpShader) = 0;
            
        protected:
            
            Rendering::IRenderSystem* mpRenderSystem;
		};
	}
}

#endif

