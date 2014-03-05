//  
//  CHashSHA256.java
//  moFlow
//  
//  Created by Ian Copland on 16/12/2014
//  Copyright (c)2014 Tag Games Limited - All rights reserved
//

package com.chillisource.core;

import java.security.MessageDigest;

//===============================================================
/// CHashSHA256
///
/// Static methods for hashing using SHA256.
//===============================================================
public class HashSHA256 
{
	//-------------------------------------------------------
	/// Generate
	///
	/// The calculates the SHA-256 hash of the given byte
	/// array and returns it as a 32 byte array.
	///
	/// @param The data to hash.
	/// @return The 32 byte array.
	//-------------------------------------------------------
	public static byte[] Generate(final byte[] inabyData)
	{
		byte[] abyOutput = null;
	    try
	    {
	        MessageDigest digest = MessageDigest.getInstance("SHA-256");
	        digest.update(inabyData, 0, inabyData.length);
	        abyOutput = digest.digest();
	    }
	    catch (Exception e)
	    {
	    	android.util.Log.e("MoFlow", ExceptionUtils.ConvertToString(e));
	    }
	    
	    return abyOutput;
	}
}
