//
//  DialogueBoxListener.mm
//  Chilli Source
//
//  Created by Ian Copland on 05/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxListener.h>

#import <CSBackend/Platform/iOS/Core/DialogueBox/DialogueBoxSystem.h>

@implementation DialogueBoxListener
//------------------------------------------------------------
//------------------------------------------------------------
-(id) initWithDialogueBoxSystem:(ChilliSource::iOS::DialogueBoxSystem*)system
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
        // NO = 0, YES = 1
        if(buttonIndex == 0)
        {
            dialogueBoxSystem->OnSystemConfirmDialogResult(alertView.tag, ChilliSource::Core::DialogueBoxSystem::DialogueResult::k_cancel);
        }
        else
        {
            dialogueBoxSystem->OnSystemConfirmDialogResult(alertView.tag, ChilliSource::Core::DialogueBoxSystem::DialogueResult::k_confirm);
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