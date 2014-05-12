//
//  CSGLViewController.mm
//  Chilli Source
//  Created by Scott Downie on 09/05/2014.
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

#import <ChilliSource/Backend/Platform/iOS/Core/Base/CSGLViewController.h>

#import <ChilliSource/Backend/Platform/iOS/Core/Base/Screen.h>
#import <ChilliSource/Backend/Platform/iOS/Input/Pointer/PointerSystem.h>
#import <ChilliSource/Core/Base/Application.h>

@implementation CSGLViewController

//-------------------------------------------------------------
//-------------------------------------------------------------
- (id)initWithDelegate:(id)in_delegate
{
    if((self = [super init]))
	{
        EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        BOOL isContextSet = [EAGLContext setCurrentContext:context];
        
        if(context == nil || isContextSet == NO)
        {
            CS_LOG_FATAL("Cannot Create OpenGL ES 2.0 Context");
        }
        
        GLKView* view = [[GLKView alloc] initWithFrame:[[UIScreen mainScreen] bounds] context:context];
        view.drawableColorFormat = GLKViewDrawableColorFormatRGB565;
        view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
        view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
        view.drawableMultisample = GLKViewDrawableMultisampleNone;
        view.userInteractionEnabled = YES;
        view.enableSetNeedsDisplay = NO;
        view.delegate = in_delegate;
        
        self.view = view;
        self.delegate = in_delegate;
        
        return self;
    }

    return nil;
    
    //TODO: Suspend rendering
    //TODO: Set frame rate
}
//-------------------------------------------------------------
/// Called by the OS to fetch the supported orientations for
/// the GL view.
///
/// @author S Downie
///
/// @return Integer mask representing supported orientations
//-------------------------------------------------------------
- (NSUInteger)supportedInterfaceOrientations
{
    //---Portrait
    //return UIInterfaceOrientationMaskPortrait;
    
    //---Landscape
    //return UIInterfaceOrientationMaskLandscape;
    
    //---All
    return UIInterfaceOrientationMaskAll;
}
//-------------------------------------------------------------
/// Called by the OS to query whether the view should be
/// allowed to rotate to the given orientation
///
/// @author S Downie
///
/// @param Interface orientation to rotate to
///
/// @return Whether we support the given orientation
//-------------------------------------------------------------
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	//---Portrait
	//return interfaceOrientation == UIInterfaceOrientationPortrait || interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown;
	
    //---Landscape
	//return interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight;
	
    //---All
	return YES;
}
//-------------------------------------------------------------
/// Called by the OS immediately before a view rotation
/// occurs. This allows use to notify the engine of the orientation
/// change
///
/// @author S Downie
///
/// @param New orientation
/// @param Time in seconds of orientation animation
//-------------------------------------------------------------
- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    ChilliSource::iOS::Screen* screen = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::Screen>();
    screen->OnOrientationChanged(toInterfaceOrientation);
}
//-------------------------------------------------------------
/// Called by the OS when one or more touches start on the view
///
/// @author S Downie
///
/// @param Set of touches
/// @param Event
//-------------------------------------------------------------
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    ChilliSource::iOS::PointerSystem* pointerSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::PointerSystem>();
    CS_ASSERT(pointerSystem, "touchesBegan callback requires the pointer system.");
    
    for (UITouch* touch in touches)
    {
        pointerSystem->OnTouchBegan(touch);
    }
}
//-------------------------------------------------------------
/// Called by the OS when one or more touches are cancelled
///
/// @author S Downie
///
/// @param Set of touches
/// @param Event
//-------------------------------------------------------------
- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    ChilliSource::iOS::PointerSystem* pointerSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::PointerSystem>();
    CS_ASSERT(pointerSystem, "touchesCancelled callback requires the pointer system.");
    
    for (UITouch* touch in touches)
    {
        pointerSystem->OnTouchEnded(touch);
    }
}
//-------------------------------------------------------------
/// Called by the OS when one or more touches end on the view
///
/// @author S Downie
///
/// @param Set of touches
/// @param Event
//-------------------------------------------------------------
- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    ChilliSource::iOS::PointerSystem* pointerSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::PointerSystem>();
    CS_ASSERT(pointerSystem, "touchesEnded callback requires the pointer system.");
    
	for (UITouch* touch in touches)
    {
        pointerSystem->OnTouchEnded(touch);
    }
}
//-------------------------------------------------------------
/// Called by the OS when one or more touches that have been
/// started are then moved
///
/// @author S Downie
///
/// @param Set of touches
/// @param Event
//-------------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent*)event
{
    ChilliSource::iOS::PointerSystem* pointerSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::PointerSystem>();
    CS_ASSERT(pointerSystem, "touchesEnded callback requires the pointer system.");
    
    for (UITouch* touch in touches)
    {
        pointerSystem->OnTouchMoved(touch);
    }
}
//-------------------------------------------------------------
/// Dealloc
///
/// @author S Downie
//-------------------------------------------------------------
- (void) dealloc
{
    GLKView* view = (GLKView*)self.view;
    [view.context release];
    view.context = nil;
    [view release];
    
    [super dealloc];
}

@end