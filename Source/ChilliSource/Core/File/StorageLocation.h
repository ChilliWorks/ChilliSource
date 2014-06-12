//
//  StorageLocation.h
//  Chilli Source
//
//  Created by I Copland on 31/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_FILE_STORAGELOCATION_H_
#define _CHILLISOURCE_CORE_FILE_STORAGELOCATION_H_

namespace ChilliSource
{
    namespace Core
    {
        //-----------------------------------------------------------------
        /// An enum describing the different possible locations for
        /// files to be located.
        ///
        /// @author I Copland
        //-----------------------------------------------------------------
        enum class StorageLocation
        {
            k_none,
            k_root,
            k_package,
            k_saveData,
            k_cache,
            k_DLC,
            k_chilliSource
        };
    }
}

#endif
