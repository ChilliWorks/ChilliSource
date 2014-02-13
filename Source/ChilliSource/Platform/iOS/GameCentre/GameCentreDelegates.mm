//
//  GameCentreDelegates.mm
//  MoFlow
//
//  Created by Stuart McGaw on 18/03/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#import <ChilliSource/Platform/iOS/GameCentre/GameCentreDelegates.h>
#import <ChilliSource/Platform/iOS/GameCentre/GameCentreSystem.h>

GameCentreTurnBasedMatchmakerViewControllerDelegate * gameCentreTurnBasedMatchmakerViewControllerDelegate = nil;
GameCentreTurnBasedEventHandlerDelegate * gameCentreTurnBasedEventHandlerInstance = nil;
GameCentreLeaderboardDelegate * leaderboardDelegateInstance = nil;
GameCentreAchievementsDelegate * achievementDelegateInstance = nil;
GameCentreAuthenticationListener* authenticationListener = nil;
#ifdef GAMECENTRE_DISABLE_ANIMATIONS
BOOL kbUseAnimationForDismiss = NO;
#else
BOOL kbUseAnimationForDismiss = YES;
#endif

@implementation GameCentreTurnBasedMatchmakerViewControllerDelegate
+(GameCentreTurnBasedMatchmakerViewControllerDelegate*) instance
{
	if (!gameCentreTurnBasedMatchmakerViewControllerDelegate)
    {
		gameCentreTurnBasedMatchmakerViewControllerDelegate = [[GameCentreTurnBasedMatchmakerViewControllerDelegate alloc] init];
	}
    
	return gameCentreTurnBasedMatchmakerViewControllerDelegate;
}

- (void)turnBasedMatchmakerViewControllerWasCancelled:(GKTurnBasedMatchmakerViewController *)viewController
{
    mWasCancelledEvent.Invoke();
}

// Matchmaking has failed with an error
- (void)turnBasedMatchmakerViewController:(GKTurnBasedMatchmakerViewController *)viewController didFailWithError:(NSError *)error
{
    mDidFailEvent.Invoke();    
}

// A turned-based match has been found, the game should start
- (void)turnBasedMatchmakerViewController:(GKTurnBasedMatchmakerViewController *)viewController didFindMatch:(GKTurnBasedMatch *)match
{
    mDidFindMatchEvent.Invoke(match);
}

// Called when a users chooses to quit a match and that player has the current turn.  The developer should call playerQuitInTurnWithOutcome:nextPlayer:matchData:completionHandler: on the match passing in appropriate values.  They can also update matchOutcome for other players as appropriate.
- (void)turnBasedMatchmakerViewController:(GKTurnBasedMatchmakerViewController *)viewController playerQuitForMatch:(GKTurnBasedMatch *)match
{
    NSUInteger currentIndex = [match.participants indexOfObject:match.currentParticipant];
    GKTurnBasedParticipant *part;
    
    for (int i = 0; i < [match.participants count]; i++) {
        part = [match.participants objectAtIndex:
                (currentIndex + 1 + i) % match.participants.count];
        if (part.matchOutcome != GKTurnBasedMatchOutcomeQuit) {
            break;
        } 
    }
    [match participantQuitInTurnWithOutcome:
     GKTurnBasedMatchOutcomeQuit nextParticipant:part 
                                  matchData:match.matchData completionHandler:nil];    
}

- (moFlo::IEvent<GameCentreTurnBasedMatchmakerViewControllerWasCancelledEvent>&) GetWasCancelledEvent
{
    return mWasCancelledEvent;
}

- (moFlo::IEvent<GameCentreTurnBasedMatchmakerViewControllerDidFailEvent>&) GetDidFailEvent
{
    return mDidFailEvent;
}

- (moFlo::IEvent<GameCentreTurnBasedMatchmakerViewControllerDidFindMatchEvent>&) GetDidFindMatchEvent
{
    return mDidFindMatchEvent;
}

@end

@implementation GameCentreTurnBasedEventHandlerDelegate
+(GameCentreTurnBasedEventHandlerDelegate*) instance
{
	if (!gameCentreTurnBasedEventHandlerInstance)
    {
		gameCentreTurnBasedEventHandlerInstance = [[GameCentreTurnBasedEventHandlerDelegate alloc] init];
	}
    
	return gameCentreTurnBasedEventHandlerInstance;
}

- (moFlo::IEvent<HandleInviteFromGameCenterEventDelegate>&) GetHandleInviteFromGameCenterEvent
{
    return mHandleInviteFromGameCenterEvent;
}

- (moFlo::IEvent<HandleTurnEventForMatchEventDelegate>&) GetHandleTurnEventForMatchEvent
{
    return mHandleTurnEventForMatchEvent;
}

