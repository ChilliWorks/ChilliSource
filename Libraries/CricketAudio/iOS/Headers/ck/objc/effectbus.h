// Copyright 2016 Cricket Technology
// www.crickettechnology.com

#import <Foundation/Foundation.h>

@class CkoEffect;

/** Objective-C interface for CkEffectBus.
  See ck/effectbus.h for method documentation. */
@interface CkoEffectBus : NSObject

@property(nonatomic, assign) BOOL bypassed;
@property(nonatomic, assign) float wetDryRatio;
@property(nonatomic, assign, nullable) CkoEffectBus* outputBus;

- (void) addEffect:(nonnull CkoEffect*)effect;
- (void) removeEffect:(nonnull CkoEffect*)effect;
- (void) removeAllEffects;

- (void) reset;

+ (nullable CkoEffectBus*) newEffectBus;
+ (nonnull CkoEffectBus*) globalEffectBus;

@end





