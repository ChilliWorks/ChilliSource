//
//  NSStringUtils.cpp
//  Chilli Source
//
//  Created by I Copland on 04/04/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

@implementation NSStringUtils

//----------------------------------------------------------
//----------------------------------------------------------
+ (std::string) newUTF8StringWithNSString:(NSString*)string
{
    if(string == nil)
    {
        return "";
    }
    
    return [string cStringUsingEncoding:NSUTF8StringEncoding];
}
//----------------------------------------------------------
//----------------------------------------------------------
+ (NSString*) newNSStringWithUTF8String:(const std::string&)string
{
    if(string.length() == 0)
    {
        return [[NSString alloc] init];
    }
    
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding (kCFStringEncodingUTF8);
    return [[NSString alloc] initWithBytes:string.data() length:string.size() encoding:encode];
}

@end

#endif