//
//  VirtualTextEntryDelegate.mm
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

#import <CSBackend/Platform/iOS/Input/TextEntry/TextEntryDelegate.h>

#import <CSBackend/Platform/iOS/Input/TextEntry/TextEntry.h>

@implementation TextEntryDelegate

//---------------------------------------------------------
//---------------------------------------------------------
-(TextEntryDelegate*) initWithTextEntry:(CSBackend::iOS::TextEntry*) in_textEntry
{
    if(self = [super init])
    {
        textEntry = in_textEntry;
        return self;
    }
    return nil;
}
//---------------------------------------------------------
/// Called when the text buffer changes.
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
    NSString* updateText = [textField.text stringByReplacingCharactersInRange:range withString:string];
    
    return textEntry->OnTextUpdated(updateText);
}
//---------------------------------------------------------
/// @author S Downie
///
/// @param The text field.
///
/// @return Whether or not text editing should end.
//---------------------------------------------------------
-(BOOL) textFieldShouldReturn:(UITextField*)textField
{
    textEntry->Deactivate();
    return NO;
}

@end

#endif