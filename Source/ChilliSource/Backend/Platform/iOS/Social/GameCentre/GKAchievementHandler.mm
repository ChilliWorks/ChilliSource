//
//  GKAchievementHandler.m
//
//  Created by Benjamin Borowski on 9/30/10.
//  Copyright 2010 Typeoneerror Studios. All rights reserved.
//  $Id$
//

#import <GameKit/GameKit.h>
#import "GKAchievementHandler.h"
#import "GKAchievementNotification.h"

static GKAchievementHandler *defaultHandler = nil;

#pragma mark -

@interface GKAchievementHandler(internal)

- (void)displayNotification:(GKAchievementNotification *)notification;

@end

#pragma mark -

@implementation GKAchievementHandler(internal)

- (void)displayNotification:(GKAchievementNotification *)notification
{
    if (self.image != nil)
    {
        [notification setImage:self.image];
    }
    else
    {
        [notification setImage:nil];
    }

    [_topView addSubview:notification];
    [notification animateIn];
}

@end

#pragma mark -

@implementation GKAchievementHandler

@synthesize image=_image;

#pragma mark -

+ (GKAchievementHandler *)defaultHandler
{
    if (!defaultHandler) defaultHandler = [[self alloc] init];
    return defaultHandler;
}

- (id)init
{
    self = [super init];
    if (self != nil)
    {
        _topView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
        _queue = [[NSMutableArray alloc] initWithCapacity:0];
        self.image = [UIImage imageNamed:@"gk-icon.png"];
    }
    return self;
}

- (void)dealloc
{
    [_queue release];
    [_image release];
    [super dealloc];
}

#pragma mark -

- (CGRect) getAchievementStartFrame
{
	CGRect screenRect = [[UIScreen mainScreen] bounds];
	CGRect achievementFrameSize = kGKAchievementDefaultSize;
	CGFloat fFrameX = (screenRect.size.width - achievementFrameSize.size.width) / 2.0f;
	CGFloat fFrameY = -(achievementFrameSize.size.height + 1);
	CGFloat fFrameWidth = achievementFrameSize.size.width;
	CGFloat fFrameHeight = achievementFrameSize.size.height;
	CGRect achievementFrame = CGRectMake(fFrameX, fFrameY, fFrameWidth, fFrameHeight);
	
	return achievementFrame;
}

- (void)notifyAchievement:(GKAchievementDescription *)achievement
{
    GKAchievementNotification *notification = [[[GKAchievementNotification alloc] initWithAchievementDescription:achievement] autorelease];
    notification.frame = [self getAchievementStartFrame];
    notification.handlerDelegate = self;

    [_queue addObject:notification];
    if ([_queue count] == 1)
    {
        [self displayNotification:notification];
    }
}

- (void)notifyAchievementTitle:(NSString *)title andMessage:(NSString *)message
{
    GKAchievementNotification *notification = [[[GKAchievementNotification alloc] initWithTitle:title andMessage:message] autorelease];
    notification.frame = [self getAchievementStartFrame];
    notification.handlerDelegate = self;

    [_queue addObject:notification];
    if ([_queue count] == 1)
    {
        [self displayNotification:notification];
    }
}

#pragma mark -
#pragma mark GKAchievementHandlerDelegate implementation

- (void)didHideAchievementNotification:(GKAchievementNotification *)notification
{
    [_queue removeObjectAtIndex:0];
    if ([_queue count])
    {
        [self displayNotification:(GKAchievementNotification *)[_queue objectAtIndex:0]];
    }
}

@end
