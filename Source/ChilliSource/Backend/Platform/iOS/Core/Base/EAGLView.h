//
//  EAGLView.h
//  moFlo - iOS
//
//  Created by Scott Downie on 28/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _EAGL_VIEW_H_
#define _EAGL_VIEW_H_

#include <ChilliSource/ChilliSource.h>

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

enum class EAGLColourMode
{
    k_RGB565,
    k_RGBA8
};

@interface EAGLView : UIView 
{

}

-(id) init:(BOOL)landscape andColourMode:(EAGLColourMode)ineMode; 
+(EAGLView*) sharedInstance;

@property (nonatomic, assign) UIViewController* viewController;

@end

#endif