/*
 *  TwitterAuthenticationActivity.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 09/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Social/Twitter/TwitterAuthenticationActivity.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Base/Screen.h>

#include <ChilliSource/Social/Twitter/TwitterPostSystem.h>

#include <UIKit/UIKit.h>

const NSInteger		kdwButtonTagPositive			= 0;
const NSInteger		kdwButtonTagNegative			= 1;

const NSInteger		kdwAlertViewNotFound			= -1;
const NSInteger		kdwAlertViewIndexError			= 0;
const NSInteger		kdwAlertViewIndexCancel			= 1;

const CGFloat		kfDefaultLabelPointHeight		= 22.0f;
const CGFloat		kfDefaultButtonPointHeight		= 44.0f;
const CGFloat		kfDefaultButtonPointWidth		= 80.0f;
const CGFloat		kfDefaultPaddingPointSizeTop	= 5.0f;

namespace ChilliSource
{
	namespace iOS
	{
		//-----------------------------------------------
		/// Constructor
		///
		//-----------------------------------------------
		CTwitterAuthenticationActivity::CTwitterAuthenticationActivity() : mpBaseView(nil),
																		   mpBusyMessage(nil),
																		   mpBusyView(nil),
																		   mpWebView(nil),
																		   mpConfirmPINButton(nil),
																		   mpCancelPINButton(nil),
																		   mpPINLabel(nil),
																		   mpPINTextFeild(nil),
																		   mpErrorAlert(nil),
																		   mpCancelAlert(nil),
																		   mpWebDelegate(nil),
																		   mpPINDelegate(nil),
																		   mpAlertDelegate(nil),
																		   mvUnifiedSize(1.0f,1.0f,0.0f,0.0f)
		{
		}
		//-----------------------------------------------
		/// Begin
		//-----------------------------------------------
		void CTwitterAuthenticationActivity::Present()
		{
			mvPointSize = (Core::CScreen::GetOrientedDimensions() / Core::CScreen::GetDensity());
			mvAbsoluteSize = (mvPointSize * mvUnifiedSize.GetRelative()) + mvUnifiedSize.GetAbsolute();
			mvAbsolutePositionTopLeft = (mvPointSize - mvAbsoluteSize) / 2;
			mvAbsolutePositionCentre = mvAbsolutePositionTopLeft + (mvAbsoluteSize / 2);
			
			if(!mpBaseView)
			{
				mpBaseView = [[UIView alloc] initWithFrame:CGRectMake(mvAbsolutePositionTopLeft.x, mvAbsolutePositionTopLeft.y, mvAbsoluteSize.x, mvAbsoluteSize.y)];
				mpBaseView.backgroundColor = [UIColor blackColor];
				mpBaseView.opaque = NO;
				mpBaseView.hidden = NO;
			}
			
			if(!mpBusyMessage)
			{
				mpBusyMessage = [[UILabel alloc] initWithFrame:CGRectMake(mvAbsolutePositionTopLeft.x, mvAbsolutePositionCentre.y, mvAbsoluteSize.x, mvAbsoluteSize.y / 10)];
				mpBusyMessage.backgroundColor = [UIColor clearColor];
				mpBusyMessage.opaque = NO;
				mpBusyMessage.hidden = NO;
				mpBusyMessage.text = @"Contacting Twitter...";
				mpBusyMessage.textColor = [UIColor whiteColor];
				mpBusyMessage.textAlignment = NSTextAlignmentCenter;
				[mpBaseView addSubview:mpBusyMessage];
			}
			
			if(!mpBusyView)
			{
				mpBusyView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
				[mpBaseView addSubview:mpBusyView];
				mpBusyView.center = CGPointMake(mvAbsolutePositionCentre.x, mvAbsolutePositionCentre.y - (mvAbsoluteSize.y / 4));
				mpBusyView.hidesWhenStopped = YES;
				mpBusyView.hidden = NO;
				[mpBusyView startAnimating];
			}
			
			if(!mpWebView)
			{
				Core::UnifiedVector2 vWebViewUnifiedSize = Core::UnifiedVector2(0.0f, 0.0f, mvPointSize.x, (mvPointSize.y - (kfDefaultButtonPointHeight + (kfDefaultPaddingPointSizeTop * 2))));
				Core::Vector2 vWebAbsoluteSize = (mvPointSize * vWebViewUnifiedSize.GetRelative()) + vWebViewUnifiedSize.GetAbsolute();
				Core::Vector2 vWebAbsolutePosition = (mvPointSize - vWebAbsoluteSize);
				
				//Create the view and present it, centered on screen
				mpWebView = [[UIWebView alloc] initWithFrame:CGRectMake(vWebAbsolutePosition.x, vWebAbsolutePosition.y, vWebAbsoluteSize.x, vWebAbsoluteSize.y)];
				mpWebView.backgroundColor = [UIColor clearColor];
				mpWebView.opaque = NO;
				mpWebView.hidden = YES;
				[mpBaseView addSubview:mpWebView];
			}
			
			if(!mpPINLabel)
			{
				CGFloat fWidth = 35.0f;
				CGFloat fHeight = kfDefaultLabelPointHeight;
				CGFloat fY = (kfDefaultPaddingPointSizeTop + ((kfDefaultButtonPointHeight / 2) - (kfDefaultLabelPointHeight / 2)));
				mpPINLabel = [[UILabel alloc] initWithFrame:CGRectMake(mvAbsolutePositionTopLeft.x, fY, fWidth, fHeight)];
				mpPINLabel.textAlignment = NSTextAlignmentLeft;
				mpPINLabel.textColor = [UIColor whiteColor];
				mpPINLabel.backgroundColor = [UIColor clearColor];
				mpPINLabel.text = @"PIN:";
				[mpBaseView addSubview:mpPINLabel];
			}
			
			if(!mpPINTextFeild)
			{
				CGFloat fWidth = 96.0f;
				CGFloat fHeight = kfDefaultLabelPointHeight;
				CGFloat fY = (kfDefaultPaddingPointSizeTop + ((kfDefaultButtonPointHeight / 2) - (kfDefaultLabelPointHeight / 2)));
				mpPINTextFeild = [[UITextField alloc] initWithFrame:CGRectMake(mvAbsolutePositionTopLeft.x + 40.0f, fY, fWidth, fHeight)];
				mpPINTextFeild.backgroundColor = [UIColor whiteColor];
				mpPINTextFeild.opaque = NO;
				mpPINTextFeild.hidden = YES;
				mpPINTextFeild.placeholder = @"Enter PIN";
				mpPINTextFeild.autocapitalizationType = UITextAutocapitalizationTypeNone;
				mpPINTextFeild.autocorrectionType = UITextAutocorrectionTypeNo;
				mpPINTextFeild.enablesReturnKeyAutomatically = NO;
				mpPINTextFeild.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
				mpPINTextFeild.returnKeyType = UIReturnKeyDone;
				[mpBaseView addSubview:mpPINTextFeild];
			}
			
			if(!mpPINDelegate)
			{
				mpPINDelegate = [[UITwitterTextFieldDelegate alloc] init];
				[mpPINDelegate SetCPPDelegate:this];
				[mpPINDelegate SetTextField:mpPINTextFeild];
				mpPINTextFeild.delegate = mpPINDelegate;
			}
			
			if(!mpConfirmPINButton)
			{
				CGFloat fWidth = kfDefaultButtonPointWidth;
				CGFloat fHeight = kfDefaultButtonPointHeight;
				
				mpConfirmPINButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
				mpConfirmPINButton.hidden = YES;
				[mpConfirmPINButton addTarget:mpPINDelegate action:@selector(OnConfirmButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
				[mpConfirmPINButton setTitle:@"Confirm" forState:UIControlStateNormal];
				[mpConfirmPINButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal]; 
				[mpConfirmPINButton setFrame:CGRectMake(mvAbsolutePositionTopLeft.x + mvAbsoluteSize.x - fWidth - 5, kfDefaultPaddingPointSizeTop, fWidth, fHeight)];
				[mpBaseView addSubview:mpConfirmPINButton];
			}
			
			if(!mpCancelPINButton)
			{
				CGFloat fWidth = kfDefaultButtonPointWidth;
				CGFloat fHeight = kfDefaultButtonPointHeight;
				
				mpCancelPINButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
				mpCancelPINButton.hidden = YES;
				[mpCancelPINButton addTarget:mpPINDelegate action:@selector(OnCancelButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
				[mpCancelPINButton setTitle:@"Cancel" forState:UIControlStateNormal];
				[mpCancelPINButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal]; 
				[mpCancelPINButton setFrame:CGRectMake(mvAbsolutePositionTopLeft.x + mvAbsoluteSize.x - (fWidth * 2) - (5 * 2), kfDefaultPaddingPointSizeTop, fWidth, fHeight)];
				[mpBaseView addSubview:mpCancelPINButton];
			}
			
			Display();
		}
		//-----------------------------------------------
		/// Presents the authorise URL
		//-----------------------------------------------
		void CTwitterAuthenticationActivity::PresentURL(const std::string& instrURL)
		{
			if(!instrURL.empty())
				ShowAuthoriseURL(instrURL);
		}
		//-----------------------------------------------
		/// Save PIN entered by user
		//-----------------------------------------------
		bool CTwitterAuthenticationActivity::SavePIN(const std::string& instrPIN)
		{
			bool bResult = false;
			std::string strPIN = instrPIN;
			
			if(Social::TwitterPIN::kudwTwitterPINLength != instrPIN.size())
			{
				// Handle error here
				CS_ERROR_LOG("Invalid PIN length!");
				strPIN.clear();
			}
			else
			{
				bResult = true;
			}
			
			mstrPIN.assign(strPIN);
			
			return bResult;
		}
		//-----------------------------------------------
		/// Save PIN From TextField
		//-----------------------------------------------
		bool CTwitterAuthenticationActivity::SavePINFromTextField(UITextField* inpTextField)
		{
			std::string strPIN;
			
			if([mpPINTextFeild isEqual:inpTextField])
			{
				if(mpPINTextFeild.text != nil)
				{
					strPIN = Core::CStringUtils::NSStringToString(mpPINTextFeild.text);
				}
			}
			else
				strPIN.clear();
			
			return SavePIN(strPIN);
		}
		//-----------------------------------------------
		/// Dismiss
		//-----------------------------------------------
		void CTwitterAuthenticationActivity::Dismiss()
		{
			if(mpBaseView)
			{
				if(mpBusyMessage)
				{
					[mpBusyMessage removeFromSuperview];
					[mpBusyMessage release];
				}
				
				if(mpBusyView)
				{
					[mpBusyView stopAnimating];
					[mpBusyView removeFromSuperview];
					[mpBusyView release];
				}
				
				if(mpWebView)
				{
					[mpWebView stopLoading];
					[mpWebView removeFromSuperview];
					[mpWebView release];
					[mpWebDelegate release];
				}
				
				if(mpPINLabel)
				{
					[mpPINLabel removeFromSuperview];
					[mpPINLabel release];
				}
				
				if(mpPINTextFeild)
				{
					[mpPINTextFeild removeFromSuperview];
					[mpPINTextFeild release];
					[mpPINDelegate release];
				}
				
				if(mpConfirmPINButton)
				{
					[mpConfirmPINButton removeFromSuperview];
				}
				
				if(mpCancelPINButton)
				{
					[mpCancelPINButton removeFromSuperview];
				}
				
				if(mpErrorAlert)
				{
					[mpErrorAlert release];
				}
				
				if(mpCancelAlert)
				{
					[mpCancelAlert release];
				}
				
				[mpBaseView removeFromSuperview];
				[mpBaseView release];
				
				mpBusyMessage = nil;
				mpBusyView = nil;
				mpWebView = nil;
				mpWebDelegate = nil;
				mpPINTextFeild = nil;
				mpPINDelegate = nil;
				mpConfirmPINButton = nil;
				mpCancelPINButton = nil;
				mpErrorAlert = nil;
				mpCancelAlert = nil;
				mpBaseView = nil;
				
				mOnDismissedEvent.NotifyConnections(this);
			}
		}
		
		//-----------------------------------------------
		/// Dismiss Text Field Keyboard
		//-----------------------------------------------
		void CTwitterAuthenticationActivity::DismissTextFieldKeyboard()
		{
			if(mpPINTextFeild)
			{
				if([mpPINTextFeild isFirstResponder] && [mpPINTextFeild canResignFirstResponder])
					[mpPINTextFeild resignFirstResponder];
			}
		}
		//-----------------------------------------------
		/// Get Alert View Index
		//-----------------------------------------------
		NSInteger CTwitterAuthenticationActivity::GetAlertViewIndex(UIAlertView* inpAlertView)
		{
			if(mpErrorAlert)
			{
				if([mpErrorAlert isEqual:inpAlertView])
				{
					return kdwAlertViewIndexError;
				}
			}
			
			if(mpCancelAlert)
			{
				if([mpCancelAlert isEqual:inpAlertView])
				{
					return kdwAlertViewIndexCancel;
				}
			}
			
			return kdwAlertViewNotFound;
		}
		//-----------------------------------------------
        /// Show/Hide the PIN entry screen
        //-----------------------------------------------
		void CTwitterAuthenticationActivity::ShowPINEntry(bool inbShow)
		{
			if(mpWebView)
				mpWebView.hidden = (BOOL)(inbShow == false);
			
			if(mpPINTextFeild)
				mpPINTextFeild.hidden = (BOOL)(inbShow == false);
			
			if(mpConfirmPINButton)
				mpConfirmPINButton.hidden = (BOOL)(inbShow == false);
			
			if(mpCancelPINButton)
				mpCancelPINButton.hidden = (BOOL)(inbShow == false);
		}
		//-----------------------------------------------
        /// Show/Hide the busy screen
        //-----------------------------------------------
        void CTwitterAuthenticationActivity::ShowBusyScreen(bool inbShow)
		{
			if(mpBusyMessage)
				mpBusyMessage.hidden = (BOOL)(inbShow == false);
			
			if(mpBusyView)
				mpBusyView.hidden = (BOOL)(inbShow == false);
		}
		//-----------------------------------------------
        /// On PIN Complete
        //-----------------------------------------------
		void CTwitterAuthenticationActivity::OnPINComplete()
		{
			if(mOnPINResultDelegate)
			{
				ChilliSource::Social::TwitterAuthenticationActivity::AuthenticationPINResult sResult;
				
				sResult.strPIN.assign(mstrPIN);
				mOnPINResultDelegate(sResult);
			}
			Dismiss();
		}
		//-----------------------------------------------
        /// On PIN Error
        //-----------------------------------------------
		void CTwitterAuthenticationActivity::OnPINError()
		{
			if(!mpAlertDelegate)
			{
				mpAlertDelegate = [[UITwitterAlertDelegate alloc] init];
				[mpAlertDelegate SetCPPDelegate:this];
			}
			
			if(!mpErrorAlert)
			{
				mpErrorAlert = [[UIAlertView alloc]
								 initWithTitle: @"Wrong PIN"
								 message: @"Oops! The PIN you entered is not correct. Please try again."
								 delegate: mpAlertDelegate
								 cancelButtonTitle:@"OK"
								 otherButtonTitles:nil];
			}
			
			[mpErrorAlert show];
		}
		//-----------------------------------------------
        /// On PIN Cancel
        //-----------------------------------------------
		void CTwitterAuthenticationActivity::OnPINCancelled()
		{
			if(!mpAlertDelegate)
			{
				mpAlertDelegate = [[UITwitterAlertDelegate alloc] init];
				[mpAlertDelegate SetCPPDelegate:this];
			}
			
			if(!mpCancelAlert)
			{
				mpCancelAlert = [[UIAlertView alloc]
								  initWithTitle: @"Cancel Twitter Sign-In?"
								  message: @"Are you sure you want to cancel signing into Twitter?"
								  delegate: mpAlertDelegate
								  cancelButtonTitle:@"Yes"
								  otherButtonTitles:@"No",nil];
			}
			
			[mpCancelAlert show];
//			Dismiss();
		}
		
		//--- Private
		
		//-----------------------------------------------
        /// Display
        //-----------------------------------------------
        void CTwitterAuthenticationActivity::Display()
		{
			ShowPINEntry(false);
			ShowBusyScreen(true);
			[[EAGLView sharedInstance] addSubview:mpBaseView];
		}
		//-----------------------------------------------
		/// Show authorise web view
		//-----------------------------------------------
		void CTwitterAuthenticationActivity::ShowAuthoriseURL(const std::string& instrURL)
		{
			if(!mpWebView)
			{
				Dismiss();
			}
			else
			{
				NSURL* pUrl = [NSURL URLWithString:Core::CStringUtils::StringToNSString(instrURL)];
				[mpWebView loadRequest:[NSURLRequest requestWithURL:pUrl]];
				mpWebView.hidden = NO;
				mpWebDelegate = [[UITwitterAuthenticationWebDelegate alloc] init];
				[mpWebDelegate SetCPPDelegate:this];
				mpWebView.delegate = mpWebDelegate;
			}
		}
	}
}

//--- Web delegate

@implementation UITwitterAuthenticationWebDelegate

-(void) SetCPPDelegate:(ChilliSource::iOS::CTwitterAuthenticationActivity*) inpDelegate
{
	mpDelegate = inpDelegate;
}

-(BOOL)webView:(UIWebView*)webView shouldStartLoadWithRequest:(NSURLRequest*)request navigationType:(UIWebViewNavigationType)navigationType 
{
	if(navigationType == UIWebViewNavigationTypeLinkClicked)
	{
		[[UIApplication sharedApplication] openURL:request.URL];
		return NO;
	}
	else 
	{
		mpDelegate->ShowBusyScreen(false);
		mpDelegate->ShowPINEntry(true);
		return YES;
	}
}

-(void) webViewDidFinishLoad:(UIWebView *)webView
{
	// Do nothing
}

-(void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSLog(@"%@", [error localizedDescription]);
}

@end

//--- Text field delegate

@implementation UITwitterTextFieldDelegate

-(void) SetCPPDelegate:(ChilliSource::iOS::CTwitterAuthenticationActivity *) inpDelegate
{
	mpDelegate = inpDelegate;
}

-(void) SetTextField:(UITextField*)inpTextField
{
	mpTextField = inpTextField;
}

-(void) OnConfirmButtonPressed:(id)inpObject
{
	mpDelegate->DismissTextFieldKeyboard();
	if(mpDelegate->SavePINFromTextField(mpTextField))
		mpDelegate->OnPINComplete();
	else
		mpDelegate->OnPINError();
}

-(void) OnCancelButtonPressed:(id)inpObject
{
	mpDelegate->OnPINCancelled();
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	[textField resignFirstResponder];
	return YES;
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
	
}

@end

//--- Alert delegate

@implementation UITwitterAlertDelegate

-(void) SetCPPDelegate:(ChilliSource::iOS::CTwitterAuthenticationActivity *) inpDelegate
{
	mpDelegate = inpDelegate;
}

-(void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	NSInteger dwAlertViewIndex = mpDelegate->GetAlertViewIndex(alertView);
	
	if(kdwButtonTagPositive == buttonIndex)
	{
		if(kdwAlertViewIndexCancel == dwAlertViewIndex)
			mpDelegate->Dismiss();
	}
	else
	if(kdwButtonTagNegative == buttonIndex)
	{
		// You can do stuff here
	}
}

@end
