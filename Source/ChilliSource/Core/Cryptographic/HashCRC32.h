//
//  HashCRC32.h
//  iOSTemplate
//
//  Created by Scott Downie on 06/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_HASH_CRC_32_H_
#define _MO_FLO_HASH_CRC_32_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace HashCRC32
        {
            //-------------------------------------------------------------
            /// Generate Hash Code
            ///
            /// @param String to hash
            /// @return CRC 32 hash code
            //-------------------------------------------------------------
            u32 GenerateHashCode(const std::string &instrVal);
            //-------------------------------------------------------------
            /// Generate Hash Code
            ///
            /// @param Null terminated string to hash
            /// @return CRC 32 hash code
            //-------------------------------------------------------------
            u32 GenerateHashCode(const s8* instrVal);
            //-------------------------------------------------------------
            /// Generate Hash Code
            ///
            /// @param Data to hash
            /// @param Size of data in bytes
            /// @return CRC 32 hash code
            //-------------------------------------------------------------
            u32 GenerateHashCode(const s8* inbyVal, u32 inudwSizeInBytes);
        }
    }
}

#endif