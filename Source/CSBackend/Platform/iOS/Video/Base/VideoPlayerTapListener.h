//
//  VideoPlayerTapListener.h
//  Chilli Source
//  Created by Ian Copland on 16/08/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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
/// sets up the listener with the view and the delegate
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