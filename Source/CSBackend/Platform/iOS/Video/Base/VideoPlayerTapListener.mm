//
//  VideoPlayerTapListener.mm
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

#import <CSBackend/Platform/iOS/Video/Base/VideoPlayerTapListener.h>
#import <QuartzCore/QuartzCore.h>
#import <ChilliSource/Core/Base/Screen.h>

using namespace CSBackend;
using namespace CSBackend::iOS;

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
        if (mTappedDelegate != nullptr)
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

#endif