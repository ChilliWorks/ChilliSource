//
//  EAGLView.mm
//  moFlo - iOS
//
//  Created by Scott Downie on 28/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#import <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>

#include <ChilliSource/Backend/Platform/iOS/Input/Pointer/PointerSystem.h>
#include <ChilliSource/Core/Base/Application.h>

static EAGLView* gpGLView = nil;

@implementation EAGLView

@synthesize viewController;

+ (Class)layerClass 
{
    return [CAEAGLLayer class];
}


//----------------------------------------------------------
/// Init With Frame
///
/// Create the view with the given bounds
///
/// @param Whether the application is landscape or portrait
/// @param Backing colour mode
/// @return The initialised view
//----------------------------------------------------------
-(id) init:(BOOL)landscape andColourMode:(EAGLColourMode)ineMode
{
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    
    if(landscape)
    {
        std::swap(screenSize.width, screenSize.height);
    }
    
	if(!(self = [super initWithFrame:CGRectMake(0, 0, screenSize.width, screenSize.height)])) 
	{
		return nil;
	}
	
	self.multipleTouchEnabled = YES;
    
    NSString* strColourMode = nil;
    switch(ineMode)
    {
        case EAGLColourMode::k_RGB565:
            strColourMode = kEAGLColorFormatRGB565;
            break;
        case EAGLColourMode::k_RGBA8:
            strColourMode = kEAGLColorFormatRGBA8;
            break;
    }
	
	// Get the layer
	CAEAGLLayer* pLayer			= (CAEAGLLayer*)self.layer;
	pLayer.opaque				= YES;
	pLayer.drawableProperties	= [NSDictionary dictionaryWithObjectsAndKeys:
								   [NSNumber numberWithBool:NO], 
								   kEAGLDrawablePropertyRetainedBacking, 
								   strColourMode, 
								   kEAGLDrawablePropertyColorFormat, 
								   nil];
    
	bool bIsiOS4_0 = ([[[UIDevice currentDevice] systemVersion] floatValue] >= 4.0f);
    if(bIsiOS4_0)
    {
        f32 fScale = [UIScreen mainScreen].scale;
        self.contentScaleFactor = fScale;
        self.layer.contentsScale = fScale;
    }
    
	gpGLView = self;
	
    return self;
}

+(EAGLView*) sharedInstance
{
	return gpGLView;
}
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
    ChilliSource::iOS::PointerSystem* pointerSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::PointerSystem>();
    
    if (pointerSystem != nullptr)
    {
        for (UITouch* touch in touches)
        {
            pointerSystem->OnTouchBegan(touch);
        }
	}
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    ChilliSource::iOS::PointerSystem* pointerSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::PointerSystem>();
    
    if (pointerSystem != nullptr)
    {
        for (UITouch* touch in touches)
        {
            pointerSystem->OnTouchEnded(touch);
        }
	}
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    ChilliSource::iOS::PointerSystem* pointerSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::PointerSystem>();
    
    if (pointerSystem != nullptr)
    {
        for (UITouch* touch in touches)
        {
            pointerSystem->OnTouchEnded(touch);
        }
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    ChilliSource::iOS::PointerSystem* pointerSystem = ChilliSource::Core::Application::Get()->GetSystem<ChilliSource::iOS::PointerSystem>();
    
    if (pointerSystem != nullptr)
    {
        for (UITouch* touch in touches)
        {
            pointerSystem->OnTouchMoved(touch);
        }
	}
}

@end
