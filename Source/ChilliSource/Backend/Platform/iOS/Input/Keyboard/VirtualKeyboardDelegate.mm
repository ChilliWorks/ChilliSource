//
//  VirtualKeyboardDelegate.mm
//  iOSTemplate
//
//  Created by Scott Downie on 18/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Input/Keyboard/VirtualKeyboardDelegate.h>

@implementation VirtualKeyboardDelegate

-(VirtualKeyboardDelegate*) initWithKeyboard:(VirtualKeyboard*) inpKeyboard
{
    self = [super init];
    
    if(self)
    {
        mpKeyboard = inpKeyboard;
    }
    
    return self;
}
-(BOOL) textField:(UITextField*)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString*)string
{
    NSString* strText = textField.text;
    
    if(string.length > 0)
    {
        return mpKeyboard->UpdateText([strText stringByAppendingString:string]);
    }
    else
    {
        if(strText.length > 0)
        {
            strText = [strText substringToIndex:(strText.length - 1)];
        }

        return mpKeyboard->UpdateText(strText);
    }
}
-(BOOL) textFieldShouldReturn:(UITextField*)textField
{
    mpKeyboard->Hide();
    return NO;
}

@end
