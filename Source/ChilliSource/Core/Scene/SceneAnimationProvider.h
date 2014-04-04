//
//  SceneAnimationLoader.h
//  Chilli Source
//
//  Created by A Glass on 19/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_SCENE_SCENEANIMATIONPROVIDER_H_
#define _CHILLISOURCE_CORE_SCENE_SCENEANIMATIONPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProviderOld.h>

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------
        /// A resource provider for loading scene animations
        /// from file.
        ///
        /// @author A Glass
        //-------------------------------------------------------
        class SceneAnimationProvider final : public ResourceProviderOld
        {
        public:
            CS_DECLARE_NAMEDTYPE(SceneAnimationProvider);
            //-------------------------------------------------
            /// Queries whether or not this class implements
            /// the interface with the given Id.
            ///
            /// @author A Glass.
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //-------------------------------------------------
            bool IsA(InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------
            /// Queries whether or not this provider can create
            /// resources with the given interface Id.
            ///
            /// @author A Glass.
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the resource can be
            /// created.
            //-------------------------------------------------
            bool CanCreateResourceOfKind(InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------
            /// Queries whether or not this provider can create
            /// resources from files with the given extension.
            ///
            /// @author A Glass.
            ///
            /// @param The extension.
            ///
            /// @return Whether or not the resource can be
            /// created.
            //-------------------------------------------------
            bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const override;
            //-------------------------------------------------
            /// Queries whether or not this provider can create
            /// resources from files with the given extension.
            ///
            /// @author A Glass.
            ///
            /// @param The extension.
            ///
            /// @return Whether or not the resource can be
            /// created.
            //-------------------------------------------------
            bool CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filepath, ResourceOldSPtr& out_resource) override;
            //----------------------------------------------------
            /// Creates a new resource from file asynchronously.
            /// The resource will be returned immediately but
            /// cannot be used until the loaded flag is set. This
            /// can be queried using IsLoaded() on the resource.
            ///
            /// @author I Copland
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param [Out] The output resource.
            ///
            /// @return Whether or not the resource async load was
            /// successfully started.
            //----------------------------------------------------
			bool AsyncCreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceOldSPtr& out_resource) override;
        };
    }
}

#endif
