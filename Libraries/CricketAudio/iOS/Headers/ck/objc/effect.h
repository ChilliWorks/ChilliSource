// Copyright 2016 Cricket Technology
// www.crickettechnology.com

#import <Foundation/Foundation.h>
#import "ck/effecttype.h"


/** Objective-C interface for CkEffect.
  See ck/effect.h for method documentation. */
@interface CkoEffect : NSObject

@property(nonatomic, assign) BOOL bypassed;
@property(nonatomic, assign) float wetDryRatio;

- (void) setParam:(int)paramId value:(float)value;

- (void) reset;

+ (nullable CkoEffect*) newEffect:(CkEffectType)type;
+ (nullable CkoEffect*) newCustomEffect:(int)id;

@end