- (moFlo::IEvent<HandleMatchEndedEventDelegate>&) GetHandleMatchEndedEvent
{
    return mHandleMatchEndedEvent;
}

- (void)handleInviteFromGameCenter:(NSArray *)playersToInvite;		
{
    mHandleInviteFromGameCenterEvent.Invoke(playersToInvite);
}

// handleTurnEventForMatch is called when a turn is passed to another participant.  Note this may arise from one of the following events:
//      The local participant has accepted an invite to a new match
//      The local participant has been passed the turn for an existing match
//      Another participant has made a turn in an existing match
// The application needs to be prepared to handle this even while the participant might be engaged in a different match
- (void)handleTurnEventForMatch:(GKTurnBasedMatch *)match didBecomeActive:(BOOL)didBecomeActive
{
    mHandleTurnEventForMatchEvent.Invoke(match, didBecomeActive == YES ? true : false);
}

// handleTurnEventForMatch is called when a turn is passed to another participant.  Note this may arise from one of the following events:
//      The local participant has accepted an invite to a new match
//      The local participant has been passed the turn for an existing match
//      Another participant has made a turn in an existing match
// The application needs to be prepared to handle this even while the participant might be engaged in a different match
- (void)handleTurnEventForMatch:(GKTurnBasedMatch *)match
{
    mHandleTurnEventForMatchEvent.Invoke(match, false);
}

// handleMatchEnded is called when the match has ended.
- (void)handleMatchEnded:(GKTurnBasedMatch *)match
{
    mHandleMatchEndedEvent.Invoke(match);
}

@end

@implementation GameCentreLeaderboardDelegate

+(GameCentreLeaderboardDelegate*) instance
{
	if (!leaderboardDelegateInstance)
    {
		leaderboardDelegateInstance = [[GameCentreLeaderboardDelegate alloc] init];
	}
    
	return leaderboardDelegateInstance;
}

- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{	
    if(viewController.parentViewController)
    {
        [viewController.parentViewController dismissModalViewControllerAnimated:kbUseAnimationForDismiss];
    }
    else if([viewController presentingViewController])
    {
        [[viewController presentingViewController] dismissModalViewControllerAnimated:kbUseAnimationForDismiss];
    }
}

@end

@implementation GameCentreAchievementsDelegate

+(GameCentreAchievementsDelegate*) instance
{
	if (!achievementDelegateInstance)
    {
		achievementDelegateInstance = [[GameCentreAchievementsDelegate alloc] init];
	}
	return achievementDelegateInstance;
}

- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController
{	
    if(viewController.parentViewController)
    {
        [viewController.parentViewController dismissModalViewControllerAnimated:kbUseAnimationForDismiss];
    }
    else if([viewController presentingViewController])
    {
        [[viewController presentingViewController] dismissModalViewControllerAnimated:kbUseAnimationForDismiss];
    }
}

@end

@implementation GameCentreAuthenticationListener
//-----------------------------------------------
/// Shared instance
//-----------------------------------------------
+(GameCentreAuthenticationListener*) instance
{
	if (!authenticationListener)
	{
		authenticationListener = [[GameCentreAuthenticationListener alloc] init];
	}
	
	return authenticationListener;
}
//-----------------------------------------------
/// Begin Listening For GK Local Authentication Changed
//-----------------------------------------------
- (void) BeginListeningForGKLocalAuthenticationChanged
{
    if(moFlo::iOSPlatform::CGameCentreSystem::IsSupported())
    {
        //Register for notifications
        [[NSNotificationCenter defaultCenter] addObserver:self 
                                                 selector:@selector(OnAuthenticationChanged) 
                                                     name:GKPlayerAuthenticationDidChangeNotificationName 
                                                   object:nil];
    }
}
//-----------------------------------------------
/// Stop Listening For GK Local Authentication Changed
//-----------------------------------------------
- (void) StopListeningForGKLocalAuthenticationChanged
{
    if(moFlo::iOSPlatform::CGameCentreSystem::IsSupported())
    {
        [[NSNotificationCenter defaultCenter] removeObserver:self 
                                                        name:GKPlayerAuthenticationDidChangeNotificationName 
                                                      object:nil];
    }
}
//-----------------------------------------------
/// Get GK Local Authentication Changed Event
///
/// @return Event Object
//-----------------------------------------------
- (moFlo::IEvent<GKNotificationEventDelegate>&) GetGKLocalAuthenticationChangedEvent
{
	return mGKPlayerAuthenticationChangeEvent;
}
//-----------------------------------------------
/// On Authentication Changed
//-----------------------------------------------
- (void) OnAuthenticationChanged
{
	mGKPlayerAuthenticationChangeEvent.Invoke();
}

@end
