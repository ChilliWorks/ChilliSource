
#include <ChilliSource/ChilliSource.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class ToastNotification;

#define kToastAnimeTime     0.4f
#define kToastDisplayTime   2.5f

#define kToastFrameStart    CGRectMake(98.0f, -53.0f, 284.0f, 52.0f)
#define kToastFrameEnd      CGRectMake(98.0f, 2.0f, 284.0f, 52.0f)

#define kToastBackground    CGRectMake(10.0f, 6.0f, 284.0f, 52.0f)
#define kToastText1         CGRectMake(20.0f, 18.0f, 264.0f, 22.0f)

#pragma mark -

@interface ToastNotification : UIView
{
    NSString* mstrMessage; 
    UIImageView* mBackgroundImage; 

    UILabel* mMessageLabel;    
}

@property (nonatomic, retain) NSString *mstrPropMessage;
@property (nonatomic, retain) UIImageView *mPropBackgroundImage;
@property (nonatomic, retain) UILabel *mPropMessageLabel;

#pragma mark -

/**
 * Create a notification with a custom title
 * @param title    Title to display in notification.
 * @return a ToastNotification view.
 */
- (id)initWithMessage:(NSString *)inpMessage;

/**
 * Show the notification.
 */
- (void)animateIn;

/**
 * Hide the notificaiton.
 */
- (void)animateOut;


@end
