//
//  SubtitlesRenderer.h
//  moFlow
//
//  Created by Ian Copland on 22/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _MOFLOW_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_
#define _MOFLOW_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_

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
    ChilliSource::iOS::VideoPlayer* m_videoPlayer;
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
-(id) initWithVideoPlayer:(ChilliSource::iOS::VideoPlayer*)in_videoPlayer view:(UIView*)inpView andSubtitles:(const CSVideo::SubtitlesCSPtr&)in_subtitles;
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
/// Converts from a moFlow alignment anchor to NSText
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