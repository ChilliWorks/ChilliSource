//
//  NotificationListener.mm
//  moFloTest
//
//  Created by Scott Downie on 01/02/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#import <ChilliSource/Backend/Platform/iOS/Core/Notification/NSNotificationAdapter.h>

#import <GameKit/GameKit.h>
#import <MediaPlayer/MediaPlayer.h>

NSNotificationAdapter* gpSharedInstance = nil;

@implementation NSNotificationAdapter

//-----------------------------------------------
/// Shared Instance
///
/// @return Singleton instance 
//-----------------------------------------------
+(NSNotificationAdapter*) sharedInstance
{
	if(!gpSharedInstance)
	{
		gpSharedInstance = [[NSNotificationAdapter alloc] init];
	}
	
	return gpSharedInstance;
}

//-----------------------------------------------
/// Begin Listening For MP Load State Changed
//-----------------------------------------------
-(void) BeginListeningForMPLoadStateChanged
{
    //Register that the load state changed (movie is ready)
    [[NSNotificationCenter defaultCenter] addObserver:gpSharedInstance 
                                             selector:@selector(OnMoviePlayerLoadStateChanged) 
                                                 name:MPMoviePlayerLoadStateDidChangeNotification 
                                               object:nil];
}
//-----------------------------------------------
/// Stop Listening For MP Load State Changed
//-----------------------------------------------
-(void) StopListeningForMPLoadStateChanged
{
    [[NSNotificationCenter defaultCenter] removeObserver:gpSharedInstance 
                                                    name:MPMoviePlayerLoadStateDidChangeNotification 
                                                  object:nil];
}

//-----------------------------------------------
/// Begin Listening For MP Playback Did Finish
//-----------------------------------------------
-(void) BeginListeningForMPPlaybackDidFinish
{
    //Register to receive a notification when the movie has finished playing. 
    [[NSNotificationCenter defaultCenter] addObserver:gpSharedInstance 
                                             selector:@selector(OnMoviePlayerPlaybackDidFinish) 
                                                 name:MPMoviePlayerPlaybackDidFinishNotification 
                                               object:nil];
}
//-----------------------------------------------
/// Stop Listening For MP Playback Did Finish
//-----------------------------------------------
-(void) StopListeningForMPPlaybackDidFinish
{
    [[NSNotificationCenter defaultCenter] removeObserver:gpSharedInstance 
                                                    name:MPMoviePlayerPlaybackDidFinishNotification 
                                                  object:nil];
}

//-----------------------------------------------
/// Get Movie Player Load State Change Event
///
/// @return Event Object
//-----------------------------------------------
-(ChilliSource::Core::IEvent<NotificationEventDelegate>&) GetMPLoadStateChangeEvent
{
    return mMPLoadStateChangeEvent;
}
//-----------------------------------------------
/// Get Movie Player Playback Did Finish Event
///
/// @return Event Object
//-----------------------------------------------
-(ChilliSource::Core::IEvent<NotificationEventDelegate>&) GetMPPlaybackDidFinishEvent
{
    return mMPPlaybackDidFinishEvent;
}
//-----------------------------------------------
/// On Movie Player Load State Changed
//-----------------------------------------------
-(void) OnMoviePlayerLoadStateChanged
{
    mMPLoadStateChangeEvent.Invoke();
}
//-----------------------------------------------
/// On Movie Player Playback Did Finish
//-----------------------------------------------
-(void) OnMoviePlayerPlaybackDidFinish
{
    mMPPlaybackDidFinishEvent.Invoke();
}

@end
