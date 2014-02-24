//
//  BaseEncoding.h
//  iOSTemplate
//
//  Created by Scott Downie on 06/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_BASE_ENCODING_H_
#define _MO_FLO_CORE_BASE_ENCODING_H_

namespace ChilliSource
{
    namespace Core
    {
        class CBaseEncoding
        {
        public:
            
            //--------------------------------------------------------------------------
            /// Base 64 Encode
            ///
            /// @param String to encode
            /// @return Base 64 encoded string
            //--------------------------------------------------------------------------
            static std::string Base64Encode(const std::string& instrVal);
            //--------------------------------------------------------------------------
            /// Base 64 Encode
            ///
            /// @param Data to encode
            /// @param Size of data in bytes
            /// @return Base 64 encoded string
            //--------------------------------------------------------------------------
            static std::string Base64Encode(const s8* inpData, u32 inudwSize);
            //--------------------------------------------------------------------------
            /// Base 64 Decode
            ///
            /// @param Base 64 encoded string
            /// @return Decoded string
            //--------------------------------------------------------------------------
            static std::string Base64Decode(const std::string& instrVal);
            //--------------------------------------------------------------------------
            /// Base 64 Decode
            ///
            /// @param Base 64 encoded string
            /// @param Decode data array
            /// @param Decode data array size
            /// @return Size of decoded data;
            //--------------------------------------------------------------------------
            static s32 Base64Decode(const std::string& instrVal, s8* inpDataOut, u32 inudwSize);
            //--------------------------------------------------------------------------
            /// Char to Hex
            /// Converts a value in the range [0,255] to hex value
            ///
            /// @param Integer value in range [0,255]
            /// @return String hex representation of integer
            //--------------------------------------------------------------------------
            static std::string CharToHex(u8 inubyDec);
            //--------------------------------------------------------------------------
            /// Encodes a URL to use correct escape values
            ///
            /// @param String to URL encode
            /// @return URL encoded string
            //--------------------------------------------------------------------------
            static std::string URLEncode(const std::string& instrURL);
            //--------------------------------------------------------------------------
            /// Hex to Dec
            ///
            /// @param A single hexidecimal value to convert
            /// @return Base ten representation of hex value or -1
            //--------------------------------------------------------------------------
            static u8 HexToDec(const u8* inpHex);
            //--------------------------------------------------------------------------
            /// Decodes a URL to a string
            ///
            /// @param String to URL dncode
            /// @return decoded string
            //--------------------------------------------------------------------------
            static std::string URLDecode(const std::string& instrSrc);
        };
    }
}

#endif