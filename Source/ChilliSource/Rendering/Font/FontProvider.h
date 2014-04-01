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
			/// @param Type to compare
            ///
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const override;
			
		private:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            FontProvider() = default;
			//----------------------------------------------------------------------------
			/// Load the font from the external file into a resource.
            ///
            /// @author S Downie
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] Resource object
            ///
			/// @return Whether the resource was created successfully
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Load the font from the external file into a resource on a background
            /// thread
            ///
            /// @author S Downie
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] Resource object
            ///
			/// @return Whether the resource loading was scheduled
			//----------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
		};
	}
}

#endif
