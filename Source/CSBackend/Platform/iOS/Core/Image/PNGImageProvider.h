//
//  PNGImageProvider.h
//  Chilli Source
//  Created by S McGaw on 01/10/2010.
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

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_PLATFORM_IOS_IMAGE_PNGIMAGEPROVIDER_H_
#define _CSBACKEND_PLATFORM_IOS_IMAGE_PNGIMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/Image/PNGImageProvider.h>

namespace CSBackend
{
	namespace iOS
	{
        //----------------------------------------------------------------
        /// The iOS backend for the PNG resource provider.
        ///
        /// @author S McGaw
        //----------------------------------------------------------------
		class PNGImageProvider final : public CSCore::PNGImageProvider
		{
		public:
            CS_DECLARE_NAMEDTYPE(PNGImageProvider);
			//----------------------------------------------------------
			/// Is the object of the given interface type
            ///
            /// @author S McGaw
            ///
			/// @param Interface type to query
            ///
			/// @return Whether the object is of given type
			//----------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The resource type this provider can load
            //-------------------------------------------------------
            CSCore::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------
			/// Whether or not the provider can create resources from
            /// files with the given extension.
            ///
			/// @author S McGaw
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
            /// @author S McGaw
            ///
            /// @param The storage location to load from
			/// @param File path to resource
            /// @param Options to customise the creation
			/// @param [Out] Resource
			//-----------------------------------------------------------
			void CreateResourceFromFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, const CSCore::IResourceOptionsBaseCSPtr& in_options, const CSCore::ResourceSPtr& out_resource) override;
            //----------------------------------------------------
            /// Creates a new resource from file asynchronously.
            /// Completion delegate is called when load is finished.
            /// Check the resource load state for success or failure
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param Options to customise the creation
            /// @param Completion delegate
            /// @param [Out] The output resource.
            //----------------------------------------------------
			void CreateResourceFromFileAsync(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, const CSCore::IResourceOptionsBaseCSPtr& in_options, const CSCore::ResourceProvider::AsyncLoadDelegate& in_delegate, const CSCore::ResourceSPtr& out_resource) override;
        private:
            
            friend CSCore::PNGImageProviderUPtr CSCore::PNGImageProvider::Create();
            //-----------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-----------------------------------------------------------
            PNGImageProvider() = default;
		};
	}
}

#endif

#endif
