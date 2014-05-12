//
//  CSAppDelegate.mm
//  Chilli Source
//  Created by Scott Downie on 08/02/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#import <ChilliSource/Backend/Platform/iOS/Core/Base/CSAppDelegate.h>

#import <ChilliSource/Backend/Platform/iOS/Core/Base/CSGLViewController.h>
#import <ChilliSource/Backend/Platform/iOS/Core/Notification/LocalNotificationSystem.h>
#import <ChilliSource/Backend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>
#import <ChilliSource/Core/Base/Application.h>

CSAppDelegate* singletonInstance = nil;

@implementation CSAppDelegate

//--------------------------------------------------------------------
//--------------------------------------------------------------------
+ (CSAppDelegate*) sharedInstance
{
    return singletonInstance;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
- (void) addAppDelegateListener:(id<UIApplicationDelegate>)in_delegate
{
    [subdelegates addObject:in_delegate];
}

#pragma mark -
#pragma mark Application lifecycle
//-------------------------------------------------------------
/// Called when the app is first launched. The application can
/// be launched with a dictionary of options.
///
/// The event is passed onto the listeners.
///
/// @author S Downie
///
/// @param Application
/// @param Launching options
//-------------------------------------------------------------
- (BOOL) application:(UIApplication*)in_application didFinishLaunchingWithOptions:(NSDictionary*)in_launchingOptions
{
    singletonInstance = self;
    
    subdelegates = [[NSMutableArray alloc] init];
    
    csApplication = CreateApplication();
    
#if TARGET_IPHONE_SIMULATOR
    if ([NSClassFromString(@"WebView") respondsToSelector:@selector(_enableRemoteInspector)])
    {
        [NSClassFromString(@"WebView") performSelector:@selector(_enableRemoteInspector)];
    }
#endif
    
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	viewController = [[CSGLViewController alloc] initWithDelegate:self];
    [window setRootViewController: viewController];
    [window makeKeyAndVisible];
    
    csApplication->Init();
    
    ChilliSource::iOS::LocalNotificationSystem::ApplicationDidFinishLaunchingWithOptions(in_application, in_launchingOptions);
    
    isFirstActive = YES;
    
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate application:in_application didFinishLaunchingWithOptions:in_launchingOptions];
    }
    
	return YES;
}
//-------------------------------------------------------------
/// Called when the app is suspended. This can occur
/// when the user exits the application of as a result
/// of interruptions such as phone calls.
///
/// The event is passed onto the listeners.
///
/// @author S Downie
///
/// @param Application
//-------------------------------------------------------------
- (void)applicationWillResignActive:(UIApplication*)in_application
{
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate applicationWillResignActive:in_application];
    }
    
    UIBackgroundTaskIdentifier bgTask = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil];
    
    dispatch_async(dispatch_get_main_queue(), ^
    {
        csApplication->Background();
        [[UIApplication sharedApplication] endBackgroundTask:bgTask];
    });
}
//-------------------------------------------------------------
/// Called when the app is backgrounded.
/// The event is passed onto the listeners.
///
/// @author S Downie
///
/// @param Application
//-------------------------------------------------------------
- (void)applicationDidEnterBackground:(UIApplication*)in_application
{
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate applicationDidEnterBackground:in_application];
    }
    
    UIBackgroundTaskIdentifier bgTask = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil];
    
    dispatch_async(dispatch_get_main_queue(), ^
    {
        csApplication->Suspend();
        [[UIApplication sharedApplication] endBackgroundTask:bgTask];
    });
}
//-------------------------------------------------------------
/// Called when the app is resumed after being in the background.
/// The event is passed onto the listeners.
///
/// @author S Downie
///
/// @param Application
//-------------------------------------------------------------
- (void)applicationWillEnterForeground:(UIApplication*)in_application
{
    csApplication->Resume();
    
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate applicationWillEnterForeground:in_application];
    }
}
//-------------------------------------------------------------
/// Called when the app is resumed after being started or
/// re-activated. The event is passed onto the listeners.
///
/// @author S Downie
///
/// @param Application
//-------------------------------------------------------------
- (void)applicationDidBecomeActive:(UIApplication*)in_application
{
    if(isFirstActive == YES)
    {
        isFirstActive = NO;
        csApplication->Resume();
    }
    
    csApplication->Foreground();
    
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate applicationDidBecomeActive:in_application];
    }
}
//-------------------------------------------------------------
/// Called when the app is about to terminate. Allowing CS
/// to suspend safely. The event is passed onto the listeners.
///
/// @author S Downie
///
/// @param Application
//-------------------------------------------------------------
- (void)applicationWillTerminate:(UIApplication*)in_application
{
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate applicationWillTerminate:in_application];
    }
    
    UIBackgroundTaskIdentifier bgTask = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil];
        
    dispatch_async(dispatch_get_main_queue(), ^
    {
        csApplication->Suspend();
        [[UIApplication sharedApplication] endBackgroundTask:bgTask];
    });
}

