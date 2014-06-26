//
//  VideoOverlayView.h
//  Chilli Source
//  Created by Ian Copland on 11/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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
#import <UIKit/UIKit.h>

//---------------------------------------------------
/// A view that overlays the video view. This consumes
/// touches so the EAGLView doesn't receive them during
/// video playback.
///
/// @author Ian Copland
//---------------------------------------------------
@interface VideoOverlayView : UIView
{
}

//---------------------------------------------------
/// Constructor
///
/// @author Ian Copland
///
/// @param The frame.
//---------------------------------------------------
- (id) initWithFrame:(CGRect)in_frame;
//---------------------------------------------------
/// Called when a touch begins over this view.
///
/// @author Ian Copland
///
/// @param The touches.
/// @param The event.
//---------------------------------------------------
- (void) touchesBegan:(NSSet *)in_touches withEvent:(UIEvent*)in_event;
//---------------------------------------------------
/// Called when a touch moves over this view.
///
/// @author Ian Copland
///
/// @param The touches.
/// @param The event.
//---------------------------------------------------
- (void) touchesMoved:(NSSet *)in_touches withEvent:(UIEvent*)in_event;
//---------------------------------------------------
/// Called when a touch ends over this view.
///
/// @author Ian Copland
///
/// @param The touches.
/// @param The event.
//---------------------------------------------------
- (void) touchesEnded:(NSSet *)in_touches withEvent:(UIEvent*)in_event;
//---------------------------------------------------
/// Called when a touch is cancelled over this view.
///
/// @author Ian Copland
///
/// @param The touches.
/// @param The event.
//---------------------------------------------------
- (void) touchesCancelled:(NSSet *)in_touches withEvent:(UIEvent*)in_event;

@end

#endif