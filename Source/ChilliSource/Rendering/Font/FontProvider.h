//
//  FontProvider.h
//  Chilli Source
//  Created by Scott Downie on 26/10/2010.
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

#ifndef _CHILLISOURCE_RENDERING_FONT_FONTPROVIDER_H_
#define _CHILLISOURCE_RENDERING_FONT_FONTPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //-------------------------------------------------------
        /// Factory class from loading font resources
        /// from file. The font resource is actually a compound
        /// resource of supported characters and kerning info
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class FontProvider final : public Core::ResourceProvider
		{
		public:

            CS_DECLARE_NAMEDTYPE(FontProvider);
            
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static FontProviderUPtr Create();
			//-------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface to compare
            ///
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The type of resource loaded by this provider
			//----------------------------------------------------------------------------
            Core::InterfaceIDType GetResourceType() const override;
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
            FontProvider() = default;
			//----------------------------------------------------------------------------
			/// Load the font from the external file into a resource. Check resource
            /// load state for success or failure
            ///
            /// @author S Downie
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Options to customise the creation
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Load the font from the external file into a resource on a background
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
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
            /// Function that does the heavy lifting of font loading. The 2 creation
            /// methods delegate to this one.
            ///
            /// @author S Downie
			///
            /// @param The storage location to load from
			/// @param Filename
            /// @param Completion delegate
			/// @param [Out] Font resource
			//----------------------------------------------------------------------------
			void LoadFont(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource);
		};
	}
}

#endif
