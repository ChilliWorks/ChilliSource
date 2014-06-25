//
//  CSAnimProvider.h
//  Chilli Source
//  Created by Ian Copland on 17/10/2011.
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
        /// @author Ian Copland
		//-------------------------------------------------------------------------
		class CSAnimProvider final : public Core::ResourceProvider
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(CSAnimProvider);
            
            //-------------------------------------------------------------------------
            /// Factory method for creating new anim loader
            ///
            /// @author Ian Copland
            //-------------------------------------------------------------------------
            static CSAnimProviderUPtr Create();
			//-------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Interface to compare
            ///
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return The resource type this provider can create
            //----------------------------------------------------
            Core::InterfaceIDType GetResourceType() const override;
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
			/// Load a skinned animation from the given CSAnim file. Check the load state of the
            /// resource for success or failure.
            ///
            /// @author Ian Copland
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Options to customise the creation
			/// @param [Out] the output resource pointer
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// Load a skinned animation from the given CSAnim file on a background
            /// thread. Delegate is called on completion. Check the load state of the
            /// resource for success or failure
            ///
            /// @author Ian Copland
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Options to customise the creation
            /// @param Delegate
			/// @param [Out] the output resource pointer
			//----------------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// Loads an anim file into a SkinnedAnimation.
            ///
            /// @author Ian Copland
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Completion delegate
			/// @param [Out] the output resource pointer
			//----------------------------------------------------------------------------
			void ReadSkinnedAnimationFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const SkinnedAnimationSPtr& out_resource) const;
		};
	}
}

#endif
