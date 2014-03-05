//  
//  CHashCRC32.java
//  moFlow
//  
//  Created by Ian Copland on 09/01/2014
//  Copyright (c)2014 Tag Games Limited - All rights reserved
//

package com.chillisource.core;

import java.util.zip.CRC32;

//===============================================================
/// CHashCRC32
///
/// Static methods for hashing using CRC-32.
//===============================================================
public class HashCRC32 
{
	//-------------------------------------------------------
	/// Generate
	///
	/// The calculates the CRC-32 hash of the given byte
	/// array and returns it as an integer.
	///
	/// @param The data to hash.
	/// @return The integer hash.
	//-------------------------------------------------------
	public static int Generate(final byte[] inabyData)
	{
        CRC32 crc32 = new CRC32();
        crc32.update(inabyData);
        return (int)crc32.getValue();
	}
}
