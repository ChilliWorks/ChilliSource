//
//  MoFlowUIDocument.h
//  MMCoHMoFlow
//
//  Extends the Apple UIDocument class for use in the iCloud System
//
//  Created by Hugh McLaughlin on 16/07/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/ChilliSource.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface MoFlowUIDocument : UIDocument

//@property (strong) NSString * fileContents;
@property (strong) NSMutableData * fileContents;

//Returns the raw contents as MutableData
- (NSMutableData*)contents;

//Returns the length of the file
- (int)getContentLength;

//Returns the contents as s8* data
- (s8*)contentsAsData;

//Returns the contents as a std::string
- (const std::string)contentsAsSTDString;

@end
