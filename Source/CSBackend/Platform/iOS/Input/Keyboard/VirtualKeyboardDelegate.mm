//
//  VirtualKeyboardDelegate.mm
//  Chilli Source
//  Created by Scott Downie on 18/07/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#endif