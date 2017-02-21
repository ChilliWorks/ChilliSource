//
//  DialogueBoxListener.mm
//  ChilliSource
//  Created by Ian Copland on 05/03/2014.
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

#import <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxListener.h>

#import <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxSystem.h>

@implementation DialogueBoxListener
//------------------------------------------------------------
//------------------------------------------------------------
-(id) initWithDialogueBoxSystem:(CSBackend::iOS::DialogueBoxSystem*)system
{
    if (self = [super init])
    {
        dialogueBoxSystem = system;
        return self;
    }
    return nil;
}
//------------------------------------------------------------
//------------------------------------------------------------
-(void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (dialogueBoxSystem != nullptr)
    {
        //we know the Id is in the range of a u32 as we set it when the confirm dialogue was created meaning we can cast to that.
        u32 tag = static_cast<u32>(alertView.tag);
        
        // NO = 0, YES = 1
        if(buttonIndex == 0)
        {
            dialogueBoxSystem->OnSystemConfirmDialogResult(tag, ChilliSource::DialogueBoxSystem::DialogueResult::k_cancel);
        }
        else
        {
            dialogueBoxSystem->OnSystemConfirmDialogResult(tag, ChilliSource::DialogueBoxSystem::DialogueResult::k_confirm);
        }
    }
}
//------------------------------------------------------------
//------------------------------------------------------------
-(void) dealloc
{
    dialogueBoxSystem = nullptr;
    [super dealloc];
}

@end


#endif
