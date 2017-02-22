// Copyright 2016 Cricket Technology
// www.crickettechnology.com

#import <Foundation/Foundation.h>


/** Objective-C interface for CkMixer.
  See ck/mixer.h for method documentation. */
@interface CkoMixer : NSObject

@property(nonatomic, copy, nonnull) NSString* name;
@property(nonatomic, assign) float volume;
@property(nonatomic, readonly) float mixedVolume;
@property(nonatomic, assign) BOOL paused;
@property(nonatomic, readonly) BOOL mixedPauseState;
@property(nonatomic, assign, nullable) CkoMixer* parent;

+ (nonnull CkoMixer*) master;

+ (nullable CkoMixer*) find:(nonnull NSString*)name;

+ (nullable CkoMixer*) newMixer:(nonnull NSString*)name;
+ (nullable CkoMixer*) newMixer:(nonnull NSString*)name parent:(nullable CkoMixer*)parent;

@end



