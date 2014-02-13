//
//  VideoPlayerTapListener.mm
//  moFlow
//
//  Created by Ian Copland on 16/08/2012.
//  Copyright 2012 Tag Games. All rights reserved.
//

#import <ChilliSource/Platform/iOS/Video/VideoPlayerTapListener.h>
#import <QuartzCore/QuartzCore.h>
#include <ChilliSource/Platform/iOS/Input/TouchScreen.h>
#include <ChilliSource/Core/Screen.h>


using namespace moFlo;
using namespace moFlo::iOSPlatform;

@implementation CVideoPlayerTapListener
//--------------------------------------------------------
/// Init
//--------------------------------------------------------
-(id) init
{
    //Do that daft ObjC thing of setting self to super init.
    if(!(self = [super init]))
	{
		return nil;
	}
    
    mpRecogniser = nil;
    mpView = nil;
    mbReceivedTap = false;
    mpDisplayLink = [CADisplayLink displayLinkWithTarget: self selector: @selector(OnUpdate)];
    [mpDisplayLink addToRunLoop: [NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    mpDisplayLink.paused = YES;
    
    return self;
}
//--------------------------------------------------------
/// Setup With View
//--------------------------------------------------------
-(void) SetupWithView:(UIView*)inpView AndDelegate:(VideoPlayerTappedDelegate)inTappedDelegate
{
    mTappedDelegate = inTappedDelegate;
    
    //try and create a gesture recogniser and add it to the video player.
    if (inpView != nil && [inpView respondsToSelector:@selector(addGestureRecognizer:)])
    {
        //add the gesture recogniser to the tappable view.
        mpView = inpView;
        mpRecogniser = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(OnTap:)];
        mpRecogniser.numberOfTapsRequired = 1;
        [mpView addGestureRecognizer:mpRecogniser];
        [mpRecogniser setEnabled:YES];
    }
    else
    {
        mpView = nil;
        mpRecogniser = nil;
    }
    
    //start the updater
    if (mpDisplayLink != nil)
    {
        mpDisplayLink.paused = NO;
    }
}
//--------------------------------------------------------
/// OnTap
//--------------------------------------------------------
-(void)OnTap:(UIGestureRecognizer*)gestureRecogniser
{
    if (gestureRecogniser.state == UIGestureRecognizerStateEnded)
    {
        mbReceivedTap = true;
    }
}
//--------------------------------------------------------
/// On Update
//--------------------------------------------------------
-(void)OnUpdate
{
    if (mbReceivedTap == true)
    {
        if (mTappedDelegate != NULL)
        {
            mTappedDelegate();
        }
        
        mbReceivedTap = false;
    }
}
//--------------------------------------------------------
/// Reset
//--------------------------------------------------------
-(void) Reset
{
    if (mpView != nil)
    {
        if ([mpView respondsToSelector:@selector(removeGestureRecognizer:)] && mpRecogniser != nil)
        {
            //remove the gesture from the view
            [mpView removeGestureRecognizer:mpRecogniser];
            [mpRecogniser release];
            mpRecogniser = nil;
        }
        
        //cleanup the view
        mpView = nil;
    }
    
    if (mpDisplayLink != nil)
    {
        mpDisplayLink.paused = YES;
    }
    
    mbReceivedTap = false;
}
//--------------------------------------------------------
/// dealloc
//--------------------------------------------------------
-(void) dealloc
{
    [mpDisplayLink invalidate];
    [super dealloc];
}
@end
