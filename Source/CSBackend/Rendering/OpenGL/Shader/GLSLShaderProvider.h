//
//  GLSLShaderProvider.h
//  Chilli Source
//  Created by Scott Downie on 11/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CSBACKEND_RENDERING_OPENGL_SHADER_GLSLSHADERPROVIDER_H_
#define _CSBACKEND_RENDERING_OPENGL_SHADER_GLSLSHADERPROVIDER_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace CSBackend
{
	namespace OpenGL
	{
        //----------------------------------------------------------
        /// Loads GLSL shader from shader files into a shader resource.
        /// The file contains both vertex and fragment shaders
        ///
        /// @author S Downie
        //----------------------------------------------------------
		class GLSLShaderProvider final : public CSCore::ResourceProvider
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(GLSLShaderProvider);
            
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static GLSLShaderProviderUPtr Create();
			//-------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface to compare
            ///
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The type of resource loaded by this provider
			//----------------------------------------------------------------------------
            CSCore::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;
            
        private:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            GLSLShaderProvider() = default;
			//----------------------------------------------------------------------------
			/// Load the shader from the external file into a resource. Check resource
            /// load state for success or failure
            ///
            /// @author S Downie
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Options to customise the creation
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(CSCore::StorageLocation in_location, const std::string& in_filePath, const CSCore::IResourceOptionsBaseCSPtr& in_options, const CSCore::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Load the shader from the external file into a resource on a background
            /// thread. Delegate is called when the load is complete. Check resource
            /// load state for success or failure.
            ///
            /// @author S Downie
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Options to customise the creation
            /// @param Completion delegate
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFileAsync(CSCore::StorageLocation in_location, const std::string& in_filePath, const CSCore::IResourceOptionsBaseCSPtr& in_options, const CSCore::ResourceProvider::AsyncLoadDelegate& in_delegate, const CSCore::ResourceSPtr& out_resource) override;
		};
	}
}

#endif
