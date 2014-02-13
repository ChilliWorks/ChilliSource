/*
 *  FacebookUtils.mm
 *  moFlo
 *
 *  Created by Nicolas Tanda on 03/12/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#import "FacebookUtils.h"


@implementation URLParser
    /**
     * A function for parsing URL parameters.
     */
    + (NSDictionary*)parseURLParams:(NSString *)query
    {
        NSArray *pairs = [query componentsSeparatedByString:@"&"];
        NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
        for (NSString *pair in pairs)
        {
            NSArray *kv = [pair componentsSeparatedByString:@"="];
            NSString *val =
            [[kv objectAtIndex:1]
             stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
            
            [params setObject:val forKey:[kv objectAtIndex:0]];
        }
        return params;
    }
@end
