//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_CORE_IMAGE_PNGIMAGEPROVIDER_H_
#define _CSBACKEND_PLATFORM_RPI_CORE_IMAGE_PNGIMAGEPROVIDER_H_

#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/PNGImageProvider.h>

namespace CSBackend
{
	namespace RPi
	{
		/// The Raspberry Pi backend for the PNG Image Provider. This loads
		/// PNG images from file.
		///
		class PNGImageProvider final : public ChilliSource::PNGImageProvider
		{
		public:
			CS_DECLARE_NAMEDTYPE(PNGImageProvider);

			/// Queries whether or not this system implements the
			/// interface with the given Id.
			///
			/// @param interfaceId
			///		The id of the interface that we need to check if this system implements
			///
			/// @retrun TRUE if the system 'IsA' interface
			///
			bool IsA(ChilliSource::InterfaceIDType iinterfaceId) const override;

			/// @return The resource type this provider can load
			///
			ChilliSource::InterfaceIDType GetResourceType() const override;

			/// @param extension
			///		Extension to check that the system can load
			///
			/// @return TRUE if the object can load a resource with given extension
			///
			bool CanCreateResourceWithFileExtension(const std::string& extension) const override;

			/// Creates a new image resource from file. Check the
			/// resource load state for success or failure
			///
			/// @param location
			///		The storage location to load from
			/// @param filePath
			///		File path to resource
			/// @param options
			///		Options to customise the creation
			/// @param [Out] out_resource
			///		Assigned when the resource creation is finished
			///
			void CreateResourceFromFile(ChilliSource::StorageLocation location, const std::string& filePath, const ChilliSource::IResourceOptionsBaseCSPtr& options, const ChilliSource::ResourceSPtr& out_resource) override;

			/// Creates a new image resource from file asynchronously. Check the
			/// resource load state for success or failure. Delgate is called when creation
			/// is finished
			///
			/// @param location
			///		The storage location to load from
			/// @param filePath
			///		File path to resource
			/// @param options
			///		Options to customise the creation
			/// @param delegate
			///		Delegate called when finished
			/// @param [Out] out_resource
			///		Assigned when the resource creation is finished
			///
			void CreateResourceFromFileAsync(ChilliSource::StorageLocation location, const std::string& filePath, const ChilliSource::IResourceOptionsBaseCSPtr& options, const ChilliSource::ResourceProvider::AsyncLoadDelegate& delegate, const ChilliSource::ResourceSPtr& out_resource) override;
			
		private:
			friend ChilliSource::PNGImageProviderUPtr ChilliSource::PNGImageProvider::Create();
			//-----------------------------------------------------------
			/// Private constructor to force use of factory method
			///
			/// @author Ian Copland
			//-----------------------------------------------------------
			PNGImageProvider() = default;
		};
	}
}

#endif

#endif
