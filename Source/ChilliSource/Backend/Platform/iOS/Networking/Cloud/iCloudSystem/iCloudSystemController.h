//
//  ICloudSystemController.h
//  MMCoHMoFlow
//
//  Created by Hugh McLaughlin on 16/07/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/ChilliSource.h>
#import <ChilliSource/Backend/Platform/iOS/Networking/Cloud/iCloudSystem/ICloudSystem.h>
#import <ChilliSource/Core/Base/FastDelegate.h>
#import <ChilliSource/Networking/Cloud/CloudStorageSystem.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface CiCloudSystemController : NSObject
{
    ChilliSource::Networking::CloudStorageSystem::OnQueryFilesCompletedDelegate mpcQueryFilesCompleteDelegate;
    
    std::vector<NSURL*> mpcCachedResults;
    NSMutableArray *maOpenedFiles;
}

@property (strong) id query;

+ (CiCloudSystemController*) sharedInstance;

//-------------------------------------------------------------
/// openDocument
/// Attempts to open a document from cloud, if it doesnt exist it will create one based on args.
/// When the document is opened the delegate callback to the system will be invoked
/// @param instrFileName - Path to the file from iCloud ubiquity container 
/// @param incOpenFileCompleteDelegate - Delegate to call when the document has been opened/created, this is also called if there is an error (the system will handle it)
/// @param inbCreateIfNonExistant - used to tell the system to create file on the cloud if it doesnt already
//-------------------------------------------------------------
- (void)openDocument:(NSString*)instrFileName :(ChilliSource::iOS::CiCloudSystem::OnOpenCloudFileCompletedDelegate) incOpenFileCompleteDelegate :(BOOL)inbCreateIfNonExistant;

//-------------------------------------------------------------
/// writeDocumentWithRelativePath
/// Attempts to write contents to an existing file
/// @param instrFileName - Path to the file from iCloud ubiquity container 
/// @param InstrContents - Contents to write
/// @param InpCompleteDelegate - Delegate to call when completed
//-------------------------------------------------------------
- (void)writeDocumentWithRelativePath:(NSString*)InstrFileName :(NSMutableData*)InstrContents :(ChilliSource::Networking::CloudStorageSystem::OnSyncFileCompletedDelegate)InpCompleteDelegate;

//-------------------------------------------------------------
/// writeDocumentWithAbsolutePath
/// Attempts to write contents to an existing file
/// @param instrFileName - Absolute Path to the file 
/// @param InstrContents - Contents to write
/// @param InpCompleteDelegate - Delegate to call when completed
//-------------------------------------------------------------
- (void)writeDocumentWithAbsolutePath:(NSString*)InstrFileName :(NSMutableData*)InstrContents :(ChilliSource::Networking::CloudStorageSystem::OnSyncFileCompletedDelegate)InpCompleteDelegate;

//Querys iCloud for a list of files in cloud directory
- (void)queryContentsOfICloudDirectory:(ChilliSource::Networking::CloudStorageSystem::OnQueryFilesCompletedDelegate)incQueryFilesDelegate;

//-------------------------------------------------------------
/// stringForState
/// Constructs a string representing the state of a MoFloUIDocument
/// @param DocState - Document state as Integer 
/// @return String for the state
//-------------------------------------------------------------
+ (NSString *)stringForState:(UIDocumentState)state;

//////////////////////
//  Query Callbacks //
//////////////////////

//-------------------------------------------------------------
/// onQueryFilesCallback
/// Called when the queryContentsOfICloudDirectory is completed, will call the delegate with the filenames
/// @param Area - area on cloud saving to, 
/// @param RelativeFilePath - Additional path to file from Area directory
/// @return Constructed Path
//-------------------------------------------------------------
- (void)onQueryFilesCallback:(NSNotification*)incNotification;


@end
