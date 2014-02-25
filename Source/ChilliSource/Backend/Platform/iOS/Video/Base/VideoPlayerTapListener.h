//
//  VideoPlayerTapListener.h
//  moFlow
//
//  Created by Ian Copland on 16/08/2012.
//  Copyright 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_IOS_VIDEO_VIDEOPLAYERTAPLISTENER_H_
#define _MOFLOW_PLATFORM_IOS_VIDEO_VIDEOPLAYERTAPLISTENER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/FastDelegate.h>

#import <Foundation/Foundation.h>
#include <MediaPlayer/MediaPlayer.h>

@class UIView;
@class UITapGestureRecognizer;
@class CADisplayLink;

namespace ChilliSource
{
    namespace iOS
    {
        //========================================================
        /// Delegates
        //========================================================
        typedef fastdelegate::FastDelegate0<> VideoPlayerTappedDelegate;
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
    ChilliSource::iOS::VideoPlayerTappedDelegate mTappedDelegate;
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
-(void) SetupWithView:(UIView*)inpView AndDelegate:(ChilliSource::iOS::VideoPlayerTappedDelegate)inTappedDelegate;
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