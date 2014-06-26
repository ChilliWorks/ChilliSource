//
//  ResourceProvider.h
//  Chilli Source
//  Created by Scott Downie on 30/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_RESOURCE_RESOURCEPROVIDER_H_
#define _CHILLISOURCE_CORE_RESOURCE_RESOURCEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace Core
	{
        //------------------------------------------------------------
		/// ResourceProviderOld defines an interface for systems which
        /// produce resource objects from files.
        ///
        /// @author S Downie
        //------------------------------------------------------------
		class ResourceProvider : public AppSystem
		{
		public:
            
            //----------------------------------------------------
            /// Delegate called from async load methods when the
            /// load finishes. Check the load state of the resource
            /// for success or failure
            ///
            /// @author S Downie
            ///
            /// @param Resource
            //----------------------------------------------------
            using AsyncLoadDelegate = std::function<void(const std::shared_ptr<Resource>&)>;
            
            CS_DECLARE_NAMEDTYPE(ResourceProvider);
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return The resource type this provider can create
            //----------------------------------------------------
			virtual InterfaceIDType GetResourceType() const = 0;
            //----------------------------------------------------
            /// Allows querying of the resource type this provider
            /// can create.
            ///
            /// @author S Downie
            ///
            /// @param The extension of the resource file.
            ///
            /// @return Whether or not the resource can be created.
            //----------------------------------------------------
			virtual bool CanCreateResourceWithFileExtension(const std::string& in_extension) const = 0;
            //----------------------------------------------------
            /// Creates a new resource from file.
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param Options to customise the creation
            /// @param Delegate to callback on completion either success or failure
            /// @param [Out] The output resource.
            //----------------------------------------------------
			virtual void CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource) = 0;
            //----------------------------------------------------
            /// Creates a new resource from file asynchronously.
            /// Check the load state on the resource for success
			/// or failure
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param Options to customise the creation
			/// @param Completion delegate
            /// @param [Out] The output resource.
            //----------------------------------------------------
			virtual void CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource) = 0;
            //----------------------------------------------------
            /// Options can be used to customise the loading
            /// of resources. Not all resource will require
            /// custom options so this function will return null.
            ///
            /// Default options are required for resources
            /// that use options in order to make options optional
            /// but ensure resource pool cache is correct
            ///
            /// @author S Downie
            ///
            /// @retrun Default options (null)
            //----------------------------------------------------
            virtual IResourceOptionsBaseCSPtr GetDefaultOptions() const;
            //----------------------------------------------------
            /// Destructor.
            ///
            /// @author S Downie
            //----------------------------------------------------
			virtual ~ResourceProvider() {}
		};
	}	
}

#endif