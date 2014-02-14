//
//  ICloudSystemController.m
//  MMCoHMoFlow
//
//  Created by Hugh McLaughlin on 16/07/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#import <ChilliSource/Backend/Platform/iOS/Networking/CloudStorage/iCloudSystem/ICloudSystemController.h>
#import <ChilliSource/Backend/Platform/iOS/Networking/CloudStorage/iCloudSystem/MoFlowUIDocument.h>

@implementation CiCloudSystemController

static id _instance;
@synthesize query = _query;

+ (CiCloudSystemController*) sharedInstance
{
    if(_instance == nil)
    {
        _instance = [[CiCloudSystemController alloc] init];
    }
    
    return _instance;
}

- (id)init
{
    self = [super init];
    return self;
}

- (void)queryContentsOfICloudDirectory:(moFlo::ICloudStorageSystem::OnQueryFilesCompletedDelegate)incQueryFilesDelegate;
{
	if(NSClassFromString(@"NSMetadataQuery") != nil)
	{
		mpcQueryFilesCompleteDelegate = incQueryFilesDelegate;
		
		id query = [[NSClassFromString(@"NSMetadataQuery") alloc] init];
		_query = query;
		[query setSearchScopes:[NSArray arrayWithObject:NSMetadataQueryUbiquitousDocumentsScope]];
		NSPredicate *pred = [NSPredicate predicateWithFormat: @"%K LIKE '*'", NSMetadataItemFSNameKey];
		[query setPredicate:pred];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onQueryFilesCallback:) name:NSMetadataQueryDidFinishGatheringNotification object:query];
		
		[query startQuery];
	}
}

- (void)openDocument:(NSString*)instrFileName :(moFlo::iOSPlatform::CiCloudSystem::OnOpenCloudFileCompletedDelegate) incOpenFileCompleteDelegate :(BOOL)inbCreateIfNonExistant
{
    NSURL *ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
    NSURL *ubiquitousPackage = [ubiq URLByAppendingPathComponent:instrFileName];
    
    NSLog(@"ICloudSystemController::openDocument - Opening Document - %@ ", ubiquitousPackage.absoluteString);
    MoFlowUIDocument *doc = [[MoFlowUIDocument alloc] initWithFileURL:ubiquitousPackage];

    [doc openWithCompletionHandler:^(BOOL fileExists)
     {
         if(fileExists)
         {
             //Let the system know that this doc has been opened
             incOpenFileCompleteDelegate(doc, NO);
         }
         else if(inbCreateIfNonExistant)
         {
             MoFlowUIDocument *newDoc = [[MoFlowUIDocument alloc] initWithFileURL:ubiquitousPackage];
             
             [newDoc saveToURL:[newDoc fileURL] forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL saveSuccess) {
                 if (saveSuccess)
                 {
                     [newDoc openWithCompletionHandler:^(BOOL success)
                      {
                          if(success)
                          {
                              //Let the system know that this doc has been opened
                              incOpenFileCompleteDelegate(newDoc, YES);
                          }
                          else
                          {
                              assert(false);
                          }
                      }];
                 }
                 else
                 {
                     ERROR_LOG("ICloudSystemController::openDocument:: Document could not be saved!");
                     incOpenFileCompleteDelegate(newDoc, NO);
                 }
             }];
         }
         else {
             ERROR_LOG("ICloudSystemController::openDocument - File not found, not creating cloud file");
             incOpenFileCompleteDelegate(doc, NO);
         }
         
     }];
    
    [doc release];
}

