package com.chillisource.toolutils;

import java.io.DataOutputStream;
import java.io.FileOutputStream;

public class LittleEndianOutputStream 
{
	//-------------------------------------------------------
	/// Member Data
	//-------------------------------------------------------
	DataOutputStream mStream;
	//-------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------
	public LittleEndianOutputStream(final String inFilename) throws Exception
	{
		mStream = new DataOutputStream(new FileOutputStream(inFilename));
	}
	//-------------------------------------------------------
	/// Close
	///
	/// Closes the stream.
	//-------------------------------------------------------
	public void close() throws Exception
	{
		mStream.close();
	}
	//-------------------------------------------------------
	/// Write Char
	///
	/// Writes a single char to the output stream.
	///
	/// @param the char.
	//-------------------------------------------------------
	public void writeChar(char in_data) throws Exception
	{
		writeByte((byte)(in_data 			& 0xFF));
		writeByte((byte)(in_data >>> 8 		& 0xFF));
	}
	//-------------------------------------------------------
	/// Write Byte
	///
	/// Writes a single byte to the output stream.
	///
	/// @param the byte.
	//-------------------------------------------------------
	public void writeByte(byte inbyData) throws Exception
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
	public void writeBytes(byte[] inabyData) throws Exception
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
	public void writeBoolean(boolean inbData) throws Exception
	{
		if (inbData == true)
			writeByte((byte)1);
		else
			writeByte((byte)0);
	}
	//-------------------------------------------------------
	/// Write Int
	///
	/// Writes a single Integer to the output stream.
	///
	/// @param the integer.
	//-------------------------------------------------------
	public void writeInt(int indwData) throws Exception
	{
		writeByte((byte)(indwData 			& 0xFF));
		writeByte((byte)(indwData >>> 8 	& 0xFF));
		writeByte((byte)(indwData >>> 16 	& 0xFF));
		writeByte((byte)(indwData >>> 24	& 0xFF));
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
	public void writeUnsignedInt(long indwData) throws Exception
	{
		writeByte((byte)(indwData 			& 0xFF));
		writeByte((byte)(indwData >>> 8 	& 0xFF));
		writeByte((byte)(indwData >>> 16 	& 0xFF));
		writeByte((byte)(indwData >>> 24	& 0xFF));
	}
	//-------------------------------------------------------
	/// Write Short
	///
	/// Writes a single unsigned short to the output stream.
	///
	/// @param the short.
	//-------------------------------------------------------
	public void writeShort(short inwData) throws Exception
	{
		writeByte((byte)(inwData 			& 0xFF));
		writeByte((byte)(inwData >>> 8 		& 0xFF));
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
	public void writeUnsignedShort(int indwData) throws Exception
	{
		writeByte((byte)(indwData 			& 0xFF));
		writeByte((byte)(indwData >>> 8 	& 0xFF));
	}
	//-------------------------------------------------------
	/// Write Float
	///
	/// Writes a single float to the output stream.
	///
	/// @param the float.
	//-------------------------------------------------------
	public void writeFloat(float infData) throws Exception
	{
		writeInt(Float.floatToIntBits(infData));
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
	public void writeAsciiString(final String instrData) throws Exception
	{
		for (int i = 0; i < instrData.length(); i++)
		{
			writeByte((byte)instrData.charAt(i));
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
	public void writeNullTerminatedAsciiString(final String instrData) throws Exception
	{
		writeAsciiString(instrData);
		writeByte((byte)0);
	}
	//-------------------------------------------------------
	/// Write Utf8 String
	///
	/// Converts the given string to UTF-8 format and writes
	/// it to the output stream.
	///
	/// @param the string.
	//-------------------------------------------------------
	public void writeUtf8String(final String instrData) throws Exception
	{
		byte[] abyUtf8 = StringUtils.stringToUTF8Bytes(instrData);
		writeBytes(abyUtf8);
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
	public void writeNullTerminatedUtf8String(final String instrData) throws Exception
	{
		writeUtf8String(instrData);
		writeByte((byte)0);
	}
}
