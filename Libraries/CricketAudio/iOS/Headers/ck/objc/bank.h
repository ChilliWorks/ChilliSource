// Copyright 2016 Cricket Technology
// www.crickettechnology.com

#import <Foundation/Foundation.h>
#import "ck/objc/pathtype.h"


/** Objective-C interface for CkBank.
  See ck/bank.h for method documentation. */
@interface CkoBank : NSObject

@property(nonatomic, readonly) BOOL loaded;
@property(nonatomic, readonly) BOOL failed;
@property(nonatomic, readonly, nonnull) NSString* name;
@property(nonatomic, readonly) int numSounds;

- (nonnull NSString*) soundName:(int)index;

+ (nullable CkoBank*) newBank:(nonnull NSString*)path;
+ (nullable CkoBank*) newBank:(nonnull NSString*)path pathType:(CkPathType)pathType;
+ (nullable CkoBank*) newBank:(nonnull NSString*)path pathType:(CkPathType)pathType offset:(int)offset length:(int)length;
+ (nullable CkoBank*) newBankAsync:(nonnull NSString*)path;
+ (nullable CkoBank*) newBankAsync:(nonnull NSString*)path pathType:(CkPathType)pathType;
+ (nullable CkoBank*) newBankAsync:(nonnull NSString*)path pathType:(CkPathType)pathType offset:(int)offset length:(int)length;
+ (nullable CkoBank*) newBankFromMemory:(nonnull void*)buf bytes:(int)bytes;

+ (nullable CkoBank*) find:(nonnull NSString*)name;

@end




