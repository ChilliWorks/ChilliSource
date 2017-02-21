//
//  PVRImageProvider.h
//  ChilliSource
//  Created by Scott Downie on 08/05/2014.
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

#ifndef _CHILLISOURCE_CORE_IMAGE_PVRIMAGEPROVIDER_H_
#define _CHILLISOURCE_CORE_IMAGE_PVRIMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
    //---------------------------------------------------------------
    /// An image loader that provides PVR images
    ///
    /// @author S Downie
    //---------------------------------------------------------------
    class PVRImageProvider final : public ResourceProvider
    {
    public:
        CS_DECLARE_NAMEDTYPE(PVRImageProvider);
        //----------------------------------------------------------
        /// Is the object of the given interface type
        ///
        /// @author S Downie
        ///
        /// @param Interface type to query
        ///
        /// @return Whether the object is of given type
        //----------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //-------------------------------------------------------
        /// @author S Downie
        ///
        /// @return The resource type this provider can load
        //-------------------------------------------------------
        InterfaceIDType GetResourceType() const override;
        //----------------------------------------------------------
        /// Whether or not the provider can create resources from
        /// files with the given extension.
        ///
        /// @author S Downie
        ///
        /// @param Extension to compare against
        ///
        /// @return Whether the object can load a resource with that
        /// extension
        //----------------------------------------------------------
        bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;
        //----------------------------------------------------------
        /// Creates a new image resource from file. Check the
        /// resource load state for success or failure
        ///
        /// @author S Downie
        ///
        /// @param The storage location to load from
        /// @param File path to resource
        /// @param Options to customise the creation
        /// @param [Out] Resource
        //-----------------------------------------------------------
        void CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource) override;
        //----------------------------------------------------
        /// Creates a new resource from file asynchronously.
        /// Completion delegate is called when load is finished.
        /// Check the resource load state for success or failure
        ///
        /// @author S Downie
        ///
        /// @param The storage location.
        /// @param The filepath.
        /// @param Options to customise the creation
        /// @param Completion delegate
        /// @param [Out] The output resource.
        //----------------------------------------------------
        void CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource) override;
    
    private:
        friend class Application;
        //-------------------------------------------------------
        /// Create the platform dependent backend
        ///
        /// @author S Downie
        ///
        /// @return New backend instance
        //-------------------------------------------------------
        static PVRImageProviderUPtr Create();
        //-------------------------------------------------------
        /// Private constructor to force use of factory method
        ///
        /// @author S Downie
        //-------------------------------------------------------
        PVRImageProvider() = default;
    };
}

#endif
