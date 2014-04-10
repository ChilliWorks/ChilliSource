//
//  NSStringUtils.cpp
//  Chilli Source
//
//  Created by I Copland on 04/04/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>

#include <ChilliSource/Core/String/UTF8String.h>

@implementation NSStringUtils

//----------------------------------------------------------
//----------------------------------------------------------
+ (std::string) newStringWithNSString:(NSString*)string
{
    if(string == nil)
    {
        return "";
    }
    
    return [string cStringUsingEncoding:NSUTF8StringEncoding];
}
//----------------------------------------------------------
//----------------------------------------------------------
+ (CSCore::UTF8String) newUTF8StringWithNSString:(NSString*)string
{
    if (string == nil)
    {
        return "";
    }
    
    return [string cStringUsingEncoding:NSUTF8StringEncoding];
}
//----------------------------------------------------------
//----------------------------------------------------------
+ (NSString*) newNSStringWithString:(const std::string&)string
{
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding (kCFStringEncodingUTF8);
    return [[NSString alloc] initWithBytes:string.data() length:string.size() encoding:encode];
}
//----------------------------------------------------------
//----------------------------------------------------------
+ (NSString*) newNSStringWithUTF8String:(const CSCore::UTF8String&)string
{
    if(string.length() == 0)
    {
        return [[NSString alloc] init];
    }
    
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding (kCFStringEncodingUTF8);
    return [[NSString alloc] initWithBytes:string.data() length:string.data_size() encoding:encode];
}

@end