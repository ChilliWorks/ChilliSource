//
//  CSAppDelegate.h
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

#import <ChilliSource/ChilliSource.h>
#import <GLKit/GLKit.h>
#import <UIKit/UIKit.h>

//--------------------------------------------------------------------
/// iOS application delegate. This receieves events from the OS
/// and controls the lifecycle and view hierarchy of the application.
///
/// iOS libraries can register as listeners to obtain the app delegate
/// events
///
/// @author S Downie
//--------------------------------------------------------------------
@interface CSAppDelegate : NSObject <UIApplicationDelegate, GLKViewControllerDelegate, GLKViewDelegate>
{
    UIWindow* window;
    GLKViewController* viewControllerInternal;
    CSCore::Application* csApplication;
    
    //Used to manually invoke OnForeground to conform to
    //ChilliSource lifecycle events
    BOOL isFirstActive;
    
    //Used to ensure that the Update event is only called
    //between has become active and will resign active
    BOOL isActive;
    
    NSMutableArray* subdelegates;
}
//--------------------------------------------------------------------
/// @author Ian Copland
///
/// @return The view controller instance.
//--------------------------------------------------------------------
@property (readonly) GLKViewController* viewController;
//--------------------------------------------------------------------
/// @author S Downie
///
/// @return Singleton instance of the application delegate
//--------------------------------------------------------------------
+ (CSAppDelegate*) sharedInstance;
//--------------------------------------------------------------------
/// Add another UIApplication as a listener of AppDelegate events.
/// This is useful for 3rd party iOS libraries that require
/// hooks into the app delegate.
///
/// @author S Downie
///
/// @param UIApplicationDelegate
//--------------------------------------------------------------------
- (void) addAppDelegateListener:(id<UIApplicationDelegate>)in_delegate;
//--------------------------------------------------------------------
/// Remove another UIApplication as a listener of AppDelegate events.
///
/// @author S Downie
///
/// @param UIApplicationDelegate
//--------------------------------------------------------------------
- (void) removeAppDelegateListener:(id<UIApplicationDelegate>)in_delegate;
//--------------------------------------------------------------------
/// Set the target frame rate in frames per second
///
/// @author S Downie
///
/// @param Target FPS (15, 30, 60, etc)
//--------------------------------------------------------------------
- (void) setPreferredFPS:(NSUInteger)in_fps;

@end

#endif
