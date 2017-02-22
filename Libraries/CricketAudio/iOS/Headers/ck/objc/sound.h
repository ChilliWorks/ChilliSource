// Copyright 2016 Cricket Technology
// www.crickettechnology.com

#import "ck/objc/pathtype.h"
#import "ck/objc/attenuationmode.h"

@class CkoBank;
@class CkoMixer;
@class CkoEffectBus;

/** Objective-C interface for CkSound.
  See ck/sound.h for method documentation. */
@interface CkoSound : NSObject

@property(nonatomic, assign, null_resettable) CkoMixer* mixer;
@property(nonatomic, readonly) BOOL ready;
@property(nonatomic, readonly) BOOL failed;
@property(nonatomic, readonly) BOOL playing;
@property(nonatomic, assign) BOOL paused;
@property(nonatomic, assign) int loopCount;
@property(nonatomic, readonly) int loopStart;
@property(nonatomic, readonly) int loopEnd;
@property(nonatomic, readonly) int currentLoop;
@property(nonatomic, readonly) BOOL loopReleased;
@property(nonatomic, assign) float volume;
@property(nonatomic, assign) int playPosition;
@property(nonatomic, assign) float playPositionMs;
@property(nonatomic, readonly) float mixedVolume;
@property(nonatomic, assign) float pan;
@property(nonatomic, assign) float pitchShift;
@property(nonatomic, assign) float speed;
@property(nonatomic, assign, nullable) CkoSound* nextSound;
@property(nonatomic, readonly) int length;
@property(nonatomic, readonly) float lengthMs;
@property(nonatomic, readonly) int sampleRate;
@property(nonatomic, readonly) int channels;
@property(nonatomic, assign, nullable) CkoEffectBus* effectBus;
@property(nonatomic, assign) BOOL threeDEnabled;
@property(nonatomic, readonly) BOOL isVirtual;

- (void) play;
- (void) stop;

- (void) setLoop:(int)startFrame endFrame:(int)endFrame;
- (void) releaseLoop;

- (void) setPanMatrixLL:(float)ll LR:(float)lr RL:(float)rl RR:(float)rr;
- (void) getPanMatrixLL:(nonnull float*)ll LR:(nonnull float*)lr RL:(nonnull float*)rl RR:(nonnull float*)rr;

- (void) set3dPositionX:(float)x y:(float)y z:(float)z;
- (void) get3dPositionX:(nonnull float*)x y:(nonnull float*)y z:(nonnull float*)z;
- (void) set3dVelocityX:(float)x y:(float)y z:(float)z;
- (void) get3dVelocityX:(nonnull float*)x y:(nonnull float*)y z:(nonnull float*)z;

+ (void) set3dListenerPositionEyeX:(float)eyeX eyeY:(float)eyeY eyeZ:(float)eyeZ
           lookAtX:(float)lookAtX lookAtY:(float)lookAtY lookAtZ:(float)lookAtZ
           upX:(float)upX upY:(float)upY upZ:(float)upZ;
+ (void) get3dListenerPositionEyeX:(nonnull float*)eyeX eyeY:(nonnull float*)eyeY eyeZ:(nonnull float*)eyeZ
           lookAtX:(nonnull float*)lookAtX lookAtY:(nonnull float*)lookAtY lookAtZ:(nonnull float*)lookAtZ
           upX:(nonnull float*)upX upY:(nonnull float*)upY upZ:(nonnull float*)upZ;
+ (void) set3dListenerVelocityX:(float)x y:(float)y z:(float)z;
+ (void) get3dListenerVelocityX:(nonnull float*)x y:(nonnull float*)y z:(nonnull float*)z;

+ (void) set3dAttenuationMode:(CkAttenuationMode)mode nearDist:(float)nearDist farDist:(float)farDist farVol:(float)farVol;
+ (void) get3dAttenuationMode:(nonnull CkAttenuationMode*)mode nearDist:(nonnull float*)nearDist farDist:(nonnull float*)farDist farVol:(nonnull float*)farVol;

+ (void) set3dSoundSpeed:(float)speed;
+ (float) get3dSoundSpeed;

+ (float) getSoundSpeedCentimetersPerSecond;
+ (float) getSoundSpeedMetersPerSecond;
+ (float) getSoundSpeedInchesPerSecond;
+ (float) getSoundSpeedFeetPerSecond;

+ (nullable CkoSound*) newBankSound:(nullable CkoBank*)bank index:(int)index;
+ (nullable CkoSound*) newBankSound:(nullable CkoBank*)bank name:(nonnull NSString*)name;
+ (nullable CkoSound*) newStreamSound:(nonnull NSString*)path; 
+ (nullable CkoSound*) newStreamSound:(nonnull NSString*)path pathType:(CkPathType)pathType;
+ (nullable CkoSound*) newStreamSound:(nonnull NSString*)path pathType:(CkPathType)pathType offset:(int)offset length:(int)length extension:(nonnull NSString*)extension;
#if CK_PLATFORM_IOS
+ (nullable CkoSound*) newAssetStreamSound:(nonnull NSURL*)url; 
#endif

@end


