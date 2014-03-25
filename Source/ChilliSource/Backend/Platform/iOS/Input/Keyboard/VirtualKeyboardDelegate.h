//
//  VirtualKeyboardDelegate.h
//  Chilli Source
//
//  Created by Scott Downie on 18/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_INPUT_KEYBOARD_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_INPUT_KEYBOARD_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>

#include <UIKit/UIKit.h>

//---------------------------------------------------------
/// A delegate class for listening for changes from the
/// iOS virtual keyboard.
///
/// @author S Downie
//---------------------------------------------------------
@interface VirtualKeyboardDelegate : NSObject<UITextFieldDelegate>
{
    ChilliSource::iOS::Keyboard* keyboard;
}
//---------------------------------------------------------
/// Constructor.
///
/// @author S Downie
///
/// @param The keyboard system.
///
/// @return The new instance of the delegate.
//---------------------------------------------------------
-(VirtualKeyboardDelegate*) initWithKeyboard:(ChilliSource::iOS::Keyboard*) keyboardSystem;

@end

#endif