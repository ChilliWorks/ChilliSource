//
//  EAGLView.h
//  moFlo - iOS
//
//  Created by Scott Downie on 28/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _EAGL_VIEW_H_
#define _EAGL_VIEW_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>

//================================================================
/// Description:
///
/// OpenGL ES view controller. iOS specific view controller
/// with no wrapper.
//================================================================

namespace EAGLColourMode
{
    enum Mode
    {
        RGB565,
        RGBA8
    };
}

@interface EAGLView : UIView 
{

}

-(id) init:(BOOL)landscape andColourMode:(EAGLColourMode::Mode)ineMode; 
+(EAGLView*) sharedInstance;

@property (nonatomic, assign) UIViewController* viewController;

@end

#endif