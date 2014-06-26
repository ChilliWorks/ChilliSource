//
//  SubtitlesRenderer.mm
//  Chilli Source
//  Created by Ian Copland on 22/02/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#import <CSBackend/Platform/iOS/Video/Base/SubtitlesRenderer.h>

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Base/Screen.h>
#import <ChilliSource/Core/String/StringUtils.h>
#import <ChilliSource/Core/Localisation/LocalisedText.h>
#import <ChilliSource/Video/Base/Subtitles.h>

#import <QuartzCore/QuartzCore.h>

@implementation CSubtitlesRenderer
//--------------------------------------------------------
//--------------------------------------------------------
-(id) initWithVideoPlayer:(CSBackend::iOS::VideoPlayer*)in_videoPlayer view:(UIView*)inpView andSubtitles:(const CSVideo::SubtitlesCSPtr&)in_subtitles
{
    if(!(self = [super init]))
	{
		return nil;
	}
    
    m_videoPlayer = in_videoPlayer;
    mpBaseView = inpView;
    m_screen = CSCore::Application::Get()->GetSystem<CSCore::Screen>();
    
    //create the display link
    mpDisplayLink = [CADisplayLink displayLinkWithTarget: self selector: @selector(OnUpdate)];
    [mpDisplayLink addToRunLoop: [NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    
    //load the subtitles
    CS_ASSERT(in_subtitles != nullptr, "Cannot render null subtitles");
    mpSubtitles = in_subtitles;
    
    return self;
}
//--------------------------------------------------------
/// On Update
//--------------------------------------------------------
-(void) OnUpdate
{
    //get the current subtitles
    f32 fPosition = m_videoPlayer->GetCurrentTime();
    TimeIntervalMs currentTimeMS = (TimeIntervalMs)(fPosition * 1000.0f);
    
    if (mCurrentTimeMS != currentTimeMS)
    {
        mCurrentTimeMS = currentTimeMS;
        auto pSubtitleArray = mpSubtitles->GetSubtitlesAtTime(mCurrentTimeMS);
        auto localisedText = mpSubtitles->GetLocalisedText().get();

        //add any new subtitles
        for (auto it = pSubtitleArray.begin(); it != pSubtitleArray.end(); ++it)
        {
            
            auto mapEntry = maTextViewMap.find(*it);
            if (mapEntry == maTextViewMap.end())
            {
                [self AddTextViewWithSubtitle:*it andLocalisedText:localisedText];
            }
        }

        //update the current text views
        for (auto it = maTextViewMap.begin(); it != maTextViewMap.end(); ++it)
        {
            [self UpdateTextView:it->second Subtitle:it->first Time:mCurrentTimeMS];
        }

        //removes any text views that are no longer needed.
        for (auto it = maSubtitlesToRemove.begin(); it != maSubtitlesToRemove.end(); ++it)
        {
            auto mapEntry = maTextViewMap.find(*it);
            if (mapEntry != maTextViewMap.end())
            {
                [mapEntry->second removeFromSuperview];
                [mapEntry->second release];
                maTextViewMap.erase(mapEntry);
            }
        }
        maSubtitlesToRemove.clear();
    }
}
//--------------------------------------------------------
/// Add Text View
//--------------------------------------------------------
-(void) AddTextViewWithSubtitle:(const CSVideo::Subtitles::Subtitle*)inpSubtitle andLocalisedText:(const CSCore::LocalisedText*) in_localisedText
{
    //get the style
    const CSVideo::Subtitles::Style* pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->m_styleName);
    if (pStyle == nullptr)
    {
        CS_LOG_ERROR("Cannot find style '" + inpSubtitle->m_styleName + "' in subtitles.");
        return;
    }
    
    //create the new text view
    UITextView* pNewTextView = [[UITextView alloc] initWithFrame: [self CalculateTextBoxRect:pStyle->m_bounds]];
    [mpBaseView addSubview:pNewTextView];
    [mpBaseView bringSubviewToFront:pNewTextView];
    pNewTextView.backgroundColor = [UIColor clearColor];
    
    //setup the text.
    NSString* text = [NSStringUtils newNSStringWithUTF8String:in_localisedText->GetText(inpSubtitle->m_localisedTextId)];
    NSString* fontName = [NSStringUtils newNSStringWithUTF8String:pStyle->m_fontName];
    [pNewTextView setText:text];
    [pNewTextView setFont:[UIFont fontWithName:fontName size: pStyle->m_fontSize]];
    [pNewTextView setTextColor:[UIColor colorWithRed:pStyle->m_colour.r green:pStyle->m_colour.g blue:pStyle->m_colour.b alpha:0.0f]];
    [pNewTextView setUserInteractionEnabled:NO];
    [text release];
    [fontName release];
    
    [self SetAlignment: pNewTextView WithAnchor: pStyle->m_alignment];
    maTextViewMap.insert(std::make_pair(inpSubtitle, pNewTextView));
}
//--------------------------------------------------------
/// Update Text View
//--------------------------------------------------------
-(void) UpdateTextView:(UITextView*)inpTextView Subtitle:(const CSVideo::Subtitles::Subtitle*)inpSubtitle Time:(TimeIntervalMs)inTimeMS
{
    const CSVideo::Subtitles::Style* pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->m_styleName);
    
    f32 fFade = 0.0f;
    s64 lwRelativeTime = ((s64)inTimeMS) - ((s64)inpSubtitle->m_startTimeMS);
    s64 lwDisplayTime = ((s64)inpSubtitle->m_endTimeMS) - ((s64)inpSubtitle->m_startTimeMS);
    
    //subtitle should not be displayed yet so remove
    if (lwRelativeTime < 0)
    {
        [self RemoveTextView:inpSubtitle];
    }
    
    //fading in
    else if (lwRelativeTime < pStyle->m_fadeTimeMS)
    {
        fFade = ((f32)lwRelativeTime) / ((f32)pStyle->m_fadeTimeMS);
    }
    
    //active
    else if (lwRelativeTime < lwDisplayTime - pStyle->m_fadeTimeMS)
    {
        fFade = 1.0f;
    }
    
    //fading out
    else if (lwRelativeTime < lwDisplayTime)
    {
        fFade = 1.0f - (((f32)lwRelativeTime - (lwDisplayTime - pStyle->m_fadeTimeMS)) / ((f32)pStyle->m_fadeTimeMS));
    }
    
    //should no longer be displayed so remove
    else if (lwRelativeTime >= lwDisplayTime)
    {
        [self RemoveTextView:inpSubtitle];
    }
    
    [inpTextView setTextColor:[UIColor colorWithRed:pStyle->m_colour.r green:pStyle->m_colour.g blue:pStyle->m_colour.b alpha:(fFade * pStyle->m_colour.a)]];
}
//--------------------------------------------------------
/// Remove Text View
//--------------------------------------------------------
-(void) RemoveTextView:(const CSVideo::Subtitles::Subtitle*)inpSubtitle
{
    maSubtitlesToRemove.push_back(inpSubtitle);
}
//--------------------------------------------------------
/// Set Alignment
//--------------------------------------------------------
-(void) SetAlignment:(UITextView*)inpView WithAnchor:(CSRendering::AlignmentAnchor)ineAnchor
{
    inpView.textAlignment = [self TextAlignmentFromAnchor: ineAnchor];
    
    switch (ineAnchor)
    {
        case CSRendering::AlignmentAnchor::k_topLeft:
        case CSRendering::AlignmentAnchor::k_topCentre:
        case CSRendering::AlignmentAnchor::k_topRight:
        {
            inpView.contentOffset = (CGPoint){.x = 0.0f, .y = 0.0f};
            break;
        }
        case CSRendering::AlignmentAnchor::k_middleLeft:
        case CSRendering::AlignmentAnchor::k_middleCentre:
        case CSRendering::AlignmentAnchor::k_middleRight:
        {
            f32 fBoxSize = [inpView bounds].size.height;
            f32 fContentSize = [inpView contentSize].height;
            f32 fOffset = (fBoxSize - fContentSize);
            if (fOffset < 0.0f)
            {
                fOffset = 0.0f;
            }
            inpView.contentOffset = (CGPoint){.x = 0.0f, .y = -fOffset / 2.0f};
            break;
            break;
        }
        case CSRendering::AlignmentAnchor::k_bottomLeft:
        case CSRendering::AlignmentAnchor::k_bottomCentre:
        case CSRendering::AlignmentAnchor::k_bottomRight:
        {
            f32 fBoxSize = [inpView bounds].size.height;
            f32 fContentSize = [inpView contentSize].height;
            f32 fOffset = (fBoxSize - fContentSize);
            if (fOffset < 0.0f)
            {
                fOffset = 0.0f;
            }
            inpView.contentOffset = (CGPoint){.x = 0.0f, .y = -fOffset};
            break;
        }
        default:
             CS_LOG_WARNING("Could not set vertical alignment.");
            break;
    }
}
//--------------------------------------------------------
/// Text Alignment From Anchor
//--------------------------------------------------------
-(NSTextAlignment) TextAlignmentFromAnchor:(CSRendering::AlignmentAnchor)ineAnchor
{
    switch (ineAnchor)
    {
        case CSRendering::AlignmentAnchor::k_topLeft:
        case CSRendering::AlignmentAnchor::k_middleLeft:
        case CSRendering::AlignmentAnchor::k_bottomLeft:
            return NSTextAlignmentLeft;
        case CSRendering::AlignmentAnchor::k_topCentre:
        case CSRendering::AlignmentAnchor::k_middleCentre:
        case CSRendering::AlignmentAnchor::k_bottomCentre:
            return NSTextAlignmentCenter;
        case CSRendering::AlignmentAnchor::k_topRight:
        case CSRendering::AlignmentAnchor::k_middleRight:
        case CSRendering::AlignmentAnchor::k_bottomRight:
            return NSTextAlignmentRight;
        default:
            CS_LOG_WARNING("Could not convert alignment anchor to NSTextAlignment.");
            return NSTextAlignmentLeft;
    }
}
//--------------------------------------------------------
/// Calculate Text Box Rect
///
/// @return the rectangle in which the text box should
/// appear on screen.
//--------------------------------------------------------
-(CGRect) CalculateTextBoxRect:(const CSCore::Rectangle&)inRelativeBounds
{
    CSCore::Vector2 vScreenDimensions(m_screen->GetResolution().x * m_screen->GetInverseDensityScale(), m_screen->GetResolution().y * m_screen->GetInverseDensityScale());
    CSCore::Vector2 vVideoDimensions = m_videoPlayer->GetVideoDimensions();
    float fScreenAspectRatio = vScreenDimensions.x / vScreenDimensions.y;
    float fVideoAspectRatio = vVideoDimensions.x / vVideoDimensions.y;
    
    CSCore::Vector2 vVideoViewDimensions;
    if (fScreenAspectRatio < fVideoAspectRatio)
    {
        vVideoViewDimensions.x = vScreenDimensions.x;
        vVideoViewDimensions.y = vScreenDimensions.x * (vVideoDimensions.y / vVideoDimensions.x);
    }
    else
    {
        vVideoViewDimensions.x = vScreenDimensions.y * (vVideoDimensions.x / vVideoDimensions.y);
        vVideoViewDimensions.y = vScreenDimensions.y;
    }
    
    CSCore::Vector2 vVideoViewTopLeft = (vScreenDimensions - vVideoViewDimensions) * 0.5f;
    return CGRectMake(vVideoViewTopLeft.x + inRelativeBounds.Left() * vVideoViewDimensions.x, vVideoViewTopLeft.y + inRelativeBounds.Top() * vVideoViewDimensions.y, inRelativeBounds.vSize.x * vVideoViewDimensions.x, inRelativeBounds.vSize.y * vVideoViewDimensions.y);
    
}

//--------------------------------------------------------
/// CleanUp
///
/// Cleans up any possible retainers, should be called before releasing
//--------------------------------------------------------
-(void) CleanUp
{
    [mpDisplayLink invalidate];
}

//--------------------------------------------------------
/// Dealloc
//--------------------------------------------------------
-(void) dealloc
{
    [super dealloc];
}
@end

#endif
