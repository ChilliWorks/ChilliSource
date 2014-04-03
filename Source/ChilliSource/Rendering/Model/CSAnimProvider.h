//
//  CSAnimProvider.h
//  Chilli Source
//
//  Created by Ian Copland on 17/10/2011.
//  Copyright 2011 Tag Games Ltd. All rights reserved.
//


#ifndef _CHILLISOURCE_RENDERING_MODEL_CSANIMPROVIDER_H_
#define _CHILLISOURCE_RENDERING_MODEL_CSANIMPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-------------------------------------------------------------------------
        /// Loads CS Anim files into a SkinnedAnimation resource
        ///
        /// @author I Copland
		//-------------------------------------------------------------------------
		class CSAnimProvider final : public Core::ResourceProvider
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(CSAnimProvider);
            
            //-------------------------------------------------------------------------
            /// Factory method for creating new anim loader
            ///
            /// @author I Copland
            //-------------------------------------------------------------------------
            static CSAnimProviderUPtr Create();
			//-------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Interface to compare
            ///
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Type to compare
            ///
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const override;

		private:
			//----------------------------------------------------------------------------
			/// Load a skinned animation from the given CSAnim file
            ///
            /// @author I Copland
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] the output resource pointer
            ///
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// Load a skinned animation from the given CSAnim file on a background
            /// thread
            ///
            /// @author I Copland
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] the output resource pointer
            ///
			/// @return whether or not async load was started
			//----------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// Loads an anim file into a SkinnedAnimation.
            ///
            /// @author I Copland
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] the output resource pointer
            ///
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			void ReadSkinnedAnimationFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const SkinnedAnimationSPtr& out_resource) const;
		};
	}
}

#endif
