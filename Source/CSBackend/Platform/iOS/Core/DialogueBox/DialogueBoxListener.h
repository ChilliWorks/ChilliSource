//
//  DialogueBoxListener.h
//  Chilli Source
//
//  Created by Ian Copland on 05/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//------------------------------------------------------------
/// Listener for callbacks from system message boxes.
//------------------------------------------------------------
@interface DialogueBoxListener : NSObject
{
    CSBackend::iOS::DialogueBoxSystem* dialogueBoxSystem;
}
//------------------------------------------------------------
/// Constructor
///
/// @author I Copland
///
/// @param The dialogue box system.
//------------------------------------------------------------
-(id) initWithDialogueBoxSystem:(CSBackend::iOS::DialogueBoxSystem*)system;
//------------------------------------------------------------
/// Listens for callbacks from alert views.
///
/// @author I Copland
///
/// @param The alert view.
/// @param The button index.
//------------------------------------------------------------
-(void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;
//------------------------------------------------------------
/// Destructor
///
/// @author I Copland
//------------------------------------------------------------
-(void) dealloc;

@end

#endif