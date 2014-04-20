//
//  StringParser.h
//  Chilli Source
//
//  Created by Ian Copland on 25/02/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_STRING_STRINGPARSER_H_
#define _CHILLISOURCE_CORE_STRING_STRINGPARSER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2Old.h>
#include <ChilliSource/Core/Math/Vector3Old.h>
#include <ChilliSource/Core/Math/Vector4Old.h>
#include <ChilliSource/Core/Math/Matrix4x4Old.h>
#include <ChilliSource/Core/Math/QuaternionOld.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------
        /// Parse a string to a 32 bit float. If the string cannot be
        /// be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The float value.
        //------------------------------------------------------------
        f32 ParseF32(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 32 bit signed integer. If the string
        /// cannot be be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The integer value.
        //------------------------------------------------------------
        s32 ParseS32(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 32 bit unsigned integer. If the string
        /// cannot be be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The integer value.
        //------------------------------------------------------------
        u32 ParseU32(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 64 bit signed integer. If the string
        /// cannot be be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The integer value.
        //------------------------------------------------------------
        s64 ParseS64(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 32 bit unsigned integer. If the string
        /// cannot be be parsed this will return 0.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The integer value.
        //------------------------------------------------------------
        u64 ParseU64(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 32 bit unsigned integer. True will be
        /// returned if the string is "true", "yes" or "1" and false
        /// will be returned in all other cases.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The boolean value.
        //------------------------------------------------------------
        bool ParseBool(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 2 component vector. The vector must be
        /// in the format "x y", i.e F32 values, space delimited. Failure
        /// to parse will result in a zero vector.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The vector value.
        //------------------------------------------------------------
        Vector2Old ParseVector2Old(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 3 component vector. The vector must be
        /// in the format "x y z", i.e F32 values, space delimited. Failure
        /// to parse will result in a zero vector.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The vector value.
        //------------------------------------------------------------
        Vector3Old ParseVector3Old(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a 4 component vector. The vector must be
        /// in the format "x y z w", i.e F32 values, space delimited.
        /// Failure to parse will result in a zero vector.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The vector value.
        //------------------------------------------------------------
        Vector4Old ParseVector4Old(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a matrix. The matrix should be provided
        /// in row major format, space delimited. failure to parse will
        /// result in an identity matrix.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The matrix value.
        //------------------------------------------------------------
        Matrix4x4Old ParseMatrix4(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a quaternion. The quaternion must be
        /// in the format "w x y z", i.e F32 values, space delimited.
        /// Failure to parse will result in an identity quaternion.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The quaternion value.
        //------------------------------------------------------------
        QuaternionOld ParseQuaternionOld(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a string to a colour. The colour must be
        /// in the format "r g b a", i.e F32 values, space delimited.
        /// "r g b" will also be accepted. Failure to parse will result
        /// in a black colour.
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The colour value.
        //------------------------------------------------------------
        Colour ParseColour(const std::string& in_string);
        //------------------------------------------------------------
        /// Parse a storage location. Acceptable storage locations are:
        ///  - Package
        ///  - DLC
        ///  - SaveData
        ///
        /// @author Ian Copland
        ///
        /// @param The string.
        ///
        /// @return The colour value.
        //------------------------------------------------------------
        StorageLocation ParseStorageLocation(const std::string &in_string);
    }
}

#endif
