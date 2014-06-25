//
//  WebViewDelegate.mm
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

#import <CSBackend/Platform/iOS/Web/Base/WebViewDelegate.h>

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

@implementation WebViewDelegate
//----------------------------------------------------
//----------------------------------------------------
-(id) initWithSystem:(CSBackend::iOS::WebView*)system
{
    if (self = [super init])
    {
        webViewSystem = system;
        return self;
    }
    return nil;
}
//----------------------------------------------------
//----------------------------------------------------
-(void) onDismissButtonPressed:(id)object
{
	webViewSystem->Dismiss();
}
//----------------------------------------------------
//----------------------------------------------------
-(BOOL)webView:(UIWebView*)webView shouldStartLoadWithRequest:(NSURLRequest*)request navigationType:(UIWebViewNavigationType)navigationType
{
	if(navigationType == UIWebViewNavigationTypeLinkClicked)
	{
        if([[[request URL] scheme] isEqualToString:@"file"] == TRUE)
        {
            //Load all other links externally
            [[UIApplication sharedApplication] openURL:request.URL];
            return NO;
        }
	}
    
    return YES;
}
//----------------------------------------------------
//----------------------------------------------------
-(void) webViewDidFinishLoad:(UIWebView*)webView
{
    webViewSystem->OnViewDidFinishLoad();
}
//----------------------------------------------------
//----------------------------------------------------
-(void)webView:(UIWebView*)webView didFailLoadWithError:(NSError*)error
{
    CS_LOG_ERROR([NSStringUtils newUTF8StringWithNSString:[error localizedDescription]]);
    
    if(webViewSystem)
    {
        webViewSystem->Dismiss();
    }
}
@end

#endif