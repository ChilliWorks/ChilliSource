//
//  VideoPlayerTapListener.h
//  moFlow
//
//  Created by Ian Copland on 16/08/2012.
//  Copyright 2012 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#import <ChilliSource/ChilliSource.h>

#import <Foundation/Foundation.h>
#import <MediaPlayer/MediaPlayer.h>

#import <functional>

@class UIView;
@class UITapGestureRecognizer;
@class CADisplayLink;

namespace CSBackend
{
    namespace iOS
    {
        //========================================================
        /// Delegates
        //========================================================
        typedef std::function<void()> VideoPlayerTappedDelegate;
    }
}
//========================================================
/// Video Player Tap Listener
///
/// This is used to get tap events back from the
/// movie player controller.
//========================================================
@interface CVideoPlayerTapListener : NSObject
{
    UITapGestureRecognizer* mpRecogniser;
    CADisplayLink* mpDisplayLink;
    bool mbReceivedTap;
    CSBackend::iOS::VideoPlayerTappedDelegate mTappedDelegate;
    UIView* mpView;
}
//--------------------------------------------------------
/// init
///
/// Initialises the listener.
//--------------------------------------------------------
-(id) init;
//--------------------------------------------------------
/// Setup View
///
/// sets up the listener with the the view and
/// the delegate
///
/// @param the view.
/// @param the tapped delegate.
//--------------------------------------------------------
-(void) SetupWithView:(UIView*)inpView AndDelegate:(CSBackend::iOS::VideoPlayerTappedDelegate)inTappedDelegate;
//--------------------------------------------------------
/// On Tap
///
/// Method called when the view is tapped.
///
/// @param The gesture sending the message.
//--------------------------------------------------------
-(void) OnTap:(UIGestureRecognizer*)gestureRecogniser;
//--------------------------------------------------------
/// On Update
///
/// Called each frame.
//--------------------------------------------------------
-(void) OnUpdate;
//--------------------------------------------------------
/// Reset
///
/// Cleans up the view and delegate, restoring
/// the listener back to its orginal state ready to be
/// used again.
//--------------------------------------------------------
-(void) Reset;
//--------------------------------------------------------
/// Dealloc
///
/// destroys the listener
//--------------------------------------------------------
-(void) dealloc;

@end

#endif