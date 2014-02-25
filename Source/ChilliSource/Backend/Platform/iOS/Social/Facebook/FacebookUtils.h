/*
 *  FacebookUtils.h
 *  moFlo
 *
 *  Created by Nicolas Tanda on 03/12/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PLATFORM_IOS_SOCIAL_FACEBOOK_FACEBOOKUTILS_H_
#define _MOFLO_PLATFORM_IOS_SOCIAL_FACEBOOK_FACEBOOKUTILS_H_

#include <ChilliSource/ChilliSource.h>
#import <UIKit/UIKit.h>

@interface URLParser : NSObject 

+(NSDictionary*)parseURLParams:(NSString *)query;

@end

#endif
