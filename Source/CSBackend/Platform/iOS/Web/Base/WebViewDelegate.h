//
//  WebViewDelegate.h
//  Chilli Source
//  Created by Ian Copland on 14/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_IOS

#import <ChilliSource/ChilliSource.h>
#import <CSBackend/Platform/iOS/Web/Base/WebView.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//---------------------------------------------------------
/// A delegate for receiving callbacks from the web view.
///
/// @author S Downie
//---------------------------------------------------------
@interface WebViewDelegate : NSObject<UIWebViewDelegate>
{
	CSBackend::iOS::WebView* webViewSystem;
}
//----------------------------------------------------
/// Constructor for creating the delegate with the
/// webview system to relay on events to.
///
/// @author S Downie
///
/// @param The web view system.
//----------------------------------------------------
-(id) initWithSystem:(CSBackend::iOS::WebView*)system;
//----------------------------------------------------
/// Called when the dismiss button is pressed. This
/// is for internal use and should not be called
/// manually by the user.
///
/// @author S Downie
///
/// @param The object.
//----------------------------------------------------
-(void) onDismissButtonPressed:(id)object;

@end

#endif
