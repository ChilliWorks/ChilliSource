//
//  NSStringUtils.h
//  Chilli Source
//
//  Created by I Copland on 04/04/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_CORE_STRING_NSSTRINGUTILS_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_CORE_STRING_NSSTRINGUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>

#include <Foundation/Foundation.h>

//------------------------------------------------------------------
/// A collection of functions for working with NSStrings on iOS,
/// for example functions for converting to and from std::string.
///
/// @author I Copland
//------------------------------------------------------------------
@interface NSStringUtils : NSObject 
{
}
//----------------------------------------------------------
/// Converts from a NSString to a std::string
///
/// @author S Downie
///
/// @param The NSString
///
/// @return The std::string
//----------------------------------------------------------
+ (std::string) newStringWithNSString:(NSString*)string;
//----------------------------------------------------------
/// Converts from a NSString to a UTF8 string.
///
/// @author S Downie
///
/// @param The NSString
///
/// @return The UTF8 String.
//----------------------------------------------------------
+ (CSCore::UTF8String) newUTF8StringWithNSString:(NSString*)string;
//----------------------------------------------------------
/// Converts from an std::string to a NSString.
///
/// @author S Downie
///
/// @param The std::string.
///
/// @return The NSString.
//----------------------------------------------------------
+ (NSString*) newNSStringWithString:(const std::string&)string;
//----------------------------------------------------------
/// Converts from an UTF8 String to a NSString.
///
/// @author S Downie
///
/// @param The UTF8 String
///
/// @return The NSString.
//----------------------------------------------------------
+ (NSString*) newNSStringWithUTF8String:(const CSCore::UTF8String&)string;

@end

#endif
