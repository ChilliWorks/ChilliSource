/*
 *  TwitterAuthenticationActivity.h
 *  moFlow
 *
 *  Created by Robert Henning on 09/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_PLATFORM_IOS_SOCIAL_TWITTER_TWITTER_AUTHENTICATION_ACTIVITY_H_
#define _MO_FLO_PLATFORM_IOS_SOCIAL_TWITTER_TWITTER_AUTHENTICATION_ACTIVITY_H_

#include <ChilliSource/Social/Twitter/TwitterAuthenticationActivity.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>

#import <UIKit/UIWebView.h>
#import <UIKit/UITextField.h>
#import <UIKit/UIAlertView.h>

@class UIView;
@class UILabel;
@class UIActivityIndicatorView;
@class UIButton;
@class UITwitterAuthenticationWebDelegate;
@class UITwitterTextFieldDelegate;
@class UITwitterAlertDelegate;
@protocol UIWebViewDelegate;
@protocol UITextFieldDelegate;
@protocol UIAlertViewDelegate;

namespace ChilliSource
{
	namespace iOS
	{
		class CTwitterAuthenticationActivity : public Social::ITwitterAuthenticationActivity
		{
		public:
			CTwitterAuthenticationActivity();
			//-----------------------------------------------
			/// Present
			/// 
			/// Starts the activity by presenting a view with
			/// a 'busy, please wait' message
			//-----------------------------------------------
			void Present();
			//-----------------------------------------------
			/// Presents the authorise URL
			/// 
			/// Set the URL received from Twitter to visit
			/// in order to see PIN
			///
			/// @param URL
			//-----------------------------------------------
			void PresentURL(const std::string& instrURL);
			//-----------------------------------------------
			/// Save PIN
			/// 
			/// Saves the PIN entered by the user
			//-----------------------------------------------
			bool SavePIN(const std::string& instrPIN);
			//-----------------------------------------------
            /// Save PIN From TextField
            ///
            /// Check if the supplied UITextField is
			/// mpPINTextFeild and if so saves the PIN
			///
			/// @param UITextField
            //-----------------------------------------------
			bool SavePINFromTextField(UITextField* inpTextField);
			//-----------------------------------------------
			/// Dismiss
			/// 
			/// Dismiss the web view interface
			//-----------------------------------------------
			void Dismiss();
			//-----------------------------------------------
			/// Dismiss Text Field Keyboard
			/// 
			/// Dismiss the keyboard
			//-----------------------------------------------
			void DismissTextFieldKeyboard();
			//-----------------------------------------------
			/// Get Alert View Index
			///
			/// Get the index for an alert view
			///
			/// @param Alert view index or -1 is not found
			//-----------------------------------------------
			NSInteger GetAlertViewIndex(UIAlertView* inpAlertView);
			//-----------------------------------------------
			/// Show PIN Entry
			///
			/// Shows/Hides PIN entry UI
			///
			/// @param true to display, false to hide
			//-----------------------------------------------
			void ShowPINEntry(bool inbShow);
			//-----------------------------------------------
			/// Show Busy Screen
			///
			/// Shows/Hides busy UI
			///
			/// @param true to display, false to hide
			//-----------------------------------------------
			void ShowBusyScreen(bool inbShow);
			//-----------------------------------------------
			/// On PIN Complete
			///
			/// Called when the user has entered a valid PIN
			//-----------------------------------------------
			void OnPINComplete();
			//-----------------------------------------------
			/// On PIN Error
			///
			/// Called when the user enters an invalid PIN
			//-----------------------------------------------
			void OnPINError();
			//-----------------------------------------------
			/// On PIN Cancel
			///
			/// Called when the user cancels entering a PIN
			//-----------------------------------------------
			void OnPINCancelled();
		
		private:
			//-----------------------------------------------
            /// Display
            ///
            /// Adds the webview to the main view
            //-----------------------------------------------
            void Display();
			//-----------------------------------------------
			/// Show authorise web view
			///
			/// Displays the authorise URL for Twitter so the
			/// user can login to see their PIN
			//-----------------------------------------------
			void ShowAuthoriseURL(const std::string& instrURL);
			
			UIView*									mpBaseView;
			UILabel*								mpBusyMessage;
			UIActivityIndicatorView*				mpBusyView;
			UIWebView*								mpWebView;
			UIButton*								mpConfirmPINButton;
			UIButton*								mpCancelPINButton;
			UILabel*								mpPINLabel;
			UITextField*							mpPINTextFeild;
			UIAlertView*							mpErrorAlert;
			UIAlertView*							mpCancelAlert;
			UITwitterAuthenticationWebDelegate*		mpWebDelegate;
			UITwitterTextFieldDelegate*				mpPINDelegate;
			UITwitterAlertDelegate*					mpAlertDelegate;
			
			moFlo::UnifiedVector2					mvUnifiedSize;
			moFlo::Core::CVector2					mvPointSize;
			moFlo::Core::CVector2					mvAbsoluteSize;
			moFlo::Core::CVector2					mvAbsolutePositionTopLeft;
			moFlo::Core::CVector2					mvAbsolutePositionCentre;
		};
	}
}

@interface UITwitterAuthenticationWebDelegate : NSObject<UIWebViewDelegate>
{
	moFlo::iOSPlatform::CTwitterAuthenticationActivity* mpDelegate;
}
-(void) SetCPPDelegate:(moFlo::iOSPlatform::CTwitterAuthenticationActivity*) inpDelegate;

@end

@interface UITwitterTextFieldDelegate : NSObject<UITextFieldDelegate>
{
	moFlo::iOSPlatform::CTwitterAuthenticationActivity* mpDelegate;
	UITextField* mpTextField;
}
-(void) SetCPPDelegate:(moFlo::iOSPlatform::CTwitterAuthenticationActivity*) inpDelegate;
-(void) SetTextField:(UITextField*)inpTextField;

@end

@interface UITwitterAlertDelegate : NSObject<UIAlertViewDelegate>
{
	moFlo::iOSPlatform::CTwitterAuthenticationActivity* mpDelegate;
}
-(void) SetCPPDelegate:(moFlo::iOSPlatform::CTwitterAuthenticationActivity*) inpDelegate;

@end

#endif
