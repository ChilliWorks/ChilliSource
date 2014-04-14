//
//  SubtitlesRenderer.h
//  moFlow
//
//  Created by Ian Copland on 22/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_
#define _MOFLOW_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Video/Base/Subtitles.h>
#include <ChilliSource/Backend/Platform/iOS/Video/Base/VideoPlayer.h>

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
    ChilliSource::Video::SubtitlesCSPtr mpSubtitles;
    ChilliSource::iOS::VideoPlayer* m_videoPlayer;
    std::unordered_map<const ChilliSource::Video::Subtitles::Subtitle*, UITextView*> maTextViewMap;
    std::vector<const ChilliSource::Video::Subtitles::Subtitle*> maSubtitlesToRemove;
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
-(id) initWithVideoPlayer:(ChilliSource::iOS::VideoPlayer*)in_videoPlayer view:(UIView*)inpView andSubtitles:(const ChilliSource::Video::SubtitlesCSPtr&)in_subtitles;
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
//--------------------------------------------------------
-(void) AddTextView:(const ChilliSource::Video::Subtitles::Subtitle*)inpSubtitle;
//--------------------------------------------------------
/// Update Text View
///
/// Updates the given text view.
///
/// @param The text view.
/// @param the subtitle.
//--------------------------------------------------------
-(void) UpdateTextView:(UITextView*)inpTextView Subtitle:(const ChilliSource::Video::Subtitles::Subtitle*)inpSubtitle Time:(TimeIntervalMs)inTimeMS;
//--------------------------------------------------------
/// Remove Text View
///
/// Queues the text view paired with the given subtitle
/// for removal.
///
/// @param The subtitle.
//--------------------------------------------------------
-(void) RemoveTextView:(const ChilliSource::Video::Subtitles::Subtitle*)inpSubtitle;
//--------------------------------------------------------
/// Set Alignment
///
/// Sets the alignment of the text in the given text view.
///
/// @param the UITextView.
/// @param The alignment achor.
//--------------------------------------------------------
-(void) SetAlignment:(UITextView*)inpView WithAnchor:(ChilliSource::Rendering::AlignmentAnchor)ineAnchor;
//--------------------------------------------------------
/// Text Alignment From Anchor
///
/// Converts from a moFlow alignment anchor to NSText
/// alignment.
///
/// @param The alignment achor.
/// @return the NSTextAlignment.
//--------------------------------------------------------
-(NSTextAlignment) TextAlignmentFromAnchor:(ChilliSource::Rendering::AlignmentAnchor)ineAnchor;
//--------------------------------------------------------
/// Calculate Text Box Rect
///
/// @return the rectangle in which the text box should
/// appear on screen.
//--------------------------------------------------------
-(CGRect) CalculateTextBoxRect:(const ChilliSource::Core::Rectangle&)inRelativeBounds;
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