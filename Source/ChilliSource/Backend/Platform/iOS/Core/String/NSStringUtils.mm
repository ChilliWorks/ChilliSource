//
//  NSStringUtils.cpp
//  Chilli Source
//
//  Created by I Copland on 04/04/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>

#include <ChilliSource/Core/String/UTF8String.h>

#include <Foundation/Foundation.h>

namespace ChilliSource
{
    namespace iOS
    {
        namespace NSStringUtils
        {
            //----------------------------------------------------------
            //----------------------------------------------------------
            std::string NSStringToString(NSString* in_nsString)
            {
                if(in_nsString == nil)
                {
                    return "";
                }
                
                return [in_nsString cStringUsingEncoding:NSUTF8StringEncoding];
            }
            //----------------------------------------------------------
            //----------------------------------------------------------
            Core::UTF8String NSStringToUTF8String(NSString* in_string)
            {
                if (in_string == nil)
                {
                    return "";
                }
                
                NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding (kCFStringEncodingUTF8);
                NSData* nsData = [in_string dataUsingEncoding : encode];
                
                return Core::UTF8String((Core::UTF8String::CodePoint*) [nsData bytes], [nsData length]);
            }
            //----------------------------------------------------------
            //----------------------------------------------------------
            NSString* StringToNSString(const std::string& instrString)
            {
                return [NSString stringWithCString:instrString.c_str() encoding: NSUTF8StringEncoding];
            }
            //----------------------------------------------------------
            //----------------------------------------------------------
            NSString* UTF8StringToNSString(const Core::UTF8String& instrString)
            {
                s8* data = (s8*)instrString.data();
                size_t length = instrString.data_size();
                
                if(data == nullptr || data == 0)
                {
                    return [NSString string];
                }
                
                NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding (kCFStringEncodingUTF8);
                NSString* result = [[[NSString alloc] initWithBytes:data length:length encoding:encode] autorelease];
                return result;
            }
        }
    }
}