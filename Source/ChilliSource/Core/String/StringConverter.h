/** 
 * File: StringConverter.h
 * Date: 25/10/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_STRINGCONVERTER_H_
#define _MOFLO_CORE_STRINGCONVERTER_H_

#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Colour.h>
#include <ChilliSource/Core/FileIO/FileSystem.h>

namespace moFlo
{
	namespace Core
	{
		class CStringConverter
		{
		public:
			/** Converts a f32 to a String. */
			static std::string ToString(f32 inVal, unsigned short precision = 6, 
										unsigned short width = 0, char fill = ' ', 
										std::ios::fmtflags flags = std::ios::fmtflags(0) );
            
			static std::string ToString(f64 inVal, unsigned short precision = 6,
										unsigned short width = 0, char fill = ' ',
										std::ios::fmtflags flags = std::ios::fmtflags(0) );

			/** Converts an int to a String. */
			static std::string ToString(s32 inVal, unsigned short width, 
										char fill, 
										std::ios::fmtflags flags = std::ios::fmtflags(0) );
            static std::string ToString(s32 inVal);


			/** Converts an unsigned int to a String. */
			static std::string ToString(u32 inVal,
										unsigned short width, char fill, 
										std::ios::fmtflags flags = std::ios::fmtflags(0) );
            static std::string ToString(u32 inVal);
            
#ifdef TARGET_OS_IPHONE
            static std::string ToString(size_t inVal);
#endif
			
			/** Converts a long to a String. */
			static std::string ToString(s64 inVal, 
										unsigned short width = 0, char fill = ' ', 
										std::ios::fmtflags flags = std::ios::fmtflags(0) );


			static std::string ToString(u64 inVal, 
										unsigned short width = 0, char fill = ' ', 
										std::ios::fmtflags flags = std::ios::fmtflags(0) );


			
			/** Converts a boolean to a String. 
			 @param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
			 */
			static std::string ToString(bool inVal, bool yesNo = false);
			/** Converts a Vector2 to a String. 
			 @remarks
			 Format is "x y" (i.e. 2x f32 inValues, space delimited)
			 */
			static std::string ToString(const CVector2& inVal);

			/** Converts a Vector3 to a String. 
			 @remarks
			 Format is "x y z" (i.e. 3x f32 inValues, space delimited)
			 */
			static std::string ToString(const CVector3& inVal);

			/** Converts a Vector4 to a String. 
			 @remarks
			 Format is "x y z w" (i.e. 4x f32 inValues, space delimited)
			 */
			static std::string ToString(const CVector4& inVal);

			/** Converts a Matrix3 to a String. 
			 @remarks
			 Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
			 */
			
			static std::string ToString(const CMatrix4x4& inVal);

			static std::string ToString(const CQuaternion& inVal);

			/** Converts a ColourValue to a String. 
			 @remarks
			 Format is "r g b a" (i.e. 4x f32 inValues, space delimited). 
			 */
			static std::string ToString(const CColour& inVal);

            /** Converts a Storage Location to a string.
			 */
            static std::string ToString(STORAGE_LOCATION instrStorageLocation);
            
			/** Converts a StringVector to a string.
			 @remarks
			 Strings must not contain spaces since space is used as a delimiter in
			 the output.
			 */
			/** Converts a std::string to a f32. 
			 @returns
			 0.0 if the inValue could not be Parsed, otherwise the f32 version of the String.
			 */
			static f32 ParseFloat(const std::string& inVal);

			/** Converts a std::string to a whole number. 
			 @returns
			 0.0 if the inValue could not be Parsed, otherwise the numeric version of the String.
			 */
			static int ParseInt(const std::string& inVal);

			/** Converts a std::string to a whole number. 
			 @returns
			 0.0 if the inValue could not be Parsed, otherwise the numeric version of the String.
			 */
			static unsigned int ParseUnsignedInt(const std::string& inVal);

			/** Converts a std::string to a whole number. 
			 @returns
			 0.0 if the inValue could not be Parsed, otherwise the numeric version of the String.
			 */
			static long ParseLong(const std::string& inVal);

			/** Converts a std::string to a whole number. 
			 @returns
			 0.0 if the inValue could not be Parsed, otherwise the numeric version of the String.
			 */
			static unsigned long ParseUnsignedLong(const std::string& inVal);

			/** Converts a std::string to a boolean. 
			 @remarks
			 Returns true if case-insensitive match of the start of the string
			 matches "true", "yes" or "1", false otherwise.
			 */
			static bool ParseBool(const std::string& inVal);

			/** Parses a Vector2 out of a String.
			 @remarks
			 Format is "x y" ie. 2 f32 components, space delimited. Failure to Parse returns
			 Vector2::ZERO.
			 */
			static CVector2 ParseVector2(const std::string& inVal);

			/** Parses a Vector3 out of a String.
			 @remarks
			 Format is "x y z" ie. 3 f32 components, space delimited. Failure to Parse returns
			 Vector3::ZERO.
			 */
			static CVector3 ParseVector3(const std::string& inVal);

			/** Parses a Vector4 out of a String.
			 @remarks
			 Format is "x y z w" ie. 4 f32 components, space delimited. Failure to Parse returns
			 Vector4::ZERO.
			 */
			static CVector4 ParseVector4(const std::string& inVal);

			/** Parses a Matrix4 out of a String.
			 @remarks
			 Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
			 '01' means row 0 column 1 etc. Failure to Parse returns Matrix4::IDENTITY.
			 */
			static CMatrix4x4 ParseMatrix4(const std::string& inVal);

			/** Parses a Quaternion out of a String. 
			 @remarks
			 Format is "w x y z" (i.e. 4x f32 inValues, space delimited). 
			 Failure to Parse returns Quaternion::IDENTITY.
			 */
			static CQuaternion ParseQuaternion(const std::string& inVal);

			/** Parses a ColourValue out of a String. 
			 @remarks
			 Format is "r g b a" (i.e. 4x f32 inValues, space delimited), or "r g b" which implies
			 an alpha inValue of 1.0 (opaque). Failure to Parse returns ColourValue::Black.
			 */
			static CColour ParseColourValue(const std::string& inVal);
            
            /** Parses a Storage Location out of a String.
			 */
            static STORAGE_LOCATION ParseStorageLocation(const std::string &instrFunc);
            
			/** Checks the std::string is a inValid number inValue. */
			static bool isNumber(const std::string& inVal);		
		};
	}
}

#endif