- (void)writeDocumentWithRelativePath:(NSString*)InstrFileName :(NSMutableData*)InstrContents :(moFlo::ICloudStorageSystem::OnSyncFileCompletedDelegate)InpCompleteDelegate
{
    NSURL *ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
    NSURL *ubiquitousPackage = [ubiq URLByAppendingPathComponent:InstrFileName];
    
    NSLog(@"ICloudSystemController::writeDocument - Writing Document to cloud - %@ ", ubiquitousPackage.absoluteString);
    MoFlowUIDocument *doc = [[MoFlowUIDocument alloc] initWithFileURL:ubiquitousPackage];

    std::string instrContents = std::string((s8*)InstrContents, [InstrContents length]);
    [doc setFileContents:InstrContents];

    [doc saveToURL:[doc fileURL] forSaveOperation:UIDocumentSaveForOverwriting completionHandler:^(BOOL success) {            
        if (success) {
            [doc closeWithCompletionHandler:^(BOOL fileCloseSuccess) 
             {
                 if(fileCloseSuccess)
                 {
                     //Call the complete delegate to let the listener know the operation has completed
                     if(InpCompleteDelegate)
                         InpCompleteDelegate();
                     
                     [doc release];
                 }
                 else
                 {
                     ERROR_LOG("ICloudSystemController::writeDocumentWithRelativePath - Document Failed to close succesfully");
                 }
             }];                
        }
    }];
}

- (void)writeDocumentWithAbsolutePath:(NSString*)InstrFileName :(NSMutableData*)InstrContents :(moFlo::ICloudStorageSystem::OnSyncFileCompletedDelegate)InpCompleteDelegate
{
    NSLog(@"ICloudSystemController::writeDocument - Writing Document - %@ ", InstrFileName);
    MoFlowUIDocument *doc = [[MoFlowUIDocument alloc] initWithFileURL:([NSURL URLWithString:InstrFileName])];

    [doc setFileContents:InstrContents];
    
    [doc saveToURL:[doc fileURL] forSaveOperation:UIDocumentSaveForOverwriting completionHandler:^(BOOL success) {            
        if (success) {
            [doc closeWithCompletionHandler:^(BOOL fileCloseSuccess) 
            {
                if(fileCloseSuccess)
                {
                    //Call the complete delegate to let the listener know the operation has completed
                    if(InpCompleteDelegate)
                        InpCompleteDelegate();
                }
                else
                {
                    ERROR_LOG("ICloudSystemController::writeDocumentWithAbsolutePath - Document Failed to close succesfully");
                }
            }];                
        }
        else 
        {
            ERROR_LOG("ICloudSystemController::writeDocumentWithAbsolutePath - Document could not be saved succesfully");
        }
    }];
}

#pragma mark Query Callbacks

- (void)onQueryFilesCallback:(NSNotification*)incNotification
{
    if(mpcQueryFilesCompleteDelegate)
    {
        //Clear out the cached contents
        mpcCachedResults.clear();
        
        moFlo::ICloudStorageSystem::ICloudFileList mvFileList;
        
        NSEnumerator *e = [[[incNotification object] results] objectEnumerator];
        NSMetadataItem *result;
        
        while (result = [e nextObject])
        {
            //Get the URL and store the cached results
            NSURL *url1 = [result valueForAttribute:NSMetadataItemURLKey];
            mpcCachedResults.push_back(url1);
            
            //Store the results to a vector for returning to the system;
            NSLog(@"Query Result - %@", [url1 absoluteString] );
            mvFileList.push_back(std::string([[url1 absoluteString] UTF8String]));
        }
    
        mpcQueryFilesCompleteDelegate(mvFileList);
    }
}

+ (NSString *)stringForState:(UIDocumentState)state {
    NSMutableArray * states = [NSMutableArray array];
    if (state == 0) {
        [states addObject:@"Normal"];
    }
    if (state & UIDocumentStateClosed) {
        [states addObject:@"Closed"];
    }
    if (state & UIDocumentStateInConflict) {
        [states addObject:@"In Conflict"];
    }
    if (state & UIDocumentStateSavingError) {
        [states addObject:@"Saving error"];
    }
    if (state & UIDocumentStateEditingDisabled) {
        [states addObject:@"Editing disabled"];
    }
    return [states componentsJoinedByString:@", "];
}

@end
