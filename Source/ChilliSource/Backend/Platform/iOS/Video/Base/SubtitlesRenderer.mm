//
//  SubtitlesRenderer.mm
//  moFlow
//
//  Created by Ian Copland on 22/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Video/Base/SubtitlesRenderer.h>

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Video/Base/VideoPlayerActivity.h>
#include <ChilliSource/Video/Base/Subtitles.h>

#import <QuartzCore/QuartzCore.h>

@implementation CSubtitlesRenderer
//--------------------------------------------------------
//--------------------------------------------------------
-(id) initWithVideoPlayer:(ChilliSource::iOS::VideoPlayerActivity*)inpVideoPlayer view:(UIView*)inpView andSubtitles:(const ChilliSource::Video::SubtitlesCSPtr&)in_subtitles
{
    if(!(self = [super init]))
	{
		return nil;
	}
    
    mpVideoPlayer = inpVideoPlayer;
    mpBaseView = inpView;
    
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
    f32 fPosition = mpVideoPlayer->GetTime();
    TimeIntervalMs currentTimeMS = (TimeIntervalMs)(fPosition * 1000.0f);
    
    if (mCurrentTimeMS != currentTimeMS)
    {
        mCurrentTimeMS = currentTimeMS;
        auto pSubtitleArray = mpSubtitles->GetSubtitlesAtTime(mCurrentTimeMS);

        //add any new subtitles
        for (auto it = pSubtitleArray.begin(); it != pSubtitleArray.end(); ++it)
        {
            
            auto mapEntry = maTextViewMap.find(*it);
            if (mapEntry == maTextViewMap.end())
            {
                [self AddTextView:*it];
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
-(void) AddTextView:(const ChilliSource::Video::Subtitles::Subtitle*)inpSubtitle
{
    //get the style
    const ChilliSource::Video::Subtitles::StyleCUPtr& pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->m_styleName);
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
    [pNewTextView setText: ChilliSource::iOS::NSStringUtils::UTF8StringToNSString(ChilliSource::Core::LocalisedText::GetText(inpSubtitle->m_textId))];
    [pNewTextView setFont:[UIFont fontWithName: ChilliSource::iOS::NSStringUtils::StringToNSString(pStyle->m_fontName) size: pStyle->m_fontSize]];
    [pNewTextView setTextColor:[UIColor colorWithRed:pStyle->m_colour.r green:pStyle->m_colour.g blue:pStyle->m_colour.b alpha:0.0f]];
    [pNewTextView setEditable:NO];
    [pNewTextView setUserInteractionEnabled:NO];
    
    [self SetAlignment: pNewTextView WithAnchor: pStyle->m_alignment];
    maTextViewMap.insert(std::make_pair(inpSubtitle, pNewTextView));
}
//--------------------------------------------------------
/// Update Text View
//--------------------------------------------------------
-(void) UpdateTextView:(UITextView*)inpTextView Subtitle:(const ChilliSource::Video::Subtitles::Subtitle*)inpSubtitle Time:(TimeIntervalMs)inTimeMS
{
    const ChilliSource::Video::Subtitles::StyleCUPtr& pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->m_styleName);
    
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
    
    inpTextView.textColor = [UIColor colorWithRed:pStyle->m_colour.r green:pStyle->m_colour.g blue:pStyle->m_colour.b alpha:fFade * pStyle->m_colour.a];
}
//--------------------------------------------------------
/// Remove Text View
//--------------------------------------------------------
-(void) RemoveTextView:(const ChilliSource::Video::Subtitles::Subtitle*)inpSubtitle
{
    maSubtitlesToRemove.push_back(inpSubtitle);
}
//--------------------------------------------------------
/// Set Alignment
//--------------------------------------------------------
-(void) SetAlignment:(UITextView*)inpView WithAnchor:(ChilliSource::Rendering::AlignmentAnchor)ineAnchor
{
    inpView.textAlignment = [self TextAlignmentFromAnchor: ineAnchor];
    
    switch (ineAnchor)
    {
        case ChilliSource::Rendering::AlignmentAnchor::k_topLeft:
        case ChilliSource::Rendering::AlignmentAnchor::k_topCentre:
        case ChilliSource::Rendering::AlignmentAnchor::k_topRight:
        {
            inpView.contentOffset = (CGPoint){.x = 0.0f, .y = 0.0f};
            break;
        }
        case ChilliSource::Rendering::AlignmentAnchor::k_middleLeft:
        case ChilliSource::Rendering::AlignmentAnchor::k_middleCentre:
        case ChilliSource::Rendering::AlignmentAnchor::k_middleRight:
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
        case ChilliSource::Rendering::AlignmentAnchor::k_bottomLeft:
        case ChilliSource::Rendering::AlignmentAnchor::k_bottomCentre:
        case ChilliSource::Rendering::AlignmentAnchor::k_bottomRight:
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
-(NSTextAlignment) TextAlignmentFromAnchor:(ChilliSource::Rendering::AlignmentAnchor)ineAnchor
{
    switch (ineAnchor)
    {
        case ChilliSource::Rendering::AlignmentAnchor::k_topLeft:
        case ChilliSource::Rendering::AlignmentAnchor::k_middleLeft:
        case ChilliSource::Rendering::AlignmentAnchor::k_bottomLeft:
            return NSTextAlignmentLeft;
        case ChilliSource::Rendering::AlignmentAnchor::k_topCentre:
        case ChilliSource::Rendering::AlignmentAnchor::k_middleCentre:
        case ChilliSource::Rendering::AlignmentAnchor::k_bottomCentre:
            return NSTextAlignmentCenter;
        case ChilliSource::Rendering::AlignmentAnchor::k_topRight:
        case ChilliSource::Rendering::AlignmentAnchor::k_middleRight:
        case ChilliSource::Rendering::AlignmentAnchor::k_bottomRight:
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
-(CGRect) CalculateTextBoxRect:(const ChilliSource::Core::Rectangle&)inRelativeBounds
{
    ChilliSource::Core::Vector2 vScreenDimensions(ChilliSource::Core::Screen::GetOrientedWidth() * ChilliSource::Core::Screen::GetInverseDensity(), ChilliSource::Core::Screen::GetOrientedHeight() * ChilliSource::Core::Screen::GetInverseDensity());
    ChilliSource::Core::Vector2 vVideoDimensions = mpVideoPlayer->GetVideoDimensions();
    float fScreenAspectRatio = vScreenDimensions.x / vScreenDimensions.y;
    float fVideoAspectRatio = vVideoDimensions.x / vVideoDimensions.y;
    
    ChilliSource::Core::Vector2 vVideoViewDimensions;
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
    
    ChilliSource::Core::Vector2 vVideoViewTopLeft = (vScreenDimensions - vVideoViewDimensions) * 0.5f;
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
