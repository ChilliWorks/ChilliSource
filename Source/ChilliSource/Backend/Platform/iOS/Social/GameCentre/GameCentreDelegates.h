//
//  GameCentreDelegates.h
//  MoFlow
//
//  Created by Stuart McGaw on 18/03/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_PLATFORM_IOS_GAMECENTREDELEGATES_H_
#define _MOFLO_PLATFORM_IOS_GAMECENTREDELEGATES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/GenericEvent.h>

#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>
#import <GameKit/GKTurnBasedMatch.h>


typedef fastdelegate::FastDelegate0<> GKNotificationEventDelegate;
typedef fastdelegate::FastDelegate0<> GameCentreTurnBasedMatchmakerViewControllerWasCancelledEvent;
typedef fastdelegate::FastDelegate0<> GameCentreTurnBasedMatchmakerViewControllerDidFailEvent;
typedef fastdelegate::FastDelegate1<GKTurnBasedMatch*> GameCentreTurnBasedMatchmakerViewControllerDidFindMatchEvent;

typedef fastdelegate::FastDelegate1<NSArray*> HandleInviteFromGameCenterEventDelegate;
typedef fastdelegate::FastDelegate2<GKTurnBasedMatch*, bool> HandleTurnEventForMatchEventDelegate;
typedef fastdelegate::FastDelegate1<GKTurnBasedMatch*> HandleMatchEndedEventDelegate;

@interface GameCentreTurnBasedMatchmakerViewControllerDelegate : NSObject <GKTurnBasedMatchmakerViewControllerDelegate> 
{
    ChilliSource::Core::CEvent0<GameCentreTurnBasedMatchmakerViewControllerWasCancelledEvent>    mWasCancelledEvent;
    ChilliSource::Core::CEvent0<GameCentreTurnBasedMatchmakerViewControllerDidFailEvent>         mDidFailEvent;
    ChilliSource::Core::CEvent1<GameCentreTurnBasedMatchmakerViewControllerDidFindMatchEvent>    mDidFindMatchEvent;
}

+(GameCentreTurnBasedMatchmakerViewControllerDelegate*) instance;

// Matchmaking was cancelled by the user
- (void)turnBasedMatchmakerViewControllerWasCancelled:(GKTurnBasedMatchmakerViewController *)viewController;

// Matchmaking has failed with an error
- (void)turnBasedMatchmakerViewController:(GKTurnBasedMatchmakerViewController *)viewController didFailWithError:(NSError *)error;

// A turned-based match has been found, the game should start
- (void)turnBasedMatchmakerViewController:(GKTurnBasedMatchmakerViewController *)viewController didFindMatch:(GKTurnBasedMatch *)match;

// Called when a users chooses to quit a match and that player has the current turn.  The developer should call playerQuitInTurnWithOutcome:nextPlayer:matchData:completionHandler: on the match passing in appropriate values.  They can also update matchOutcome for other players as appropriate.
- (void)turnBasedMatchmakerViewController:(GKTurnBasedMatchmakerViewController *)viewController playerQuitForMatch:(GKTurnBasedMatch *)match;

- (ChilliSource::Core::IEvent<GameCentreTurnBasedMatchmakerViewControllerWasCancelledEvent>&) GetWasCancelledEvent;
- (ChilliSource::Core::IEvent<GameCentreTurnBasedMatchmakerViewControllerDidFailEvent>&) GetDidFailEvent;
- (ChilliSource::Core::IEvent<GameCentreTurnBasedMatchmakerViewControllerDidFindMatchEvent>&) GetDidFindMatchEvent;
@end

@interface GameCentreTurnBasedEventHandlerDelegate : NSObject <GKTurnBasedEventHandlerDelegate> 
{
    ChilliSource::Core::CEvent1<HandleInviteFromGameCenterEventDelegate>     mHandleInviteFromGameCenterEvent;
    ChilliSource::Core::CEvent2<HandleTurnEventForMatchEventDelegate>        mHandleTurnEventForMatchEvent;
    ChilliSource::Core::CEvent1<HandleMatchEndedEventDelegate>               mHandleMatchEndedEvent;
}
+(GameCentreTurnBasedEventHandlerDelegate*) instance;

- (ChilliSource::Core::IEvent<HandleInviteFromGameCenterEventDelegate>&) GetHandleInviteFromGameCenterEvent;
- (ChilliSource::Core::IEvent<HandleTurnEventForMatchEventDelegate>&) GetHandleTurnEventForMatchEvent;
- (ChilliSource::Core::IEvent<HandleMatchEndedEventDelegate>&) GetHandleMatchEndedEvent;

- (void)handleInviteFromGameCenter:(NSArray *)playersToInvite;		

// handleTurnEventForMatch is called when a turn is passed to another participant.  Note this may arise from one of the following events:
//      The local participant has accepted an invite to a new match
//      The local participant has been passed the turn for an existing match
//      Another participant has made a turn in an existing match
// The application needs to be prepared to handle this even while the participant might be engaged in a different match
- (void)handleTurnEventForMatch:(GKTurnBasedMatch *)match didBecomeActive:(BOOL)didBecomeActive;
- (void)handleTurnEventForMatch:(GKTurnBasedMatch *)match;

// handleMatchEnded is called when the match has ended.
- (void)handleMatchEnded:(GKTurnBasedMatch *)match;

@end

@interface GameCentreLeaderboardDelegate : NSObject <GKLeaderboardViewControllerDelegate>{
    
}
+(GameCentreLeaderboardDelegate*) instance;
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController;
@end

@interface GameCentreAchievementsDelegate : NSObject<GKAchievementViewControllerDelegate>
{
	
}

+(GameCentreAchievementsDelegate*) instance;
- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController;

@end

@interface GameCentreAuthenticationListener : NSObject
{
	//---Game Kit
	ChilliSource::Core::CEvent0<GKNotificationEventDelegate> mGKPlayerAuthenticationChangeEvent;
}
//-----------------------------------------------
/// Shared instance
//-----------------------------------------------
+(GameCentreAuthenticationListener*) instance;
//-----------------------------------------------
/// Begin Listening For GK Local Authentication Changed
//-----------------------------------------------
- (void) BeginListeningForGKLocalAuthenticationChanged;
//-----------------------------------------------
/// Stop Listening For GK Local Authentication Changed
//-----------------------------------------------
- (void) StopListeningForGKLocalAuthenticationChanged;
//-----------------------------------------------
/// Get GK Local Authentication Changed Event
///
/// @return Event Object
//-----------------------------------------------
- (ChilliSource::Core::IEvent<GKNotificationEventDelegate>&) GetGKLocalAuthenticationChangedEvent;
//-----------------------------------------------
/// On Authentication Changed
//-----------------------------------------------
- (void) OnAuthenticationChanged;
@end



#endif