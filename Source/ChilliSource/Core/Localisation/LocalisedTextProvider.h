//
//  LocalisedTextProvider.h
//  Chilli Source
//  Created by Scott Downie on 25/04/2014.
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

#ifndef _CHILLISOURCE_CORE_LOCALISATION_LOCALISEDTEXTPROVIDER_H_
#define _CHILLISOURCE_CORE_LOCALISATION_LOCALISEDTEXTPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Core
	{
		//---------------------------------------------------------------
		/// Provider that loads localised text files and corresponding
		/// keys based on the current device language. All text is
        /// UTF8 encoded
		///
		/// @author S Downie
		//---------------------------------------------------------------
		class LocalisedTextProvider final : public ResourceProvider 
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(LocalisedTextProvider);
            
            //----------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface Id
			///
			/// @return Whether object is of given type
			//----------------------------------------------------
			bool IsA(InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------
			/// @author S Downie
			///
			/// @return The resource type this provider can create
			//----------------------------------------------------
			InterfaceIDType GetResourceType() const override;
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
			bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;
			//----------------------------------------------------
			/// Creates a localised text resource from the given file.
			/// Check the load state of the resource for
			/// success or failure
			///
			/// @author S Downie
			///
			/// @param The storage location.
			/// @param The filepath.
            /// @param Options to customise the creation
			/// @param Delegate to callback on completion either success or failure
			/// @param [Out] The output resource.
			//----------------------------------------------------
			void CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource) override;
			//----------------------------------------------------
			/// Creates a localised text resource asynchronously.
			/// The delegate will be called on completion. Check
			/// the load state for success or failure.
			///
			/// @author S Downie
			///
			/// @param The storage location.
			/// @param The filepath.
            /// @param Options to customise the creation
			/// @param Completion delegate
			/// @param [Out] The output resource.
			//----------------------------------------------------
			void CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource) override;

		private:

			friend class Application;
			//-------------------------------------------------------
			/// Factory method
			///
			/// @author S Downie
			///
			/// @return New backend with ownership transferred
			//-------------------------------------------------------
			static LocalisedTextProviderUPtr Create();
			//-------------------------------------------------------
			/// Private constructor to force use of factory method
			///
			/// @author S Downie
			//-------------------------------------------------------
			LocalisedTextProvider() = default;
		};
	}
}

#endif