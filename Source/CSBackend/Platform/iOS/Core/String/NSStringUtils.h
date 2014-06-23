//
//  NSStringUtils.h
//  Chilli Source
//  Created by Ian Copland on 04/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_PLATFORM_IOS_CORE_STRING_NSSTRINGUTILS_H_
#define _CSBACKEND_PLATFORM_IOS_CORE_STRING_NSSTRINGUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>

#include <Foundation/Foundation.h>

//------------------------------------------------------------------
/// A collection of functions for working with NSStrings on iOS,
/// for example functions for converting to and from std::string.
///
/// @author Ian Copland
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
/// @return The std::string containing UTF-8 codepoints
//----------------------------------------------------------
+ (std::string) newUTF8StringWithNSString:(NSString*)string;
//----------------------------------------------------------
/// Converts from an std::string to a NSString.
///
/// @author S Downie
///
/// @param The std::string of UTF-8 codepoints.
///
/// @return The NSString.
//----------------------------------------------------------
+ (NSString*) newNSStringWithUTF8String:(const std::string&)string;

@end

#endif

#endif