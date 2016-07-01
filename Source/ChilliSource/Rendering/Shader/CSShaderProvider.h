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

#ifndef _CSBACKEND_RENDERING_SHADER_CSSHADERPROVIDER_H_
#define _CSBACKEND_RENDERING_SHADER_CSSHADERPROVIDER_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
    /// Loads CS shader from shader files into a shader resource. The file contains both
    /// vertex and fragment shaders
    ///
    class CSShaderProvider final : public ResourceProvider
    {
    public:
        CS_DECLARE_NAMEDTYPE(CSShaderProvider);

        /// Allows querying of whether or not this system implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// @return The type of resource loaded by this provider
        ///
        InterfaceIDType GetResourceType() const noexcept override;
        
        /// @param extension
        ///     The extension to compare.
        ///
        /// @return Whether the object can create a resource with the given extension
        ///
        bool CanCreateResourceWithFileExtension(const std::string& extension) const noexcept override;
        
    private:
        friend class Application;
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static CSShaderProviderUPtr Create() noexcept;
        
        CSShaderProvider() noexcept = default;
        
        /// Load the shader from the external file into a resource. Check resource
        /// load state for success or failure
        ///
        /// @param location
        ///     The storage location to load from
        /// @param filePath
        ///     The file path
        /// @param options
        ///     Options to customise the creation of the resource.
        /// @param resource
        ///     The output resource.
        ///
        void CreateResourceFromFile(StorageLocation location, const std::string& filePath, const IResourceOptionsBaseCSPtr& options, const ResourceSPtr& resource) noexcept override;

        /// Load the shader from the external file into a resource on a background
        /// thread. Delegate is called when the load is complete. Check resource
        /// load state for success or failure.
        ///
        /// @param location
        ///     The storage location to load from
        /// @param filePath
        ///     The file path
        /// @param options
        ///     Options to customise the creation of the resource.
        /// @param delegate
        ///     The completion delegate.
        /// @param resource
        ///     The output resource.
        ///
        void CreateResourceFromFileAsync(StorageLocation location, const std::string& filePath, const IResourceOptionsBaseCSPtr& options, const ResourceProvider::AsyncLoadDelegate& delegate, const ResourceSPtr& resource) noexcept override;
    };
}

#endif