#pragma mark -
#pragma mark GL Lifecycle
//-------------------------------------------------------------
/// The main update loop for the application
///
/// @author S Downie
///
/// @param GL view controller
//-------------------------------------------------------------
- (void)glkViewControllerUpdate:(GLKViewController*)controller;
{
    ChilliSource::Core::Application::Get()->Update(controller.timeSinceLastUpdate, controller.timeSinceFirstResume);
}
//-------------------------------------------------------------
/// The main render loop for the application
///
/// @author S Downie
///
/// @param GL view
/// @param Canvas size
//-------------------------------------------------------------
- (void)glkView:(GLKView*)view drawInRect:(CGRect)rect
{
    ChilliSource::Core::Application::Get()->Render();
}

#pragma mark -
#pragma mark Memory management
//-------------------------------------------------------------
/// Called when the device is low on memory and tells the
/// application to purge its resource caches. Passes
/// the event onto the listeners
///
/// @author S Downie
///
/// @param Application
//-------------------------------------------------------------
- (void)applicationDidReceiveMemoryWarning:(UIApplication*)in_application
{
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate applicationDidReceiveMemoryWarning:in_application];
    }
    
    csApplication->ApplicationMemoryWarning();
}

#pragma mark -
#pragma mark Local notifications
//-------------------------------------------------------------
/// Called when the app receives a local notification from
/// the OS. This notification is passed into the LocalNotificationSystem
/// and the listeners
///
/// @author S Downie
///
/// @param Application
/// @param Notification
//-------------------------------------------------------------
- (void)application:(UIApplication*)in_application didReceiveLocalNotification:(UILocalNotification*)in_notification
{
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate application:in_application didReceiveLocalNotification:in_notification];
    }
    
    ChilliSource::iOS::LocalNotificationSystem::ApplicationDidReceiveLocalNotification(in_application, in_notification);
}

#pragma mark -
#pragma mark Remote notifications
//-------------------------------------------------------------
/// Called when a request for a remote notification token
/// is successful. The token is passed to the RemoteNotificationSystem
/// and the listeners
///
/// @author S Downie
///
/// @param Application
/// @param Token
//-------------------------------------------------------------
- (void)application:(UIApplication*)in_application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)in_deviceToken
{
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate application:in_application didRegisterForRemoteNotificationsWithDeviceToken:in_deviceToken];
    }
    
	ChilliSource::iOS::RemoteNotificationSystem* remoteNotificationSystem = CSCore::Application::Get()->GetSystem<ChilliSource::iOS::RemoteNotificationSystem>();
    if(nullptr != remoteNotificationSystem)
    {
        CS_LOG_WARNING("Received new remote notification token");
        remoteNotificationSystem->OnRemoteTokenReceived(in_deviceToken);
    }
    else
    {
        CS_LOG_ERROR("Error: Unable to get remote notification system. Has the system been created?");
    }
}
//-------------------------------------------------------------
/// Called when the request for a remote notification token
/// fails
///
/// @author S Downie
///
/// @param Application
/// @param Error
//-------------------------------------------------------------
- (void)application:(UIApplication*)in_application didFailToRegisterForRemoteNotificationsWithError:(NSError*)in_error
{
	NSLog(@"Failed to receive remote notification token %@", [in_error localizedDescription]);
    
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate application:in_application didFailToRegisterForRemoteNotificationsWithError:in_error];
    }
}
//-------------------------------------------------------------
/// Called when the app receives a remote/push notification
/// from an external sender. This funnels the notification into
/// the RemoteNotification system and invokes the delegate listeners
///
/// @author S Downie
///
/// @param Application
/// @param Dictionary containing the notification data
//-------------------------------------------------------------
- (void)application:(UIApplication*)in_application didReceiveRemoteNotification:(NSDictionary*)in_userInfo
{
	ChilliSource::iOS::RemoteNotificationSystem* remoteNotificationSystem = CSCore::Application::Get()->GetSystem<ChilliSource::iOS::RemoteNotificationSystem>();
    if(nullptr != remoteNotificationSystem)
    {
        CS_LOG_VERBOSE("Received remote notification");
        remoteNotificationSystem->OnRemoteNotificationReceived(in_application, in_userInfo);
    }
    else
    {
        CS_LOG_ERROR("Error: Unable to get remote notification system. Has the system been created?");
    }
    
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        [delegate application:in_application didReceiveRemoteNotification:in_userInfo];
    }
}
//-------------------------------------------------------------
/// Dealloc cleans up the application and UIViews
///
/// @author S Downie
//-------------------------------------------------------------
- (void)dealloc
{
    csApplication->Destroy();
	CS_SAFEDELETE(csApplication);
    
	[viewController release];
    [window release];
    [subdelegates release];
    
    singletonInstance = nil;
    
    [super dealloc];
}

@end
