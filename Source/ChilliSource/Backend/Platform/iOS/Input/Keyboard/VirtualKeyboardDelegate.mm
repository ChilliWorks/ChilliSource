//
//  VirtualKeyboardDelegate.mm
//  iOSTemplate
//
//  Created by Scott Downie on 18/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Input/Keyboard/VirtualKeyboardDelegate.h>

#include <ChilliSource/Backend/Platform/iOS/Input/Keyboard/Keyboard.h>

@implementation VirtualKeyboardDelegate

//---------------------------------------------------------
//---------------------------------------------------------
-(VirtualKeyboardDelegate*) initWithKeyboard:(ChilliSource::iOS::Keyboard*) keyboardSystem
{
    if(self = [super init])
    {
        keyboard = keyboardSystem;
        return self;
    }
    return nil;
}
//---------------------------------------------------------
/// Called when the keyboard text changes.
///
/// @author S Downie
///
/// @param The text field in which the text has changed.
/// @param The range of characters that have changed.
/// @param The replacement string.
///
/// @return Whether or not this change has been accepted.
//---------------------------------------------------------
-(BOOL) textField:(UITextField*)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString*)string
{
    NSString* text = textField.text;
    
    if(string.length > 0)
    {
        return keyboard->OnTextUpdated([text stringByAppendingString:string]);
    }
    else
    {
        if(text.length > 0)
        {
            text = [text substringToIndex:(text.length - 1)];
        }

        return keyboard->OnTextUpdated(text);
    }
}
//---------------------------------------------------------
/// @author S Downie
///
/// @param The text field.
/// @return Whether or not text editing should end.
//---------------------------------------------------------
-(BOOL) textFieldShouldReturn:(UITextField*)textField
{
    keyboard->SetTextInputEnabled(false);
    return NO;
}

@end
