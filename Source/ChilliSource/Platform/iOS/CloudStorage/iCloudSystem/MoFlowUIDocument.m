//
//  MoFlowUIDocument.m
//  MMCoHMoFlow
//
//  Created by Hugh McLaughlin on 16/07/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#import "MoFlowUIDocument.h"

@implementation MoFlowUIDocument

NSString* EMPTY_STRING = @"";

@synthesize fileContents;

// Called whenever the application reads data from the file system
- (BOOL)loadFromContents:(id)contents ofType:(NSString *)typeName error:(NSError **)outError
{
    
    if ([contents length] > 0) {
        NSMutableData* data = [NSMutableData dataWithData:contents];
        self.fileContents = data;
        
    } else 
    {
        //Make sure there is some contents to read
        self.fileContents = [NSMutableData dataWithData:[EMPTY_STRING dataUsingEncoding:NSUTF8StringEncoding]]; // When the note is created we assign some default content
    }

    [[NSNotificationCenter defaultCenter] postNotificationName:@"noteModified" 
                                                        object:self];        
    
    return YES;
    
}

// Called whenever the application (auto)saves the content of a note
- (id)contentsForType:(NSString *)typeName error:(NSError **)outError 
{
    //Make sure there is some contents to write
    if ([self.fileContents length] == 0) 
    {
        self.fileContents = [NSMutableData dataWithData:[EMPTY_STRING dataUsingEncoding:NSUTF8StringEncoding]];
    }

    return self.fileContents;
    
}

- (NSMutableData*)contents
{
    return self.fileContents;
}

- (int)getContentLength
{
    return [[self contents] length];
}

- (s8*)contentsAsData;
{
    NSMutableData* pData = self.fileContents;
    
    //Return a casted s8* from the contents mutablebytes
    return (s8*)[pData mutableBytes];
}

- (const std::string)contentsAsSTDString;
{
    //Construct a string 
    return std::string([self contentsAsData], [self getContentLength]);
}
@end
