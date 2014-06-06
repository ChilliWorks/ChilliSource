
#ifdef CS_TARGETPLATFORM_IOS

#import <GameKit/GameKit.h>
#import <CSBackend/Platform/iOS/Core/Notification/ToastNotification.h>

#pragma mark -

@interface ToastNotification(internal)

- (void)animationInDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context;
- (void)animationOutDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context;
- (void)delegateCallback:(SEL)selector withObject:(id)object;

@end

#pragma mark -

@implementation ToastNotification(internal)

- (void)animationInDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
    [self performSelector:@selector(animateOut) withObject:nil afterDelay:kToastDisplayTime];
}

- (void)animationOutDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
    [self removeFromSuperview];
    [self release];
}

- (void)delegateCallback:(SEL)selector withObject:(id)object
{

}

@end

#pragma mark -

@implementation ToastNotification

@synthesize mPropBackgroundImage= mBackgroundImage;
@synthesize mstrPropMessage = mstrMessage;
@synthesize mPropMessageLabel = mMessageLabel;

#pragma mark -

- (id)initWithMessage:(NSString *)inpMessage;
{
    CGRect frame = kToastFrameStart;
    self.mstrPropMessage = inpMessage;
    self = [self initWithFrame:frame];
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    if ((self = [super initWithFrame:frame]))
    {
        // create the GK background
        UIImage *backgroundStretch = [[UIImage imageNamed:@"gk-notification.png"] stretchableImageWithLeftCapWidth:8.0f topCapHeight:0.0f];
        UIImageView *tBackground = [[UIImageView alloc] initWithFrame:kToastBackground];
        tBackground.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        tBackground.image = backgroundStretch;
        self.mPropBackgroundImage = tBackground;
        self.opaque = NO;
        [tBackground release];
        [self addSubview:self.mPropBackgroundImage];

        // create the text label
        UILabel *tTextLabel = [[UILabel alloc] initWithFrame:kToastText1];
        tTextLabel.textAlignment = NSTextAlignmentCenter;
        tTextLabel.backgroundColor = [UIColor clearColor];
        tTextLabel.textColor = [UIColor whiteColor];
        tTextLabel.font = [UIFont fontWithName:@"HelveticaNeue-Bold" size:15.0f];
        tTextLabel.text = self.mstrPropMessage;
        self.mPropMessageLabel = tTextLabel;
        [tTextLabel release];
        [self addSubview:self.mPropMessageLabel];
    }
    return self;
}

- (void)dealloc
{
    [mBackgroundImage release];
    [mMessageLabel release];
    [mstrMessage release];
    
    [super dealloc];
}


#pragma mark -

- (void)animateIn
{
    [self delegateCallback:@selector(willShowAchievementNotification:) withObject:self];
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:kToastAnimeTime];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationBeginsFromCurrentState:YES];
    [UIView setAnimationDidStopSelector:@selector(animationInDidStop:finished:context:)];
    self.frame = kToastFrameEnd;
    [UIView commitAnimations];
}

- (void)animateOut
{
    [self delegateCallback:@selector(willHideAchievementNotification:) withObject:self];
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:kToastAnimeTime];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationBeginsFromCurrentState:YES];
    [UIView setAnimationDidStopSelector:@selector(animationOutDidStop:finished:context:)];
    self.frame = kToastFrameStart;
    [UIView commitAnimations];
}

@end

#endif