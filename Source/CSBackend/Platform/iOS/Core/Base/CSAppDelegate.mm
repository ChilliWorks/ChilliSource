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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Core/Base/CSAppDelegate.h>

#import <CSBackend/Platform/iOS/Core/Base/CSGLViewController.h>
#import <CSBackend/Platform/iOS/Core/Notification/LocalNotificationSystem.h>
#import <CSBackend/Platform/iOS/Core/Notification/RemoteNotificationSystem.h>
#import <ChilliSource/Core/Base/Application.h>

CSAppDelegate* singletonInstance = nil;

@implementation CSAppDelegate

@synthesize viewController = viewControllerInternal;

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
//--------------------------------------------------------------------
//--------------------------------------------------------------------
- (void) removeAppDelegateListener:(id<UIApplicationDelegate>)in_delegate
{
    [subdelegates removeObject:in_delegate];
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
- (void) setPreferredFPS:(NSUInteger)in_fps
{
    viewControllerInternal.preferredFramesPerSecond = in_fps;
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
    
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	viewControllerInternal = [[CSGLViewController alloc] initWithDelegate:self];
    [window setRootViewController: viewControllerInternal];
    [window makeKeyAndVisible];
    
    csApplication->Init();
    
    isFirstActive = YES;
    
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        if([delegate respondsToSelector:@selector(application:didFinishLaunchingWithOptions:)])
        {
            [delegate application:in_application didFinishLaunchingWithOptions:in_launchingOptions];
        }
    }
    
    CSBackend::iOS::LocalNotificationSystem::ApplicationDidFinishLaunchingWithOptions(in_application, in_launchingOptions);
    
	return YES;
}
//-------------------------------------------------------------
/// Called when the app is forwarded a URL.
///
/// @author S Downie
///
/// @param Application
/// @param URL
/// @param Application that send URL
/// @param Annotation
//-------------------------------------------------------------
- (BOOL) application:(UIApplication*)in_application openURL:(NSURL*)in_url sourceApplication:(NSString*)in_sourceApplication annotation:(id)in_annotation
{
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        if([delegate respondsToSelector:@selector(application:openURL:sourceApplication:annotation:)])
        {
            if([delegate application:in_application openURL:in_url sourceApplication:in_sourceApplication annotation:in_annotation] == YES)
            {
                return YES;
            }
        }
    }
    
    return NO;
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
        if([delegate respondsToSelector:@selector(applicationWillResignActive:)])
        {
            [delegate applicationWillResignActive:in_application];
        }
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
        if([delegate respondsToSelector:@selector(applicationDidEnterBackground:)])
        {
            [delegate applicationDidEnterBackground:in_application];
        }
    }
    
    UIBackgroundTaskIdentifier bgTask = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil];
    
    dispatch_async(dispatch_get_main_queue(), ^
    {
        isActive = NO;
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
    //Sometimes iOS steals the context and doesn't return it.
    GLKView* view = (GLKView*)viewControllerInternal.view;
    if([EAGLContext currentContext] != view.context)
    {
        [EAGLContext setCurrentContext:view.context];
    }
    
    csApplication->Resume();
    isActive = YES;
    
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        if([delegate respondsToSelector:@selector(applicationWillEnterForeground:)])
        {
            [delegate applicationWillEnterForeground:in_application];
        }
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
        
        //Sometimes iOS steals the context and doesn't return it.
        GLKView* view = (GLKView*)viewControllerInternal.view;
        if([EAGLContext currentContext] != view.context)
        {
            [EAGLContext setCurrentContext:view.context];
        }
        
        csApplication->Resume();
        isActive = YES;
    }
    
    csApplication->Foreground();
    
    for(id<UIApplicationDelegate> delegate in subdelegates)
    {
        if([delegate respondsToSelector:@selector(applicationDidBecomeActive:)])
        {
            [delegate applicationDidBecomeActive:in_application];
        }
    }
    
    CSBackend::iOS::LocalNotificationSystem::ApplicationDidBecomeActive(in_application);
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
        if([delegate respondsToSelector:@selector(applicationWillTerminate:)])
        {
            [delegate applicationWillTerminate:in_application];
        }
    }
    
    UIBackgroundTaskIdentifier bgTask = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:nil];
        
    dispatch_async(dispatch_get_main_queue(), ^
    {
        isActive = NO;
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
    if (isActive == YES)
    {
        CSCore::Application::Get()->Update(controller.timeSinceLastUpdate, controller.timeSinceFirstResume);
    }
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
    if (isActive == YES)
    {
        //Sometimes iOS steals the context and doesn't return it.
        if([EAGLContext currentContext] != view.context)
        {
            [EAGLContext setCurrentContext:view.context];
        }
        
        CSCore::Application::Get()->Render();
    }
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
        if([delegate respondsToSelector:@selector(applicationDidReceiveMemoryWarning:)])
        {
            [delegate applicationDidReceiveMemoryWarning:in_application];
        }
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
        if([delegate respondsToSelector:@selector(application:didReceiveLocalNotification:)])
        {
            [delegate application:in_application didReceiveLocalNotification:in_notification];
        }
    }
    
    CSBackend::iOS::LocalNotificationSystem::ApplicationDidReceiveLocalNotification(in_application, in_notification);
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
        if([delegate respondsToSelector:@selector(application:didRegisterForRemoteNotificationsWithDeviceToken:)])
        {
            [delegate application:in_application didRegisterForRemoteNotificationsWithDeviceToken:in_deviceToken];
        }
    }
    
	CSBackend::iOS::RemoteNotificationSystem* remoteNotificationSystem = CSCore::Application::Get()->GetSystem<CSBackend::iOS::RemoteNotificationSystem>();
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
        if([delegate respondsToSelector:@selector(application:didFailToRegisterForRemoteNotificationsWithError:)])
        {
            [delegate application:in_application didFailToRegisterForRemoteNotificationsWithError:in_error];
        }
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
	CSBackend::iOS::RemoteNotificationSystem* remoteNotificationSystem = CSCore::Application::Get()->GetSystem<CSBackend::iOS::RemoteNotificationSystem>();
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
        if([delegate respondsToSelector:@selector(application:didReceiveRemoteNotification:)])
        {
            [delegate application:in_application didReceiveRemoteNotification:in_userInfo];
        }
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
    
	[viewControllerInternal release];
    [window release];
    [subdelegates release];
    
    singletonInstance = nil;
    
    [super dealloc];
}

@end

#endif
