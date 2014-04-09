//
//  FontProvider.h
//  Chilli Source
//
//  Created by Scott Downie on 26/10/2010.
//  Copyright 2010 Tag Games Ltd. All rights reserved.
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
			/// @param Filename
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Load the font from the external file into a resource on a background
            /// thread. Delegate is called when the load is complete. Check resource
            /// load state for success or failure.
            ///
            /// @author S Downie
			///
            /// @param The storage location to load from
			/// @param Filename
            /// @param Completion delegate
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource) override;
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
            void LoadFont(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource);
		};
	}
}

#endif
