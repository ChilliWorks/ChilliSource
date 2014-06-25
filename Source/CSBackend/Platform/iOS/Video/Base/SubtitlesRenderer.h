//
//  SubtitlesRenderer.h
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

#ifndef _CHILLISOURCE_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_
#define _CHILLISOURCE_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_

#import <ChilliSource/ChilliSource.h>
#import <ChilliSource/Video/Base/Subtitles.h>
#import <CSBackend/Platform/iOS/Video/Base/VideoPlayer.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//========================================================
/// Subtitles Renderer
///
/// Renders subtitles on top of the video player.
//========================================================
@interface CSubtitlesRenderer : NSObject
{
    UIView* mpBaseView;
    CADisplayLink* mpDisplayLink;
    CSCore::Screen* m_screen;
    CSVideo::SubtitlesCSPtr mpSubtitles;
    CSBackend::iOS::VideoPlayer* m_videoPlayer;
    std::unordered_map<const CSVideo::Subtitles::Subtitle*, UITextView*> maTextViewMap;
    std::vector<const CSVideo::Subtitles::Subtitle*> maSubtitlesToRemove;
    TimeIntervalMs mCurrentTimeMS;
}
//--------------------------------------------------------
/// init With Video Player
///
/// Initialises the subtitles renderer.
///
/// @param Video player
/// @param The view.
/// @param The subtitles.
//--------------------------------------------------------
-(id) initWithVideoPlayer:(CSBackend::iOS::VideoPlayer*)in_videoPlayer view:(UIView*)inpView andSubtitles:(const CSVideo::SubtitlesCSPtr&)in_subtitles;
//--------------------------------------------------------
/// On Update
///
/// Called each frame.
//--------------------------------------------------------
-(void) OnUpdate;
//--------------------------------------------------------
/// Add Text View
///
/// Adds a new text view for the given subtitle.
///
/// @param the subtitle.
/// @param Localised text resource
//--------------------------------------------------------
-(void) AddTextViewWithSubtitle:(const CSVideo::Subtitles::Subtitle*)inpSubtitle andLocalisedText:(const CSCore::LocalisedText*) in_localisedText;
//--------------------------------------------------------
/// Update Text View
///
/// Updates the given text view.
///
/// @param The text view.
/// @param the subtitle.
//--------------------------------------------------------
-(void) UpdateTextView:(UITextView*)inpTextView Subtitle:(const CSVideo::Subtitles::Subtitle*)inpSubtitle Time:(TimeIntervalMs)inTimeMS;
//--------------------------------------------------------
/// Remove Text View
///
/// Queues the text view paired with the given subtitle
/// for removal.
///
/// @param The subtitle.
//--------------------------------------------------------
-(void) RemoveTextView:(const CSVideo::Subtitles::Subtitle*)inpSubtitle;
//--------------------------------------------------------
/// Set Alignment
///
/// Sets the alignment of the text in the given text view.
///
/// @param the UITextView.
/// @param The alignment achor.
//--------------------------------------------------------
-(void) SetAlignment:(UITextView*)inpView WithAnchor:(CSRendering::AlignmentAnchor)ineAnchor;
//--------------------------------------------------------
/// Text Alignment From Anchor
///
/// Converts from a Chilli Source alignment anchor to NSText
/// alignment.
///
/// @param The alignment achor.
/// @return the NSTextAlignment.
//--------------------------------------------------------
-(NSTextAlignment) TextAlignmentFromAnchor:(CSRendering::AlignmentAnchor)ineAnchor;
//--------------------------------------------------------
/// Calculate Text Box Rect
///
/// @return the rectangle in which the text box should
/// appear on screen.
//--------------------------------------------------------
-(CGRect) CalculateTextBoxRect:(const CSCore::Rectangle&)inRelativeBounds;
//--------------------------------------------------------
/// CleanUp
///
/// Cleans up any possible retainers, should be called before releasing
//--------------------------------------------------------
-(void) CleanUp;
//--------------------------------------------------------
/// Dealloc
///
/// destroys the listener
//--------------------------------------------------------
-(void) dealloc;
@end

#endif

#endif
