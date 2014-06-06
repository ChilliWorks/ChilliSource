//  
//  CException.java
//  moFlow
//  
//  Created by Ian Copland on 07/01/2014
//  Copyright (c)2014 Tag Games Limited - All rights reserved
// 

package com.chillisource.core;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

//==================================================================
/// AES Encrpytion
///
/// A series of methods for use when using AES Encryption.
//==================================================================
public class AESEncryption 
{
	//------------------------------------------------------------
	/// Encrypt
	///
	/// Encrypts the given data with AES Encryption.
	///
	/// @param The input data. This does not need to be padded.
	/// @param The private key.
	/// @return The encrypted data.
	//------------------------------------------------------------
	public static byte[] Encrypt(byte[] inabyData, byte[] inabyPrivateKey)
	{
		byte[] abyOutput = null;
		try
		{
			Cipher cipher = Cipher.getInstance("AES");
	        SecretKeySpec keySpec = new SecretKeySpec(inabyPrivateKey, "AES");
	        cipher.init(Cipher.ENCRYPT_MODE, keySpec);
	        abyOutput = cipher.doFinal(inabyData);
		}
		catch (Exception e)
		{
			ExceptionUtils.ConvertToString(e);
		}
        return abyOutput;
	}
	//------------------------------------------------------------
	/// Decrypt
	///
	/// Decrypts the given data with AES Encryption.
	///
	/// @param The encrypted data.
	/// @param The private key.
	/// @return The data.
	//------------------------------------------------------------
	public static byte[] Decrypt(byte[] inabyEncryptedData, byte[] inabyPrivateKey)
	{
		byte[] abyOutput = null;
		try
		{
			Cipher cipher = Cipher.getInstance("AES");
	        SecretKeySpec keySpec = new SecretKeySpec(inabyPrivateKey, "AES");
	        cipher.init(Cipher.DECRYPT_MODE, keySpec);
	        abyOutput = cipher.doFinal(inabyEncryptedData);
		}
		catch (Exception e)
		{
			ExceptionUtils.ConvertToString(e);
		}
        return abyOutput;
	}
}
