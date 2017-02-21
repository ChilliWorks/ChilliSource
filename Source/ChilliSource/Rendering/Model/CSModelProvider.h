//
//  CSModelProvider.h
//  ChilliSource
//  Created by Ian Copland on 25/08/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_MODEL_CSMODELPROVIDER_H_
#define _CHILLISOURCE_RENDERING_MODEL_CSMODELPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
    //-------------------------------------------------------------------------
    /// Loads model files into a Model resource.
    //-------------------------------------------------------------------------
    class CSModelProvider final : public ResourceProvider
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(CSModelProvider);
        
        //-------------------------------------------------------------------------
        /// Factory method for creating provider
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------------
        static CSModelProviderUPtr Create();
        //-------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param Interface to compare
        ///
        /// @return Whether the object implements the given interface
        //-------------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------
        /// @author S Downie
        ///
        /// @return The resource type this provider can create
        //----------------------------------------------------
        InterfaceIDType GetResourceType() const override;
        //----------------------------------------------------------------------------
        /// @author Ian Copland
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
        /// @author Ian Copland
        ///
        /// @param The storage location to load from
        /// @param File path
        /// @param Options to customise the creation
        /// @param [Out] the output resource pointer
        //----------------------------------------------------------------------------
        void CreateResourceFromFile(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource) override;
        //----------------------------------------------------------------------------
        /// Create mesh resource from model file on a background thread
        ///
        /// @author Ian Copland
        ///
        /// @param The storage location to load from
        /// @param File path
        /// @param Options to customise the creation
        /// @param Delegate to callback on completion either success or failure
        /// @param [Out] the output resource pointer
        //----------------------------------------------------------------------------
        void CreateResourceFromFileAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource) override;
        //----------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The storage location to load from
        /// @param File path
        /// @param Delegate to callback on completion either success or failure
        /// @param the output resource pointer
        //----------------------------------------------------------------------------
        void LoadMeshDataTask(StorageLocation in_location, const std::string& in_filePath, const AsyncLoadDelegate& in_delegate, const ModelSPtr& out_resource);
    };
}

#endif
