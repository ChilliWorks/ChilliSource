package com.taggames.toolutils;

import java.io.DataOutputStream;
import java.io.FileOutputStream;

public class CLittleEndianOutputStream 
{
	//-------------------------------------------------------
	/// Member Data
	//-------------------------------------------------------
	DataOutputStream mStream;
	//-------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------
	public CLittleEndianOutputStream(final String inFilename) throws Exception
	{
		mStream = new DataOutputStream(new FileOutputStream(inFilename));
	}
	//-------------------------------------------------------
	/// Close
	///
	/// Closes the stream.
	//-------------------------------------------------------
	public void Close() throws Exception
	{
		mStream.close();
	}
	//-------------------------------------------------------
	/// Write Byte
	///
	/// Writes a single byte to the output stream.
	///
	/// @param the byte.
	//-------------------------------------------------------
	public void WriteByte(byte inbyData) throws Exception
	{
		mStream.writeByte(inbyData);
	}
	//-------------------------------------------------------
	/// Write Bytes
	///
	/// Writes a byte array to the output stream.
	///
	/// @param the byte array
	//-------------------------------------------------------
	public void WriteBytes(byte[] inabyData) throws Exception
	{
		for (byte byByte : inabyData)
		{
			mStream.writeByte(byByte);
		}
	}
	//-------------------------------------------------------
	/// Write Boolean
	///
	/// Writes a single boolean to the output stream.
	///
	/// @param the boolean.
	//-------------------------------------------------------
	public void WriteBoolean(boolean inbData) throws Exception
	{
		if (inbData == true)
			WriteByte((byte)1);
		else
			WriteByte((byte)0);
	}
	//-------------------------------------------------------
	/// Write Byte
	///
	/// Writes a single Integer to the output stream.
	///
	/// @param the integer.
	//-------------------------------------------------------
	public void WriteInt(int indwData) throws Exception
	{
		WriteByte((byte)(indwData 			& 0xFF));
		WriteByte((byte)(indwData >>> 8 	& 0xFF));
		WriteByte((byte)(indwData >>> 16 	& 0xFF));
		WriteByte((byte)(indwData >>> 24	& 0xFF));
	}
	//-------------------------------------------------------
	/// Write Unsigned Int
	///
	/// Writes a single unsigned integer to the output stream.
	/// As java doesnt handle unsigned types, this uses a long.
	/// This will give weird results if the given value is
	/// bigger than max unsigned int.
	///
	/// @param a long representing the unsigned it.
	//-------------------------------------------------------
	public void WriteUnsignedInt(long indwData) throws Exception
	{
		WriteByte((byte)(indwData 			& 0xFF));
		WriteByte((byte)(indwData >>> 8 	& 0xFF));
		WriteByte((byte)(indwData >>> 16 	& 0xFF));
		WriteByte((byte)(indwData >>> 24	& 0xFF));
	}
	//-------------------------------------------------------
	/// Write Short
	///
	/// Writes a single unsigned short to the output stream.
	///
	/// @param the short.
	//-------------------------------------------------------
	public void WriteShort(short inwData) throws Exception
	{
		WriteByte((byte)(inwData 			& 0xFF));
		WriteByte((byte)(inwData >>> 8 		& 0xFF));
	}
	//-------------------------------------------------------
	/// Write Unsigned Short
	///
	/// Writes a single unsigned short to the output stream.
	/// As java doesnt handle unsigned types, this uses an int.
	/// This will give weird results if the given value is
	/// bigger than max unsigned short.
	///
	/// @param an int representing the unsigned it.
	//-------------------------------------------------------
	public void WriteUnsignedShort(int indwData) throws Exception
	{
		WriteByte((byte)(indwData 			& 0xFF));
		WriteByte((byte)(indwData >>> 8 	& 0xFF));
	}
	//-------------------------------------------------------
	/// Write Float
	///
	/// Writes a single float to the output stream.
	///
	/// @param the float.
	//-------------------------------------------------------
	public void WriteFloat(float infData) throws Exception
	{
		WriteInt(Float.floatToIntBits(infData));
	}
	//-------------------------------------------------------
	/// Write Ascii String
	///
	/// Converts the given string to ASCII format and writes
	/// it to the output stream. If the string contains characters
	/// that are not available in ascii they will be not be
	/// correctly written.
	///
	/// @param the string.
	//-------------------------------------------------------
	public void WriteAsciiString(final String instrData) throws Exception
	{
		for (int i = 0; i < instrData.length(); i++)
		{
			WriteByte((byte)instrData.charAt(i));
		}
	}
	//-------------------------------------------------------
	/// Write Null Terminated Ascii String
	///
	/// Converts the given string to ASCII format and writes
	/// it to the output stream. If the string contains characters
	/// that are not available in ascii they will be not be
	/// correctly written. The string will be terminated with
	/// a null byte.
	///
	/// @param the string.
	//-------------------------------------------------------
	public void WriteNullTerminatedAsciiString(final String instrData) throws Exception
	{
		WriteAsciiString(instrData);
		WriteByte((byte)0);
	}
	//-------------------------------------------------------
	/// Write Utf8 String
	///
	/// Converts the given string to UTF-8 format and writes
	/// it to the output stream.
	///
	/// @param the string.
	//-------------------------------------------------------
	public void WriteUtf8String(final String instrData) throws Exception
	{
		byte[] abyUtf8 = SCStringUtils.StringToUTF8Bytes(instrData);
		WriteBytes(abyUtf8);
	}
	//-------------------------------------------------------
	/// Write Null Terminated Utf8 String
	///
	/// Converts the given string to UTF-8 format and writes
	/// it to the output stream. The string will be terminated 
	/// with a null byte.
	///
	/// @param the string.
	//-------------------------------------------------------
	public void WriteNullTerminatedUtf8String(final String instrData) throws Exception
	{
		WriteUtf8String(instrData);
		WriteByte((byte)0);
	}
}
